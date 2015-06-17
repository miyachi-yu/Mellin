//
// contour.cc
//  
// Made by Yoshiyuki Miyachi
// Login   <miyachi@yoshiyuki-desktop>
// 
// Started on  Tue Sep 30 14:53:16 2008 Yoshiyuki Miyachi
// Started on  Tue Sep 30 14:53:16 2008 Yoshiyuki Miyachi
//

#include <iostream>
#include "InverseMellinHelper.hh"
#include <Utility/Arguments.hh>

#include <FragXsec_T/FragXsec.hh>
#include <unpolDIS/StrFunc.hh>
#include <unpolSIDIS/StrFunc.hh>
#include <unpolDISXsec/StrFunc.hh>
#include <unpolDISXsec/xXsec.hh>
#include <unpolSIDISXsec/StrFunc.hh>
#include <unpolSIDISXsec/StrFuncSub.hh>
#include <polDIS/StrFunc.hh>
#include <polSIDIS/StrFunc.hh>

#include <Xsec/xXsec.hh>

#include <TApplication.h>

using namespace std;

void Usage(){
  
  Utility::Arguments& args = Utility::Arguments::ref();

  vector< string > FFopts       = Fragmentation::Evo::options( QCD::Flavor::PIp );
  vector< string > FFKopts      = Fragmentation::Evo::options( QCD::Flavor::KAp );
  vector< string > polPDFopts   = polPDF::Evo::options();
  vector< string > unpolPDFopts = unpolPDF::Evo::options();
  
  for( int i = 0; i < FFopts.size(); i++ )
    args.usage( FFopts[ i ], "", "pion fragmentation initial distribution");
  
  for( int i = 0; i < FFKopts.size(); i++ )
    args.usage( FFKopts[ i ], "", "kaon fragmentation initial distribution");
  
  for( int i = 0; i < polPDFopts.size(); i++ )
    args.usage( polPDFopts[ i ], "", "polarized PDF initial distribution");
  
  for( int i = 0; i < unpolPDFopts.size(); i++ )
    args.usage( unpolPDFopts[ i ], "", "unpolarized PDF initial distribution");
  
  args.usage( "mode", "1", "Drawing mode (1-6)" );
  args.usage( "min", "3.0", "Lowest value" );
  args.usage( "max", "10.0", "Highest value" );
  args.usage( "n", "20", "Number of points between min and max" );
  
}

