/*!
  \file  testG1.cc
  \brief test program for polarized structure functions

  This is a test and an example code for polarized structure functions
  implemented in libpolDIS.so.

  You may get some feeling how to use polDIS::StrFunc class.

 */
#include <Utility/Timer.hh>

#include <Evolution/Kernel.hh>
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

int main( int argc, char* argv[] ) {
  
  Arguments& args = *( Arguments::instance( argc, argv ) );

  vector< double > q2(0);
  q2.push_back(   1.0 );
  q2.push_back(   2.0 );
  q2.push_back(   5.0 );
  q2.push_back(  10.0 );
  q2.push_back(  20.0 );
  q2.push_back(  50.0 );
  q2.push_back( 200.0 );
  q2.push_back( 500.0 );
  
  int nx = 6;
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

    Xsection::xXsec g1x( &g1, 4, 5, 1.0E-4 );            // MellinInversion
    g1x.setParameter( offset, angle );
    g1x.upper( length );
    
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
	  
	  
	  timer.start();
	  for( int i = 0; i < x.size(); i++ ){
	    cout << setw( 13 ) <<  x[ i ] * g1x( x[ i ] ) / 2.0 << flush;
	  }
	  timer.stop();
	  cout << setw( 13 ) << timer.check( Timer::Sec ) << " sec" << flush;
	  
	}
	cout << endl;
      }
    }
  }
  
  catch ( int error ) {
  }

  return 0;
}
