#include "Laguerre.hh"
#include <Utility/Arguments.hh>
#include <iostream>
#include <iomanip>
#include <sstream>

using namespace std;
using namespace Transform;

int main( int argc, char* argv[] ) {
  
  Utility::Arguments& args = Utility::Arguments::ref( argc, argv );
  
  double k = args.get( "k", 1.0 );
  double n = args.get( "n", 3 );

  Laguerre lag( 0, k );
  
  cout << "*****************************************************" << endl;
  cout << setw(12) << "x" << flush;
  for( int i = 0; i < n; i++ ){
    ostringstream ostr;
    ostr << "L_" << i + 1 << "^" << k << "(x)";
    cout << setw(12) << ostr.str()   << flush ;
  }
  cout << endl;
  
  for( double x = 0.1; x < 4.0; x += 0.1 ){
    cout << setw(12) << x         << flush ;
    for( int i = 0; i < n; i++ ){
      lag.set( i + 1, k );
      cout << setw(12) << lag( x ) << flush ;
    }
    cout << endl;
  }
  
  
  cout << "Absissa:" << endl;
  cout << "   check with http://mathworld.wolfram.com/Laguerre-GaussQuadrature.html" << endl;
  for( int i = 0; i < n; i++ ){

    cout << "---------------------------------------------------------" << endl;
    lag.set( i + 1, k );

    vector< double > x = lag.absissa();
    cout << " xi: " << flush;
    for( int j = 0; j < x.size(); j++ ) cout << setw( 14 ) << x[j] << flush ;
    cout << endl;

    vector< double > w = lag.weight();
    cout << " wi: " << flush;
    for( int j = 0; j < w.size(); j++ ) cout << setw( 14 ) << w[j] << flush ;
    cout << endl;
    
  }
  
  
  return 0;
};
