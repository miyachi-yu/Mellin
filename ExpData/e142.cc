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

namespace E142 {
  
  DataSet g1n() throw( int ) {
    
    StrFncParam::F2pSMC  F2;
    StrFncParam::R1998   R;
    StrFncParam::F1Param F1( F2, R );
    
    string path = data_path + "E142_1996_g1n";
    ifstream ifs( path.c_str() );
    if( ! ifs ){
      cerr << "Fail to open data file " << path << endl; 
      throw( 1 );
    } else {
	cout << "Loading " << path << endl;
    }
    
    DataSet ds;
    ds.info() = "E142,1996,g1n";
    ds.ref()  = "Phys.Rev.D54(96)6620";

    int length = 1024;
    char buff[ length ];
    int count = -1;
    while( ifs.getline( buff, length ) ){
      istringstream istr( buff );
      DataPoint dp;
      double xmin, xmax, x, q2, a1, da1, sa1, g1, stat, sys;

      if( istr 
	  >> xmin >> xmax >> x >> q2
	  >> a1 >> da1 >> sa1
	  >> g1 >> stat >> sys ){
	
	dp.kine( xmin, "xmin",      "" );
	dp.kine( xmax, "xmax",      "" );
	dp.kine(    x,    "x",      "" );
	dp.kine(   q2,  "Q^2", "GeV^2" );
	
	dp.data(   g1,  "g1n",      "" );
	dp.stat() = stat;
	dp.syst(  sys, "sys" );
	
	ds.push_back( dp );
      }
    }
    return ds;
  }
  
}