Xsection::Xsec* make_xsec(){
  
  Utility::Arguments& args = Utility::Arguments::ref();
  
  bool isXsec = args.hasOpt( "Xsec" );
  if( isXsec ){
    ARG_LOG << "\t--Xsec=true           Xsection insted of StrFunc " << endl;
  } else {
    ARG_LOG << "\t--Xsec=false          StrFunc " << endl;
  }
  
  //  cross section object preparation
  vector< string > FFopts  = Fragmentation::Evo::options( QCD::Flavor::PIp );
  vector< string > FFKopts = Fragmentation::Evo::options( QCD::Flavor::KAp );
  vector< string > polPDFopts   = polPDF::Evo::options();
  vector< string > unpolPDFopts = unpolPDF::Evo::options();
  
  vector< string >::iterator itrFF   = args.hasOpt( FFopts );
  vector< string >::iterator itrFFK  = args.hasOpt( FFKopts );
  
  vector< string >::iterator itrPol  = args.hasOpt( polPDFopts );
  vector< string >::iterator itrUpol = args.hasOpt( unpolPDFopts );
  
  if( itrUpol != unpolPDFopts.end() ) {
    
    ARG_LOG << "\t--" << *itrUpol << "=" << args.getOpt( *itrUpol ) << endl;
    
    if( itrFF != FFopts.end() ){                               // F1pi
      
      ARG_LOG << "\t--" << *itrFF << "=" << args.getOpt( *itrFF ) << endl;
      if( isXsec ) {
	
	if( args.hasOpt( "XsecSub" ) ){
	  return new unpolSIDISXsec::StrFuncSub( args,
						 args.get( "XsecSub", 0 ),
						 Xsection::Charge::p, QCD::Flavor::PIp );
	} else {
	  return new unpolSIDISXsec::StrFunc( args, 
					      Xsection::Charge::p, QCD::Flavor::PIp );
	}
      } else {
	return new unpolSIDIS::StrFunc( args, 
					Xsection::Charge::p, QCD::Flavor::PIp );
      }      
    } else if( itrFFK != FFKopts.end() ){                      // F1Ka
      
      ARG_LOG << "\t--" << *itrFFK << "=" << args.getOpt( *itrFFK ) << endl;
      if( isXsec ) 
	return new unpolSIDISXsec::StrFunc( args, 
					    Xsection::Charge::p, QCD::Flavor::KAp );
      else 
	return new unpolSIDIS::StrFunc( args, 
					Xsection::Charge::p, QCD::Flavor::KAp );
      
    } else {                                                   // F1
      if( isXsec )
	return new unpolDISXsec::StrFunc( args, Xsection::Charge::p );
      else 
	return new unpolDIS::StrFunc( args, Xsection::Charge::p );
    }
    
  } if( itrPol != polPDFopts.end() ) {
    
    ARG_LOG << "\t--" << *itrPol << "=" << args.getOpt( *itrPol ) << endl;
    
    if( itrFF != FFopts.end() ){                               // g1pi
      ARG_LOG << "\t--" << *itrFF << "=" << args.getOpt( *itrFF ) << endl;
      return new polSIDIS::StrFunc( args, 
				    Xsection::Charge::p, QCD::Flavor::PIp );
      
    } else if( itrFFK != FFKopts.end() ){                      // g1Ka
      ARG_LOG << "\t--" << *itrFFK << "=" << args.getOpt( *itrFFK ) << endl;
      return new polSIDIS::StrFunc( args, 
				    Xsection::Charge::p, QCD::Flavor::KAp );
    } else { // g1
      return new polDIS::StrFunc( args, Xsection::Charge::p );
    }
    
  } else {
    
    if( itrFF != FFopts.end() ){                      // FragXsecPI
      ARG_LOG << "\t--" << *itrFF << "=" << args.getOpt( *itrFF ) << endl;
      new FragXsec_T::FragXsec( args, QCD::Flavor::PIp );
    } else if( itrFFK != FFKopts.end() ){             // FragXsecKA
      ARG_LOG << "\t--" << *itrFFK << "=" << args.getOpt( *itrFFK ) << endl;
      return new FragXsec_T::FragXsec( args, QCD::Flavor::KAp );
    } else {
      // nothing
    }
    
  }
  return NULL;
}


