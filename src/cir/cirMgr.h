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
   CirMgr(): globalDFSRef(0)  {}
   ~CirMgr() { clearGate();}

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

   // helper functions
   bool buildDFSList () ;
   bool DFS          ( CirGate*, int = 0 ) ;
   void clearGate    () ;

   // helper data fields
   set<int>  definedList;
   // PI, AAG.
   set<int>  usedList;
   // PO, fanin of AAG.
   size_t    globalDFSRef;

   vector< int > DefButNUsedList;
   vector< int > UnDefinedList;

   map< int, CirGate* > GateList;

   vector< int > PIIDList;
   vector< int > POIDList;
   vector< pair< int, CirGate*>> Island;
   vector< int> MILOA;
   // TODO, shall have a field remaining symbol msg.

   vector< pair<CirGate*, int> > DFSList;

};

bool myPairIntCharCmp (
    const pair< int, char>& ,
    const pair< int, char>& ) ; 

#endif // CIR_MGR_H
