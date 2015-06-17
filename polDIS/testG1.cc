/*!
  \file  testG1.cc
  \brief test program for polarized structure functions

  This is a test and an example code for polarized structure functions
  implemented in libpolDIS.so.

  You may get some feeling how to use polDIS::StrFunc class.

 */
#include <Utility/Timer.hh>

#include <Evolution/Kernel.hh>
#include <Tranform/PolyInt.hh>

#include <polPDF/Evolution.hh>
#include <Xsec/XsecComp.hh>
#include <Xsec/xXsec.hh>

#include <iostream>
#include <iomanip>
#include <vector>

#include "StrFunc.hh"
#include "CoeffFunc.hh"

using namespace Utility;
using namespace std;

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

  vx.push_back( 0.001 );
  vx.push_back( 0.002 );
  vx.push_back( 0.003 );
  vx.push_back( 0.004 );
  vx.push_back( 0.005 );
  vx.push_back( 0.006 );
  vx.push_back( 0.007 );
  vx.push_back( 0.008 );
  vx.push_back( 0.009 );

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

int main( int argc, char* argv[] ) {
  
  Arguments& args = *( Arguments::instance( argc, argv ) );

  vector< double > q2(0);
  q2.push_back(   1.0 );
  q2.push_back(   2.0 );
  q2.push_back(   5.0 );
  q2.push_back(  10.0 );
  q2.push_back(  20.0 );
  q2.push_back(  50.0 );

  //  q2.push_back( 200.0 );
  //  q2.push_back( 500.0 );
  
  int nx = args.get( "nx", 6 ) ;
  vector< double > x( 0 );
  for( double lx = - 3.0; lx < 0.0; lx += 3.0 / nx )
    x.push_back( pow( 10.0, lx ) );
  
  double offset = args.get( "offset", 2.0  );  
  double angle  = args.get( "angle",  0.55  );  
  double length = args.get( "length", 15.0 );  
  
  try {
    
    Timer timer;
    
    // --------------------------------------------------------------
    //                       new implementation
    // --------------------------------------------------------------
    polDIS::StrFunc g1( args, Xsection::Charge::p );
    dynamic_cast< Xsection::CKernel* >( g1.xsecComp()->coeff() )
      ->constNf( false );
    dynamic_cast< polPDF::Evo* >
      ( g1.xsecComp()->vevo()[0] )->constNf( false );

    Xsection::xXsec g1x( &g1, 4, 5, 1.0E-3 );            // MellinInversion
    g1x.setParameter( offset, angle );
    g1x.upper( length );

    if( 1 ) path_check( q2, g1x );

    
    if( 0 ) {
      g1.setQ2( 2.5 );
      // at first, let's calculate one point
      g1x( 0.002 );
      // next, try to dump cached infromation
      Evolution::Kernel *k =
	dynamic_cast< Evolution::Kernel* >( g1.xsecComp()->vevo()[0] );
      k->dglapMatrix().cache_dump( cout );
      
    } else {
      
      for( int iq2 = 0; iq2 < q2.size(); iq2++ ){
	
	if( iq2 == 0 ){
	  cout << setw(4)  << "Q^2" << setw(18) << "g_1(N=1)" << flush;
	  for( int i = 0; i < x.size(); i++ ) 
	    cout << setw( 13 ) << x[ i ] << flush;
	  cout << endl;
	} 
	
	
	// --------------------------------------------------------- //
	//    performance in x space :  new implementation
	// --------------------------------------------------------- //
	{
	  
	  g1.setQ2( q2[ iq2 ] );
	  double n = 1.0;
	  //	for( double n = 1.0; n < 10.0; n+=0.1 ){
	  cout << setw(4) << q2[ iq2 ]
	       << setw(18) << g1( complex< double >( n, 0.0 ) ) << flush;
	  //	}
	  
	  Transform::PolyInt pi;
	  
	  timer.start();
	  for( int i = 0; i < x.size(); i++ ){
	    pi[ x[ i ] ] = 0.5 * g1x( x[ i ] );
	    cout << setw( 13 ) <<  x[ i ] * pi[ x[ i ] ] << flush;
	  }
	  timer.stop();
	  cout << setw( 13 ) << timer.check( Timer::Sec ) << " sec" << flush;	  

	  cout << endl;
	  cout << " *** check interpolation " << endl;
	  cout << setw(16) << "x"
	       << setw(16) << "polyint"
	       << setw(16) << "original"
	       << endl;
	  
	  for( double lx = - 3.0; lx < 0.0; lx += 3.0 / nx / 5.0 ){
	    double xx = pow( 10.0, lx );
	    if( xx > x[ x.size() - 1 ] ) continue;
	    double inter = pi( xx ) ; 
	    double orig  = 0.5 * g1x( xx );
	    cout << setw(16) << xx
		 << setw(16) << inter
		 << setw(16) << orig
		 << setw(16) << 100.0 * ( inter - orig ) / orig << "%"
		 << endl;
	  }
	  
	}
	cout << endl;
      }
    }
  }
  
  catch ( int error ) {
  }

  return 0;
}
