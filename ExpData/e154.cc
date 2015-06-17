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

namespace E154 {
  
  DataSet g1n() throw( int ) {
    
    DataSet ds;
    string path = data_path + "E154_1997_g1n";
    ifstream ifs( path.c_str() );
    if( ! ifs ){
      cerr << "Fail to open data file " << path << endl; 
	throw( 1 );
    } else {
      cout << "Loading " << path << endl;
    }

    ds.info() = "E154,1997,g1n";
    ds.ref()  = "Phys.Lett.B405(1997)180-190";

    int length = 1024;
    char buff[ length ];
    int count = -1;
    while( ifs.getline( buff, length ) ){
      istringstream istr( buff );
      DataPoint dp;
      double x, q2, g1, stat, sys;

      if( istr 
	  >> x >> q2
	  >> g1 >> stat >> sys ){
	
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
