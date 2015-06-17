
#include "EMC_Multi.hh"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <vector>

using namespace std;

namespace Exp{
  
  namespace EMC_Multi {
    
    DataSet Multi( const Exp::TARGET& t, const Exp::HADRON& h ) throw( int ) {
      
      if( h != Exp::Hp && h != Hm ){
	cerr << "Hadron type must be Hp or Hm" << endl;
	throw( 1 );
      }
      
      if( t == Exp::NEU ){
	cerr << "target type must be PRO or DEU" << endl;
	throw( 2 );
      }
      
      string path = data_path + "EMC_Multiplicity_1991.dat";
      
      ifstream ifs( path.c_str() );
      if( ! ifs ){
	cerr << "Fail to open data file " << path << endl; 
	throw( 1 );
      } else {
	cout << "Loading " << path << endl;
      }
      
      DataSet ds;
      ds.info() = "EMC,1991,Multi_" + Exp::tarname( t );
      ds.ref()  = "Z.Phys.C52(1991)361";      
      ds.cite() = "Ashman:1991cj";
      
      string key = 
	string( h == Exp::Hp  ?  "h+" :"h-" ) + " " +
	string( t == Exp::PRO ? "proton" : "deuteron" );
      
      double scale = ( t == Exp::PRO ? 0.11 : 0.08 );
      ds.norm()   = scale;
      ds.ncerr()  = 1.0;
      ds.ncoef()  = 0.0;
      ds.ncstat() = "free";

      vector< double > zmin( 6, 0.0 );
      vector< double > zmax( 6, 0.0 );
      
      zmin[ 0 ] = 0.10; zmax[ 0 ] = 0.15;
      zmin[ 1 ] = 0.15; zmax[ 1 ] = 0.25;
      zmin[ 2 ] = 0.25; zmax[ 2 ] = 0.35;
      zmin[ 3 ] = 0.35; zmax[ 3 ] = 0.45;
      zmin[ 4 ] = 0.45; zmax[ 4 ] = 0.60;
      zmin[ 5 ] = 0.60; zmax[ 5 ] = 1.00;
      
      vector< double > z( 6, 0.0 );
      vector< double > dz( 6, 0.0 );
      
      int size = 1024;
      char buff[ size ];
      bool state = false;
      
      while( ifs.getline( buff, size ) ){
	
	string bufstr( buff );

	if( bufstr == key )    state = true;
	if( bufstr == "done" ) state = false;
	
	if( ! state ) continue;
	
	istringstream iss( buff );
	
	
	double x, q2;
	
	if( !( iss >> x >> q2 
	       >> z[ 0 ] >> dz[ 0 ]
	       >> z[ 1 ] >> dz[ 1 ]
	       >> z[ 2 ] >> dz[ 2 ]
	       >> z[ 3 ] >> dz[ 3 ]
	       >> z[ 4 ] >> dz[ 4 ]
	       >> z[ 5 ] >> dz[ 5 ]
	       ) ){
	  continue;
	}
	
	vector< DataPoint > dp( 6 );
	for( int i = 0; i < dp.size(); i++ ){

	  dp[ i ].kine(         x, "xmin",    "" );
	  dp[ i ].kine(         x, "xmax",    "" );
	  dp[ i ].kine(         x, "x",    "" );
	  
	  dp[ i ].kine(        q2, "Q^2 max",  "GeV^2" );
	  dp[ i ].kine(        q2, "Q^2 min",  "GeV^2" );
	  dp[ i ].kine(        q2, "Q^2 ave",  "GeV^2" );
	  dp[ i ].kine(        q2, "Q^2",  "GeV^2" );

	  dp[ i ].kine( zmin[ i ], "zmin", "" );
	  dp[ i ].kine( zmax[ i ], "zmax", "" );
	  dp[ i ].kine( 0.5 * ( zmax[ i ] + zmin[ i ] ), "zave", "" );
	  dp[ i ].kine( 0.5 * ( zmax[ i ] + zmin[ i ] ), "z", "" );
	  
	  dp[ i ].data( z[ i ], "Multi_" + Exp::tarname( t ) + "_" + Exp::hadname( h ), "" ); 
	  dp[ i ].stat() = dz[ i ]; 

	  dp[ i ].kine( 400.0, "s", "GeV^2" ); 

	  ds.push_back( dp[ i ] );
	}
	
	
      }
      
      return ds;  
    }
    
  }
  
}
