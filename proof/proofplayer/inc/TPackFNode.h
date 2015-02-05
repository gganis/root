// @(#)root/proofplayer:$Id$
// Author: G Ganis 2/2015

/*************************************************************************
 * Copyright (C) 1995-2002, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOT_TPackFNode
#define ROOT_TPackFNode

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TPackFNode                                                           //
//                                                                      //
// Packetizer auxilliary class to describe a file node                  //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


class TPackFNode : public TNamed {

private:
   TList         *fFiles;           //  Files to be processed from this node
   TObject       *fNext;            //! Cursor in fFiles
   TList         *fActFiles;        //  Files still to be processed
   TObject       *fActFNext;        //! Cursor in fActFiles
   Int_t          fActWrks;         //  Number of workers processing from this node
   Int_t          fActWrksLoc;      //  Number of local workers processing files from this node

public:
   TFileNode(const char *name);
   ~TPackFNode() { delete fFiles; delete fActFiles; }

   // We may need sorting
   Bool_t       IsSortable() const { return kTRUE; }
 
   // Worker counters
   void          IncNWrks(Bool_t local = kFALSE) { if (local) { fActWrksLoc++}; fActWrks++; }
   void          DecNWrks(Bool_t local = kFALSE) { if (local) { fActWrksLoc--}; fActWrks--; }
   inline Int_t  GetNWrks(Bool_t local = kFALSE) const { return (local) ? fActWrksLoc : fActWrks ; }

   // Locality measure
   inline Bool_t IsLocal(const char *worker) const { return (worker == fName) ? kTRUE : kFALSE; }

   // File counters
   inline Int_t  GetNFiles const { return fFiles->GetSize(); }
   inline Int_t  GetNActFiles const { return fActFiles->GetSize(); }


   void        IncMySlaveCnt() { fMySlaveCnt++; }
   void        IncSlaveCnt(const char *slave) { if (fNodeName != slave) fSlaveCnt++; }
   void        DecSlaveCnt(const char *slave) { if (fNodeName != slave) fSlaveCnt--; R__ASSERT(fSlaveCnt >= 0); }
   Int_t       GetSlaveCnt() const {return fMySlaveCnt + fSlaveCnt;}
   Int_t       GetNumberOfActiveFiles() const { return fActFiles->GetSize(); }

   const char *GetName() const { return fNodeName.Data(); }

   void Add(TDSetElement *elem)
   {
      TFileStat *f = new TFileStat(this,elem);
      fFiles->Add(f);
      if (fUnAllocFileNext == 0) fUnAllocFileNext = fFiles->First();
   }

   TFileStat *GetNextUnAlloc()
   {
      TObject *next = fUnAllocFileNext;

      if (next != 0) {
         // make file active
         fActFiles->Add(next);
         if (fActFileNext == 0) fActFileNext = fActFiles->First();

         // move cursor
         fUnAllocFileNext = fFiles->After(fUnAllocFileNext);
      }

      return (TFileStat *) next;
   }

   TFileStat *GetNextActive()
   {
      TObject *next = fActFileNext;

      if (fActFileNext != 0) {
         fActFileNext = fActFiles->After(fActFileNext);
         if (fActFileNext == 0) fActFileNext = fActFiles->First();
      }

      return (TFileStat *) next;
   }

   void RemoveActive(TFileStat *file)
   {
      if (fActFileNext == file) fActFileNext = fActFiles->After(file);
      fActFiles->Remove(file);
      if (fActFileNext == 0) fActFileNext = fActFiles->First();
   }

   Int_t Compare(const TObject *other) const
   {
      // Must return -1 if this is smaller than obj, 0 if objects are equal
      // and 1 if this is larger than obj.
      const TPackFNode *obj = dynamic_cast<const TPackFNode*>(other);
      if (!obj) {
         Error("Compare", "input is not a TPacketizer::TPackFNode object");
         return 0;
      }

      Int_t myVal = GetSlaveCnt();
      Int_t otherVal = obj->GetSlaveCnt();
      if (myVal < otherVal) {
         return -1;
      } else if (myVal > otherVal) {
         return 1;
      } else {
         return 0;
      }
   }

   void Print(Option_t *) const
   {
      cout << "OBJ: " << IsA()->GetName() << "\t" << fNodeName
           << "\tMySlaveCount " << fMySlaveCnt
           << "\tSlaveCount " << fSlaveCnt << endl;
   }

   void Reset()
   {
      fUnAllocFileNext = fFiles->First();
      fActFiles->Clear();
      fActFileNext = 0;
      fSlaveCnt = 0;
      fMySlaveCnt = 0;
   }
};





#ifndef ROOT_TVirtualPacketizer
#include "TVirtualPacketizer.h"
#endif


class TMessage;
class TTimer;
class TTree;
class TMap;
class TProofStats;


class TPacketizer : public TVirtualPacketizer {

public:              // public because of Sun CC bug
   class TFileNode;
   class TFileStat;
   class TSlaveStat;

private:
   TList    *fPackets;      // all processed packets

   TList    *fFileNodes;    // nodes with files
   TList    *fUnAllocated;  // nodes with unallocated files
   TList    *fActive;       // nodes with unfinished files
   TMap     *fSlaveStats;   // slave status, keyed by correspondig TSlave

   Long64_t  fPacketSize;   // global base packet size
                                 // It can be set with PROOF_PacketSize
                                 // parameter, in the input list.
   Int_t     fMaxPerfIdx;   // maximum of our slaves' performance index

   Long_t    fMaxSlaveCnt;  // maximum number of workers per filenode (Long_t to avoid
                            // warnings from backward compatibility support)
   Int_t     fPacketAsAFraction; // used to calculate the packet size
                                 // fPacketSize = fTotalEntries / (fPacketAsAFraction * nslaves)
                                 // fPacketAsAFraction can be interpreted as follows:
                                 // assuming all slaves have equal processing rate, packet size
                                 // is (#events processed by 1 slave) / fPacketSizeAsAFraction.
                                 // It can be set with PROOF_PacketAsAFraction in input list.

   TPacketizer();
   TPacketizer(const TPacketizer&);     // no implementation, will generate
   void operator=(const TPacketizer&);  // error on accidental usage

   TFileNode     *NextUnAllocNode();
   void           RemoveUnAllocNode(TFileNode *);

   TFileNode     *NextActiveNode();
   void           RemoveActiveNode(TFileNode *);

   TFileStat     *GetNextUnAlloc(TFileNode *node = 0);
   TFileStat     *GetNextActive();
   void           RemoveActive(TFileStat *file);

   void           Reset();
   void           ValidateFiles(TDSet *dset, TList *slaves, Long64_t maxent = -1, Bool_t byfile = kFALSE);

public:
   TPacketizer(TDSet *dset, TList *slaves, Long64_t first, Long64_t num,
                TList *input, TProofProgressStatus *st);
   virtual ~TPacketizer();

   TDSetElement *GetNextPacket(TSlave *sl, TMessage *r);
   Long64_t      GetEntriesProcessed(TSlave *sl) const;

   Float_t       GetCurrentRate(Bool_t &all);
   Int_t         GetActiveWorkers();

   ClassDef(TPacketizer,0)  //Generate work packets for parallel processing
};

#endif
