#include "CGamma.hh"
#include <iostream>
#include <iomanip>

using namespace std;
using namespace Transform;

int main( int argc, char* argv[] ){
  CGamma g;

  // test for several constants
  
  cout 
    << "G(2/3)*G(1/3)*sqrt(3) - 2 *pi = 0\n\t"
    << g( 2.0 / 3.0 ) * g( 1.0 / 3.0 ) * sqrt( 3.0 ) - 2.0 * M_PI
    << endl;
  
  cout 
    << "G(3/4)*G(1/4)-sqrt(2)*pi = 0\n\t"
    << g( 3.0 / 4.0 ) * g( 1.0 / 4.0 ) - sqrt( 2.0 ) * M_PI
    << endl;

  cout 
    << "G(3/5)*F(2/5)-sqrt(2.0-2.0/sqrt(5))*pi = 0\n\t"
    << g( 0.6 ) * g( 0.4 ) - sqrt( 2.0 - 2.0 / sqrt( 5.0 ) ) * M_PI
    << endl;

  cout 
    << "G(4/5)*F(1/5)-sqrt(2.0+2.0/sqrt(5))*pi = 0\n\t"
    << g( 0.8 ) * g( 0.2 ) - sqrt( 2.0 + 2.0 / sqrt( 5.0 ) ) * M_PI
    << endl;

  cout 
    << "G(1/2) - sqrt(pi) = 0\n\t"
    << g( 0.5 ) - sqrt( M_PI )
    << endl;

  cout 
    << "G(3/2) - sqrt(pi) / 2 = 0\n\t"
    << g( 1.5 ) - 0.5 * sqrt( M_PI )
    << endl;

  cout 
    << "G(5/2) - 3 / 4 * sqrt(pi)  = 0\n\t"
    << g( 2.5 ) - 0.75 * sqrt( M_PI )
    << endl;

  cout 
    << "G(7/2) - 15 / 8 * sqrt(pi)  = 0\n\t"
    << g( 3.5 ) - 15.0 / 8.0 * sqrt( M_PI )
    << endl;

  cout 
    << "G(9/2) - 105 / 16 * sqrt(pi)  = 0\n\t"
    << g( 4.5 ) - 105.0 / 16.0 * sqrt( M_PI )
    << endl;

  for( double x = 0.1; x < 1.0; x += 0.1 ){
    
    double c  = sin( M_PI * x ) / M_PI;
    double gv = - x * ( g(x) * g( -x ) ).real();
    double g2 = ( g(x) * g( 1.0 - x ) ).real();
    
    cout 
      << setw(10) << x
      << setw(12) << gv
      << setw(12) << g2
      << setw(12) << 1.0 / c
      << endl;
  }
  
  

  for( double x = 10.0; x < 400.0; x += 1.0 ){
    cout << setw(6) << x 
	 << setw(16) << g( x ).real() 
	 << endl;
  }




  return 0;
}
