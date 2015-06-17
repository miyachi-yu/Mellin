/*!
  \file  testG1h.cc
  \brief test program for polarized structure functions

  This is a test and an example code for polarized structure functions
  implemented in libpolSIDIS.so.

  You may get some feeling how to use polSIDIS::StrFunc class.

*/
#include <Utility/Timer.hh>
#include <Tranform/GridIntegration.hh>
#include <Evolution/PDFx.hh>
#include <Xsec/xXsec.hh>
#include <Xsec/xzXsec.hh>
#include <Xsec/CKernel.hh>

#include <iostream>
#include <iomanip>
#include <sstream>

#include "StrFunc.hh"
#include "XsecComp.hh"

using namespace std;
using namespace Utility;
using namespace QCD;
using namespace Evolution;

void dump( Xsection::xXsec& f, const vector< double >& lx ){
  
  Arguments& args = Arguments::ref();

  complex< double > n = args.get( "N", complex< double >( 2.0, 0.0 ) );
  complex< double > m = args.get( "M", complex< double >( 2.0, 0.0 ) );
  
  cout << setw( 4 ) << f.moment()->q2() 
       << setw( 15 ) << ( *f.moment() )( n, m ) << flush;
  
  vector< double > x( 1, 0.0 );
  vector< double > z( 2, 0.0 );
  z[ 0 ] = args.get( "zmin", 0.2 );
  z[ 1 ] = args.get( "zmax", 0.8 );
  double dz = z[ 1 ] - z[ 0 ];
  
  Utility::Timer timer;
  timer.start();
  for( int i = 0; i < lx.size(); i++ ){
    x[ 0 ] = lx[ i ];
    cout << setw( 12 ) << 0.5 * x[ 0 ] * f( x, z ) * dz << flush;
  }
  timer.stop();
  
  cout << setw( 12 ) << timer.check( Timer::Sec ) << " sec" << endl;;
 
}

void dump( Xsection::xzXsec& f, const vector< double >& lx ){
  
  Arguments& args = Arguments::ref();
  
  complex< double > n = args.get( "N", complex< double >( 2.0, 0.0 ) );
  complex< double > m = args.get( "M", complex< double >( 2.0, 0.0 ) );
  
  cout << setw( 4 ) << f.moment()->q2() 
       << setw( 15 ) << ( *f.moment() )( n, m ) << flush;
  
  vector< double > x( 1, 0.0 );
  vector< double > z( 2, 0.0 );
  z[ 0 ] = args.get( "zmin", 0.2 );
  z[ 1 ] = args.get( "zmax", 0.8 );
  double dz = z[ 1 ] - z[ 0 ];
  
  Utility::Timer timer;
  timer.start();
  for( int i = 0; i < lx.size(); i++ ){
    x[ 0 ] = lx[ i ];
    cout << setw( 12 ) << 0.5 * x[ 0 ] * f( x, z ) * dz << flush;
  }
  timer.stop();
  
  cout << setw( 12 ) << timer.check( Timer::Sec ) << " sec" << endl;;
}

void dump( Xsection::xXsec& f, const vector< double >& lx, const double& q2 ){
  
  Arguments& args = Arguments::ref();
  
  if( q2 != 0.0 ){
    f.moment()->setQ2( q2 );
    dump( f, lx );
  } else {
    cout << setw( 4 ) << "Q^2" << setw( 15 ) << "g_1h(N,M)" << flush;
    for( int i = 0; i < lx.size(); i++ ) cout << setw( 12 ) << lx[i] << flush;
    cout << endl;
  }
}

void dump( Xsection::xzXsec& f, const vector< double >& lx, const double& q2 ){
  
  Arguments& args = Arguments::ref();
  
  if( q2 != 0.0 ){
    f.moment()->setQ2( q2 );
    dump( f, lx );
  } else {
    cout << setw( 4 ) << "Q^2" << setw( 15 ) << "g_1h(N,M)" << flush;
    for( int i = 0; i < lx.size(); i++ ) cout << setw( 12 ) << lx[i] << flush;
    cout << endl;
  }
}

