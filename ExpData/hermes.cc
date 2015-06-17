#include "DataSet.hh"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;
using namespace Exp;

namespace HERMES {
  
  DataSet g1p() throw( int ) {
  
    string path = data_path + "HERMES06_g1p.dat";

    ifstream ifs( path.c_str() );
    if( ! ifs ){
      cerr << "Fail to open data file " << path << endl; 
      throw( 1 );
    } else {
	cout << "Loading " << path << endl;
    }
    
    DataSet ds;
    ds.info() = "HERMES,2006,g1p";
    ds.ref()  = "Phys.Rev.D75(2007)012007";
    ds.cite() = "Airapetian:2007mh";
    
    int length = 2048 ;
    char buff[ length ];
    int count = -1;
    
    while( ifs.getline( buff, length ) ){
      
      string strbuf( buff );
      if( strbuf.substr( 0, 15 ) == "Data in 45 bins" ) count = 0;
      if( count == 48 ) break;
      if( count > 3 ){
	//      cout << buff << endl;
	DataPoint g1p, g1d;
	
	double x, q2, p, dp, dps1, dps2, d, dd, dds1, dds2; 
	istringstream ist( buff );
	if( !( ist >> x >> q2 
	       >> g1p.data() >> g1p.stat() >> dps1 >> dps2 
	       >> g1d.data() >> g1d.stat() >> dps1 >> dps2 ) ){
	  cerr << "Format error: " << buff << endl;
	} 
	
	g1p.info() = "g1p";
	g1p.unit() = "";
	
	g1p.kine( x, "x",    "" );
	g1p.kine( q2, "Q^2", "GeV^2" );
	g1p.syst( dps1, "exp" );
	g1p.syst( dps2, "qcd" );

	ds.push_back( g1p );
      }
      
      if( count != -1 ) count++;
    }
    
    return ds;
  }
  
  DataSet g1d() throw( int ) {
    
    string path = data_path + "HERMES06_g1p.dat";

    ifstream ifs( path.c_str() );
    if( ! ifs ){
      cerr << "Fail to open data file " << path << endl; 
      throw( 1 );
    } else {
      cout << "Loading " << path << endl;
    }
    
    DataSet ds;
    ds.info() = "HERMES,2006,g1d";
    ds.ref()  = "Phys.Rev.D75(2007)012007";
    ds.ref()  = "Airapetian:2007mh";

    int length = 2048 ;
    char buff[ length ];
    int count = -1;
    
    while( ifs.getline( buff, length ) ){
      
      string strbuf( buff );
      if( strbuf.substr( 0, 15 ) == "Data in 45 bins" ) count = 0;
      if( count == 48 ) break;
      if( count > 3 ){
	//      cout << buff << endl;
	DataPoint g1p, g1d;
	
	double x, q2, p, dp, dps1, dps2, d, dd, dds1, dds2; 
	istringstream ist( buff );
	if( !( ist >> x >> q2 
	       >> g1p.data() >> g1p.stat() >> dps1 >> dps2 
	       >> g1d.data() >> g1d.stat() >> dps1 >> dps2 ) ){
	  cerr << "Format error: " << buff << endl;
	} 
	
	g1d.info() = "g1d";
	g1d.unit() = "";
	g1d.kine( x, "x",    "" );
	g1d.kine( q2, "Q^2", "GeV^2" );
	g1d.syst( dds1, "exp" );
	g1d.syst( dds2, "qcd" );
	
	ds.push_back( g1d );
      }
      
      if( count != -1 ) count++;
    }
    
    return ds;
  }


  DataSet g1n() throw( int ) {
    
    string path = data_path + "HERMES_1997_g1n";

    ifstream ifs( path.c_str() );
    if( ! ifs ){
      cerr << "Fail to open data file " << path << endl; 
      throw( 1 );
    } else {
      cout << "Loading " << path << endl;
    }
     
    DataSet ds;
    ds.info() = "HERMES,1997,g1n";
    ds.ref()  = "Phys.Lett.B404(1997)383";
    ds.cite() = "Ackerstaff:1997ws";

    int length = 2048 ;
    char buff[ length ];
    int count = -1;
    
    while( ifs.getline( buff, length ) ){
      
      DataPoint dp;
      double xmin, xmax, x, q2, a1, da1, sa1, g1, dg1, sg1;
      istringstream ist( buff );
      if( !( ist 
	     >> xmin >> xmax >> x >> q2 
	     >> a1 >> da1 >> sa1
	     >> g1 >> dg1 >> sg1 ) ) continue;
      
      dp.kine( xmin, "xmin",      "" );
      dp.kine( xmax, "xmax",      "" );
      dp.kine(    x,    "x",      "" );
      dp.kine(   q2,  "Q^2", "GeV^2" );
      
      dp.data(   g1,  "g1n",      "" );
      dp.stat() = dg1;
      dp.syst(  sg1, "sys" );
      
      ds.push_back( dp );
    }
    
    return ds;
  }
  
  string name( int type ){
    switch( type ){
    case 10: return "A1ep";
    case 20: return "A1h+p";
    }
    return "";
  }

  DataSet g1hp() throw( int ) { 

    string path = data_path + "HERMES_2005_A1h_p";

    ifstream ifs( path.c_str() );
    if( ! ifs ){
      cerr << "Fail to open data file " << path << endl; 
      throw( 1 );
    } else {
      cout << "Loading " << path << endl;
    }
    
    DataSet ds;
    ds.info() = "HERMES,2005,A1h";
    ds.ref()  = "Phys.Rev.D71(2005)012003";
    
    int length = 2048 ;
    char buff[ length ];
    int count = -1;
    
    while( ifs.getline( buff, length ) ){
      
      DataPoint dp;
      
      int year, type, ix, iy, iz;
      double x, y, z, q2, eta, gamma, D, a1, da1, sa1, dmc;
      istringstream ist( buff );
      if( !( ist 
	     >> year >> type >> ix >> iy >> iz
	     >> x >> y >> z >> q2 >> eta >> gamma >> D 
	     >> a1 >> da1 >> sa1 >> dmc ) ) continue;
      
      dp.kine(     x,     "x",      "" );
      dp.kine(     y,     "y",      "" );
      dp.kine(     z,     "z",      "" );
      dp.kine(    q2,   "Q^2", "GeV^2" );
      dp.kine(   eta,   "eta",      "" );
      dp.kine( gamma, "gamma",      "" );
      
      dp.data(   a1,  name( type ),  "" );
      
      dp.stat() = da1;
      dp.syst(  sa1, "sys" );
      dp.syst(  dmc,  "MC" );
      
      ds.push_back( dp );
    }
    
    return ds;
  }
  
};
