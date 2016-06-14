#include <Utility/Arguments.hh>
#include <Evolution/PDFx.hh>
#include <QCD/PdfParametrization.hh>
#include <QCD/PdfBase.hh>

#include "Evolution.hh"

#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <fstream>

using namespace std;
using namespace Utility;
using namespace QCD;

int main( int argc, char* argv[] ) {
  
  Arguments& args = Arguments::ref( argc, argv );
  args.set( "Stat", "BS15.xml" );
  
  try {
    
    unpolPDF::Evo *evolv     = unpolPDF::Evo::instance();
    PdfParametrization* pdfs = evolv->initialDistribution();
    evolv->setQ2( args.get( "Q2", pdfs->scale() ) );
    
    double lxmin = args.get( "lxmin", -3.0 );
    double lxmax = args.get( "lxmax",  0.0 );
    int nx       = args.get( "nx", 30 );
    double dlx   = ( lxmax - lxmin ) / nx;
    vector< double > x;
    for( int i = 0; i < nx; i++ ) x.push_back( pow( 10.0, lxmin + dlx * i ) );
    
    vector< PdfBase* >& qs = pdfs->pdfs();
    
    ofstream ofs1( "bs15-x.dat" );
    ofs1 << "#" << setw(12) << "x";
    for( int j = 0; j < qs.size(); j++ ) ofs1 << setw(13) << qs[ j ]->name();
    ofs1 << endl;
    
    for( int i = 0; i < nx; i++ ) {
      ofs1 << setw(13) << x[ i ];
      for( int j = 0; j < qs.size(); j++ ){
	ofs1 << setw(13) << qs[j]->x( x[ i ] );
      }
      ofs1 << endl;
    }
    
    double length = args.get( "length", 25.0 );
    double angle  = args.get( "angle",   0.51 );
    double offset = args.get( "offset",  1.6 );
    double precision = args.get( "precision", 1.0E-5 );
    
    int    ndiv1 = args.get( "ndiv1", 6 );
    int    ndiv2 = args.get( "ndiv2", 8 );
    
    ofstream ofs2( "bs15-mellin.dat" );
    ofs2 << "# --- configuration" << endl;
    ofs2 << "#" << setw(12) << "angle"
	 << setw(13) << "length" 
	 << setw(13) << "offset"
      	 << setw(13) << "precision"
	 << setw(13) << "div1"
	 << setw(13) << "div2" << endl;
    ofs2 << "#" << setw(12) << angle
	 << setw(13) << length
	 << setw(13) << offset
	 << setw(13) << precision
	 << setw(13) << ndiv1
	 << setw(13) << ndiv2 << endl;

    ofs2 << "#" << setw(12) << "x";
    for( int j = 0; j < qs.size(); j++ ) ofs2 << setw(13) << qs[ j ]->name();
    ofs2 << endl;
    for( int i = 0; i < nx; i++ ) {
      ofs2 << setw(13) << x[ i ];
      for( int j = 0; j < qs.size(); j++ ){
	
	int pid = QCD::Flavor::id( qs[j]->name() );
	Evolution::PDFx *ptrQ = 
	  new Evolution::PDFx( evolv, (QCD::Flavor::PARTON) pid,
			       ndiv1, ndiv2,
			       precision, length, offset, angle );
	
	ofs2 << setw(13) << (*ptrQ)( x[ i ] );
	delete ptrQ;
      }
      ofs2 << endl;
      
    }

  }
  
  catch( int error ){
    cerr << __FILE__ << ":" << __LINE__ << "\tcatch error ("
	 << error << ") !!" << endl;
    return 1;
  }
  
  return 0;
}
