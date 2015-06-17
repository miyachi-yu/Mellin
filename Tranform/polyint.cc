#include "PolyInt.hh"
#include "CBeta.hh"
#include <iostream>
#include <iomanip>

#include <Utility/Arguments.hh>

using namespace std;
using namespace Transform;

int main( int argc, char* argv[] ){
  
  Utility::Arguments& args = Utility::Arguments::ref( argc, argv );
  
  PolyInt pi;
  CBeta cb( args.get( "alpha", 1.0 ), args.get( "beta", 1.0 ) );
  
  
  for( double x = 0.1; x < 1.0; x += 0.1 ){
    pi[ x ] = cb.x( x );
  }

  
  cout << "----------------------------------------------------------" << endl;
  cout << "-                       stored data                      -" << endl;
  cout << "----------------------------------------------------------" << endl;
  for( PolyInt::iterator itr = pi.begin();
       itr != pi.end(); itr++ ){
    cout << setw(12) << itr->first
	 << setw(12) << cb.x( itr->first )
	 << setw(12) << itr->second
	 << endl;
  }

  cout << "----------------------------------------------------------" << endl;
  cout << "-                      interpolation                     -" << endl;
  cout << "----------------------------------------------------------" << endl;
  if( 1 )
    {
      for( double x = 0.11; x < 1.0; x += 0.01 )
	//	double x = 0.045;
	{
	  cout 
	    << setw(12) << x
	    << setw(12) << cb.x( x )
	    << setw(12) << pi( x )
	    << endl;
	}
  
    }
  
  return 0;
}
