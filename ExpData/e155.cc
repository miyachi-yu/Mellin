#include "DataSet.hh"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <StrFncParam/ALLM97.hh>
#include <StrFncParam/F2pSMC.hh>
#include <StrFncParam/F2nSMC.hh>
#include <StrFncParam/F2dSMC.hh>
#include <StrFncParam/R1998.hh>
#include <StrFncParam/F1Param.hh>

using namespace std;
using namespace Exp;

namespace E155 {
  
  
  DataSet g1p() throw( int ) {
    
    string path = data_path + "E155_2000_g1pn.dat";
    ifstream ifs( path.c_str() );
    if( ! ifs ){
      cerr << "Fail to open data file " << path << endl; 
      throw( 1 );
    } else {
      cout << "Loading " << path << endl;
    }
        
    DataSet ep;
    
    StrFncParam::F2pSMC  F2;
    StrFncParam::R1998   R;
    StrFncParam::F1Param F1( F2, R );
    // StrFncParam::ALLM97  allm;
    // StrFncParam::F1Param f1( allm );
    
    ep.info() = "E155,2000,g1p";
    ep.ref() = "Phys.Lett.B493(2000)19";
    
    int length = 2048 ;
    char buff[ length ];
    int count = -1;
    
    while( ifs.getline( buff, length ) ){
      
      if( buff[0] == '-' ) count = 0;
      if( count < 0 )      continue;
      
      istringstream ist( buff );
      
      double x, q2, g1p, dg1p, sg1p, g1n, dg1n, sg1n;
      if( ! ( ist >> x >> q2 >> g1p >> dg1p >> sg1p >> g1n >> dg1n >> sg1n  ) )
	continue;
      
      DataPoint p, n;
      p.kine( x,         "x",      "" );
      p.kine( q2,      "Q^2", "GeV^2" );
      
      double f1p = F1( x, q2 );
      
      p.data( g1p * f1p,  "g1p", "" );
      p.stat() = dg1p * f1p;
      p.syst( sg1p * f1p, "sys" ); 
      
      ep.push_back( p );
      
      count++ ;
    }
    
    return ep;
    
  };
  
  DataSet g1n() throw(int) {
    
    string path = data_path + "E155_2000_g1pn.dat";
    ifstream ifs( path.c_str() );
    if( ! ifs ){
      cerr << "Fail to open data file " << path << endl; 
      throw( 1 );
    } else {
      cout << "Loading " << path << endl;
    }
    
    StrFncParam::F2nSMC  F2;
    StrFncParam::R1998   R;
    StrFncParam::F1Param F1( F2, R );
    
    DataSet en;
    en.info() = "E155,2000,g1n";
    en.ref()  = "Phys.Lett.B493(2000)19";
    
    int length = 2048 ;
    char buff[ length ];
    int count = -1;
    
    while( ifs.getline( buff, length ) ){
      
      if( buff[0] == '-' ) count = 0;
      if( count < 0 ) continue;
      
      istringstream ist( buff );
      
      double x, q2, g1p, dg1p, sg1p, g1n, dg1n, sg1n;
      if( ! ( ist >> x >> q2 >> g1p >> dg1p >> sg1p >> g1n >> dg1n >> sg1n  ) )
	continue;
      
      DataPoint n;
      n.kine( x,         "x",      "" );
      n.kine( q2,      "Q^2", "GeV^2" );

      //      n.data( g1n, "g1n/F1n",      "" );
      //      n.stat() = dg1n;
      //      n.syst( sg1n,    "sys" ); 
      
      double f1n = F1( x, q2 );
      n.data( g1n * f1n, "g1n",      "" );
      n.stat() = dg1n * f1n;
      n.syst( sg1n * f1n,    "sys" ); 
      
      en.push_back( n );
      
      count++ ;
    }
    
    return en;
  };
  
