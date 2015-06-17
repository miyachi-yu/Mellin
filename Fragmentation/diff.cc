#include <Utility/Arguments.hh>
#include <QCD/PdfErrMatrix.hh>

#include <Evolution/PDF.hh>
#include <Evolution/PDFx.hh>
#include <Evolution/KernelDiff.hh>

#include "Evolution.hh"

#include <vector>
#include <cstdlib>
#include <iostream>
#include <iomanip>

using namespace std;
using namespace Utility;
using namespace QCD;

void dump( Evolution::PDF& pdf1, 
	   Evolution::PDF& pdf2,
	   Evolution::PDF& dpdf ){
  
  Arguments& args = Arguments::ref();
  
  vector< double > q2;
  q2.push_back(  1.0 );
  q2.push_back(  2.0 );
  q2.push_back(  5.0 );
  q2.push_back( 10.0 );
  
  q2 = args.get( "Q2", ",", q2 );
  
  for( int iq2 = 0; iq2 < q2.size(); iq2++ ){
    //    q.kernel()->clearCache();

    pdf1.kernel()->setQ2( q2[ iq2 ] );
    pdf2.kernel()->setQ2( q2[ iq2 ] );
    dpdf.kernel()->setQ2( q2[ iq2 ] );
    
    cout << setw(5) << q2[ iq2 ] << flush;
    
    for( double n = 1.0; n < args.get( "N", 4.0 ); n += 1.0 ){
      cout << setw( 18 ) << pdf1( complex< double >( n, 0.0 ) ) 
	   << flush;
    }
    
    cout << endl;
    cout << setw(5) << ""<< flush;

    for( double n = 1.0; n < args.get( "N", 4.0 ); n += 1.0 ){
      cout << setw( 18 ) << pdf2( complex< double >( n, 0.0 ) ) << flush;
    }
    
    cout << endl;
    cout << setw(5) << "diff"<< flush;

    for( double n = 1.0; n < args.get( "N", 4.0 ); n += 1.0 ){
      cout << setw( 18 ) << dpdf( complex< double >( n, 0.0 ) ) << flush;
    }
    
    cout << endl;
    cout << setw(5) << ""<< flush;

    for( double n = 1.0; n < args.get( "N", 4.0 ); n += 1.0 ){
      cout << setw( 18 ) 
	   << 
	dpdf( complex< double >( n, 0.0 ) ) +
	pdf1( complex< double >( n, 0.0 ) ) - 
	pdf2( complex< double >( n, 0.0 ) ) 
	   << flush;
    }
    
    cout << endl;
    cout << endl;
  }
  
}


int main( int argc, char* argv[] ) {
  
  Arguments& args = Arguments::ref( argc, argv );
  
  vector< string > opts = Fragmentation::Evo::options( Flavor::PIp );
  //  vector< string > opts = Fragmentation::Evo::options( Flavor::HAp );
  
  vector< string >::iterator itr;
  if( ( itr = args.hasOpt( opts ) ) == opts.end() ){
    args.err() << "one of the following option requires" << endl;
    for( int i = 0; i < opts.size(); i++ ){
      args.err() << "\t--" << opts[ i ] << endl;
    }
    return 1;
  }
  
  try {
    
    QCD::Flavor::HADRON h = QCD::Flavor::PIp;
    if( *itr == "FFK" || *itr == "KretzerFFK" || *itr == "DSSFFK" ){
      h = QCD::Flavor::KAp;
    } else if( *itr == "FFP" || *itr == "KretzerFFP" || *itr == "DSSFFP" ){
      h = QCD::Flavor::PRp;
    }
    
    Fragmentation::Evo evolv1( args, h, *itr );
    Fragmentation::Evo evolv2( args, h, *itr );
    
    QCD::PdfBase *base = evolv2.initialDistribution()->find( "gluon" );
    QCD::Parameter* par = evolv2.initialDistribution()->find( "gluon", 0 );
    par->value() *= 0.9;
    base->update();

    evolv1.clearCache();
    evolv2.clearCache();
    
    Evolution::KernelDiff devolv( &evolv2, &evolv1 );
    
    Evolution::PDF g1( &evolv1, QCD::Flavor::g );
    Evolution::PDF u1( &evolv1, QCD::Flavor::u );
    Evolution::PDF ubar1( &evolv1, QCD::Flavor::ubar );
    Evolution::PDF s1( &evolv1, QCD::Flavor::s );
    Evolution::PDF c1( &evolv1, QCD::Flavor::c );
    
    Evolution::PDF g2( &evolv2, QCD::Flavor::g );
    Evolution::PDF u2( &evolv2, QCD::Flavor::u );
    Evolution::PDF ubar2( &evolv2, QCD::Flavor::ubar );
    Evolution::PDF s2( &evolv2, QCD::Flavor::s );
    Evolution::PDF c2( &evolv2, QCD::Flavor::c );
    
    Evolution::PDF dg( &devolv, QCD::Flavor::g );
    Evolution::PDF du( &devolv, QCD::Flavor::u );
    Evolution::PDF dubar( &devolv, QCD::Flavor::ubar );
    Evolution::PDF ds( &devolv, QCD::Flavor::s );
    Evolution::PDF dc( &devolv, QCD::Flavor::c );
    
    cout << "#------------------------------------------------------#" << endl;
    cout << "#   g" << endl;
    dump( g1, g2, dg );
    cout << endl;

    cout << "#------------------------------------------------------#" << endl;
    cout << "#   u" << endl;
    dump( u1, u2, du );
    cout << endl;

    cout << "#------------------------------------------------------#" << endl;
    cout << "#   u-bar" << endl;
    dump( ubar1, ubar2, dubar );
    cout << endl;

    cout << "#------------------------------------------------------#" << endl;
    cout << "#   s" << endl;
    dump( s1, s2, ds );
    cout << endl;
    
    cout << "#------------------------------------------------------#" << endl;
    cout << "#   c" << endl;
    dump( c1, c2, dc );
    cout << endl;
    
  }
  
  catch( int error ){
    cerr << __FILE__ << ":" << __LINE__ << "\tcatch error ("
	 << error << ") !!" << endl;
    return 1;
  }
  
  return 0;
}
