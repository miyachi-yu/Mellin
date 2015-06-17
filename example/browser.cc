#include "Utility/Arguments.hh"
#include "polPDF/Evolution.hh"

using namespace std;
using namespace Utility;
using namespace QCD;

int main( int argc, char* argv[] ){

  Arguments& args = *( Arguments::instance( argc, argv ) );
  
  double q2 = ( args.hasOpt( "Q2" ) ?
                atof( args.getOpt( "Q2" ).c_str() ) : 1.0 );
  
  double n = 1.0;
  
  try {
    polPDF::Evo evolv( args );
    
    evolv.setN( complex< double >( n, 0.0 ) );
    evolv.setQ2( q2 );
    
    PdfParametrization* pdfs = evolv.initialDistribution();
    
    cout << evolv << endl;
    
  }
  
  catch( int error ){
    cerr << __FILE__ << ":" << __LINE__ << "\tcatch error ("
         << error << ") !!" << endl;
    return 1;
  }

  return 0;
};
