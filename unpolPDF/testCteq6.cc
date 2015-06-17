#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>

#include <Utility/Arguments.hh>
#include <Utility/Timer.hh>
#include <Evolution/PDF.hh>
#include <Evolution/PDFx.hh>

#include "Evolution.hh"
#include "CTEQ6pdf.hh"

using namespace std;
using namespace Utility;
using namespace QCD;
using namespace Evolution;

void dump( const double& q2, vector< double >& x, 
	   PDFLIB::CTEQ6& c6, 
	   PDFx& pdf, PDFx& pdf2,
	   const string& name = "" ){
  
  // prepare stop-watch
  Timer timer;

  // CTEQ6 original value
  cout << setw( 8 ) << name << flush;
  for( int ix = 0; ix < x.size(); ix++ ){
    cout << setw(13) << c6( x[ ix ], q2 ) << flush;
  }
  cout << endl;
  
  if(0){
  // Using KernelPdfLib class
  cout << setw( 8 ) << "EVO1" << flush;
  pdf.pdf().kernel()->setQ2( q2 );
  timer.start();
  for( int ix = 0; ix < x.size(); ix++ ){
    cout << setw(13) << pdf( x[ ix ] ) << flush;
  }
  timer.stop();
  cout 
    << setw(13) 
    << timer.check( Timer::Sec ) / x.size()
    << " s/point" << endl;
  }

  // Using KernelPdfLib class
  cout << setw( 8 ) << "EVO2" << flush;
  pdf2.pdf().kernel()->setQ2( q2 );
  timer.start();
  for( int ix = 0; ix < x.size(); ix++ ){
    cout << setw(13) << pdf2( x[ ix ] ) << flush;
  }
  timer.stop();
  cout 
    << setw(13) 
    << timer.check( Timer::Sec ) / x.size()
    << " s/point" << endl;

  cout << endl;

}


