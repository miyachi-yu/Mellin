/*!
  \file  testF1h.cc
  \brief test program for polarized structure functions

  This is a test and an example code for polarized structure functions
  implemented in libunpolSIDIS.so.

  You may get some feeling how to use polSIDIS::StructureFunction class.

*/
#include <Utility/Timer.hh>
#include <Xsec/xzXsec.hh>
#include <Xsec/xXsec.hh>
#include <Xsec/CKernel.hh>
#include <Evolution/PDFx.hh>

#include <Tranform/GridIntegration.hh>

#include <cstdlib>
#include <iostream>
#include <iomanip>

#include "StrFunc.hh"
#include "XsecComp.hh"

using namespace std;
using namespace QCD;
using namespace Evolution;
using namespace Utility;

int main( int argc, char* argv[] ) {
  
  Arguments& args = *( Arguments::instance( argc, argv ) );

  // ------------------------------------------------------------ //
  //                  Grid integration max depth                  //
  // ------------------------------------------------------------ //
  ARG_LOG << "Set Max depth to be " 
	  << ( Transform::GridIntegration::maxDepth_ =
	       args.get( "maxDepth", Transform::GridIntegration::maxDepth_ ) )
	  << endl;
  
  // Dimension for LegQuadrature
  vector< int > leg( 2 );
  leg[ 0 ] = 4;
  leg[ 1 ] = 6;
  leg = args.get( "leg", ",", leg );
  if( leg.size() < 2 ){
    ARG_ERR << "--leg should have two integer numbers. ex.) --leg=4,6" << endl;
    return 0;
  }
  
  // precision of mellin inversion
  double precision = args.get( "precision", 1.0E-3 );
  
  try {
    
    Timer timer;
    
    double q2 = args.get( "Q2", 1.0 );
    
    // --------------------------------------------------------------
    //                       new implementation
    // --------------------------------------------------------------
    unpolSIDIS::StrFunc f1h( args, Xsection::Charge::p );
    //    unpolSIDIS::StrFunc f1h( args, Xsection::Charge::d );
    f1h.setQ2( q2 );                        // Do not forget to set Q2
    //    Xsection::xzXsec f1hx( &f1h, leg[0], leg[1], 1.0E-4, 10.0, 2.0, 0.65);
    //    Xsection::xzXsec f1hx( &f1h, leg[0], leg[1], precision, 10.0, 2.0, 0.55); // LO
    Xsection::xXsec f1hx( &f1h, leg[0], leg[1], precision, 10.0, 2.0, 0.55); // LO
    
    //    Xsection::xzXsec f1hx( &f1h, leg[0], leg[1], 1.0E-2, 5.0, 2.0, 0.75); // LO
    //    Xsection::xzXsec f1hx( &f1h, leg[0], leg[1], 1.0E-2, 10.0, 2.0, 0.60);
    //    Xsection::xzXsec f1hx( &f1h, leg[0], leg[1], 1.0E-2, 10.0, 1.8, 0.55);
    //    Xsection::xzXsec f1hx( &f1h, leg[0], leg[1], 1.0E-4, 10.0, 1.8, 0.65);
    /*    cout << " Q2= " << f1h.q2() << flush;
	  cout << " " << f1h.xsecComp()->vevo().size() << flush;
	  for( int i = 0; i < f1h.xsecComp()->vevo().size(); i++ )
	  cout << " Mu2= " << f1h.xsecComp()->vevo()[i]->Q2() << flush;
	  cout << endl;*/
    
    std::vector< double > mom( 2, 0.0 );
    mom[ 1 ] = 1.0;
    f1hx.setMomParam( mom );
    // MellinInversion
    KernelBase* ker_ = f1h.xsecComp()->vevo()[ 0 ];
    Flavor::PARTON q_ =  Flavor::PARTON( 1 );
    PDFx pdfg( ker_, q_ ); pdfg.setParameter( 2.0, 0.50 );
    PDF  gpdf( ker_, q_ );
    KernelBase* kerr_ = f1h.xsecComp()->vevo()[ 1 ];
    Flavor::PARTON qr_ =  Flavor::PARTON( 1 );
    PDFx pdfgr( kerr_, qr_ ); pdfgr.setParameter( 2.0, 0.50 );
    PDF  gpdfr( kerr_, qr_ );
    
    // --------------------------------------------------------------
    //     performance in moment calculation
    // --------------------------------------------------------------

    complex< double > n( 2.0, 0.0 ), m( 2.0, 0.0 );
//    double xmin = 0.01; double xmax = 1.0;
    double xmin = 0.01; double xmax = 1.0;
    int nx = 10;
//    double dlx = ( log10( xmax ) - log10( xmin ) ) / nx;
    double dlx = ( xmax - xmin ) / nx;
//    std::vector< double > xx_( 1, 0.5 ), zz_( 1, 0.5 );
    std::vector< double > xx_( 1, 0.5 ), zz_( 2, 0.2 );
    zz_[ 0 ] = 0.8; zz_[ 1 ] = 0.2;
//    zz_[ 0 ] = 0.9; zz_[ 1 ] = 0.2;
//    zz_[ 0 ] = 0.758577575029184; zz_[ 1 ] = 0.204173794466953;

//    cout << f1hx( xx_, zz_ ) << endl;

    for( double Q2 = 0.0; Q2 < 10.0; Q2 += 1.1 ){
//    for( double Q2 = 0.0; Q2 < 1000.0; Q2 += 1.0 ){
      
      if( Q2 == 0.0 ){
	cout << setw(4)  << "Q^2" 
	     << setw(15) << "g_1h (N=2, M=2)" ;
	for( int i = 0; i < nx; i++ ) {
//	  cout << setw(12) << pow( 10.0, log10( xmin ) + dlx * i ) << flush;
	  cout << setw(12) << ( xmin + dlx * i ) << flush;
	}
	  cout << endl;
      } else {
	
	f1h.setQ2( Q2 );
	//    cout << f1h.xsecComp()->coeff()->nf() << endl;
	/*	cout << " Q2= " << f1h.q2() << flush;
	cout << " " << f1h.xsecComp()->vevo().size() << flush;
	for( int i = 0; i < f1h.xsecComp()->vevo().size(); i++ )
	     cout << " Mu2= " << f1h.xsecComp()->vevo()[i]->Q2() << flush;
	     cout << endl;*/

	// --------------------------------------------------------- //
	//    performance in x space :  new implementation
	// --------------------------------------------------------- //
	{
	  // 1st moment

	  cout << setw(4) << Q2 
	       << setw(15) << f1h( n, m ) << flush;

	  timer.start();
	  for( int i=0; i < nx; i++ ){
	    //	    double x = pow( 10.0, log10( xmin ) + dlx * i );
	    double x = xmin + dlx * i ;
	    xx_[ 0 ] = x; //zz_[ 0 ] = x;
	    cout << setw(12) << x * f1hx( xx_, zz_ ) * ( zz_[ 0 ] - zz_[ 1 ] )
	      //	    cout << setw(12) << x * f1hx( xx_, zz_ )
	      / 2.0 << flush;
	    //		 << flush;
	  }
	  timer.stop();
	  
	  double msec = timer.check( Timer::Msec );
	  cout << setw(12) << msec / 1000.0 << " sec";
	  cout << endl;
	}
	
	if( args.hasOpt( "viewPDF" ) )
	{
	  cout << setw(4) << "PDF"
	       << setw(15) << gpdf( m ) << flush;
	  
	  timer.start();
	  for( int i=0; i < nx; i++ ){
	    //	    double x = pow( 10.0, log10( xmin ) + dlx * i );
	    double x = xmin + dlx * i ;
	    cout << setw(12) << x * pdfg( x ) << flush;
	  }
	  timer.stop();
	  
	  double msec = timer.check( Timer::Msec );
	  cout << setw(12) << msec / 1000.0 << " sec";
	  
	  cout << endl;
	}
	
	
	if( args.hasOpt( "viewFF" ) )
	{
	  cout << setw(4) << "FF"
	       << setw(15) << gpdfr( m ) << flush;
	  
	  timer.start();
	  for( int i=0; i < nx; i++ ){
	    //	    double x = pow( 10.0, log10( xmin ) + dlx * i );
	    double x = xmin + dlx * i ;
	    cout << setw(12) << x * pdfgr( x ) << flush;
	  }
	  timer.stop();
	  
	  double msec = timer.check( Timer::Msec );
	  cout << setw(12) << msec / 1000.0 << " sec";
	  
	  cout << endl;
	}
	
      }
      
    }
  }

  catch ( int error ) {
  }
  
  return 0;
}
