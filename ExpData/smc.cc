#include "DataSet.hh"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;
using namespace Exp;

namespace SMC {
  
  DataSet g1p() throw( int ) {
    
    string path = data_path + "SMC_1998_g1p.dat" ;

    ifstream ifs( path.c_str() );
    if( ! ifs ){
      cerr << "Fail to open data file " << path << endl; 
      throw( 1 );
    } else {
      cout << "Loading " << path << endl;
    }

    DataSet ep;
    ep.info() = "SMC,1998,g1p";
    ep.ref()  = "Phys.Rev.D58(1998)112001";
    
    int length = 2048 ;
    char buff[ length ];
    int count = -1;
    
    while( ifs.getline( buff, length ) ){
      if( buff[0] == '-' ) count = 0;
      if( count < 0 || count > 11 ) continue;
      istringstream ist( buff );
      DataPoint dp;
      
      double xmin, xmax, x, q2, a1p, da1p, sa1p, g1p, dg1p, sg1p;
      if( ! ( ist 
	      >> xmin >> xmax >> x >> q2 
	      >> a1p >> da1p >> sa1p 
	      >> dp.data() >> dp.stat() >> sg1p ) ) continue;

      dp.info() = "g1p";
      dp.unit() = "";
      dp.kine( xmin, "xmin", "" );
      dp.kine( xmax, "xmax", "" );
      dp.kine(    x,    "x", "" );
      dp.kine(   q2,  "Q^2", "GeV^2" );
      dp.syst( sg1p, "syst" );

      ep.push_back( dp );
      
      count++ ;
    }
    return ep;
  }
  
  DataSet g1d() throw( int ) {

    string path = data_path + "SMC_1998_g1d.dat";
    
    ifstream ifs( path.c_str() );
    if( ! ifs ){
      cerr << "Fail to open data file " << path << endl; 
      throw( 1 );
    } else {
      cout << "Loading " << path << endl;
    }
    
    DataSet ep;
    ep.info() = "SMC,1998,g1d";
    ep.ref()  = "Phys.Rev.D58(1998)112001";
    
    int length = 2048 ;
    char buff[ length ];
    int count = -1;
    
    while( ifs.getline( buff, length ) ){
      if( buff[0] == '-' ) count = 0;
      istringstream ist( buff );
      DataPoint dp;
      
      double xmin, xmax, x, q2, a1p, da1p, sa1p, g1p, dg1p, sg1p;
      if( ! ( ist 
	      >> xmin >> xmax >> x >> q2 
	      >> a1p >> da1p >> sa1p 
	      >> dp.data() >> dp.stat() >> sg1p ) ) continue;
      
      dp.info() = "g1d";
      dp.unit() = "";
      dp.kine( xmin, "xmin", "" );
      dp.kine( xmax, "xmax", "" );
      dp.kine(    x,    "x", "" );
      dp.kine(   q2,  "Q^2", "GeV^2" );
      dp.syst( sg1p, "syst" );
      
      ep.push_back( dp );
    }
    return ep;
  }
  
}
