#include "DataSet.hh"

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

#include <StrFncParam/ALLM97.hh>
#include <StrFncParam/F2pSMC.hh>
#include <StrFncParam/F2nSMC.hh>
#include <StrFncParam/F2dSMC.hh>
#include <StrFncParam/R1998.hh>
#include <StrFncParam/F1Param.hh>

using namespace std;
using namespace Exp;

namespace E143 {
  
  DataSet g1p() throw( int ) {
    
    StrFncParam::F2pSMC  F2;
    StrFncParam::R1998   R;
    StrFncParam::F1Param F1( F2, R );
    
    DataSet ds;

    string path = data_path + "E143_1998_g1pF1p_detail";

    ifstream ifs( path.c_str() );
    if( ! ifs ){
      cerr << "Fail to open data file " << path << endl; 
	throw( 1 );
    } else {
      cout << "Loading " << path << endl;
    }
    
    ds.info() = "E143,1998,g1p";
    ds.ref()  = "SLAC-PUB-7753 (Feb 1998)";
    
    int length = 1024;
    char buff[ length ];
    int count = -1;
    while( ifs.getline( buff, length ) ){
      istringstream istr( buff );
      DataPoint dp;
      double xmin, xmax, x, q2, E, g1F1, stat, sys;
      if( istr 
	  >> x >> q2 >> E 
	  >> g1F1 >> stat >> sys ){
	
	double f1 = F1( x, q2 );
	
	dp.kine(  x,   "x",      "" );
	dp.kine( q2, "Q^2", "GeV^2" );
	
	dp.data( g1F1 * f1, "g1p", "" );
	dp.stat() = stat * f1;
	dp.syst(  sys * f1, "sys" );
	
	ds.push_back( dp );
      }
    }
    return ds;
  }
  
  
  DataSet g1d() throw( int ) {
    
    StrFncParam::F2dSMC  F2;
    StrFncParam::R1998   R;
    StrFncParam::F1Param F1( F2, R );
    
    string path = data_path + "E143_1998_g1dF1d_detail";

    ifstream ifs( path.c_str() );
    if( ! ifs ){
      cerr << "Fail to open data file " << path << endl; 
	throw( 1 );
    } else {
      cout << "Loading " << path << endl;
    }
    
    DataSet ds;
    ds.info() = "E143,1998,g1d";
    ds.ref()  = "SLAC-PUB-7753 (Feb 1998)";

    int length = 1024;
    char buff[ length ];
    int count = -1;
    while( ifs.getline( buff, length ) ){
      istringstream istr( buff );
      DataPoint dp;
      double xmin, xmax, x, q2, E, g1F1, stat, sys;
      if( istr 
	  >> x >> q2 >> E 
	  >> g1F1 >> stat >> sys ){
	
	double f1 = F1( x, q2 );
	
	dp.kine(  x,   "x",      "" );
	dp.kine( q2, "Q^2", "GeV^2" );
	
	dp.data(    g1F1 * f1, "g1d", "" );
	dp.stat() = stat * f1;
	dp.syst(    sys  * f1, "sys" );
	
	ds.push_back( dp );
      }
    }
    return ds;
  }
  
}
