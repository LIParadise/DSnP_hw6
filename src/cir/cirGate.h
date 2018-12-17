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
    CirGate(): _parent{0}, _active(false), _lineNo(0), _gateID(0), _gateDFSRef(0) {}
    CirGate(int i) : _parent{0}, _active(false), _lineNo(0), _gateID(i), _gateDFSRef(0) {}
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
    set<size_t>::iterator findChild                ( size_t ) const;
    int                   findParent               ( size_t ) const;
    void     setLineCnt  ( unsigned )      ;
    void     setGateId   ( unsigned )      ;
    void     setGateRef  ( size_t s ) { _gateDFSRef = s ; }
    size_t   getGateRef  ( )          { return _gateDFSRef ; }
    void     setActive   ( )          { _active = true; }
    void     unsetActive ( )          { _active = false; }
    bool     isActive    ()           { return _active; }

    size_t            _parent[2];
    set<size_t>       _child;

  private:

    bool             _active;
    unsigned         _lineNo;
    unsigned         _gateID;
    size_t           _gateDFSRef;

  protected:

};

class POGate : public CirGate {
  public:
    POGate(): CirGate(), _symbolMsg(""), refGateVar(0) {}
    POGate(int gid, int refid): CirGate(gid), _symbolMsg( "" ), refGateVar(refid ) {}
    virtual string getTypeStr()    const { return "PO"; }
    int            getRefGateVar() const { return refGateVar; }
    virtual void printGate() const ;
  private:
    string _symbolMsg;
    int    refGateVar;
};
class PIGate : public CirGate {
  public:
    PIGate(): CirGate(), _symbolMsg("") {}
    PIGate(int i): CirGate(i), _symbolMsg( "" ) {}
    virtual string getTypeStr() const { return "PI"; }
    virtual void printGate() const ;
  private:
    string _symbolMsg;
};
class AAGate : public CirGate {
  public:
    AAGate(): CirGate(), _IsDefined(true) {}
    AAGate(bool boolean): CirGate(), _IsDefined(boolean) {}
    AAGate(int i, bool boolean) : CirGate(i), _IsDefined( boolean ) {}
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
size_t   getInvert  ( const void* const ) ;
size_t   getNonInv  ( const void* const ) ;
size_t   getXorInv  ( const void* const ) ;
bool     isInverted ( const size_t ) ;
bool     isInverted ( const unsigned ) ;
bool     isInverted ( const int ) ;
CirGate* getPtr     ( size_t  ) ;

#endif // CIR_GATE_H
