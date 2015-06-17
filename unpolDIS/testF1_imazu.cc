/*!
  \file  testG1.cc
  \brief test program for unpolarized structure functions

  This is a test and an example code for unpolarized structure functions
  implemented in libunpolDIS.so.

  You may get some feeling how to use unpolDIS::StructureFunction class.

 */
#include <Utility/Timer.hh>

#include <Evolution/Kernel.hh>
#include <unpolPDF/Evolution.hh>
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

  double offset; double angle; double length;
  if( !args.hasOpt( "CTEQ6" ) ){
  offset = args.get( "offset", 2.0  );  
  angle  = args.get( "angle",  0.55  );  
  length = args.get( "length", 15.0 );  
  } else {
  offset = args.get( "offset", 2.0  );  
  angle  = args.get( "angle",  0.50 );  
  length = args.get( "length", 20.0 );  
  }

  try {
    
    Timer timer;
    
    // --------------------------------------------------------------
    //                       new implementation
    // --------------------------------------------------------------
    unpolDIS::StrFunc f1( args, Xsection::Charge::p );
    dynamic_cast< Xsection::CKernel* >( f1.xsecComp()->coeff() )
      ->constNf( false );
// if( args.hasOpt( "CTEQ6" ) ) f1.xsecComp()->coeff()->alpha().atMz( 0.118 );
    if( !args.hasOpt( "CTEQ6" ) )
      dynamic_cast< unpolPDF::Evo* >
	( f1.xsecComp()->vevo()[0] )->constNf( false );

    Xsection::xXsec f1x( &f1, 4, 5, 1.0E-4 );            // MellinInversion
    f1x.setParameter( offset, angle );
    f1x.upper( length );
    
    if( 0 ) {
      f1.setQ2( 2.5 );
      // at first, let's calculate one point
      f1x( 0.002 );
      // next, try to dump cached infromation
      Evolution::Kernel *k =
	dynamic_cast< Evolution::Kernel* >( f1.xsecComp()->vevo()[0] );
      k->dglapMatrix().cache_dump( cout );

    } else {
      
      for( int iq2 = 0; iq2 < q2.size(); iq2++ ){
	
	if( iq2 == 0 ){
	  cout << setw(4)  << "Q^2" << setw(18) << "f_1(N=2)" << flush;
	  for( int i = 0; i < x.size(); i++ ) 
	    cout << setw( 13 ) << x[ i ] << flush;
	  cout << endl;
	} 
	
	
	// --------------------------------------------------------- //
	//    performance in x space :  new implementation
	// --------------------------------------------------------- //
	{
	  
	  f1.setQ2( q2[ iq2 ] );
	  double n = 2.0;
	  //	for( double n = 1.0; n < 10.0; n+=0.1 ){
	  cout << setw(4) << q2[ iq2 ]
	       << setw(18) << f1( complex< double >( n, 0.0 ) ) << flush;
	  //	}
	  
	  
	  timer.start();
	  for( int i = 0; i < x.size(); i++ ){
	    cout << setw( 13 ) <<  x[ i ] * f1x( x[ i ] ) / 2.0 << flush;
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
