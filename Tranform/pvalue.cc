#include "Pvalue.hh"
#include <iostream>
#include <iomanip>

#include "Utility/Arguments.hh"

using namespace std;
using namespace Transform;

int main( int argc, char* argv[] ){
  
  Utility::Arguments& args = Utility::Arguments::ref( argc, argv );
  
  double k = args.get( "dof", 100.0 );
  Pvalue p( k );
  //  double dk = 3.0 * sqrt( 2.0 * k );
  double n = 3.0;
  double dk = n * sqrt( 2.0 * k );
  
  //  for( double x = k - dk ; x < k + dk; x += 0.1 * dk ){
  for( double x = k - dk ; x <= k + dk; x += dk / n ){
    
    if( x < 0.0 ) continue;
    
    args.log() 
      << setw(10) << x 
      << setw(10) << x / k 
      << setw(16) << ( p.f() )( x )
      << setw(16) << p( x )
      << endl;

  }

  return 0;
}
