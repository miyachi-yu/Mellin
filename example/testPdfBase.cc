// $Id: testPdfBase.cc,v 1.14 2008/05/29 04:09:11 miyachi Exp $
/*!
  \file   testPdfbase.cc
  \brief  example program for PdfParameterizaton and PdfBase class.

  This program construct PdfParameterization object from the XML configuration
  file. The example of the configuration file can be found as QCD/pdfset.xml.
  
  The path of the configuration file must be spcified with the command line
  argument using an option --pdf=[], as defined in the souce code for the 
  Config object.

 */
#include <Utility/Arguments.hh>
#include <Utility/Timer.hh>

#include <QCD/PdfBase.hh>
#include <QCD/PdfParametrization.hh>
#include <QCD/ConfigPdfBase.hh>
#include <QCD/ConfigFFbase.hh>

#include <Tranform/InverseMellin.hh>
#include <Tranform/LegQuadrature.hh>
#include <Tranform/GridIntegration.hh>

#include <cmath>
#include <iomanip>

using namespace std;
using namespace QCD;
using namespace Utility;
using namespace Transform;

void dump( PdfParametrization& param ){

  cout << "***********************************************************" << endl;
  cout << "\n\n" << endl;
  cout << param << endl;
  cout << "\n\n" << endl;

  Timer timer;

  LegQuadrature meth1(4);  // Gauss Legendre with 4 inter points
  LegQuadrature meth2(6);  // Gauss Legendre with 6 inter points
  GridIntegration grid( &meth1, &meth2, 4, 0.001 );
  
  InverseMellin IM( &grid );
  IM.setParameter( 1.5, 0.8 * M_PI );
  IM.upper( 13. );
  
  vector< PdfBase* > pdfs = param.pdfs();
  
  for( int i = 0; i < pdfs.size(); i++ ){

    cout << "********************************************************" << endl;
    cout << *( pdfs[ i ] ) << endl;
    
    if( pdfs[i]->src() != "" ) continue;
    
    InverseMellin& f = 
      dynamic_cast< InverseMellin& >(  IM.transform( pdfs[ i ] ) ); 
    
    cout << endl;
    cout 
      << setw(16) << "x"
      << setw(16) << "Mellin^-1"
      << setw(16) << "f(x)"
      << setw(16) << "Residual(%)"
      << endl;
    cout 
      << setw(16) << "------------"
      << setw(16) << "------------"
      << setw(16) << "------------"
      << setw(16) << "------------"
      << endl;
    
    for( double lx = -5; lx < 0; lx+=0.25 ){
      double x = pow( 10, lx );
      double invm = f( x );
      double func = pdfs[ i ]->x( x ) ;
      cout << setw(16) << x
	   << setw(16) << invm
	   << setw(16) << func
	   << setw(16) << 100.0 * ( 1.0 - ( invm / func ) )
	   << " %"
	   << endl;
    }

    cout << endl;
    cout << "********************************************************" << endl;
    cout << endl;
    
    cout << "Partial momnet test" << endl;
    
    for( double x1 = 0.1; x1 < 0.9; x1 += 0.1 ){
    double x2 = x1 + 0.1;
    
    cout << setw(5) << x1 
	 << setw(5) << x2 << flush;

    timer.start();
    cout << setw(12) << f.partial( x1, x2 ) << flush;
    cout << setw(10) << timer.check( Timer::Msec ) << " (ms)" << flush ;
    
    timer.start();
    
    cout << setw(12) << grid.integral( x1, x2, f ) / ( x2 - x1 ) << flush;
    cout << setw(10) << timer.check( Timer::Msec ) << " (ms)" << flush;

    cout << endl;
    }
    

  }

  cout << "\n\n" << endl;
  
}


int main( int argc, char* argv[] ){
  
  Arguments& args = *( Arguments::instance( argc, argv ) );
  
  if( ! args.hasOpt( "polPDF" ) ){
    cerr << "--polPDF=  is required." << endl;
  }
  
  if( ! args.hasOpt( "FF" ) ){
    cerr << "--FF=  is required." << endl;
  }
  
  ConfigPdfBase confPdf( "polPDF" );
  PdfParametrization param( confPdf );
  dump( param );
  
  ConfigFFbase confFF( "FF" );
  PdfParametrization ffs( confFF );
  dump( ffs );
  
  return 0;
}
