#include <iostream>
#include <iomanip>
#include <vector>
#include <map>
#include <fstream>

#include <Utility/Arguments.hh>
#include <Utility/Timer.hh>
#include <Tranform/GridIntegration.hh>
#include <Tranform/LegQuadrature.hh>
#include <Tranform/LaguerreQuadrature.hh>
#include <Tranform/InverseMellin.hh>

#include "FragXsec.hh"

using namespace std;

map< double, vector< double > > cmap;
vector< double > lq2(0);
vector< double > lz(0);

void loadcmap( const string& fname ){
  ifstream ifs( fname.c_str() );
  if( ! ifs ) return;
  
  double z, c0, c1, c2;
  cout << endl;
  
  cout 
    << setw(11) << "z"
    << setw(11) << "c0"
    << setw(11) << "c1"
    << setw(11) << "c2"
    << endl
    << endl;
  
  while( ifs >> z >> c0 >> c1 >> c2 ){
    
    cout << setw(11) << z
	 << setw(11) << c0
	 << setw(11) << c1
	 << setw(11) << c2
	 << endl;
    
    if( cmap.find( z ) == cmap.end() ) cmap[ z ] = vector< double >( 3, 0.0 );
    cmap[ z ][ 0 ] = c0;
    cmap[ z ][ 1 ] = c1;
    cmap[ z ][ 2 ] = c2;
    
  }
  
  ifs.close();
  cout << endl;
}

void savecmap( const string& fname ){
  ofstream ofs( fname.c_str() );
  
  map< double, vector< double > >::iterator itr;
  for( itr = cmap.begin(); itr != cmap.end(); itr++ ){
    
    ofs
      << setw(11) << itr->first
      << setw(11) << itr->second[0]
      << setw(11) << itr->second[1]
      << setw(11) << itr->second[2]
      << endl;
    
  }
  ofs.close();
}

void dump( Transform::InverseMellin *f, const double& z ){

  if( f == NULL ) {
    cout << setw(11) << z << flush;
    return;
  } 
  
  if( f->contour() == Transform::InverseMellin::STEEPEST ){
    
    if( cmap.find( z ) == cmap.end() ){
      f->F().x( z );
      cmap[ z ] = vector< double >( 3, 0.0 );
      cmap[ z ][ 0 ] = f->F().c0();
      cmap[ z ][ 1 ] = f->F().c1( cmap[ z ][ 0 ] );
      cmap[ z ][ 2 ] = f->F().c2( cmap[ z ][ 0 ] );
    }
    
    f->steepestDescent( cmap[ z ][ 0 ], cmap[ z ][ 1 ], cmap[ z ][ 2 ] );
  }
  
  cout << setw(11) << (*f)( z ) << flush;
}

void dump( Transform::InverseMellin *f = NULL, const string& title = "" ) {
  
  cout << setw(11) << title << flush;
  Utility::Timer timer;
  timer.start();
  for( int i = 0; i < lz.size(); i++ ) dump( f, lz[ i ] );
  timer.stop();
  if( f != NULL ){
    cout << setw(11) << timer.check( Utility::Timer::Msec ) / lz.size()
	 << " ms/p" << flush;
  }
  
  cout << endl;
  
}

void contour( const double& z, Xsection::Xsec *xsec, Transform::InverseMellin *f ){

  for( int i = 0; i < lq2.size(); i++ ){
    xsec->setQ2( lq2[ i ] );
    f->F().x( z );
    double c0 = f->F().c0();
    double c1 = f->F().c1( c0 );
    double c2 = f->F().c2( c0 );
    
    f->steepestDescent( c0, c1, c2 );
    
    cout 
      << setw( 6 ) << z
      << setw( 12 ) << lq2[ i ]
      << setw( 14 ) << c0
      << setw( 14 ) << c1
      << setw( 14 ) << c2
      << setw( 14 ) << (*f)( z )
      << endl;
    
  }

}

