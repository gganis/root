/*****************************************************************************
 * Project: RooFit                                                           *
 * Package: RooFitCore                                                       *
 * @(#)root/roofitcore:$Id$
 * Authors:                                                                  *
 *   WV, Wouter Verkerke, UC Santa Barbara, verkerke@slac.stanford.edu       *
 *   DK, David Kirkby,    UC Irvine,         dkirkby@uci.edu                 *
 *                                                                           *
 * Copyright (c) 2000-2005, Regents of the University of California          *
 *                          and Stanford University. All rights reserved.    *
 *                                                                           *
 * Redistribution and use in source and binary forms,                        *
 * with or without modification, are permitted according to the terms        *
 * listed in LICENSE (http://roofit.sourceforge.net/license.txt)             *
 *****************************************************************************/

/**
\file RooCategoryProxy.cxx
\class RooCategoryProxy
\ingroup Roofitcore

RooCategoryProxy is the proxy implementation for RooAbsCategory objects
A RooCategoryProxy is the general mechanism to store references
to RooAbsCategoriess inside a RooAbsArg

RooCategoryProxy provides a cast operator to Int_t and 'const char*', allowing
the proxy to functions a Int_t/'const char*' on the right hand side of expressions.
**/


#include "RooFit.h"
#include "Riostream.h"
#include "RooArgSet.h"
#include "RooCategoryProxy.h"

using namespace std;

ClassImp(RooCategoryProxy)
;


////////////////////////////////////////////////////////////////////////////////
/// Constructor with owner and proxied category object

RooCategoryProxy::RooCategoryProxy(const char* Name, const char* desc, RooAbsArg* owner,
				   Bool_t valueServer, Bool_t shapeServer, Bool_t ownArg) : 
  RooArgProxy(Name, desc, owner, valueServer, shapeServer, ownArg)
{
}



////////////////////////////////////////////////////////////////////////////////
/// Constructor with owner and proxied category object

RooCategoryProxy::RooCategoryProxy(const char* Name, const char* desc, RooAbsArg* owner, RooAbsCategory& ref,
				   Bool_t valueServer, Bool_t shapeServer, Bool_t ownArg) : 
  RooArgProxy(Name, desc, owner, ref, valueServer, shapeServer, ownArg)
{
}



////////////////////////////////////////////////////////////////////////////////
/// Copy constructor

RooCategoryProxy::RooCategoryProxy(const char* Name, RooAbsArg* owner, const RooCategoryProxy& other) : 
  RooArgProxy(Name, owner, other) 
{
}



////////////////////////////////////////////////////////////////////////////////
/// Destructor

RooCategoryProxy::~RooCategoryProxy() 
{
}



////////////////////////////////////////////////////////////////////////////////
/// Return RooAbsCategoryLValye pointer of contained object if
/// it is indeed an lvalue

RooAbsCategoryLValue* RooCategoryProxy::lvptr() const 
{
  // Assert that the held arg is an LValue
  RooAbsCategoryLValue* Lvptr = dynamic_cast<RooAbsCategoryLValue*>(_arg) ;
  if (!Lvptr) {
    cout << "RooCategoryProxy(" << name() << ")::INTERNAL error, expected " << _arg->GetName() << " to be an lvalue" << endl ;
    assert(0) ;
  }
  return Lvptr ;
}



////////////////////////////////////////////////////////////////////////////////
/// Change object held in proxy into newRef

Bool_t RooCategoryProxy::setArg(RooAbsCategory& newRef) 
{
  if (absArg()) {
    if (TString(arg().GetName()!=newRef.GetName())) {
      newRef.setAttribute(Form("ORIGNAME:%s",arg().GetName())) ;
    }
    return changePointer(RooArgSet(newRef),kTRUE) ;
  } else {
    return changePointer(RooArgSet(newRef),kFALSE,kTRUE);
  }
}
