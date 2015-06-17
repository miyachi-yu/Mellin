#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cmath>

#include <Tranform/Romberg.hh>
#include <Utility/Arguments.hh>
#include <Utility/Timer.hh>
#include <Tranform/GridIntegration.hh>
#include <Evolution/Kernel.hh>
#include <Fragmentation/Evolution.hh>
#include <Xsec/XsecComp.hh>
#include <Xsec/xXsec.hh>

#include "FragXsec.hh"

using namespace std;
using namespace Utility;
using namespace Transform;

void path_check( const double& x, const double& Q2, Xsection::xXsec& xxs ){

  vector< double > vlength;  
  
  vlength.push_back( 20.0 );
  vlength.push_back( 19.0 );
  vlength.push_back( 18.0 );
  vlength.push_back( 17.0 );
  vlength.push_back( 16.0 );
  vlength.push_back( 15.0 );
  vlength.push_back( 14.0 );
  vlength.push_back( 13.0 );
  vlength.push_back( 12.0 );
  vlength.push_back( 11.0 );
  vlength.push_back( 10.0 );
  vlength.push_back(  9.0 );
  vlength.push_back(  8.0 );
  vlength.push_back(  7.0 );
  vlength.push_back(  6.0 );
  vlength.push_back(  5.0 );
  vlength.push_back(  4.0 );
  
  cout << "   Q2 = " << setw(14) << Q2 << endl;
  cout << "   x  = " << setw(14) << x  << endl;
  
  xxs.moment()->setQ2( Q2 );
  
  cout << setw(14) << "length"
       << setw(14) << "f(x)"
       << setw(14) << "ms"
       << setw(14) << "diff(%)"
       << endl;
  
  double ref = 0.0;
  
  for( vector< double >::iterator itr = vlength.begin();
       itr != vlength.end();
       itr++ ){

    double val;
    cout << setw(14) << *itr << flush;
    Utility::Timer timer;
    timer.start();
    xxs.upper( *itr );
    cout << setw(14) << ( val = xxs( x ) ) << flush;
    cout << setw(14) << timer.check( Utility::Timer::Msec ) << flush;
    
    if( itr == vlength.begin() ) ref = val;
    else cout << setw(14) << 100.0 * ( val - ref ) / ref << flush;

    cout << endl;
    
  }
  
}


void path_check( vector< double >& lq2, Xsection::xXsec& xxs ){

  Utility::Timer timer;
  vector< double > vx;

  vx.push_back( 0.01 );
  vx.push_back( 0.02 );
  vx.push_back( 0.03 );
  vx.push_back( 0.04 );
  vx.push_back( 0.05 );
  vx.push_back( 0.06 );
  vx.push_back( 0.07 );
  vx.push_back( 0.08 );
  vx.push_back( 0.09 );
  vx.push_back( 0.10 );
  vx.push_back( 0.20 );
  vx.push_back( 0.40 );
  vx.push_back( 0.50 );
  vx.push_back( 0.60 );
  vx.push_back( 0.70 );
  vx.push_back( 0.80 );

  cout << "# ------------------------------------------------------ #" << endl;
  cout << "#    Contour parameter study:                            #" << endl;
  cout << "# ------------------------------------------------------ #" << endl;

  for( vector< double >::iterator iQ2 = lq2.begin();
       iQ2 != lq2.end();
       iQ2++ ){

    for( vector< double >::iterator iX = vx.begin();
	 iX != vx.end();
	 iX++ ){
      
      path_check( *iX, *iQ2, xxs );
    }
    
  }
}


void dump( vector< double >& lq2, Xsection::xXsec& xxs ){
  
  cout << "# ------------------------------------------------------ #" << endl;
  cout << "#    calculation in x space" << endl;
  
  Utility::Timer timer;
  vector< double > vx;
  vx.push_back( 0.01 );
  vx.push_back( 0.02 );
  vx.push_back( 0.04 );
  vx.push_back( 0.06 );
  vx.push_back( 0.10 );
  vx.push_back( 0.20 );
  vx.push_back( 0.40 );
  vx.push_back( 0.60 );

  
  for( int i = 0; i < lq2.size(); i++ ){
    
    if( i == 0 ){
      cout << setw(8) << "Q2" << flush;
      for( int j = 0; j < vx.size(); j++ ) cout << setw(12) << vx[j] << flush;
      cout << endl;
    } 
    
    xxs.moment()->setQ2( lq2[ i ] );
    cout << setw(8) << lq2[ i ] << flush;
    timer.start();
    for( int j = 0; j < vx.size(); j++ )
      cout << setw(12) << xxs( vx[j] ) << flush;
    
    timer.stop();
    cout << setw( 12 ) << timer.check( Timer::Sec ) << " sec" << flush;
    
    Evolution::Kernel *k =
      dynamic_cast< Evolution::Kernel* >( xxs.moment()->xsecComp()->vevo()[0] );
    
    if( k ){
      cout << setw(6) << k->dglapMatrix().cache_size() << flush;
    }
    cout << endl;

  }
  cout << "# ------------------------------------------------------ #" << endl;
}


