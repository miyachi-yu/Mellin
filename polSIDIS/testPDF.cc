#include <iostream>
#include <iomanip>

#include <Utility/Timer.hh>

#include <Xsec/Charge.hh>
#include <Xsec/xzXsec.hh>
#include <Xsec/CKernel.hh>

#include <Evolution/PDF.hh>
#include <Evolution/PDFx.hh>

#include "StrFunc.hh"
#include "XsecComp.hh"


using namespace std;
using namespace Utility;

void dump( Xsection::Xsec& xsec, Evolution::PDF& f, Evolution::PDF& g ){
  
  vector< double > q2;
  q2.push_back(  1.0 );
  q2.push_back(  2.0 );
  q2.push_back(  5.0 );
  q2.push_back( 10.0 );
  
  for( int iq2 = 0; iq2 < q2.size(); iq2++ ){
    
    xsec.setQ2( q2[ iq2 ] );
    cout << "Q^2 = " << setw(5) << q2[ iq2 ] << endl;;

    for( double n = 1.0; n < 5.0; n += 1.0 ){
      
      cout << "n:" << setw( 4 ) << n << flush;
      for( double m = 1.0; m < 5.0; m += 1.0 ){
	cout << setw(18) << xsec( n, m ) << flush;
      }
      cout << endl;
      

      cout << "n:" << setw( 4 ) << n << flush;
      for( double m = 1.0; m < 5.0; m += 1.0 ){
	xsec( n, m );
	cout << setw(18) << xsec.xsecComp()->vevo()[ 0 ]->q( QCD::Flavor::c )
	     << flush;
      }
      cout << endl;
      
      cout << "n:" << setw( 4 ) << n << flush;
      for( double m = 1.0; m < 5.0; m += 1.0 ){
	xsec( n, m );
	cout << setw(18) << xsec.xsecComp()->vevo()[ 1 ]->q( QCD::Flavor::c )
	     << flush;
      }
      cout << endl;
      
      

    }



    cout << "q(n) " << flush;
    for( double n = 1.0; n < 5.0; n += 1.0 )
      cout << setw( 18 ) << f( complex< double >( n, 0.0 ) ) << flush;
    
    cout << endl;
    
    cout << "q(m) " << flush;
    for( double m = 1.0; m < 5.0; m += 1.0 )
      cout << setw( 18 ) << g( complex< double >( m, 0.0 ) ) << flush;
    
      cout << endl;

  }
}


int main( int argc, char* argv[] ){
  
  Arguments *args = Arguments::instance( argc, argv );
  
  if( ! args->hasOpt( "FF" ) )     return 1;
  if( ! args->hasOpt( "polPDF" ) ) return 2;
  
  polSIDIS::StrFunc g1h( *args, Xsection::Charge::p );
  
  Evolution::KernelBase *evo1 = g1h.xsecComp()->vevo()[ 0 ];
  Evolution::KernelBase *evo2 = g1h.xsecComp()->vevo()[ 1 ];
  
  Evolution::PDF u1( evo1, QCD::Flavor::u );
  Evolution::PDF u2( evo2, QCD::Flavor::u );
  
  dump( g1h, u1, u2 );

  
  return 0;
}
