/****************************************************************************
  FileName     [ cirMgr.cpp ]
  PackageName  [ cir ]
  Synopsis     [ Define cir manager functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2008-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#include <algorithm>
#include <iostream>
#include <iterator>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cstdio>
#include <ctype.h>
#include <cassert>
#include <cstring>
#include "cirMgr.h"
#include "cirGate.h"
#include "util.h"

using namespace std;

// TODO: Implement memeber functions for class CirMgr

/*******************************/
/*   Global variable and enum  */
/*******************************/
CirMgr* cirMgr = 0;

enum CirParseError {
   EXTRA_SPACE,
   MISSING_SPACE,
   ILLEGAL_WSPACE,
   ILLEGAL_NUM,
   ILLEGAL_IDENTIFIER,
   ILLEGAL_SYMBOL_TYPE,
   ILLEGAL_SYMBOL_NAME,
   MISSING_NUM,
   MISSING_IDENTIFIER,
   MISSING_NEWLINE,
   MISSING_DEF,
   CANNOT_INVERTED,
   MAX_LIT_ID,
   REDEF_GATE,
   REDEF_SYMBOLIC_NAME,
   REDEF_CONST,
   NUM_TOO_SMALL,
   NUM_TOO_BIG,

   DUMMY_END
};

/**************************************/
/*   Static varaibles and functions   */
/**************************************/
static unsigned lineNo = 0;  // in printint, lineNo needs to ++
static unsigned colNo  = 0;  // in printing, colNo needs to ++
static char buf[1024];
static string errMsg;
static int errInt;
static CirGate *errGate;

