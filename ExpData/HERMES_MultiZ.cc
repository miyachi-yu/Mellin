#include "HERMES_MultiZ.hh"
#include "HERMES_MultiQ2.hh"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>

using namespace std;

namespace Exp{
  
  namespace HERMES_MultiZ {

    DataSet Multi( const Exp::TARGET& t, const Exp::HADRON& h ) throw( int ) {
      
      string path = data_path + "HERMES_Multiplicity_vs_z.dat";
    
      ifstream ifs( path.c_str() );
      if( ! ifs ){
	cerr << "Fail to open data file " << path << endl;
	throw( 1 );
      } else {
	cout << "Loading " << path << endl;
      }
      
      DataSet ds;
      ds.info() = "HERMES_Multi_" + tarname( t ) + "_" + hadname( h );
      ds.ref()  = "HERMES 2005 phD thisis Achim";      
      
      int had = h;
      int tar = t;
      string str;
      int count = -1;
      
      while( getline( ifs, str ) ){
	
	if( atoi( str.substr( 0, 1 ).c_str() ) == tar &&
	    atoi( str.substr( 2, 2 ).c_str() ) == had ) count = 0;
	
	if( count > 0  ){

	  istringstream iss( str );
	  DataPoint mul;
	  
	  double zmin, zmax, mult, RICHu, RICHl, mc, evou, evol; 
	  
	  if( !( iss >> zmin >> zmax >>  mul.data() >> mul.stat() 
		 >> RICHu >> RICHl >> mc >> evou >> evol ) ) 
	    break;
	  
	  mul.info() = "Multi_Z_" + tarname( t ) + "_" + hadname( h );
	  mul.unit() = "";
	  
	  //achim-paper
	  vector< double > xx_ = HERMES_MultiQ2::getx( 2.6 ); 
	  // set q2 at 3.6 to obtain 2.58 data
	  //achim-data
	  vector< double > kine_ 
	    = HERMES_MultiQ2::getkinev( had, 1.0, 1.0, 2.4, 2.6, true );
	  
	  mul.kine( zmin, "zmin", "" );
	  mul.kine( zmax, "zmax", "" );
	  mul.kine( 0.5 * ( zmax + zmin ), "zave", "" );
	  mul.kine( 2.5, "Q^2 ave", "GeV^2" );
	  mul.kine( 2.5, "Q^2 min", "GeV^2" );
	  mul.kine( 2.5, "Q^2 max", "GeV^2" );
	  mul.kine( xx_[ 1 ], "xmin", "" );
	  mul.kine( xx_[ 0 ], "xmax", "" );
	  mul.kine( kine_[ 1 ], "xave", "" );
	  mul.syst( RICHu , "RICH upper" );
	  mul.syst( RICHl, "RICH lower");
	  mul.syst( mc, "MC" );
	  mul.syst( evou , "evo upper" );
	  mul.syst( evol, "evo lower");
	  
	  mul.kine( 56.2, "s", "GeV^2" ); 

	  ds.push_back( mul );
	}
	
	if( count != -1 ) count++;
      }
      
      return ds;  
    }
    
  }
}
