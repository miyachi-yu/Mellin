#include "CZeta.hh"

using namespace Transform;

#include <iostream>
#include <iomanip>
#include <cmath>
#include <cstdlib>

using namespace std;

void test( const CZeta::MODE& mode ){
  CZeta zeta;
  zeta.mode( mode );
  
  cout << "Test of CZeta implementation" << endl;
  
  cout << "zeta(2): "
       << setw(16) << zeta( 2.0 )
       << setw(16) << M_PI * M_PI / 6.0
       << endl;

  cout << "zeta(3): "
       << setw(16) << zeta( 3.0 )
       << setw(16) << 1.2020569032
       << endl;
  
  cout << "zeta(4): "
       << setw(16) << zeta( 4.0 )
       << setw(16) << pow( M_PI, 4 ) / 90.0
       << endl;

  cout << "zeta(5): "
       << setw(16) << zeta( 5.0 )
       << setw(16) << 1.0369277551
       << endl;

  cout << "zeta(6): "
       << setw(16) << zeta( 6.0 )
       << setw(16) << pow( M_PI, 6 ) / 945.0
       << endl;

  cout << "zeta(7): "
       << setw(16) << zeta( 7.0 )
       << setw(16) << 1.0083492774
       << endl;

  cout << "zeta(8): "
       << setw(16) << zeta( 8.0 )
       << setw(16) << pow( M_PI, 8 ) / 9450.0
       << endl;

  cout << "zeta(.5):"
       << setw(16) << zeta( 0.5 )
       << setw(16) << -1.4603545088
       << endl;
  
}

int main( int argc, char* argv[] ){
  
  test( CZeta::simple );
  test( CZeta::havil2003 );
  
  return 0;
}