  DataSet g1d() throw(int) {
    
    string path = data_path + "E155_1999_g1d";
    ifstream ifs( path.c_str() );
    if( ! ifs ){
      cerr << "Fail to open data file " << path << endl; 
      throw( 1 );
    } else {
      cout << "Loading " << path << endl;
    }
    
    StrFncParam::F2nSMC  F2;
    StrFncParam::R1998   R;
    StrFncParam::F1Param F1( F2, R );
    
    DataSet en;
    en.info() = "E155,1999,g1d";
    en.ref()  = "Phys.Lett.B463(1999)339";
    
    int length = 2048 ;
    char buff[ length ];
    int count = -1;
    
    while( ifs.getline( buff, length ) ){
      
      istringstream ist( buff );
      double x, q2, g1F1, dg1F1, sg1F1, g1, dg1, sg1;
      if( ! ( ist >> x >> q2 
	      >> g1F1 >> dg1F1 >> sg1F1 
	      >> g1 >> dg1 >> sg1  ) )
	continue;
      
      DataPoint n;
      n.kine( x,    "x",      "" );
      n.kine( q2, "Q^2", "GeV^2" );
      
      //      n.data( g1n, "g1n/F1n",      "" );
      //      n.stat() = dg1n;
      //      n.syst( sg1n,    "sys" ); 
      
      n.data( g1, "g1d",      "" );
      n.stat() = dg1;
      n.syst( sg1, "sys" ); 
      
      en.push_back( n );
      
      count++ ;
    }
    
    return en;
  };
  
};

void e155( DataSet& ep, DataSet& en ){
  
  
  //  StrFncParam::ALLM97  allm;
  //  StrFncParam::F1Param f1( allm );
  
  StrFncParam::F2pSMC  F2p;
  StrFncParam::F2nSMC  F2n;
  StrFncParam::R1998   R;
  StrFncParam::F1Param F1p( F2p, R );
  StrFncParam::F1Param F1n( F2n, R );
  
  string path = data_path + "E155_2000_g1pn.dat";
  ifstream ifs( path.c_str() );
  if( ! ifs ){
    cerr << "Fail to open data file " << path << endl; 
    throw( 1 );
  } else {
    cout << "Loading " << path << endl;
  }

  ep.info() = "E155,2000,g1p";
  en.info() = "E155,2000,g1n";
  
  ep.ref() = "Phys.Lett.B493(2000)19";
  en.ref() = "Phys.Lett.B493(2000)19";
  
  int length = 2048 ;
  char buff[ length ];
  int count = -1;
  
  while( ifs.getline( buff, length ) ){
    
    if( buff[0] == '-' ) count = 0;
    if( count < 0 ) continue;
    
    istringstream ist( buff );
    
    double x, q2, g1p, dg1p, sg1p, g1n, dg1n, sg1n;
    if( ! ( ist >> x >> q2 >> g1p >> dg1p >> sg1p >> g1n >> dg1n >> sg1n  ) )
      continue;
    
    DataPoint p, n;
    p.kine( x,         "x",      "" );
    p.kine( q2,      "Q^2", "GeV^2" );
    
    double f1p = F1p( x, q2 );
    double f1n = F1n( x, q2 );
    
    p.data( g1p * f1p,  "g1p", "" );
    p.stat() = dg1p * f1p;
    p.syst( sg1p * f1p, "sys" ); 
    
    //    p.data( g1p, "g1p/F1p",      "" );
    //    p.stat() = dg1p;
    //    p.syst( sg1p,    "sys" ); 
    
    n.kine( x,         "x",      "" );
    n.kine( q2,      "Q^2", "GeV^2" );

    //    n.data( g1n, "g1n/F1n",      "" );
    //    n.stat() = dg1n;
    //    n.syst( sg1n,    "sys" ); 
    
    n.data( g1n * f1n, "g1n",      "" );
    n.stat() = dg1n * f1n;
    n.syst( sg1n * f1n,    "sys" ); 
    
    ep.push_back( p );
    en.push_back( n );

    count++ ;
  }


}
