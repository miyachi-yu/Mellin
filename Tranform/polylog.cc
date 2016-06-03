#include "CPolyLog.hh"
#include "CZeta.hh"
#include <cmath>
#include <iostream>
#include <iomanip>
#include <cstdlib>

using namespace std;
using namespace Transform;

double f2( const double& x ){
  return x * ( x + 1.0 ) / pow( 1.0 - x, 3 );
}


int main( int argc, char* argv[] ){
  
  CZeta zeta;
  CPolyLog plog;
  
  double l2 = log( 2.0 );
  
  plog.set( 1.0 );
  
  cout << setw(16) << "Li(1.0, 0.5)"
       << setw(16) << plog( 0.5 )
       << setw(12) << l2
       << endl;
  
  plog.set( 2.0 );
  cout << setw(16) << "Li(2.0, 0.5)"
       << setw(16) << plog( 0.5 )
       << setw(12) << ( M_PI*M_PI - 6.0 * l2 * l2 )/12.0
       << endl;
  
  plog.set( 3.0 );
  cout << setw(16) << "Li(3.0, 0.5)"
       << setw(16) << plog( 0.5 )
       << setw(12) << ( 4.0 * pow( l2, 3 ) - 2.0 * pow( M_PI, 2 ) * l2 +
			21.0 * abs( zeta( 3.0 ) ) ) / 24.0
       << endl;

  //  plog.max(20);
  
  cout << setw(16) << "Li(0.471, -0.0051 )"
       << setw(16) << plog( 0.471, -0.0051 )
       << setw(12) << -0.00508   // from wolfram alpha
       << endl;
  
  cout << setw(16) << "Li(0.471, -0.033 )"
       << setw(16) << plog( 0.471, -0.033 )
       << setw(12) << -0.0322351   // from wolfram alpha
       << endl;
  
  cout << setw(16) << "Li(0.0431, -1.0 )"
       << setw(16) << plog( 0.0431, -1.0 )
       << setw(12) << -0.509675   // from wolfram alpha
       << endl;
  
  
  
  return 0;
}
