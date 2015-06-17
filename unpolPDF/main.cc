#include <Utility/Arguments.hh>
#include <Evolution/PDFx.hh>
#include <QCD/PdfParametrization.hh>
#include <QCD/PdfBase.hh>

#include "Evolution.hh"

#include <cstdlib>
#include <iostream>
#include <iomanip>

using namespace std;
using namespace Utility;
using namespace QCD;

int main( int argc, char* argv[] ) {
  
  Arguments& args = Arguments::ref( argc, argv );

  try {
    
    unpolPDF::Evo *evolv = unpolPDF::Evo::instance();
    PdfParametrization* pdfs = evolv->initialDistribution();
    evolv->setQ2( args.get( "Q2", pdfs->scale() ) );
    
    //      evolv->setN( args.get( "NN", complex< double >( 1.0, 0.0 ) ) );
    cout << *evolv << endl;
    //    cout << *pdfs  << endl;
    
    double lxmin = args.get( "lxmin", -2.0 );
    double lxmax = args.get( "lxmax",  0.0 );
    int nx       = args.get( "nx", 10 );
    
    double dlx   = ( lxmax - lxmin ) / nx;
    vector< double > x;
    for( int i = 0; i < nx; i++ ) x.push_back( pow( 10.0, lxmin + dlx * i ) );
    
    cout << setw(10) << "x" << flush;
    for( int i = 0; i < nx; i++ ) cout << setw(13) << x[ i ] << flush;
    cout << endl;
    
    cout << setw(10) << "" << flush;
    for( int i = 0; i < nx; i++ ) cout << setw(13) << "============" << flush;
    cout << endl;
    
    vector< PdfBase* >& qs = pdfs->pdfs();
    for( int j = 0; j < qs.size(); j++ ){
      
      int pid = QCD::Flavor::id( qs[j]->name() );
      Evolution::PDFx *ptrQ = 
	( QCD::Flavor::isParton( qs[j]->name() ) ?
	  new Evolution::PDFx( evolv, (QCD::Flavor::PARTON) pid, 6, 8, 1.0E-3, 20, 1.6, 0.6 ) :
	  new Evolution::PDFx( evolv, (QCD::Flavor::TYPE)   pid, 6, 8, 1.0E-3, 20, 1.6, 0.6 ) );
      
      if( ptrQ == NULL ) continue;
      
      cout << setw(10) << qs[j]->name() << flush;
      for( int i = 0; i < nx; i++ ) {
	cout << setw(13) << qs[j]->x( x[ i ] ) << flush;
      }
      cout << endl;
      
      
      cout << setw(10) << "" << flush;
      for( int i = 0; i < nx; i++ ) {
	cout << setw(13) << (*ptrQ)( x[ i ] ) << flush;
      }
      cout << endl;
      
      cout << setw(10) << "" << flush;
      for( int i = 0; i < nx; i++ ) cout << setw(13) << "------------" << flush;
      cout << endl;
      
      delete ptrQ;
    }

  }
  
  catch( int error ){
    cerr << __FILE__ << ":" << __LINE__ << "\tcatch error ("
	 << error << ") !!" << endl;
    return 1;
  }
  
  return 0;

}
