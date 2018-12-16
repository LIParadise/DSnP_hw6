/****************************************************************************
  FileName     [ cirMgr.h ]
  PackageName  [ cir ]
  Synopsis     [ Define circuit manager ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2008-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef CIR_MGR_H
#define CIR_MGR_H

#include <vector>
#include <string>
#include <iostream>
#include <set>
#include <map>
#include <utility>

using namespace std;

#include "cirDef.h"

extern CirMgr *cirMgr;

// TODO: Define your own data members and member functions
class CirMgr
{
public:
   CirMgr(){}
   ~CirMgr() {}

   // Access functions
   // return '0' if "gid" corresponds to an undefined gate.
   CirGate* getGate(unsigned gid) const { return 0; }

   // Member functions about circuit construction
   bool readCircuit(const string&);

   // Member functions about circuit reporting
   void printSummary() const;
   void printNetlist() const;
   void printPIs() const;
   void printPOs() const;
   void printFloatGates() const;
   void writeAag(ostream&) const;

private:

   map< int, CirGate*>  lhsID;
   map< int, CirGate*>  rhsID;
   vector< pair< int, CirGate*>> PIList;
   vector< pair< int, CirGate*>> POList;
   vector< pair< int, CirGate*>> FLList;
   vector< pair< int, CirGate*>> UGList;
   vector< pair< int, CirGate*>> Island;
   vector< pair< int, CirGate*>> GateList;
   vector< int> MILOA;
   // TODO, shall have a field remaining symbol msg.


};

bool myPairIntCirGateCmp (
    const pair< int, CirGate*>& ,
    const pair< int, CirGate*>& ) ; 

#endif // CIR_MGR_H
