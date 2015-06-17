//
// diff.cc
//  
// Made by Yoshiyuki Miyachi
// Login   <miyachi@yoshi-desktop>
// 
// Started on  Mon Feb 16 11:54:00 2009 Yoshiyuki Miyachi
// Started on  Mon Feb 16 11:54:00 2009 Yoshiyuki Miyachi
//

#include <iostream>
#include <iomanip>

#include <Utility/Timer.hh>
#include <Evolution/KernelDiff.hh>
#include <Fragmentation/Evolution.hh>
#include <polPDF/Evolution.hh>

#include <Xsec/xXsec.hh>

#include "StrFunc.hh"

using namespace std;


void configure( Xsection::xXsec *xsec ){
  
  Utility::Arguments& args = Utility::Arguments::ref();

  vector< double > off( 2, 2.0 );
  vector< double > ang( 2, 0.55 );
  vector< double > len( 2, 8.0 );
  
  off = args.get( "offset", ",", off );
  ang  = args.get( "angle", ",",  ang );
  len = args.get( "length", ",", len );
  
  xsec->setParameter( off, ang );
  xsec->upper( len );
  
}



int main( int argc, char* argv[] ){
  
  Utility::Arguments& args = Utility::Arguments::ref( argc, argv );
  
  polPDF::Evo *pdf1 = NULL;
  polPDF::Evo *pdf2 = NULL;
  Evolution::KernelDiff *dpdf = NULL;
  
  Fragmentation::Evo *ff = NULL;
  
  polSIDIS::StrFunc *f1 = NULL;
  polSIDIS::StrFunc *f2 = NULL;
  polSIDIS::StrFunc *df = NULL;
  
  Xsection::xXsec *xf1 = NULL;
  Xsection::xXsec *xf2 = NULL;
  Xsection::xXsec *dxf = NULL;

  
  vector< double > q2 ( 1, 1.0 );
  q2 = args.get( "Q2", ",", q2 );
  
  vector< double > x ( 1, 0.1 );
  x = args.get( "x", ",", x );
  
  try {
    
    pdf1 = new polPDF::Evo( args );
    pdf2 = new polPDF::Evo( args );
    
    
    string pname = args.get( "pdf", "gluon" );
    int    index = args.get( "index", 1 );

    
    QCD::PdfBase* pbase = pdf1->initialDistribution()->find( pname );
    if( pbase ){
      
      QCD::PdfBase::iterator itr = pbase->find( index );
      if( itr != pbase->end() ){
	(*pbase)[ 1 ].value() *= 0.8;
	pbase->update();
	pdf1->clearCache();
      }
      
    }
    
    dpdf = new Evolution::KernelDiff( pdf1, pdf2 );
    
    ff   = new Fragmentation::Evo( args );
    
    f1 = new polSIDIS::StrFunc( pdf1, ff );
    f2 = new polSIDIS::StrFunc( pdf2, ff );
    df = new polSIDIS::StrFunc( dpdf, ff );
    
    xf1 = new Xsection::xXsec( f1, 6, 8, 1.0E-4 );
    xf2 = new Xsection::xXsec( f2, 6, 8, 1.0E-4 );
    dxf = new Xsection::xXsec( df, 6, 8, 1.0E-2 );

    configure( xf1 );
    configure( xf2 );
    configure( dxf );
    
    vector< double > vz( 2 );
    vz[ 0 ] = 0.2;
    vz[ 1 ] = 0.8;
    
    Utility::Timer timer;
    
    for( int iq2 = 0; iq2 < q2.size(); iq2++ ){
      
      f1->setQ2( q2[ iq2 ] );
      f2->setQ2( q2[ iq2 ] );
      df->setQ2( q2[ iq2 ] );
      
      ARG_LOG << "Q^2 = " << setw(6) << q2[ iq2 ] << endl;
      
      vector< double > res( x.size(), 0.0 );
      

      ARG_LOG << setw( 6 ) << "x" << flush;
      for( int ix = 0; ix < x.size(); ix++ ){
	ARG_LOG << setw(14) << x[ ix ] << flush;
      }
      ARG_LOG << setw(14) << "time (msec)" << flush;
      ARG_LOG << setw(10)  << "cache" << flush;
      ARG_LOG << endl;
      
      ARG_LOG << setw( 6 ) << "-----" << flush;
      for( int ix = 0; ix < x.size(); ix++ ){
	ARG_LOG << setw(14) << "---------" << flush;
      }
      ARG_LOG << endl;
      
      ARG_LOG << setw( 6 ) << "F1" << flush;
      timer.start();
      for( int ix = 0; ix < x.size(); ix++ ){
	vector< double > vx( 1, x[ ix ] );
	double val = (*xf1)( vx, vz );
	res[ ix ] = val;
	ARG_LOG << setw(14) << val << flush;
      }
      timer.stop();
      ARG_LOG << setw(14) << timer.check( Utility::Timer::Msec ) << flush;
      ARG_LOG << setw(10) << f1->cache2().size() << flush;
      ARG_LOG << endl;
      
      ARG_LOG << setw( 6 ) << "F2" << flush;
      timer.start();
      for( int ix = 0; ix < x.size(); ix++ ){
	vector< double > vx( 1, x[ ix ] );
	double val = (*xf2)( vx, vz );
	res[ ix ] -= val;
	ARG_LOG << setw(14) << val << flush;
      }
      timer.stop();
      ARG_LOG << setw(14) << timer.check( Utility::Timer::Msec ) << flush;
      ARG_LOG << setw(10) << f2->cache2().size() << flush;
      ARG_LOG << endl;
      
      ARG_LOG << setw( 6 ) << "dF" << flush;
      timer.start();
      for( int ix = 0; ix < x.size(); ix++ ){
	vector< double > vx( 1, x[ ix ] );
	double val = (*dxf)( vx, vz );
	res[ ix ] -= val;
	ARG_LOG << setw(14) << val << flush;
      }
      timer.stop();
      ARG_LOG << setw(14) << timer.check( Utility::Timer::Msec ) << flush;
      ARG_LOG << setw(10) << df->cache2().size() << flush;
      ARG_LOG << endl;
      
      ARG_LOG << setw( 6 ) << "res" << flush;
      for( int ix = 0; ix < res.size(); ix++ ){
	ARG_LOG << setw(14) << res[ ix ] << flush;
      }
      ARG_LOG << endl;
      
      
      ARG_LOG << endl;
    }



  }
  
  catch ( int error ) {
    return 1;
  }
  
  if( pdf1 ) delete pdf1;
  if( pdf2 ) delete pdf2;
  if( dpdf ) delete dpdf;
  
  if( ff )   delete ff;
  
  if( f1 ) delete f1;
  if( f2 ) delete f2;
  if( df ) delete df;
  
  if( xf1 ) delete xf1;
  if( xf2 ) delete xf2;
  if( dxf ) delete dxf;
  

  return 0;
}
