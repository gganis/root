// @(#)root/proofplayer:$Id$
// Author: G Ganis 2/2015

/*************************************************************************
 * Copyright (C) 1995-2002, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOT_TPackFStat
#define ROOT_TPackFStat

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TPackFStat                                                           //
//                                                                      //
// Packetizer auxilliary class to describe file statistics              //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#ifndef ROOT_TNamed
#include "TObject.h"
#endif

class TPackFNode;
class TDSetElement;
class TPackFStat : public TObject {

public:
   // Status bits
   enum EStatusBits { kHasBeenProcessed = BIT(15) };

private:
   Bool_t         fIsDone;       // is this element processed
   TPackFNode    *fNode;         // my FileNode
   TDSetElement  *fElement;      // location of the file and its range
   Long64_t       fNextEntry;    // cursor in the range, -1 when done

public:
   TPackFStat(TPackFNode *node, TDSetElement *elem);

   inline Bool_t  IsDone() const {return TestBit(kHasBeenProcessed) ? kTRUE : kFALSE; }
   inline void    SetDone() { SetBit(kHasBeenProcessed); }
   inline TPackFNode *GetNode() const {return fNode;}
   inline TDSetElement *GetElement() const {return fElement;}
   inline Long64_t GetNextEntry() const {return fNextEntry;}
   inine  void     MoveNextEntry(Long64_t step) {fNextEntry += step;}
};

   ClassDef(TPackFStat,0)  // Describe file node
};

#endif
