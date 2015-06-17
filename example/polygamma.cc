#include "Tranform/CPolyGamma.hh"
#include <iostream>
#include <iomanip>
#include <cmath>
#include <vector>
using namespace std;
using namespace Transform;
/*
  This is an example program for complex polygamma function class,
  CPolyGamma.

 */
int main( int argc, char* argv[] ){

  /*!
    On the construction, one must need to give dimension in the
    differencial. 
   */
  CPolyGamma pg0( 0 ), pg1( 1 ), pg2( 2 ), pg3( 3 );
  cout << "---------------------------------------------" << endl;
  cout << "------  test for some special cases  --------" << endl;
  cout << "---------------------------------------------" << endl;
  
  static double K = 0.91596;
  static double PI2 = M_PI * M_PI;

  cout << "phi^1(1/4) = PI^2 + 8 * K = "
       << setw( 10 ) << PI2 + 8.0 * K << "\t" << pg1( 0.25 ) << endl;
  cout << "phi^1(1/2) = Pi^2 / 2.0   = " 
       << setw( 10 ) << 0.5 * PI2     << "\t" << pg1( 0.5 ) << endl;
  cout << "phi^3(1/2) = PI^4         = "
       << setw( 10 ) << PI2 * PI2     << "\t" << pg3( 0.5 ) << endl;
  
  
  cout << endl;
  cout << "---------------------------------------------" << endl;
  cout << "------  use of vector< CPolyGamma >  --------" << endl;
  cout << "---------------------------------------------" << endl;
  
  // one can also store in vector container
  vector< CPolyGamma > vpg( 4 );
  for( int i = 0; i < vpg.size(); i++ ){
    vpg[ i ].m( i );  // one can re-set the dimention after all.
  }
  
  for( double x = -10.0; x < 10.0; x += 0.2 ){
    cout << setw(5) << x << flush;
    for( int i = 0; i < vpg.size(); i++ ){
      cout << setw(18) << vpg[ i ]( x ) << flush;
    }
    cout << endl;
  }


  return 0;
}
