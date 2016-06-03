#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>

#include <Utility/Arguments.hh>
#include <Utility/Timer.hh>
#include <Evolution/PDF.hh>
#include <Evolution/PDFx.hh>

#include "Evolution.hh"

#include <fstream>
#include <sstream>

using namespace std;
using namespace Utility;
using namespace QCD;
using namespace Evolution;


int main( int argc, char* argv[] ){
  
  // initialization
  Arguments args = Arguments::ref( argc, argv );
  
  // prepare q2 values
  vector< double > q2;
  //  q2.push_back( 1.0 );    // lower edge of the pol DIS data
  //  q2.push_back( 1.69 );    // CTEQ6 initial scale
  //  q2.push_back( 2.5 );    // HERMES average
  //  q2.push_back( 5.0 );    // average value of pol DIS
  //  q2.push_back( 10.0 );   // average value of SMC/COMPASS
  //  q2.push_back( 20.0 );   // large Q2 sample in the fixed target exp
  //  q2.push_back( 50.0 );   // just see how PDF changes in the large Q2 
  //  q2.push_back( 100.0 );  //      region....
  //  q2.push_back( 200.0 );
  q2.push_back( 29.0 );
  q2 = args.get( "Q2", ",", q2 );

  
  // prepare PDFs based on KernelPdfLib
  unpolPDF::Evo evo( args );
  
  double length = args.get( "length", 20.0 );
  double offset = args.get( "offset", 1.8 );
  double angle  = args.get( "angle",  0.51 );
  int    leg1   = args.get( "leg1", 6 );
  int    leg2   = args.get( "leg2", 8 );
  double precision = args.get( "precision", 1.0E-4 );

  PDFx xub( &evo, Flavor::ubar, leg1, leg2, precision, length, offset, angle );
  PDFx xdb( &evo, Flavor::dbar, leg1, leg2, precision, length, offset, angle );
  
  // prepare x values
  double lxmin = args.get( "lxmin", -2.0 );
  double lxmax = args.get( "lxmax",  0.0 );
  int    nx    = args.get( "nx", 10 );
  double dlx   = ( lxmax - lxmin ) / nx;
  vector< double > x;
  for( int i = 0; i < nx; i++ ) {
    double xv = pow( 10.0, lxmin + dlx * i );
    x.push_back( xv );
  }
  
  for( int iq2 = 0; iq2 < q2.size(); iq2++ ){
    
    evo.setQ2( q2[ iq2 ] );
    // print header information
    cout << setw(6) << q2[ iq2 ] << " GeV^2" << endl;
    
    for( int ix = 0; ix < x.size(); ix++ ) {
      double dbar, ubar;
      cout << setw( 12 ) << x[ ix ] << flush;
      cout << setw( 12 ) << ( dbar = xdb( x[ ix ] ) ) << flush;
      cout << setw( 12 ) << ( ubar = xub( x[ ix ] ) ) << flush;
      cout << setw( 12 ) << dbar / ubar << endl;
    }
    
    cout << endl;
  }
  
  return 0;
}
