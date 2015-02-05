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
// TPackFStat                                                           //
//                                                                      //
// Packetizer auxilliary class to describe file statistics              //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#include "TPackFStat.h"
#include "TDSet.h"

//______________________________________________________________________________
TPackFStat::TPackFStat(TPackFNode *node, TDSetElement *elem)
           : fNode(node), fElement(elem), fNextEntry(elem->GetFirst())
{
   // Constructor

   ResetBit(kHasBeenProcessed);
}