int main( int argc, char* argv[] ){
  
  // initialization
  Arguments args = Arguments::ref( argc, argv );
  
  // prepare q2 values
  vector< double > q2;
  //  q2.push_back( 1.0 );    // lower edge of the pol DIS data
  q2.push_back( 1.69 );    // CTEQ6 initial scale
  //  q2.push_back( 2.5 );    // HERMES average
  //  q2.push_back( 5.0 );    // average value of pol DIS
  q2.push_back( 10.0 );   // average value of SMC/COMPASS
  //  q2.push_back( 20.0 );   // large Q2 sample in the fixed target exp
  //  q2.push_back( 50.0 );   // just see how PDF changes in the large Q2 
  q2.push_back( 100.0 );  //      region....
  //  q2.push_back( 200.0 );
  //  q2.push_back( 500.0 );
  q2 = args.get( "Q2", ",", q2 );

  
  // prepare PDFs based on KernelPdfLib
  unpolPDF::CTEQ6pdf evo;
  unpolPDF::Evo      evo2( args );
  
  double length = args.get( "length", 20.0 );
  double offset = args.get( "offset", 2.0 );
  double angle  = args.get( "angle",  0.55 );
  int    leg1   = args.get( "leg1", 6 );
  int    leg2   = args.get( "leg2", 8 );
  double precision = args.get( "precision", 1.0E-3 );

  PDFx xu( &evo, Flavor::u, leg1, leg2, precision, length, offset, angle );
  PDFx xd( &evo, Flavor::d, leg1, leg2, precision, length, offset, angle );
  PDFx xs( &evo, Flavor::s, leg1, leg2, precision, length, offset, angle );
  PDFx xc( &evo, Flavor::c, leg1, leg2, precision, length, offset, angle );
  PDFx xb( &evo, Flavor::b, leg1, leg2, precision, length, offset, angle );
  
  PDFx xg( &evo, Flavor::g, leg1, leg2, precision, length, offset, angle );
  
  PDFx xub( &evo, Flavor::ubar, leg1, leg2, precision, length, offset, angle );
  PDFx xdb( &evo, Flavor::dbar, leg1, leg2, precision, length, offset, angle );
  PDFx xsb( &evo, Flavor::sbar, leg1, leg2, precision, length, offset, angle );
  PDFx xcb( &evo, Flavor::cbar, leg1, leg2, precision, length, offset, angle );
  PDFx xbb( &evo, Flavor::bbar, leg1, leg2, precision, length, offset, angle );
  
  PDFx xu2( &evo2, Flavor::u, leg1, leg2, precision, length, offset, angle );
  PDFx xd2( &evo2, Flavor::d, leg1, leg2, precision, length, offset, angle );
  PDFx xs2( &evo2, Flavor::s, leg1, leg2, precision, length, offset, angle );
  PDFx xc2( &evo2, Flavor::c, leg1, leg2, precision, length, offset, angle );
  PDFx xb2( &evo2, Flavor::b, leg1, leg2, precision, length, offset, angle );
  
  PDFx xg2( &evo2, Flavor::g, leg1, leg2, precision, length, offset, angle );
  
  PDFx xub2( &evo2, Flavor::ubar, leg1, leg2, precision, length, offset, angle );
  PDFx xdb2( &evo2, Flavor::dbar, leg1, leg2, precision, length, offset, angle );
  PDFx xsb2( &evo2, Flavor::sbar, leg1, leg2, precision, length, offset, angle );
  PDFx xcb2( &evo2, Flavor::cbar, leg1, leg2, precision, length, offset, angle );
  PDFx xbb2( &evo2, Flavor::bbar, leg1, leg2, precision, length, offset, angle );
  
  PDFx xuv( &evo2, Flavor::uv, leg1, leg2, precision, length, offset, angle );
  PDFx xdv( &evo2, Flavor::dv, leg1, leg2, precision, length, offset, angle );
  
  // create CTEQ6 PDF directly from the original library
  PDFLIB::CTEQ6& c6 = *( evo.cteq6() );
  
  PDFLIB::CTEQ6 c6u = c6.u();
  PDFLIB::CTEQ6 c6d = c6.d();
  PDFLIB::CTEQ6 c6s = c6.s();
  PDFLIB::CTEQ6 c6c = c6.c();
  PDFLIB::CTEQ6 c6b = c6.b();
  
  PDFLIB::CTEQ6 c6g = c6.g();
  
  PDFLIB::CTEQ6 c6ub = c6.ubar();
  PDFLIB::CTEQ6 c6db = c6.dbar();
  PDFLIB::CTEQ6 c6sb = c6.sbar();
  PDFLIB::CTEQ6 c6cb = c6.cbar();
  PDFLIB::CTEQ6 c6bb = c6.bbar();
  
  // prepare x values
  double lxmin = args.get( "lxmin", -2.0 );
  double lxmax = args.get( "lxmax",  0.0 );
  int nx       = args.get( "nx", 10 );
  double dlx   = ( lxmax - lxmin ) / nx;
  vector< double > x;
  for( int i = 0; i < nx; i++ ) x.push_back( pow( 10.0, lxmin + dlx * i ) );
  
  for( int iq2 = 0; iq2 < q2.size(); iq2++ ){
    
    // print header information
    cout << "Q^2 = " << q2[ iq2 ] << " GeV^2" << endl;
    cout << setw( 8 ) << "" << flush;
    for( int ix = 0; ix < x.size(); ix++ ) cout << setw( 13 )  << x[ ix ]<< flush;
    cout << endl;
    
    cout << setw( 8 ) << "" << flush;
    for( int ix = 0; ix < x.size(); ix++ ) cout << setw(13) << "----------" << flush;
    cout << endl;
    
    dump( q2[ iq2 ], x, c6g, xg, xg2, "g" );
    dump( q2[ iq2 ], x, c6u, xu, xu2, "u" );
    dump( q2[ iq2 ], x, c6ub, xub, xub2, "u-bar" );
    //    dump( q2[ iq2 ], x, c6ub, xub, xuv, "uv" );
    dump( q2[ iq2 ], x, c6d, xd, xd2, "d" );
    dump( q2[ iq2 ], x, c6db, xdb, xdb2, "d-bar" );
    //    dump( q2[ iq2 ], x, c6db, xdb, xdv, "dv" );
    dump( q2[ iq2 ], x, c6s, xs, xs2, "s" );
    //    dump( q2[ iq2 ], x, c6sb, xsb, xsb2, "s-bar" );
    
    cout << endl;
  }
  
  return 0;
}