int main( int argc, char* argv[] ){
  
  Arguments& args = *( Arguments::instance( argc, argv ) );
  Timer timer;
  
  double q2     = args.get( "Q2", 1.0 );
  complex< double > a( args.get( "N", 2.0 ), 0.0 );

  double offset = args.get( "offset", 3.0  );
  double angle  = args.get( "angle",  0.55 );
  double length = args.get( "length", 8.0  );
  cout
    << "-- Mellin Inversion parameters ( c, phi, lenght ):"
    << setw( 12 ) << offset
    << setw( 12 ) << angle
    << setw( 12 ) << length
    << endl;
  
  bool verbose = args.hasOpt( "verbose" );
  
  int leg1 = args.get( "leg1", 8 );
  int leg2 = args.get( "leg2", 10 );
  cout
    << "-- Gauss-Legendre Integration " 
    << leg1 << " vs " << leg2 << endl;
  
  double precision = args.get( "precision", 1.0E-3 );
  cout
    << "-- Integration precision: " << setw( 8 ) << precision << endl;
  
  
  vector< double > lq2;
  
  //  lq2.push_back(    1.00 );
  //  lq2.push_back(    2.50 );
  //  lq2.push_back(   10.00 );
  lq2.push_back(   50.00 );
  lq2.push_back(  100.00 );
  lq2.push_back(  841.00 );
  lq2.push_back( 1156.00 );
  lq2.push_back( 1936.00 );
  lq2.push_back( 8317.44 );

  vector< string > optPi(0);
  optPi.push_back( "FF" );
  optPi.push_back( "KretzerFF" );
  optPi.push_back( "DSSFF" );
  
  vector< string > optKa(0);
  optPi.push_back( "FFK" );
  optPi.push_back( "KretzerFFK" );
  optPi.push_back( "DSSFFK" );
  
  QCD::Flavor::HADRON h = QCD::Flavor::PIp;
  vector< string >::iterator itr = args.hasOpt( optPi );
  if( itr == optPi.end() ){
    if( ( itr = args.hasOpt( optKa ) ) == optKa.end() ){
      return 1;
    } else {
      h = QCD::Flavor::KAp;
    }
  }

  try{
    
    FragXsec_T::FragXsec xsa( args, h );
    
    Fragmentation::Evo evo( args, h, *itr );
    FragXsec_T::FragXsec xsb( &evo, h );
    
    
    if( 0 ) {
      Xsection::xXsec xxsa( &xsa, leg1, leg2, precision ); 
      xxsa.setParameter( offset, angle );
      xxsa.upper( length );
      
      GridIntegration* grid = 
	dynamic_cast< GridIntegration* >( xxsa.integration() );
      if( grid ) grid->ngrid( args.get( "ngrid", 2 ) );
      
      if( 0 ) dump( lq2, xxsa );
      if( 1 ) path_check( lq2, xxsa );
    }

    
    if( 1 ) {
      Xsection::xXsec xxsb( &xsb, leg1, leg2, precision  ); 
      xxsb.setParameter( offset, angle );
      xxsb.upper( length );
      dump( lq2, xxsb );
    }

    if( 0 ){
      Romberg romberg( args.get( "K", 12 ), args.get( "eps", 1.0E-6 ) );
      Xsection::xXsec xxsc( &xsa, &romberg ); 
      xxsc.setParameter( offset, angle );
      xxsc.upper( length );
      dump( lq2, xxsc );
    }
    
    /*
      a = complex< double >( -9.85,-1.95 );
    cout << "# calculation in Mellin space" << endl;
    cout << "# ( N, Q2 ) = ( " 
	 << setw(16) << a << "," 
	 << setw( 8) << q2 
	 << " )"
	 << endl;
    
    xsec.setQ2( q2 );
    
    cout << "cross section = " << setw(16) 
	 << xsec( a ) << endl;
    if( verbose ) cout << xs << endl;
    */
    
    
  }
  
  catch( int err ){
  }
  
}

