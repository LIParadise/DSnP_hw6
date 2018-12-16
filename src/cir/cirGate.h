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
    CirGate() {}
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

  private:

    vector< size_t > _parent;
    vector< size_t > _child;
    unsigned         _lineNo;
    unsigned         _gateID;

    bool     isInverted( size_t  ) const ;
    void     setInvert ( size_t& ) ;
    CirGate* getPtr    ( size_t  ) const ;

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
    AAGate(): CirGate(), isDefined(true) {}
    AAGate(bool boolean): CirGate(), isDefined(boolean) {}
    virtual string getTypeStr() const {
      return (  (isDefined)? "AAG" : "UNDEF" );
    }
    void setDefined() { isDefined = true; }
    void setUNDEF() { isDefined = false; }
    virtual void printGate() const ;
  private:
    bool isDefined;
};

#endif // CIR_GATE_H
