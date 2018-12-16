/****************************************************************************
  FileName     [ cirGate.cpp ]
  PackageName  [ cir ]
  Synopsis     [ Define class CirAigGate member functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2008-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#include <iostream>
#include <iomanip>
#include <sstream>
#include <stdarg.h>
#include <cassert>
#include <string.h> // ffsll
#include "cirGate.h"
#include "cirMgr.h"
#include "util.h"

using namespace std;

extern CirMgr *cirMgr;

// TODO: Implement memeber functions for class(es) in cirGate.h

/**************************************/
/*   class CirGate member functions   */
/**************************************/
void
CirGate::reportGate() const
{
}

void
CirGate::reportFanin(int level) const
{
   assert (level >= 0);
}

void
CirGate::reportFanout(int level) const
{
   assert (level >= 0);
}

bool
CirGate::isInverted( size_t s ) const {
  int i = (s & 1);
  return i;
}

void
CirGate::setInvert( size_t& s) const{ 
  s = (s | 1);
}

void
CirGate::setNonInv( size_t& s ) const{
  s = (s & (~1));
}

void
CirGate::setXorInv( size_t& s ) const{
  s = (s ^ (1));
}

CirGate*
CirGate::getPtr( size_t s ) const{
  setNonInv(s);
  return reinterpret_cast< CirGate* > (s);
}

pair< set<size_t>::iterator, bool>
CirGate::insertChild( size_t s) {
  // shall NOT contain inverted info!!
  setNonInv( s );
  return _child.insert( s );
}

pair< set<size_t>::iterator, bool>
CirGate::insertParent( size_t s) {
  // may contain inverted info!!
  return _parent.insert( s );
}

set<size_t>::iterator
CirGate::findChild( size_t s ) const{
  // no inv info shall be included.
  setNonInv( s);
  return _child.find( s );
}

set<size_t>::iterator
CirGate::findParent( size_t s ) const{
  // _parent contains inv info.
  set<size_t>::iterator it;
  it = _parent.find(s);
  if( it != _parent.end() )
    return it;
  else{
    setXorInv(s);
    return _parent.find(s);
  }
}
