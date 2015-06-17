#ifndef _HERMES_SIDIS_hh_
#define _HERMES_SIDIS_hh_

#include "DataSet.hh"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>

using namespace std;

namespace Exp{

  namespace HERMES_SIDIS {

#ifdef DATA_DIR_PATH
    string top = DATA_DIR_PATH;
#else
    string top = "../Data/";
#endif
    
    
    DataSet A1h( const Exp::TARGET& t, const Exp::HADRON& h ) throw( int ) {
      
      ifstream ifs( string( top + "HERMES_2005_A1h.dat" ).c_str() );
      if( ! ifs ){
	cerr << "Fail to open data file!!" << endl;
	throw( 1 );
      } 
      
      DataSet ds;
      //      ds.info() = "HERMES,2005,A1h";
      ds.info() = "HERMES,2005,A1h_" + Exp::tarname( t ) + "_" + Exp::hadname( h );
      ds.ref()  = "Phys.Rev.D71(2005)012003";      
    
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
	  
	  double x, q2, z, sys, mc;
	  if( !( iss >> x >> q2 >> z >> A1h.data() 
		 >> A1h.stat() >> sys >> mc ) ) break;
	  
	  A1h.info() = "A1h_" + Exp::tarname( t ) + "_" + Exp::hadname( h );
	  A1h.unit() = "";
	  
	  A1h.kine( x, "x", "" );
	  A1h.kine( q2, "Q^2", "GeV^2" );
	  A1h.kine( z, "z", "" );
	  A1h.syst( sys, "syst" );
	  A1h.syst( mc, "MC" );
	  
	  ds.push_back( A1h );
	}
	
	if( count != -1 ) count++;
      }
      
      return ds;  
    }
    
    DataSet A1h_SMC( const Exp::TARGET& t, const Exp::HADRON& h ) throw( int ) {
      
      ifstream ifs( string( top + "SMC_1998_A1h.dat" ).c_str() );
      if( ! ifs ){
	cerr << "Fail to open data file!!" << endl;
	throw( 1 );
      } 
      
      DataSet ds;
      ds.info() = "SMC,1998,A1h_" + Exp::tarname( t ) + "_" + Exp::hadname( h );
      ds.ref()  = "Phys.Lett.B420(1998)180";      
      
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
	  
	  double x, q2, sys;
	  if( !( iss >> x >> q2 >> A1h.data() 
	       >> A1h.stat() >> sys ) ) break;
	  
	  A1h.info() = "A1h_" + Exp::tarname( t ) + "_" + Exp::hadname( h );
	  A1h.unit() = "";
	  
	  A1h.kine( x, "x", "" );
	  A1h.kine( q2, "Q^2", "GeV^2" );
	  A1h.syst( sys, "syst" );
	  
	  ds.push_back( A1h );
	}
	
	if( count != -1 ) count++;
      }
      
      return ds;  
    }
    
  }
  
}

#endif //  _HERMES_SIDIS_hh_
