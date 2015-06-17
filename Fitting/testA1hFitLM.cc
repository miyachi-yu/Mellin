#include "A1hFitLM.hh"

#include <iostream>

using namespace std;
using namespace Utility;

int main( int argc, char* argv[] ){
  
  Arguments& args = *( Arguments::instance( argc, argv ) );
  
  try {
    
    Fit::A1hFitLM fit( args );;
    fit.start();

  }
  
  catch( int e ){
    cerr << __FILE__ << ":" << __LINE__ 
	 << "\tcatch error (" << e << ")" << endl;
  }
  
  return 0;
}
