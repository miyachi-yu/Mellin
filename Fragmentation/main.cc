#include <Utility/Arguments.hh>
#include <QCD/PdfParametrization.hh>
#include <QCD/PdfErrMatrix.hh>
#include <Evolution/PDF.hh>
#include <Evolution/PDFx.hh>

#include "Evolution.hh"

#include <vector>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <sstream>
#include <fstream>

using namespace std;
using namespace Utility;
using namespace QCD;

void dump( Evolution::PDF& q ){
  
  Arguments& args = Arguments::ref();

  vector< double > q2;
  q2.push_back(  1.0 );
  q2.push_back(  2.0 );
  q2.push_back(  5.0 );
  q2.push_back( 10.0 );
  
  q2 = args.get( "Q2", ",", q2 );
  
  
  for( int iq2 = 0; iq2 < q2.size(); iq2++ ){
    //    q.kernel()->clearCache();
    q.kernel()->setQ2( q2[ iq2 ] );
    
    cout << setw(5) << q2[ iq2 ] << flush;
    
    for( double n = 1.0; n < args.get( "N", 4.0 ); n += 1.0 ){
      cout << setw( 18 ) << q( complex< double >( n, 0.0 ) ) << flush;
    }
    
    cout << endl;
  }
  
}

void print( Evolution::Kernel *kernel ){
  
  Arguments& args = Arguments::ref();
  double angle  = args.get( "angle",  0.55 );
  double length = args.get( "length", 10.0 );
  double offset = args.get( "offset", 2.0 );
  double prec   = args.get( "precision", 1E-3 );
  int    leg1   = args.get( "leg1", 6 );
  int    leg2   = args.get( "leg2", 8 );
  
  
  Evolution::PDFx xg( kernel,  QCD::Flavor::g,    leg1, leg2, prec, length, offset, angle );
  Evolution::PDFx xu(  kernel, QCD::Flavor::u,    leg1, leg2, prec, length, offset, angle );
  Evolution::PDFx xub( kernel, QCD::Flavor::ubar, leg1, leg2, prec, length, offset, angle );
  Evolution::PDFx xd(  kernel, QCD::Flavor::d,    leg1, leg2, prec, length, offset, angle );
  Evolution::PDFx xdb( kernel, QCD::Flavor::dbar, leg1, leg2, prec, length, offset, angle );
  Evolution::PDFx xs(  kernel, QCD::Flavor::s,    leg1, leg2, prec, length, offset, angle );
  Evolution::PDFx xsb( kernel, QCD::Flavor::sbar, leg1, leg2, prec, length, offset, angle );
  
  
  double zmin = args.get( "zmin", 0.2 );
  double zmax = args.get( "zmax", 0.8 );
  int    nz   = args.get( "nz",   20  );
  bool   logz = args.hasOpt( "logz" );
  string oname = args.get( "output", "FF" );
  
  double dz = ( logz ? log10( zmax ) - log10( zmin ) : zmax - zmin ) / nz ;
  
  vector< double > q2( 1, 10.0 );
  q2 = args.get( "Q2", ",", q2 );
  for( int iq2 = 0; iq2 < q2.size(); iq2++ ){
    
    ostringstream ostr;
    ostr << oname << "_" << q2[ iq2 ] << ".dat"; 
    
    cout << "Create " << ostr.str() << endl;
    
    ofstream ofs( ostr.str().c_str() );
    ofs << "# " << kernel->initialDistribution()->path() << endl;
    ofs << "# Q^2 = " <<  q2[ iq2 ] << endl;
    ofs << "# Inverse mellin configuration: " << endl;
    ofs << "#     angle: " << angle << endl;
    ofs << "#     offset: " << offset << endl;
    ofs << "#     length: " << length << endl;
    ofs << "# Integration configuration: " << endl;
    ofs << "#     Leg: " << leg1 << " - " << leg2 << endl; 
    ofs << "#     Precision: " << prec << endl;
    ofs << "# " << endl;
    ofs << "#[z] [g] [u] [ubar] [d] [dbar] [s] [sbar]" << endl;
    
    kernel->setQ2( q2[ iq2 ] );
    for( int i = 0; i < nz; i++ ){
      double z = ( logz ? 
		   pow( 10, log10( zmin ) + i * dz ) :
		   zmin + i * dz );
      ofs << setw(12) << z << flush;
      ofs << setw(12) << xg( z ) << flush;
      ofs << setw(12) << xu( z ) << flush;
      ofs << setw(12) << xub( z ) << flush;
      ofs << setw(12) << xd( z ) << flush;
      ofs << setw(12) << xdb( z ) << flush;
      ofs << setw(12) << xs( z ) << flush;
      ofs << setw(12) << xsb( z ) << flush;

      ofs << endl;
    }
    
    ofs.close();
  }
  
}


