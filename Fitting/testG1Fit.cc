//$Id: testG1Fit.cc,v 1.6 2008/10/01 07:46:00 miyachi Exp $
#include "G1Fit.hh"

#include <iostream>
#include <cstdlib>

#include <Utility/Arguments.hh>
#include <ExpData/ExpData.hh>
#include <Xsec/xXsec.hh>
#include <FragXsec_T/FragXsec.hh>
#include <iostream>

using namespace std;
using namespace Utility;

int main( int argc, char* argv[] ){
  
  Arguments& args = *( Arguments::instance( argc, argv ) );
  
  double xmin = ( args.hasOpt( "xmin" ) ? 
		  atof( args.getOpt( "xmin" ).c_str() ) :
		  1.0E-3 );
  
  double xmax = ( args.hasOpt( "xmax" ) ? 
		  atof( args.getOpt( "xmax" ).c_str() ) :
		  0.9 );
  
  try {
    
    Fit::G1Fit fit;
    fit.xrange( xmin, xmax );
    
    fit.start();

  }
  
  catch( int e ){
    cerr << __FILE__ << ":" << __LINE__ 
	 << "\tcatch error (" << e << ")" << endl;
  }
  
  return 0;
}
