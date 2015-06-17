#ifndef _FragXsec_hh_
#define _FragXsec_hh_

#include "DataSet.hh"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>

using namespace std;

namespace Exp{

namespace FragXsec {

  
  
  void Zconvert( double& z, const double& tz, 
		 const bool& xp, const double& q, 
		 const int& had );
  
  DataSet Xsec( const string& path, const Exp::HADRON& h ) throw( int ) {

    ifstream ifs( path.c_str() );
    if( ! ifs ){
      cerr << "Fail to open data file!!" << endl;
      throw( 1 );
    } 
    
    DataSet ds;
    
    int had = h;
    string str;
    int count = -1;
    bool xp = false;
    
    while( getline( ifs, str ) ){
      
      if( atoi( str.substr( 0, 2 ).c_str() ) == had ){
	count = 0;
	if( str.length() > 3 && str.substr( 3, 2 ) == "xp" ) xp = true;
      }
      if( count > 0 ){
	
	istringstream iss( str );
	DataPoint Xsec;
	
	double zmin, zmax, q, sys, tzmin, tzmax;
	if( !( iss >> tzmin >> tzmax >> q >> Xsec.data() 
	       >> Xsec.stat() >> sys ) ) break;
	
	Xsec.info() = "e+e-_" + Exp::hadname( h );
	Xsec.unit() = "";
	
	Zconvert( zmin, tzmin, xp, q, had );
	Zconvert( zmax, tzmax, xp, q, had );

	Xsec.kine( q * q, "Q^2", "GeV^2" );
	Xsec.kine( zmax, "zmax", "" );
	Xsec.kine( 0.5 * ( zmax + zmin ), "zmid", "" );
	Xsec.kine( zmin, "zmin", "" );
	Xsec.kine( zmax - zmin, "dz", "" );
	if( xp ){
	  Xsec.kine( tzmax, "zpmax", "" );
	  Xsec.kine( tzmin, "zpmin", "" );
	}
	Xsec.syst( sys, "syst" );
	
	ds.push_back( Xsec );
      }
      
      if( count != -1 ) count++;
    }

     return ds;  
  }


  void Zconvert( double& z, const double& tz, 
		 const bool& xp, const double& q, 
		 const int& had ){
    if( !xp ){ z = tz; return; }
    
    double Mh = ( had == 22 ? 0.139 : 
		  ( had == 32 ? 0.139 : 
		    ( had == 42 ? 0.493 : 
		      ( had == 52 ? 0.938 : 0 ) ) ) );
    double Q = q;
    double Ph = 0.5 * tz * Q;

    double Eh = sqrt( Mh * Mh + Ph * Ph );
    z = 2.0 * Eh / Q;
  }
    
}

}

#endif //  _FragXsec_hh_
