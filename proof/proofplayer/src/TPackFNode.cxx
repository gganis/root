// @(#)root/proofplayer:$Id$
// Author: G Ganis 2/2015

/*************************************************************************
 * Copyright (C) 1995-2002, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TPackFNode                                                           //
//                                                                      //
// Packetizer auxilliary class to describe a file node                  //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#include "TPackFNode.h"

#if 0
#include "Riostream.h"
#include "TDSet.h"
#include "TEnv.h"
#include "TError.h"
#include "TEventList.h"
#include "TEntryList.h"
#include "TMap.h"
#include "TMessage.h"
#include "TMonitor.h"
#include "TNtupleD.h"
#include "TObject.h"
#include "TParameter.h"
#include "TPerfStats.h"
#include "TProofDebug.h"
#include "TProof.h"
#include "TProofPlayer.h"
#include "TProofServ.h"
#include "TSlave.h"
#include "TSocket.h"
#include "TTimer.h"
#include "TUrl.h"
#include "TClass.h"
#include "TMath.h"
#include "TObjString.h"
#endif

//
// The following three utility classes manage the state of the
// work to be performed and the slaves involved in the process.
// A list of TFileNode(s) describes the hosts with files, each
// has a list of TFileStat(s) keeping the state for each TDSet
// element (file).
//
// The list of TSlaveStat(s) keep track of the work (being) done
// by each slave
//

//______________________________________________________________________________
TPackFNode::TPackFNode(const char *name) : TNamed(name,"")
           : fFiles(new TList), fNext(0),fActFiles(new TList),
             fActNext(0), fActWrks(0), fActWrksLoc(0)
{
   // Constructor

   fFiles->SetOwner();
   fActFiles->SetOwner(kFALSE);
}

//______________________________________________________________________________
void TPackFNode::Add(TDSetElement *elem)
{
   // Add a file

   TPackFStat *f = new TPackFStat(this, elem);
   fFiles->Add(f);
   if (fNext == 0) fNext = fFiles->First();

   // Done
}

//______________________________________________________________________________
TPackFStat *TPackFNode::GetNext()
{
   // Point to next file to be processed

   if (fNext) {
      // Make file active
      fActFiles->Add(fNext);
      if (fActNext == 0) fActNext = fActFiles->First();

      // Move cursor
      fNext = fFiles->After(fNext);
   }

   return (TPackFStat *) fNext;
}

//______________________________________________________________________________
TPackFStat *TPackFNode::GetNextActive()
{
   // Point to the next file being processed
   TObject *next = fActFileNext;

   if (fActNext) {
      fActNext = fActFiles->After(fActFNext);
      if (fActNext == 0) fActNext = fActFiles->First();
   }

   return (TPackFStat *) next;
}

//______________________________________________________________________________
void TPackFNode::RemoveActive(TPackFStat *file)
{
   // Remove file from the active list

   if (fActNext == file) fActNext = fActFiles->After(file);
   fActFiles->Remove(file);
   if (fActNext == 0) fActNext = fActFiles->First();
}

//______________________________________________________________________________
Int_t TPackFNode::Compare(const TObject *other) const
{
   // Compare 'this' with 'other' 
   // Return -1 if this is smaller than obj, 0 if objects are equal
   // and 1 if this is larger than obj

   const TPackFNode *obj = dynamic_cast<const TPackFNode *>(other);
   if (!obj) {
      Error("Compare", "input is not a TPackFNode object!");
      return 0;
   }

   // Metrics is the numebr of wprkesr processing data from the node
   if (GetNWrks() < obj->GetNWrks()) {
      return -1;
   } else if (GetNWrks() > obj->GetNWrks()) {
      return 1;
   } else {
      return 0;
   }
}

//______________________________________________________________________________
void TPackFNode::Print(Option_t *) const
{
   // Print function

   Printf("OBJ: TPackFNode \t%s \t%d \t%d", GetName(), fActWrksLoc, fActWrks);
}

//______________________________________________________________________________
void TPackFNode::Reset()
{
   // Reset this object

   fNext = fFiles->First();
   fActFiles->Clear();
   fActNext = 0;
   fActWrks = 0;
   fActWrksLoc = 0;
}

