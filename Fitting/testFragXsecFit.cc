//$Id: testFragXsecFit.cc,v 1.12 2008/10/01 07:46:00 miyachi Exp $
#include "FragXsecFit.hh"

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
  
  try {
    
    Fit::FragXsecFit fit( args );;
    fit.start();

  }
  
  catch( int e ){
    cerr << __FILE__ << ":" << __LINE__ 
	 << "\tcatch error (" << e << ")" << endl;
  }
  
  return 0;
}
