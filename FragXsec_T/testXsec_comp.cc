#include <Utility/Timer.hh>

#include <Evolution/Kernel.hh>
#include <Fragmentation/Evolution.hh>
#include <Xsec/xXsec.hh>

#include <iostream>
#include <iomanip>
#include <vector>

#include "FragXsec.hh"

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

  //  int nx = 6;
  int nx = 8;
  double aa = 2.0;
  vector< double > x( 0 );
  for( double lx = - aa; lx < 0.0; lx += aa / nx )
    x.push_back( pow( 10.0, lx ) );
  
  double offset = args.get( "offset", 2.0  );  
  double angle  = args.get( "angle",  0.55  );  
  double length = args.get( "length", 15.0 );  
  
  try {
    
    Timer timer;
    
    // --------------------------------------------------------------
    //                       new implementation
    // --------------------------------------------------------------

    FragXsec_T::FragXsec xsa( args, QCD::Flavor::HADRON( 1 ) );
    Xsection::xXsec xxsa( &xsa, 4, 6, 1.0E-4, 10.0, 1.8, 0.55 ); 

    // cs -> PIcs = 2 * cs 
    
    if( 0 ) {
    } else {
      
      for( int iq2 = 0; iq2 < q2.size(); iq2++ ){
	
	if( iq2 == 0 ){
	  cout << setw(4)  << "Q^2" << setw(18) << "cs_(N=2)" << flush;
	  for( int i = 0; i < x.size(); i++ ) 
	    cout << setw( 13 ) << x[ i ] << flush;
	  cout << endl;
	} 
	
	// --------------------------------------------------------- //
	//    performance in x space :  new implementation
	// --------------------------------------------------------- //
	{
	  
	  xsa.setQ2( q2[ iq2 ] );
	  double n = 2.0;
	  //	for( double n = 1.0; n < 10.0; n+=0.1 ){
	  cout << setw(4) << q2[ iq2 ]
	       << setw(18) << xsa( complex< double >( n, 0.0 ) ) << flush;
	  //	}
	  
	  timer.start();
	  for( int i = 0; i < x.size(); i++ ){
	    cout << setw( 13 ) <<  2.0 * xxsa( x[ i ] ) << flush;
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