int main( int argc, char* argv[] ){
  
  Utility::Arguments& args = Utility::Arguments::ref( argc, argv );
  if( args.hasOpt( "help" ) ) {
    Usage();
    return 1;
  }
  
  TApplication application( "contour", &argc, argv );

  
  // xsection preparation
  Xsection::Xsec *xsec = make_xsec();
  if( xsec == NULL ){
    Usage();
    return 1;
  }
  
  // scale
  double Q2 = args.get( "Q2", 2.5 );
  ARG_LOG << "\t--Q2=" << Q2 << endl;
  
  // for grid and gauss intergration
  int leg1         = args.get( "leg1", 6 );
  int leg2         = args.get( "leg2", 8 );
  int grid         = args.get( "grid", 2 );
  double precision = args.get( "precision", 5.0E-3 );
  
  Xsection::xXsec* xsecx = NULL;
  if( dynamic_cast< unpolDISXsec::StrFunc* >( xsec ) ){
    xsecx = new unpolDISXsec::xXsec( xsec, leg1, leg2, precision );
  } else {
    xsecx = new Xsection::xXsec( xsec, leg1, leg2, precision );
  }

  xsecx->setGngrid( grid );

  ARG_LOG << "\t--leg1=" << leg1 << endl;
  ARG_LOG << "\t--leg2=" << leg2 << endl;
  ARG_LOG << "\t--grid=" << grid << endl;
  ARG_LOG << "\t--precision=" << precision << endl;

  // Mellin Inversion Helper
  InverseMellinHelper helper;
  helper.set( xsecx );
  helper.Q2( Q2 );

  // Mellin inversion
  vector< double > offset( 0 );
  vector< double > angle( 0 );
  vector< double > length( 0 );
  
  if( dynamic_cast< FragXsec_T::FragXsec* >( xsec ) ){
    
    offset.resize( 1 ); offset[ 0 ] = 3.0;
    angle.resize( 1 );  angle[ 0 ]  = 0.55;
    length.resize( 1 ); length[ 0 ] = 8.0;
    
    helper.add( vector< double >( 1, 0.01 ) );
    helper.add( vector< double >( 1, 0.03 ) );
    helper.add( vector< double >( 1, 0.07 ) );
    helper.add( vector< double >( 1, 0.1 ) );
    helper.add( vector< double >( 1, 0.3 ) );
    helper.add( vector< double >( 1, 0.7 ) );

  } else if( dynamic_cast< unpolDIS::StrFunc* >( xsec ) || 
	     dynamic_cast< polDIS::StrFunc* >( xsec ) ){
    
    offset.resize( 1 ); offset[ 0 ] =  2.0;
    angle.resize( 1 );  angle[ 0 ]  =  0.55;
    length.resize( 1 ); length[ 0 ] = 15.0;
    
    helper.add( vector< double >( 1, 0.001 ) );
    helper.add( vector< double >( 1, 0.003 ) );
    helper.add( vector< double >( 1, 0.007 ) );
    helper.add( vector< double >( 1, 0.01 ) );
    helper.add( vector< double >( 1, 0.03 ) );
    helper.add( vector< double >( 1, 0.07 ) );
    helper.add( vector< double >( 1, 0.1 ) );
    helper.add( vector< double >( 1, 0.3 ) );
    helper.add( vector< double >( 1, 0.7 ) );
    
  } else if( dynamic_cast< unpolSIDIS::StrFunc* >( xsec ) ) {

    offset.resize( 2 ); offset[ 0 ] = 3.0;  offset[ 1 ] =  3.0;
    angle.resize( 2 );  angle[ 0 ]  =  0.55;  angle[ 1 ]  =  0.55; 
    length.resize( 2 ); length[ 0 ] =  8.0;   length[ 1 ] =  8.0;

    helper.add( vector< double >( 1, 0.003 ), vector< double >( 1, 0.06 ) );
    helper.add( vector< double >( 1, 0.03 ),  vector< double >( 1, 0.06 ) );
    helper.add( vector< double >( 1, 0.3 ),   vector< double >( 1, 0.06 ) );
    
    helper.add( vector< double >( 1, 0.003 ), vector< double >( 1, 0.1 ) );
    helper.add( vector< double >( 1, 0.03 ),  vector< double >( 1, 0.1 ) );
    helper.add( vector< double >( 1, 0.3 ),   vector< double >( 1, 0.1 ) );
    
    helper.add( vector< double >( 1, 0.003 ), vector< double >( 1, 0.4 ) );
    helper.add( vector< double >( 1, 0.03 ),  vector< double >( 1, 0.4 ) );
    helper.add( vector< double >( 1, 0.3 ),   vector< double >( 1, 0.4 ) );
    
  } else if( dynamic_cast< polSIDIS::StrFunc* >( xsec ) ){
    
    /*
      X:   length:   11 - 12  for           0.001 < x < 0.01
                      7 -  8  for           0.01  < x < 0.1
                      3 -  4  for           0.1   < x < 1

      Z:   length:    8 -  9  for           all z range 

     */
    offset.resize( 2 ); offset[ 0 ] =   2.0;  offset[ 1 ] =  2.0;
    angle.resize( 2 );  angle[ 0 ]  =  0.55;  angle[ 1 ]  =  0.55; 
    length.resize( 2 ); length[ 0 ] =   8.0;  length[ 1 ] =  8.0;
    
    helper.add( vector< double >( 1, 0.003 ), vector< double >( 1, 0.06 ) );
    helper.add( vector< double >( 1, 0.03 ),  vector< double >( 1, 0.06 ) );
    helper.add( vector< double >( 1, 0.3 ),   vector< double >( 1, 0.06 ) );
    
    helper.add( vector< double >( 1, 0.003 ), vector< double >( 1, 0.1 ) );
    helper.add( vector< double >( 1, 0.03 ),  vector< double >( 1, 0.1 ) );
    helper.add( vector< double >( 1, 0.3 ),   vector< double >( 1, 0.1 ) );
    
    helper.add( vector< double >( 1, 0.003 ), vector< double >( 1, 0.4 ) );
    helper.add( vector< double >( 1, 0.03 ),  vector< double >( 1, 0.4 ) );
    helper.add( vector< double >( 1, 0.3 ),   vector< double >( 1, 0.4 ) );

  } else if( dynamic_cast< unpolDISXsec::StrFunc* >( xsec ) ){
    

    /*!
      Length:     6
      Offset:     3.5
      Angle:      0.6
     */
    
    offset.resize( 1 ); offset[ 0 ] =  3.5;
    angle.resize( 1 );  angle[ 0 ]  =  0.60;
    length.resize( 1 ); length[ 0 ] =  6.0;
    
    helper.add( vector< double >( 1, 0.001 ) );
    helper.add( vector< double >( 1, 0.003 ) );
    helper.add( vector< double >( 1, 0.007 ) );
    helper.add( vector< double >( 1, 0.01 ) );
    helper.add( vector< double >( 1, 0.03 ) );
    helper.add( vector< double >( 1, 0.07 ) );
    helper.add( vector< double >( 1, 0.1 ) );
    helper.add( vector< double >( 1, 0.3 ) );
    helper.add( vector< double >( 1, 0.7 ) );

  } else if( dynamic_cast< unpolSIDISXsec::StrFunc* >( xsec ) ){
    
    // default setting
    offset.resize( 2 ); offset[ 0 ] =   3.4;  offset[ 1 ] =  3.0;
    angle.resize( 2 );  angle[ 0 ]  =  0.60;  angle[ 1 ]  =  0.60; 
    length.resize( 2 ); length[ 0 ] =   6.5;  length[ 1 ] =  7.5;
    
    /*
      0.1   <   x   <   1.0      :   length = 5.5
      0.01  <   x   <   0.1      :   length = 6.5
      0.001 <   x   <  0.01      :   length = 7.5
    */
    helper.add( vector< double >( 1, 0.005 ), vector< double >( 1, 0.1 ) );
    helper.add( vector< double >( 1, 0.005 ), vector< double >( 1, 0.7 ) );
    helper.add( vector< double >( 1, 0.01 ),  vector< double >( 1, 0.1 ) );
    helper.add( vector< double >( 1, 0.01 ),  vector< double >( 1, 0.7 ) );
    helper.add( vector< double >( 1, 0.05 ),  vector< double >( 1, 0.1 ) );
    helper.add( vector< double >( 1, 0.05 ),  vector< double >( 1, 0.7 ) );
    helper.add( vector< double >( 1, 0.1 ),   vector< double >( 1, 0.1 ) );
    helper.add( vector< double >( 1, 0.1 ),   vector< double >( 1, 0.7 ) );
    helper.add( vector< double >( 1, 0.5 ),   vector< double >( 1, 0.1 ) );
    helper.add( vector< double >( 1, 0.5 ),   vector< double >( 1, 0.7 ) );
    
  } else if( dynamic_cast< unpolSIDISXsec::StrFuncSub* >( xsec ) ){
    
    int sub = args.get( "XsecSub", 0 );
    
    switch( sub ){
      
    case 0:
      
      offset.resize( 2 ); offset[ 0 ] =   3.0;  offset[ 1 ] =  3.0;
      angle.resize( 2 );  angle[ 0 ]  =  0.55;  angle[ 1 ]  =  0.55; 
      length.resize( 2 ); length[ 0 ] =   8.0;  length[ 1 ] =  10.0;

      break;
      
    case 1:
      
      offset.resize( 2 ); offset[ 0 ] =   2.5;  offset[ 1 ] =  3.0;
      angle.resize( 2 );  angle[ 0 ]  =  0.55;  angle[ 1 ]  =  0.55; 
      length.resize( 2 ); length[ 0 ] =   6.0;  length[ 1 ] =  10.0;
      
      break;

    case 2:
      
      offset.resize( 2 ); offset[ 0 ] =   3.5;  offset[ 1 ] =  3.0;
      angle.resize( 2 );  angle[ 0 ]  =  0.55;  angle[ 1 ]  =  0.55; 
      length.resize( 2 ); length[ 0 ] =   6.0;  length[ 1 ] =  10.0;
      
      break;

    }

    /*
      0.1   <   x   <   1.0      :   length = 5.5
      0.01  <   x   <   0.1      :   length = 6.5
      0.001 <   x   <  0.01      :   length = 7.5
    */
    //    helper.add( vector< double >( 1, 0.005 ), vector< double >( 1, 0.1 ) );
    //    helper.add( vector< double >( 1, 0.005 ), vector< double >( 1, 0.7 ) );
    //    helper.add( vector< double >( 1, 0.01 ),  vector< double >( 1, 0.1 ) );
    helper.add( vector< double >( 1, 0.01 ),  vector< double >( 1, 0.7 ) );
    helper.add( vector< double >( 1, 0.05 ),  vector< double >( 1, 0.1 ) );
    helper.add( vector< double >( 1, 0.05 ),  vector< double >( 1, 0.7 ) );
    helper.add( vector< double >( 1, 0.1 ),   vector< double >( 1, 0.1 ) );
    helper.add( vector< double >( 1, 0.1 ),   vector< double >( 1, 0.7 ) );
    helper.add( vector< double >( 1, 0.5 ),   vector< double >( 1, 0.1 ) );
    helper.add( vector< double >( 1, 0.5 ),   vector< double >( 1, 0.7 ) );

  }

  // apply Mellin inversion setting 
  offset = args.get( "offset", ",", offset );
  angle  = args.get( "angle", ",",  angle );
  length = args.get( "length", ",", length );
  
  ARG_LOG << "\t--angle=" ;
  for( int i = 0; i < angle.size(); i++ ) {
    ARG_LOG << angle[ i ] << ( i != angle.size() -1 ? "," : "" );
  }
  ARG_LOG << endl;
  
  ARG_LOG << "\t--offset=" ;
  for( int i = 0; i < offset.size(); i++ ) 
    ARG_LOG << offset[ i ] << ( i != offset.size() -1 ? "," : "" );
  ARG_LOG << endl;

  ARG_LOG << "\t--length=" ;
  for( int i = 0; i < length.size(); i++ ) 
    ARG_LOG << length[ i ] << ( i != length.size() -1 ? "," : "" );
  ARG_LOG << endl;

  xsecx->setParameter( offset, angle );
  xsecx->upper( length );

  // -------------------------------------------------------------------- //
  ARG_LOG << "# --------------------------------------------------------- #" << endl;
  if( dynamic_cast< FragXsec_T::FragXsec* >( xsec ) ){
    ARG_LOG << "\tHadron Inclusive Cross-section" << endl;
  } else if ( dynamic_cast< unpolDIS::StrFunc* >( xsec ) ) {
    ARG_LOG << "\tUnpol Inclusive DIS" << endl;
  } else if ( dynamic_cast< polDIS::StrFunc* >( xsec ) ) {
    ARG_LOG << "\tPol Inclusive DIS" << endl;
  } else if ( dynamic_cast< unpolSIDIS::StrFunc* >( xsec ) ) {
    ARG_LOG << "\tUnpol Semi-Inclusive DIS" << endl;
  } else if ( dynamic_cast< polSIDIS::StrFunc* >( xsec ) ) {
    ARG_LOG << "\tPol Semi-Inclusive DIS" << endl;
  } else if ( dynamic_cast< unpolDISXsec::StrFunc* >( xsec ) ) {
    ARG_LOG << "\tUnpol Inclusive DIS Xsection" << endl;
  } else if ( dynamic_cast< unpolSIDISXsec::StrFunc* >( xsec ) ) {
    ARG_LOG << "\tUnpol Inclusive SIDIS Xsection" << endl;
  } else if ( dynamic_cast< unpolSIDISXsec::StrFuncSub* >( xsec ) ) {
    ARG_LOG 
      << "\tUnpol Inclusive SIDIS Xsection (subpart:"
      << args.get( "XsecSub", 0 ) << ") " << endl;
  }

  // 
  int    mode =  args.get( "mode", 1 );
  double min = 0.0 ;
  double max = 10.0;
  
  switch( mode ){
  case 1: // length
  case 2:
    min = args.get( "min",  2.0 );
    max = args.get( "max", 14.0 );
    break;
  case 3: // angle
  case 4:
    min = args.get( "min",  0.5 );
    max = args.get( "max",  0.65 );
    break;
  case 5:  // offset
  case 6:
    min = args.get( "min",  1.2 );
    max = args.get( "max",  4.0 );
    break;
  }
  
  int n   = args.get( "n", 20 );
  ARG_LOG << "\t--mode=" << mode << "         " << flush;
  switch( mode ){
  case 1:  ARG_LOG << "Length in X" << endl; break;
  case 2:  ARG_LOG << "Length in Z" << endl; break;
  case 3:  ARG_LOG << "Angle in X"  << endl; break;
  case 4:  ARG_LOG << "Angle in Z"  << endl; break;
  case 5:  ARG_LOG << "Offset in X" << endl; break;
  case 6:  ARG_LOG << "Offset in Z" << endl; break;
  }
  ARG_LOG << "\t--min=" << min << endl;
  ARG_LOG << "\t--max=" << max << endl;
  ARG_LOG << "\t--n=" << n << endl;

  ARG_LOG << "# --------------------------------------------------------- #" << endl;

  ARG_LOG << endl;

  helper.draw( min, max, n );
  
  // clean up objects
  if( xsecx ) delete xsecx;
  if( xsec ) delete xsec;
  
  return 0;
}
