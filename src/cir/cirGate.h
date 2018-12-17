/****************************************************************************
  FileName     [ cirGate.h ]
  PackageName  [ cir ]
  Synopsis     [ Define basic gate data structures ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2008-present LaDs(III), GIEE, NTU, Taiwan ]
 ****************************************************************************/

#ifndef CIR_GATE_H
#define CIR_GATE_H

#include <string>
#include <vector>
#include <set>
#include <iostream>
#include "cirDef.h"

using namespace std;

class CirGate;

//------------------------------------------------------------------------
//   Define classes
//------------------------------------------------------------------------
// TODO: Define your own data members and member functions, or classes
class CirGate
{
  public:
    CirGate(): _lineNo(0), _gateID(0) {}
    virtual ~CirGate() {}

    // Basic access methods
    virtual  string getTypeStr() const { return "base"; }
    unsigned getLineNo() const { return _lineNo; }
    unsigned getGateID() const { return _gateID; }

    // Printing functions
    virtual void printGate() const = 0;
    void reportGate() const;
    void reportFanin(int level) const;
    void reportFanout(int level) const;

    // public helpers.
    pair< set<size_t>::iterator, bool> insertChild ( size_t );
    pair< set<size_t>::iterator, bool> insertParent( size_t );
    set<size_t>::iterator findChild                ( size_t ) const;
    set<size_t>::iterator findParent               ( size_t ) const;
    CirGate* getPtr     ( size_t  ) const ;
    void     setLineCnt ( unsigned )      ;
    void     setGateId  ( unsigned )      ;

  private:

    set< size_t >    _parent;
    set< size_t >    _child;
    unsigned         _lineNo;
    unsigned         _gateID;

  protected:

};

class POGate : public CirGate {
  public:
    POGate(): CirGate(), _symbolMsg("") {}
    virtual string getTypeStr() const { return "PO"; }
    virtual void printGate() const ;
  private:
    string _symbolMsg;
};
class PIGate : public CirGate {
  public:
    PIGate(): CirGate(), _symbolMsg("") {}
    virtual string getTypeStr() const { return "PI"; }
    virtual void printGate() const ;
  private:
    string _symbolMsg;
};
class AAGate : public CirGate {
  public:
    AAGate(): CirGate(), _IsDefined(true) {}
    AAGate(bool boolean): CirGate(), _IsDefined(boolean) {}
    virtual string getTypeStr() const {
      return (  (_IsDefined)? "AAG" : "UNDEF" );
    }
    void setDefined() { _IsDefined = true; }
    void setUNDEF() { _IsDefined = false; }
    bool isDefined() const { return _IsDefined; }
    virtual void printGate() const ;
  private:
    bool _IsDefined;
};

size_t   getInvert  ( const size_t& ) ;
size_t   getNonInv  ( const size_t& ) ;
size_t   getXorInv  ( const size_t& ) ;
bool     isInverted ( const size_t ) ;
bool     isInverted ( const unsigned ) ;
bool     isInverted ( const int ) ;

#endif // CIR_GATE_H
