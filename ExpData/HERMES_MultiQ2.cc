#include "HERMES_MultiQ2.hh"

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>

#include <Utility/Arguments.hh>

using namespace std;

namespace Exp{
  
  namespace HERMES_MultiQ2 {
    
    DataSet Multi( const Exp::TARGET& t, const Exp::HADRON& h ) throw( int ) {
      
      Utility::Arguments& args = Utility::Arguments::ref();
      
      string path = data_path + "HERMES_Multiplicity_vs_Q2.dat";
      
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
      
      int had = h;    int tar = t;    string str;
      int count = -1;
      
      double rzmax = args.get( "zmax", 0.9 );
      double rzmin = args.get( "zmin", 0.01 );
      double Q2min = args.get( "Q2min", 0.1 );
      
      int skip = args.get( "skip", 1 );
      
      while( getline( ifs, str ) ){
	
	if( atoi( str.substr( 0, 1 ).c_str() ) == tar &&
	    atoi( str.substr( 2, 2 ).c_str() ) == had ) count = 0;
	if( count > 0 ){
	  
	  istringstream iss( str );
	  DataPoint mul;
	  
	  double zmin, zmax, q2min, q2max, mult , RICHu , RICHl ,mc;
	  
	  if( !( iss >> zmin >> zmax >> q2min >> q2max 
		 >> mul.data() >> mul.stat() >> RICHu >> RICHl >> mc ) ) break;
	  
	  if( zmax > rzmax || zmin < rzmin ) continue;
	  if( q2max < Q2min ) continue;
	  if( count % skip != 0 ){ count++; continue; }
	  
	  vector< double > xx_ = getx( 0.5 * ( q2min + q2max ) );
	  vector< double > kine_ = getkinev( had, zmin, zmax, q2min, q2max );
	  
	  mul.info() = "MultiQ2_" + tarname( t ) + "_" + hadname( h );
	  mul.unit() = "";
	  
	  mul.kine( zmin, "zmin", "" );
	  mul.kine( zmax, "zmax", "" );
	  mul.kine( kine_[ 0 ], "zave", "" );
	  
	  mul.kine( xx_[ 1 ], "xmin", "" );
	  mul.kine( xx_[ 0 ], "xmax", "" );
	  mul.kine( kine_[ 1 ], "xave", "" );
	  
	  mul.kine( q2min, "Q^2 min", "GeV^2" );
	  mul.kine( q2max, "Q^2 max", "GeV^2" );
	  mul.kine( kine_[ 2 ], "Q^2 ave", "GeV^2" );

	  mul.kine( 56.2, "s", "GeV^2" ); 
	  
	  mul.syst( RICHu , "RICH upper" );
	  mul.syst( RICHl, "RICH lower");
	  mul.syst( mc, "MC" );
	  
	  ds.push_back( mul );
	}
	
	if( count != -1 ) count++;
      }
      
      return ds;  
    }
    
    vector< double > getkinev( const int& had, const double& zmin,
			       const double& zmax,
			       const double& q2min,
			       const double& q2max, const bool& zMulti ){
      
      vector< double > tmp_( 0 );

      string path = data_path + "HERMES_Multiplicity_Achim_ref.dat";
      ifstream ifs( path.c_str() );
      if( ! ifs ){
	cerr << "Fail to open data file " << path << endl;
	throw( 1 );
      } else {
	cout << "Loading " << path << endl;
      }
      
      int count = -1; string str;
      
      while( getline( ifs, str ) ){
	
	if( atoi( str.substr( 0, 2 ).c_str() ) == had ) count = 0;
	if( count > 0 ){
	
	  istringstream iss( str );
	  
	  double bin, mult, stat, zave, Q2ave, xave;
	  
	  if( !( iss >> bin >> mult >> stat >> zave >> Q2ave >> xave ) ) break;
	  
	  if( zMulti || zave > zmin && zave < zmax )
	    if( Q2ave > q2min && Q2ave < q2max ){
	      tmp_.push_back(  zave ); // 0 -> z
	      tmp_.push_back(  xave ); // 1 -> x
	      tmp_.push_back( Q2ave ); // 2 -> q2
	      break;
	    }
	  
	}
	if( count != -1 ) count++;
      }
      
      return tmp_;
    }
    
    vector< double > getx( const double& q2 ){
      
      vector< double > tmpv( 0 );
      
      map< double, map< double, double > > tmp;
      tmp[ 0.023 ][ 0.040 ] = 1.21;
      tmp[ 0.040 ][ 0.055 ] = 1.44;
      tmp[ 0.055 ][ 0.075 ] = 1.72;
      tmp[ 0.075 ][ 0.100 ] = 2.06;
      tmp[ 0.100 ][ 0.140 ] = 2.58;
      tmp[ 0.140 ][ 0.200 ] = 3.52;
      tmp[ 0.200 ][ 0.300 ] = 5.03;
      tmp[ 0.300 ][ 0.400 ] = 7.09;
      tmp[ 0.400 ][ 0.600 ] = 9.76;
      
      for( map< double, map< double, double > >::iterator itr = tmp.begin();
	   itr != tmp.end(); itr++ ) 
	for( map< double, double >::iterator itr2 = itr->second.begin();
	     itr2 != itr->second.end(); itr2++ ) 
	  if( itr2->second > q2 ){
	    --itr;
	    tmpv.push_back( itr->second.begin()->first ); // max
	    tmpv.push_back( itr->first );  // min
	    return tmpv;
	  }
      
      tmpv.push_back( 0.600 ); // max
      tmpv.push_back( 0.400 ); // min
      return tmpv;
    }
    
  }
  
}
