//$Id: testUDratioFit.cc,v 1.1 2013/06/25 01:00:32 miyachi Exp $
#include "UDratioFit.hh"

#include <iostream>
#include <cstdlib>

#include <Utility/Arguments.hh>
#include <ExpData/ExpData.hh>
#include <Xsec/xXsec.hh>
#include <iostream>

using namespace std;
using namespace Utility;

int main( int argc, char* argv[] ){
  
  Arguments& args = Arguments::ref( argc, argv );
  
  double xmin = args.get( "xmin", 1.0E-3 );
  double xmax = args.get( "xmax", 0.9 );  
  
  try {
    
    Fit::UDratioFit fit( args );
    fit.start();
    
  }
  
  catch( int e ){
    cerr << __FILE__ << ":" << __LINE__ 
	 << "\tcatch error (" << e << ")" << endl;
  }
  
  return 0;
}