static bool
parseError(CirParseError err)
{
   switch (err) {
      case EXTRA_SPACE:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Extra space character is detected!!" << endl;
         break;
      case MISSING_SPACE:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Missing space character!!" << endl;
         break;
      case ILLEGAL_WSPACE: // for non-space white space character
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Illegal white space char(" << errInt
              << ") is detected!!" << endl;
         break;
      case ILLEGAL_NUM:
         cerr << "[ERROR] Line " << lineNo+1 << ": Illegal "
              << errMsg << "!!" << endl;
         break;
      case ILLEGAL_IDENTIFIER:
         cerr << "[ERROR] Line " << lineNo+1 << ": Illegal identifier \""
              << errMsg << "\"!!" << endl;
         break;
      case ILLEGAL_SYMBOL_TYPE:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Illegal symbol type (" << errMsg << ")!!" << endl;
         break;
      case ILLEGAL_SYMBOL_NAME:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Symbolic name contains un-printable char(" << errInt
              << ")!!" << endl;
         break;
      case MISSING_NUM:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Missing " << errMsg << "!!" << endl;
         break;
      case MISSING_IDENTIFIER:
         cerr << "[ERROR] Line " << lineNo+1 << ": Missing \""
              << errMsg << "\"!!" << endl;
         break;
      case MISSING_NEWLINE:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": A new line is expected here!!" << endl;
         break;
      case MISSING_DEF:
         cerr << "[ERROR] Line " << lineNo+1 << ": Missing " << errMsg
              << " definition!!" << endl;
         break;
      case CANNOT_INVERTED:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": " << errMsg << " " << errInt << "(" << errInt/2
              << ") cannot be inverted!!" << endl;
         break;
      case MAX_LIT_ID:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Literal \"" << errInt << "\" exceeds maximum valid ID!!"
              << endl;
         break;
      case REDEF_GATE:
         cerr << "[ERROR] Line " << lineNo+1 << ": Literal \"" << errInt
              << "\" is redefined, previously defined as "
              << errGate->getTypeStr() << " in line " << errGate->getLineNo()
              << "!!" << endl;
         break;
      case REDEF_SYMBOLIC_NAME:
         cerr << "[ERROR] Line " << lineNo+1 << ": Symbolic name for \""
              << errMsg << errInt << "\" is redefined!!" << endl;
         break;
      case REDEF_CONST:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Cannot redefine constant (" << errInt << ")!!" << endl;
         break;
      case NUM_TOO_SMALL:
         cerr << "[ERROR] Line " << lineNo+1 << ": " << errMsg
              << " is too small (" << errInt << ")!!" << endl;
         break;
      case NUM_TOO_BIG:
         cerr << "[ERROR] Line " << lineNo+1 << ": " << errMsg
              << " is too big (" << errInt << ")!!" << endl;
         break;
      default: break;
   }
   return false;
}

/**************************************************************/
/*   class CirMgr member functions for circuit construction   */
/**************************************************************/
bool
CirMgr::readCircuit(const string& fileName)
{
  fstream myfile;
  myfile.open( fileName, fstream::in );
  string    tmp_str, tmp_str1, tmp_str2;
  int       ss_pos         = 0;
  unsigned  line_count     = 0;
  unsigned  line_count_bak = 0;
  MILOA.reserve(5);

  getline( myfile, tmp_str ); // string; "aag MILOA";
  stringstream tmp_sstr ( tmp_str );
  tmp_sstr >> tmp_str; // "aag"
  for( int i = 0; i < 5; ++i ){
    int j = 0;
    tmp_sstr >> j;
    MILOA.push_back( j);
  }

  line_count = 2;
  for( int i = 0; i < MILOA[1]; ++i ) { // I
    // set definedList;
    getline( myfile, tmp_str );
    int id  = stoi( tmp_str, nullptr, 10 )/2;
    definedList.insert( id );

    // set GateList
    auto ptr = new PIGate();
    if( GateList.insert( pair< int, CirGate* > ( id, ptr ) )
       .second == false ){
      // insertion failure.
      delete ptr;
      ptr = nullptr;
    }else {
      ptr -> setLineCnt( line_count );
    }
    ++line_count;
  }
  for( int i = 0; i < MILOA[2]; ++i ) { // L
    getline( myfile, tmp_str );
    ++line_count;
  }
  for( int i = 0; i < MILOA[3]; ++i ) { // O
    // set usedList
    getline( myfile, tmp_str );
    int id  = stoi( tmp_str, nullptr, 10 )/2;
    usedList.insert( id );

    // set GateList
    auto ptr = new POGate();
    if( GateList.insert( pair< int, CirGate* > ( id, ptr ) )
       .second == false ){
      // insertion failure.
      delete ptr;
      ptr = nullptr;
    }else {
      ptr -> setLineCnt( line_count );
    }
    ++line_count;
  }

  // back up start position of A of MILOA
  ss_pos = myfile.tellg();
  line_count_bak = line_count;

  for( int i = 0; i < MILOA[4]; ++i ) { // A
    getline( myfile, tmp_str );
    stringstream ss ( tmp_str );
    ss >> tmp_str >> tmp_str1 >> tmp_str2;

    int id = stoi( tmp_str, nullptr, 10 ) / 2;
    definedList.insert( id );

    auto ptr       = new AAGate( true );
    auto ret_pair = GateList.insert( pair< int, CirGate* > ( id, ptr ));
    ptr -> setLineCnt( line_count );
    if( ret_pair . second == false ){
      // insertion failure.
      // check if valid (CirGate*)
      if( ret_pair.first->second == nullptr )
        ret_pair.first->second = ptr;
      else{
        // optional TODO 
        // report gate re-define.
      }
    }

    ptr = nullptr;
    id = stoi( tmp_str1, nullptr, 10 ) /2 ;
    usedList.insert( id );
    ret_pair = GateList.insert( pair< int, CirGate* > ( id, nullptr ));
    id = stoi( tmp_str2, nullptr, 10 ) /2;
    usedList.insert( id );
    ret_pair = GateList.insert( pair< int, CirGate* > ( id, nullptr ));

    ++line_count;
  }

  DefButNUsedList.clear();
  UnDefinedList.clear();
  DefButNUsedList.reserve( 10);
  UnDefinedList.reserve( 10);

  set_difference( definedList.begin(), definedList.end(),
                 usedList.begin(), usedList.end(),
                 back_inserter(DefButNUsedList));

  set_difference( usedList.begin(), usedList.end(),
                 definedList.begin(), definedList.end(),
                 back_inserter(UnDefinedList) );

  // island is an aag with no input specified,
  // thus not to be detected here.

  for_each( GateList.begin(), GateList.end(),
           [this] ( pair< const int, CirGate* >& p)  {
             if( p.second == nullptr ){
#ifdef DEBUG
               auto it = find( UnDefinedList.begin(),
                              UnDefinedList.end(), p.first );
               assert( it != UnDefinedList.end() &&
                      "weird UnDefinedList while reading file" );
#endif
               auto ptr = new AAGate(false);
               p.second = ptr;
             } 
           });

  // routing.
  myfile.clear();
  myfile.seekg( ss_pos );
  for( int i = 0; i < MILOA[4]; ++i ) { // A
    getline( myfile, tmp_str );
    stringstream ss ( tmp_str );
    ss >> tmp_str >> tmp_str1 >> tmp_str2;
    auto itorg = GateList.begin();
    auto itor1 = GateList.begin();
    auto itor2 = GateList.begin();
    int g_var = stoi( tmp_str, nullptr, 10 );
    int var_1 = stoi( tmp_str1, nullptr, 10 );
    int var_2 = stoi( tmp_str2, nullptr, 10 );
    int gid = g_var/2;
    int id1 = var_1/2;
    int id2 = var_2/2;

    // aag(gid) is based on id1, id2.
    // set id1 to have child gid.
    // set id2 to have child gid.
    itorg = GateList.find( gid );
    itor1 = GateList.find( id1 );
    itor2 = GateList.find( id2 );

    if( isInverted( var_1 ) ){
      itorg -> second -> insertParent( getInvert(
          reinterpret_cast<size_t>( itor1 -> second ) ) );
    }else{
      itorg -> second -> insertParent( getNonInv(
          reinterpret_cast<size_t>( itor1 -> second ) ) );
    }
    if( isInverted( var_2 ) ){
      itorg -> second -> insertParent( getInvert(
          reinterpret_cast<size_t>( itor2 -> second ) ) );
    }else{
      itorg -> second -> insertParent( getNonInv(
          reinterpret_cast<size_t>( itor2 -> second ) ) );
    }

    itor1 -> second -> insertChild ( 
      reinterpret_cast<size_t> ( itorg -> second ) );
    itor2 -> second -> insertChild ( 
      reinterpret_cast<size_t> ( itorg -> second ) );
  }

  return true;
}

/**********************************************************/
/*   class CirMgr member functions for circuit printing   */
/**********************************************************/
/*********************
  Circuit Statistics
  ==================
  PI          20
  PO          12
  AIG        130
  ------------------
  Total      162
 *********************/
void
CirMgr::printSummary() const
{
}

void
CirMgr::printNetlist() const
{
}

void
CirMgr::printPIs() const
{
  cout << "PIs of the circuit:";
  cout << endl;
}

void
CirMgr::printPOs() const
{
  cout << "POs of the circuit:";
  cout << endl;
}

void
CirMgr::printFloatGates() const
{
}

void
CirMgr::writeAag(ostream& outfile) const
{
}

bool
myPairIntCharCmp(
  const pair< int, char >& pair1,
  const pair< int, char >& pair2 ) {
  return pair1.first < pair2.first ;
}
