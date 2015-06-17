#include "HyperGeom.hh"
#include "HyperGeomInt.hh"
#include <iostream>
#include <cstdlib>

#include <Utility/Timer.hh>

using namespace std;
using namespace Transform;

int main( int argc, char* argv[] ){

  //  if( argc != 2 ) return 0;
  
  HyperGeom f;
  HyperGeomInt func;
  
  complex< double > n( 0.5, 10.0 );
  
  f.n( n );
  f.upper( 0.0 );
  if( argc == 2 ) f.lower( atof( argv[1] ) );
  
  Utility::Timer timer;

  timer.start();
  for( double z = 0.0; z < 1.0; z += 0.01 ){
    complex< double > nn( z, 0.0 );
    /*
    cout 
      << z    << "\t" 
      << f(z) << "\t" 
      << nn   << "\t"
      << func( nn )
      << endl;
    */
    f(z); func( nn );
  }
  cout << "Total time spent:\t" << timer.check( Utility::Timer::Msec ) << " ms" << endl;;
  
  timer.start();
  for( double z = 0.0; z < 1.0; z += 0.01 ){
    complex< double > nn( z, 0.0 );
    /*
    cout 
      << z    << "\t" 
      << f(z) << "\t" 
      << nn   << "\t"
      << func( nn )
      << endl;
    */
    f(z); func( nn );
  }
  cout << "Total time spent:\t" << timer.check(  Utility::Timer::Msec ) << " ms" << endl;;

  for( double z = 0.0; z < 1.0; z += 0.1 ){
    complex< double > nn( z, 0.0 );
    cout 
      << z    << "\t" 
      << f(z) << "\t" 
      << nn   << "\t"
      << func( nn )
      << endl;
  }
  
  return 0;
}