int main( int argc, char* argv[] ){
  
  Utility::Arguments& args = Utility::Arguments::ref( argc, argv );
  
  cout 
    << "+---------------------------------------------------------------------+\n"
    << "|                                                                     |\n"
    << "|           Test of Steepest Descent Contour with FragXsec_T          |\n"
    << "|                                                                     |\n"
    << "+---------------------------------------------------------------------+\n"
    << endl;
  
  cout 
    << "command name: " << setw(11) << argv[0] << endl;
  
  cout 
    << "\t--FF        " << flush;
  if( ! args.hasOpt( "FF" ) ){
    cout  << "You must specify FF configuration file" << endl;
    return 1;
  }
  cout << args.getOpt( "FF" ) << endl;
  
  int n = args.get( "n", 5 );
  cout 
    << "\t--n         " 
    << "Use " << n << "-th Laguerre Quadrature." << endl;
  
  
  string cmapfile = args.get( "cmap", "cmap.dat" );
  cout
    << "\t--cmap      "
    << "Contour info IO ( " << cmapfile << " )"<< endl;
  
  
  double q2 = args.get( "Q2", 100.0 );
  cout 
    << "\t--Q2        " 
    << "Contour initialization scale " << q2 << " GeV^2"
    << endl;
  lq2.push_back( q2 );
  
  loadcmap( cmapfile );

  lz.push_back( 0.01 );
  lz.push_back( 0.02 );
  lz.push_back( 0.05 );
  lz.push_back( 0.07 );
  lz.push_back( 0.1 );
  lz.push_back( 0.2 );
  lz.push_back( 0.23 );
  lz.push_back( 0.351 );
  lz.push_back( 0.3605 );
  lz.push_back( 0.384 );
  lz.push_back( 0.4404 );
  lz.push_back( 0.45 );
  lz.push_back( 0.5 );
  lz.push_back( 0.6 );
  lz.push_back( 0.526 );
  lz.push_back( 0.7 );
  
  //  lq2.push_back(    2.50 );
  //  lq2.push_back(   10.00 );
  //  lq2.push_back(   50.00 );
  lq2.push_back(  100.00 );
  lq2.push_back(  200.00 );
  lq2.push_back(  300.00 );
  lq2.push_back(  400.00 );
  lq2.push_back(  500.00 );
  lq2.push_back(  600.00 );
  lq2.push_back(  700.00 );
  lq2.push_back(  800.00 );
  lq2.push_back(  900.00 );
  lq2.push_back(  841.00 );
  //  lq2.push_back( 1156.00 );
  //  lq2.push_back( 1936.00 );
  //  lq2.push_back( 8317.44 );
  
  try{
    
    FragXsec_T::FragXsec xsec( args );
    
    Transform::LegQuadrature leg1( 6 );
    Transform::LegQuadrature leg2( 8 );
    Transform::GridIntegration grid( &leg1, &leg2, 4, 5.0E-3 );
    
    Transform::LaguerreQuadrature lag( n, -0.5 );
    
    Transform::InverseMellin inv1( &xsec, &grid );
    inv1.setParameter( 3.0, 0.55 );
    inv1.upper( 8.0 );
    
    Transform::InverseMellin inv2( &xsec, &lag );


    if( args.get( "order", "NLO" ) == "LO" ){
      cout 
	<< endl
	<< "*****************************   contour    *******************************"
	<< endl
	<< endl;
      
      for( int i = 0; i < lz.size(); i++ )
	contour( lz[ i ] , &xsec, &inv2 );


      cout << endl;

    }

    {
      cout 
	<< endl
	<< "*******************************   start   ********************************"
	<< endl;
      
      for( int i=0; i < lq2.size(); i++ ){
	xsec.setQ2( lq2[i] );
	cout << "  *** Q^2 = " << setw(8) << lq2[i] << " GeV^2" << endl;
	
	dump( NULL,  "z" );
	dump( &inv1, "STRAIGHT" );
	dump( &inv2, "STEEPEST" );
	
	cout << endl;
      } 
      
      cout 
	<< "*******************************    end    ********************************"
	<< endl;
    }
    
    {
      cout 
	<< endl
	<< "   --- parameters for steepest descent contour: obtained at Q^2 = "
	<< lq2[0] << " GeV^2"
	<< endl;
      
      cout 
	<< setw(11) << "z"
	<< setw(11) << "c0"
	<< setw(11) << "c1"
	<< setw(11) << "c2"
	<< endl
	<< endl;
      
      map< double, vector< double > >::iterator itr;
      for( itr = cmap.begin(); itr != cmap.end(); itr++ ){
	
	cout 
	  << setw(11) << itr->first
	  << setw(11) << itr->second[0]
	  << setw(11) << itr->second[1]
	  << setw(11) << itr->second[2]
	  << endl;
	
      }
      
      cout 
	<< endl;
    }

    savecmap( cmapfile );
    
  }
  
  catch( int e ) {
  }
  
  return 0;
};
