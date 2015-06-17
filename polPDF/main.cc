#include <Utility/Arguments.hh>

#include <Evolution/PDF.hh>
#include <Evolution/PDFx.hh>

#include "Evolution.hh"

#include <vector>
#include <cstdlib>
#include <iostream>
#include <iomanip>

using namespace std;
using namespace Utility;
using namespace QCD;

void dump( Evolution::PDF& q ){
  
  vector< double > q2;
  q2.push_back(  1.0 );
  q2.push_back(  2.0 );
  q2.push_back(  5.0 );
  q2.push_back( 10.0 );

  for( int iq2 = 0; iq2 < q2.size(); iq2++ ){
    
    q.kernel()->setQ2( q2[ iq2 ] );
    
    cout << setw(5) << q2[ iq2 ] << flush;
    
    for( double n = 1.0; n < 5.0; n += 1.0 ){
      cout << setw( 18 ) << q( complex< double >( n, 0.0 ) ) << flush;
    }
    
    cout << endl;
  }
  
  
}


int main( int argc, char* argv[] ) {
  
  Arguments& args = *( Arguments::instance( argc, argv ) );
  polPDF::Evo *evolv = NULL ;
  
  vector< string > opts( 3 );
  opts[ 0 ] = "polPDF";
  opts[ 1 ] = "DSSV";
  opts[ 2 ] = "BB";

  vector< string >::iterator itr;
  if( ( itr = args.hasOpt( opts ) ) != opts.end() ) {
    try {
      evolv = new polPDF::Evo( args, *itr );
      evolv->constNf( args.hasOpt( "constNf" ) );
      PdfParametrization* pdfs = evolv->initialDistribution();
      cout << *evolv << endl;
      
      Evolution::PDF u( evolv, QCD::Flavor::u );
      Evolution::PDF ubar( evolv, QCD::Flavor::ubar );
      Evolution::PDF c( evolv, QCD::Flavor::c );
      
      Evolution::PDFx xu( evolv, QCD::Flavor::u );
      
      //      evolv->setQ2( 10.0 );
      //      xu( 0.01 );
  
      dump( u );
      dump( ubar );
      dump( c );

      delete evolv;
    }
  
    catch( int error ){
      cerr << __FILE__ << ":" << __LINE__ << "\tcatch error ("
	   << error << ") !!" << endl;
      return 1;
    }
  }
  return 0;
}
