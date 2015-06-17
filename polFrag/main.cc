#include <Utility/Arguments.hh>
#include <Evolution/PDFx.hh>
#include "Evolution.hh"

#include <cstdlib>

using namespace std;
using namespace Utility;
using namespace QCD;
using namespace polFrag;

int main( int argc, char* argv[] ) {
  
  Arguments& args = *( Arguments::instance( argc, argv ) );
  
  try {
    
    polFrag::Evo evolv( args );
    PdfParametrization* pdfs = evolv.initialDistribution();
    cout << evolv << endl;
    
  }
  
  catch( int error ){
    cerr << __FILE__ << ":" << __LINE__ << "\tcatch error ("
	 << error << ") !!" << endl;
    return 1;
  }
  
  return 0;
}
