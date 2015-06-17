#ifndef _XYZ_hh_
#define _XYZ_hh_

#include "DataSet.hh"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

namespace Exp{

namespace XYZ {

  enum HADRON{
       Of = 00,
    Hp = 20, Hm = 21,
    PIp = 30, PIm = 31,
    Kp = 40, Km = 41,
  };

  string hadname( const XYZ::HADRON& h ){
    switch( h ){
    case Of :return "";
    case Hp : return "h+";
    case Hm : return "h-";
    case PIp : return "pi+";
    case PIm : return "pi-";
    case Kp : return "K+";
    case Km : return "K-";
    }
    return "";
  };

  enum TARGET{
    PRO = 1, NEU = 2, DEU = 3
  };
  
  string tarname( const XYZ::TARGET& t ){
    switch( t ){
    case PRO : return "PRO";
    case NEU : return "NEU";
    case DEU : return "DEU";
    }
    return "";
  };

  DataSet A1h( const XYZ::TARGET& t, 
	       const XYZ::HADRON& h ) throw( int ) {

       ifstream ifs( "../Data/ABC.dat" );
    if( ! ifs ){
      cerr << "Fail to open data file!!" << endl;
      throw( 1 );
    } 
    
    DataSet ds;
    ds.info() = "INFO_" + tarname( t );
    ds.ref()  = "REF";      
    
    int had = h;
    int tar = t;
    string str;
    int count = -1;
    
    while( getline( ifs, str ) ){
      
      if( atoi( str.substr( 0, 1 ).c_str() ) == tar &&
	  atoi( str.substr( 2, 2 ).c_str() ) == had ) count = 0;
      if( count > 0 ){

	istringstream iss( str );
	DataPoint A1h;
	

//*******select variable*************************************************

	double x ,xmax ,xmin, q2, q2max,q2min ,E ,sys ,mc;
	if( !( iss
      //xmean  >> x
      //xmax   >> xmax
      //xmin   >> xmin
      //q2mean >> q2 
      //q2max  >> q2max
      //q2min  >>q2min
      //E      >>E
	       >> A1h.data() 
	       >> A1h.stat()
      //sys    >> sys 
      //mc     >>mc
		 ) ) break;
      
	A1h.info() = "XYZ_" + tarname( t ) + "_" + hadname( h );
	A1h.unit() = "";
	
//xmean	A1h.kine( x, "x", "" );
//xmax	A1h.kine( xmax, "xmax", "" );
//xmin  A1h.kine( z\xmin, "xmin", "" );


//q2	A1h.kine( q2, "Q^2", "GeV^2" );
//q2max	A1h.kine( q2max, "Q^2 max", "GeV^2" );
//q2min	A1h.kine( q2min, "Q^2 min", "GeV^2" );

//E	A1h.kine( E, "E", "" );
//sys	A1h.syst( sys, "syst" );
//mc	A1h.syst( mc, "MC" );
	
	ds.push_back( A1h );

      }
      
      if( count != -1 ) count++;
    }

     return ds;  
  }

}

}

#endif //  _XYZ_hh_
