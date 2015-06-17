#include "CPolyGamma.hh"
#include "GeneralHarmonic.hh"
#include <iostream>
#include <iomanip>
#include <cmath>

using namespace std;
using namespace Transform;

int main( int argc, char* argv[] ){
  
  CPolyGamma pg0( 0 ), pg1( 1 ), pg2( 2 ), pg3( 3 );

  for( double x = 0.0; x < 20.0; x+=0.2 ){
    cout 
      << setw(4) << x 
      << setw(18) << pg0( x )
      << setw(18) << pg1( x )
      << setw(18) << pg2( x )
      << setw(18) << pg3( x )
      << endl;
  }
  
  cout << "---------------------------------------------" << endl;
  cout << "---------  test              ----------------" << endl;
  cout << "---------------------------------------------" << endl;
  
  static double K = 0.91596;
  static double PI2 = M_PI * M_PI;
  
  cout << "phi^1(1/4) = PI^2 + 8 * K                      "
       << setw(12) << PI2 + 8.0 * K 
       << setw(18) << pg1( 0.25 ) << endl;

  cout << "phi^1(1/2) = Pi^2 / 2.0                        " 
       << setw(12) << 0.5 * PI2     
       << setw(18) << pg1( 0.5 ) << endl;

  cout << "phi^1(1) = zeta(2) = pi^2/6                    "
       << setw(12) << pow( M_PI, 2 ) / 6.0
       << setw(18) << pg1(1)
       << endl;

  cout << "phi^2(1/4) = -2 pi^3 - 56 zeta(3)              "
       << setw(12) << -2.0 * pow( M_PI, 3 ) - 56.0 * 1.202057
       << setw(18) << pg2( 1.0 / 4.0 )
       << endl;

  cout << "phi^2(1/3) - phi^2(2/3) = -8 pi^3 / 3 / sqrt(3)" 
       << setw(12) << -8.0 * pow( M_PI, 3 ) / 3.0 / sqrt( 3.0 )
       << setw(18) << pg2( 1.0 / 3.0 ) - pg2( 2.0 / 3.0 )
       << endl;

  cout << "phi^2(1/2) / 14 = - zeta(3)                    "
       << setw(12) << -1.202057
       << setw(18) << pg2( 0.5 ) / 14.0
       << endl;

  
  cout << "phi^3(1/2) = PI^4                              "
       << setw(12) << PI2 * PI2     
       << setw(18) << pg3( 0.5 ) << endl;
  

  cout << "*****************************************************" << endl;
  cout << "phi^0(1.0) = - gamma                           "
       << setw(12) << - GeneralHarmonic::zeta[1]
       << setw(18) << pg0( 1.0 )
       << endl;

  cout << "phi^0(0.5) = - gamma - 2 ln2                   "
       << setw(12) << - GeneralHarmonic::zeta[1]  - 2.0 * log( 2.0 )
       << setw(18) << pg0( 0.5 )
       << endl;


  cout << "phi^0(1.0) - phi^0(0.5) = 2 * ln(2)            "
       << setw(12) << 2.0 * log( 2.0 )
       << setw(18) << pg0(1.0) - pg0(0.5)
       << endl;

  return 0;
}