int main( int argc, char* argv[] ) {
  
  Arguments& args = Arguments::ref( argc, argv );
  
  vector< string > optPI = Fragmentation::Evo::options( Flavor::PIp );
  vector< string > optKA = Fragmentation::Evo::options( Flavor::KAp );
  vector< string > optRE = Fragmentation::Evo::options( Flavor::REp );
  vector< string > optPR = Fragmentation::Evo::options( Flavor::PRp );
  
  vector< string > opts;
  opts.insert( opts.end(), optPI.begin(), optPI.end() );
  opts.insert( opts.end(), optKA.begin(), optKA.end() );
  opts.insert( opts.end(), optRE.begin(), optRE.end() );
  opts.insert( opts.end(), optPR.begin(), optPR.end() );

  vector< string >::iterator itr;
  if( ( itr = args.hasOpt( opts ) ) == opts.end() ){
    args.err() << "one of the following option requires" << endl;
    for( int i = 0; i < opts.size(); i++ ){
      args.err() << "\t--" << opts[ i ] << endl;
    }
    return 1;
  }
  
  try {
    
    string& key    = *itr;
    
    // check hadron type 
    QCD::Flavor::HADRON h = QCD::Flavor::PIp;
    if( key.size() > 2 ){
      string type = key.substr( key.size() - 3, 3 );
      if( type == "FFK" ){
	h = QCD::Flavor::KAp;
      } else if( type == "FFP" ) {
	h = QCD::Flavor::PRp;
      } else if( type == "FFR" ) {
	h = QCD::Flavor::REp;
      }
    }

    cout << h << endl;
    
    // create FF objects
    Fragmentation::Evo evolv( args, h, key );
    evolv.constNf( false );
    PdfParametrization* pdfs = evolv.initialDistribution();
    
    cout << evolv << endl;
    cout << *pdfs << endl;
    
    Evolution::PDF g( &evolv,    QCD::Flavor::g );
    
    Evolution::PDF u( &evolv,    QCD::Flavor::u );
    Evolution::PDF ubar( &evolv, QCD::Flavor::ubar );
    
    Evolution::PDF d( &evolv,    QCD::Flavor::d );
    Evolution::PDF dbar( &evolv, QCD::Flavor::dbar );
    
    Evolution::PDF s( &evolv,    QCD::Flavor::s );
    Evolution::PDF sbar( &evolv, QCD::Flavor::sbar );
    
    Evolution::PDF c( &evolv,    QCD::Flavor::c );
    Evolution::PDF cbar( &evolv, QCD::Flavor::cbar );
    
    Evolution::PDF b( &evolv,    QCD::Flavor::b );
    Evolution::PDF bbar( &evolv, QCD::Flavor::bbar );
    
    cout << "#------------------------------------------------------#" << endl;
    cout << "#   g" << endl;
    dump( g );
    cout << endl;
    
    cout << "#------------------------------------------------------#" << endl;
    cout << "#   u" << endl;
    dump( u );
    cout << endl;
    
    cout << "#------------------------------------------------------#" << endl;
    cout << "#   u-bar" << endl;
    dump( ubar );
    cout << endl;
    
    cout << "#------------------------------------------------------#" << endl;
    cout << "#   s" << endl;
    dump( s );
    cout << endl;
    
    cout << "#------------------------------------------------------#" << endl;
    cout << "#   c" << endl;
    dump( c );
    cout << endl;
    
    if( pdfs->emNumber() != 0 ){
      cout 
	<< "#------------------------------------------------------#" << endl
	<< "#   Error Matrix " << endl;
      
      PdfErrMatrix em( pdfs );
      cout << em << endl;
      
    }

    print( &evolv );


  }
  
  catch( int error ){
    cerr << __FILE__ << ":" << __LINE__ << "\tcatch error ("
	 << error << ") !!" << endl;
    return 1;
  }
  



  return 0;
}