int main( int argc, char* argv[] ) {

  //  create corresponding arg object through static function instance 
  //  in Arguments class     
  Arguments& args = *( Arguments::instance( argc, argv ) );
  
  args.log() 
    << "-----------  polSIDIS structure function test program : " << argv[0] 
    << endl;
  
  
  // ( x, Q^2 ) points to be calculated
  vector< double > lx( 0 );
  double xmin = args.get( "xmin", 0.01 ); 
  double xmax = args.get( "xmax", 1.0  );  
  int nx      = args.get( "nx", 10 );
  double dlx = ( xmax - xmin ) / nx;
  for( int i = 0; i < nx; i++ ) lx.push_back( xmin + dlx * i );
  
  args.log() << "**** Bjorken x:             " << flush;
  args.log() << "  --xmin=" << xmin << flush;
  args.log() << "  --xmax=" << xmax << flush;
  args.log() << "  --nx="   << nx   << endl;
  
  string strq2 = args.get( "Q2", "1.1 2.5 5.5 10.0 50.0 100.0" );
  double q2;
  vector< double > lq2( 0 );
  istringstream istr( strq2 );
  while( istr >> q2 ) lq2.push_back( q2 );
  args.log() << "**** Q2 :                   " << flush;
  args.log() << "  --q2=\"" << flush;
  for( int i = 0; i < lq2.size(); i++ ){
    args.log() << lq2[i] << " " << flush;
  }
  args.log() << "\"" << endl;
  
  // ---  paramters for straight contour ---
  vector< double > angle( 2 );
  vector< double > offset( 2 );
  vector< double > upper( 2 );
  
  angle[ 0 ]  = args.get( "angle1",  0.55 );
  offset[ 0 ] = args.get( "offset1", 2.0 );
  upper[ 0 ]  = args.get( "upper1",  10.0 );
  
  angle[ 1 ]  = args.get( "angle2",  0.55 );
  offset[ 1 ] = args.get( "offset2", 2.0 );
  upper[ 1 ]  = args.get( "upper2",  10.0 );
  
  args.log() << "**** straight contours (x):" << flush;
  args.log() << "   --offset1=" << offset[ 0 ] << flush;
  args.log() <<	"   --angle1="  << angle[ 0 ] << flush;
  args.log() <<	"   --upper1="  << upper[ 0 ] << endl;
  args.log() << "**** straight contours (z):" << flush;
  args.log() << "   --offset2=" << offset[ 1 ] << flush;
  args.log() <<	"   --angle2="  << angle[ 1 ] << flush;
  args.log() <<	"   --upper2="  << upper[ 1 ] << endl;

  int ngrid = args.get( "ngrid", 2 );
  args.log() << "**** number of grid :      " 
	     << "   --ngrid=" << ngrid << endl; 

  vector< double > mom( 2, 0.0 );
  mom[ 0 ] = args.get( "moment1", 1.0 );
  mom[ 1 ] = args.get( "moment2", 1.0 );

  std::vector< double > xx( 1, 0.5 ), zz( 2, 0.2 );
  zz[ 0 ] = args.get( "zmin", 0.2 ) ; 
  zz[ 1 ] = args.get( "zmin", 0.8 ) ;
  
  args.log() << "**** moment for integ.:    " << flush;
  args.log() << "   --moment1=" << mom[ 0 ] << flush;
  args.log() << "   --moment2=" << mom[ 1 ] << flush;
  args.log() << "   --zmin=" << zz[ 0 ] << flush;
  args.log() << "   --zmax=" << zz[ 1 ] << endl;
  
  
  int nLeg1 = args.get( "leg1", 6 );
  int nLeg2 = args.get( "leg2", 8 );
  double precision = args.get( "precision", 5.0E-3 );
  
  args.log() << "**** Mellin inversion:      " << flush;
  args.log() << "  --precision=" << precision << flush; 
  args.log() << "  --leg1=" << nLeg1 << flush;
  args.log() << "  --leg2=" << nLeg2 << endl;
  
  // ----------------- set moment parameters for partial integration
  complex< double > n = args.get( "N", complex< double >( 2.0, 0.0 ) );
  complex< double > m = args.get( "M", complex< double >( 2.0, 0.0 ) );
  args.log() << "**** moment setting         " << flush;
  args.log() << "  --N=" << n << flush;
  args.log() << "  --M=" << m << endl;
  
  args.log() << endl; 
  args.log() << endl; 

  try {
    
    // -----------------    structure functions   ----------------------
    polSIDIS::StrFunc g1hp( args, Xsection::Charge::p );
    polSIDIS::StrFunc g1hd( args, Xsection::Charge::d );
    polSIDIS::StrFunc g1hn( args, Xsection::Charge::n );
    
    polSIDIS::StrFunc& g1h = g1hp;
    
    // -----------------   Mellin Inversion -------------------------
    Xsection::xzXsec g1hx(  &g1h, nLeg1, nLeg2, precision );
    g1hx.setParameter( offset, angle );
    g1hx.upper( upper );
    g1hx.setMomParam( mom );
    dynamic_cast< Transform::GridIntegration* >( g1hx.integration() )->ngrid( ngrid );
    dynamic_cast< Transform::GridIntegration* >( g1hx.integrand().integration() )->ngrid( ngrid );

    Xsection::xXsec  g1hx2( &g1h, nLeg1, nLeg2, precision );
    g1hx2.setParameter( offset, angle );
    g1hx2.upper( upper );
    g1hx2.setMomParam( mom );
    dynamic_cast< Transform::GridIntegration* >( g1hx2.integration() )->ngrid( ngrid );
    
//    Xsection::xzXsec g1hx( &g1h, 4, 6, 1.0E-2, 10.0, 2.0, 0.75); // LO, NLO
//    Xsection::xzXsec g1hx( &g1h, 4, 6, 1.0E-2, 20.0, 2.0, 0.55); // LO
//    Xsection::xzXsec g1hx( &g1h, 4, 6, 1.0E-4, 10.0, 2.0, 0.60);
//    Xsection::xzXsec g1hx( &g1h, 4, 6, 1.0E-2, 10.0, 1.8, 0.55);
//    Xsection::xzXsec g1hx( &g1h, 4, 6, 1.0E-4, 10.0, 1.8, 0.65);
/*    cout << " Q2= " << g1h.q2() << flush;
      cout << " " << g1h.xsecComp()->vevo().size() << flush;
      for( int i = 0; i < g1h.xsecComp()->vevo().size(); i++ )
      cout << " Mu2= " << g1h.xsecComp()->vevo()[i]->Q2() << flush;
    cout << endl;
*/
    
//    if( 0 )
    {
      cout << "----- InverseMellin / DInverseMellin ----- " << endl;
      dump( g1hx2, lx, 0.0 );
      for( int iq2 = 0; iq2 < lq2.size(); iq2++ ){
	dump( g1hx2, lx, lq2[ iq2 ] );
	dump( g1hx, lx, lq2[ iq2 ] );
	cout << endl;
      } 
      cout << endl;
    }
    
    if( 0 )
    {
      
      // ---------------- PDF and FF
      KernelBase* ker_ = g1h.xsecComp()->vevo()[ 0 ];
      Flavor::PARTON q_ =  Flavor::PARTON( 0 );
      PDFx pdfg( ker_, q_ ); pdfg.setParameter( 2.0, 0.50 );
      PDF  gpdf( ker_, q_ );
      
      KernelBase* kerr_ = g1h.xsecComp()->vevo()[ 1 ];
      Flavor::PARTON qr_ =  Flavor::PARTON( 0 );
      PDFx pdfgr( kerr_, qr_ ); pdfgr.setParameter( 2.0, 0.50 );
      PDF  gpdfr( kerr_, qr_ );
      
      cout << setw(4)  << "Q^2" 
	   << setw(15) << "g_1h (N=2, M=2)" ;
      for( int i = 0; i < lx.size(); i++ ) 
	cout << setw(12) << lx[ i ] << flush;
      cout << endl;
      
      for( int iq2 = 0; iq2 < lq2.size(); iq2++ ){
	
	g1h.setQ2( lq2[ iq2 ] );
	
	Timer timer;
	
	{
	  cout << setw(4)  << g1h.q2() << setw(15) << g1h( n, m ) << flush;
	  timer.start();
	  for( int i = 0; i < lx.size(); i++ ){
	    xx[ 0 ] = lx[i];
	    cout << setw(12) 
		 << 0.5 * xx[ 0 ] * g1hx( xx, zz ) * ( zz[ 1 ] - zz[ 0 ] )
		 << flush;
	  }
	  timer.stop();
	  cout << setw(12) << timer.check( Timer::Sec ) << " sec" << endl;
	}
	
	{
	  cout << setw(4)  << "PDF1" << setw(15) << gpdf( m ) << flush;
	  timer.start();
	  for( int i = 0; i < lx.size(); i++ )
	    cout << setw(12) << lx[ i ] * pdfg( lx[ i ] ) << flush;
	  timer.stop();
	  cout << setw(12) << timer.check( Timer::Sec ) << " sec" << endl;
	}
	
	
	{
	  cout << setw(4) << "PDF2" << setw(15) << gpdfr( m ) << flush;
	  timer.start();
	  for( int i = 0; i < lx.size(); i++ )
	    cout << setw(12) << lx[i] * pdfgr( lx[i] ) << flush;
	  timer.stop();
	  cout << setw(12) << timer.check( Timer::Sec ) << " sec" << endl;
	}
	
	cout << endl;
      }
    }
  }
  
  catch ( int error ) {
  }
  
  return 0;
}
