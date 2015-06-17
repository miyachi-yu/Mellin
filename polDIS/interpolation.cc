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
#include <map>
#include <sstream>

#include "StrFunc.hh"
#include "CoeffFunc.hh"

using namespace Utility;
using namespace std;

int main( int argc, char* argv[] ) {
  
  Arguments& args = *( Arguments::instance( argc, argv ) );
  
  // -------------------------------------------------------------- //
  //                         Prepare Q2 values                      //
  // -------------------------------------------------------------- //
  vector< double > q2(0);
  istringstream istr( args.get( "Q2", "2.5 5.0 10.0" ) );
  double q2value;
  while( istr >> q2value ) q2.push_back( q2value );
  
  // -------------------------------------------------------------- //
  //                         Prepare x values                       //
  // -------------------------------------------------------------- //
  double lxmin = args.get( "lxmin", -3.0 );
  double lxmax = args.get( "lxmax",  0.0 );
  int nx       = args.get( "nx", 10 ) ;
  double dlx   = ( lxmax - lxmin ) / nx;
  vector< double > x( 0 );
  for( double lx = lxmin; lx < lxmax; lx += dlx ) 
    x.push_back( pow( 10.0, lx ) );
  
  // -------------------------------------------------------------- //
  //                  Mellin Inversion configuration                //
  // -------------------------------------------------------------- //
  double offset    = args.get( "offset", 2.0  );  
  double angle     = args.get( "angle",  0.55  );  
  double length    = args.get( "length", 15.0 );  
  int    leg1      = args.get( "leg1", 6 );
  int    leg2      = args.get( "leg2", 8 );
  double precision = args.get( "precision", 5.0E-3 ); 
  
  // -------------------------------------------------------------- //
  //                 Polynominal Interpolation at Q2                //
  // -------------------------------------------------------------- //
  map< double, Transform::PolyInt > poly;
  
  try {
    
    Timer timer;                                         // prepare timer 
    
    // -------------------------------------------------------------- //
    //               Structure function construction                  //
    // -------------------------------------------------------------- //
    polDIS::StrFunc g1( args, Xsection::Charge::p );
    
    // -------------------------------------------------------------- //
    //                         Mellin inversion                       //
    // -------------------------------------------------------------- //
    Xsection::xXsec g1x( &g1, leg1, leg2, precision );
    g1x.setParameter( offset, angle );
    g1x.upper( length );
    
    
    // --------------------------------------------------------- //
    //                loop over every q2 value                   //
    // --------------------------------------------------------- //
    cout << "**************************************************************"
	 << endl;
    cout << setw(4)  << "Q^2" << setw(18) << "g_1(N=1)" << flush;
    for( int i = 0; i < x.size(); i++ ) cout << setw( 13 ) << x[ i ] << flush;
    cout << endl;
    
    for( int iq2 = 0; iq2 < q2.size(); iq2++ ){

      double& Q2 = q2[ iq2 ];
      g1.setQ2( Q2 );
      
      // --------------------------------------------------------- //
      //                add points to PolyInt object               //
      // --------------------------------------------------------- //
      {
	cout << setw(4) << Q2 << setw(18) << "g_1(x)" << flush;
	timer.start();
	for( int i = 0; i < x.size(); i++ ){
	  poly[ Q2 ][ x[ i ] ] = 0.5 * g1x( x[ i ] );
	  cout << setw( 13 ) <<  x[ i ] * poly[ Q2 ][ x[ i ] ] << flush;
	}
	timer.stop();
	cout << setw( 13 ) << timer.check( Timer::Sec ) << " sec" << flush;
	cout << endl;
      }
      
      // --------------------------------------------------------- //
      //                   check interpolation                     //
      // --------------------------------------------------------- //
      {
	cout << "--------------------------------------------------------------"
	     << endl;
	cout << " check interpolation :" << endl;
	cout << setw(16) << "x"
	     << setw(16) << "polyint"
	     << setw(16) << "original"
	     << setw(16) << "diff(%)"
	     << endl;
	
	for( double lx = lxmin; lx < lxmax; lx += 0.2 * dlx  ){
	  double xx    = pow( 10.0, lx );
	  double inter = poly[ Q2 ]( xx ) ; 
	  double orig  = 0.5 * g1x( xx );
	  cout << setw(16) << xx
	       << setw(16) << inter
	       << setw(16) << orig
	       << setw(16) << 100.0 * ( inter - orig ) / orig
	       << endl;
	}
	
      }
      cout << endl;
    }
  }
  
  catch ( int error ) {
  }

  return 0;
}
