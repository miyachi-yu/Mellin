// $Id: DSSFFParam.cc,v 1.1 2009/07/28 04:07:31 miyachi Exp $
/*!
  \file  PdfParameterization.cc
  \brief class implementation of PDF parameterization
 */
#include "DSSFFParam.hh"
#include "DSSFF.hh"

#include <sstream>
#include <cstdlib>
#include <iomanip>

extern "C" {
#include <libgen.h>
}

using namespace std;
using namespace PDFSET;

DSSFFParam::DSSFFParam() : 
  QCD::PdfParametrization(), h_( QCD::Flavor::PIp )
{
}

DSSFFParam::DSSFFParam( DSSFFParam& conf ) : 
  QCD::PdfParametrization( conf ), h_( conf.h_ )
{
}

DSSFFParam::DSSFFParam( ConfigDSSFF& conf ) : 
  QCD::PdfParametrization(), h_( QCD::Flavor::PIp )
{
  this->path(  conf.path()  );
  this->name(  conf.name()  );
  this->scale( conf.scale() );
  this->pdfs(  conf.pdfs()  );
}

DSSFFParam::~DSSFFParam() {
}

DSSFFParam& DSSFFParam::operator<<( ConfigDSSFF& conf ){
  this->path(  conf.path()  );
  this->name(  conf.name()  );
  this->scale( conf.scale() );
  this->pdfs(  conf.pdfs()  );
  return *this;
}

bool DSSFFParam::check(){
  
  this->assign();
  
  // gluon distribution is always required....
  if( ! g_ ){
    cerr << __FILE__ << ":" << __LINE__
	 << "\tgluon is missing" << endl;
    return false;
  }
  
  // check individual quark flavor
  if( U_ && ubar_ && D_ && d_ && S_ && C_ && B_ ){
    
    h_ = QCD::Flavor::PIp;
    
  } else if( U_ && ubar_ && S_ && s_ && D_ && C_ && B_ ) {
    
    h_ = QCD::Flavor::KAp;
    
  } else if( U_ && ubar_ && D_ && dbar_ && S_ && C_ && B_ ) {
    
    h_ = QCD::Flavor::PRp;
    
  } else if( U_ && ubar_ && C_ && B_ ) {
    
    h_ = QCD::Flavor::REp;
    
  } else {
    
    cerr 
      << "DSSFF requires g, U, u-bar, D, d, S, C, B for pion\n"
      << "               g, U, u-bar, D, S, s, C, B for kaon\n"
      << "               g, U, u-bar, D, dbar, S, C, B for proton\n"
      << "U:      " << ( U_ ? "OK" : "NO" ) << "\n"
      << "u:      " << ( u_ ? "OK" : "NO" ) << "\n"
      << "u-bar:  " << ( ubar_ ? "OK" : "NO" ) << "\n"
      << "D:      " << ( D_ ? "OK" : "NO" ) << "\n"
      << "d:      " << ( d_ ? "OK" : "NO" ) << "\n"
      << "d-bar:  " << ( dbar_ ? "OK" : "NO" ) << "\n"
      << "S:      " << ( S_ ? "OK" : "NO" ) << "\n"
      << "s:      " << ( s_ ? "OK" : "NO" ) << "\n"
      << "C:      " << ( C_ ? "OK" : "NO" ) << "\n"
      << "B:      " << ( B_ ? "OK" : "NO" ) << "\n"
      << endl;
    
    return false;
  }
  
  flavors_ = true;
  return true;
}

complex< double > DSSFFParam::q( const QCD::Flavor::PARTON& q ) {
  
  static double c3 = 1.0 / 3.0;
  
  // ---------------  gluon and heavy flavors --------------- //
  switch( q ){
  case QCD::Flavor::g : return (*g_)( n_ );
    
  case QCD::Flavor::cbar : 
  case QCD::Flavor::c :   return 0.5 * (*C_)( n_ );
    
  case QCD::Flavor::bbar : 
  case QCD::Flavor::b :   return 0.5 * (*B_)( n_ );
    
  case QCD::Flavor::u:    return (*U_)( n_ ) - (*ubar_)( n_ );
  case QCD::Flavor::ubar: return (*ubar_)( n_ );
    
  case QCD::Flavor::uns: return (*U_)( n_ ) - c3 * this->q( QCD::Flavor::a0 );
  case QCD::Flavor::dns: return ( h_ == QCD::Flavor::REp ? 
				  (*U_)( n_ ) - c3 * this->q( QCD::Flavor::a0 ) : 
				  (*D_)( n_ ) - c3 * this->q( QCD::Flavor::a0 ) ); 
  case QCD::Flavor::sns: return ( h_ == QCD::Flavor::REp ? 
				  (*U_)( n_ ) - c3 * this->q( QCD::Flavor::a0 ) : 
				  (*S_)( n_ ) - c3 * this->q( QCD::Flavor::a0 ) );
  case QCD::Flavor::uv: return (*U_)( n_ ) - 2.0 * (*ubar_)( n_ );
    
  }  
  
  if( h_ == QCD::Flavor::PIp ){
    switch( q ){
    case QCD::Flavor::d:    return (*d_)( n_ ) ;
    case QCD::Flavor::dbar: return (*D_)( n_ ) - (*d_)( n_ ) ;
    case QCD::Flavor::s:
    case QCD::Flavor::sbar: return 0.5 * ( *S_ )( n_ ) ;
    case QCD::Flavor::dv:   return 2.0 * ( *d_ )( n_ ) - ( *D_ )( n_ ) ;
    case QCD::Flavor::sv:   return complex< double >( 0.0, 0.0 );
    }
  } else if( h_ == QCD::Flavor::KAp ) {
    switch( q ){
    case QCD::Flavor::d:
    case QCD::Flavor::dbar: return 0.5 * (*D_)( n_ );
    case QCD::Flavor::s:    return ( *s_ )( n_ );
    case QCD::Flavor::sbar: return ( *S_ )( n_ ) - ( *s_ )( n_ );
    case QCD::Flavor::dv:   return complex< double >( 0.0, 0.0 );
    case QCD::Flavor::sv:   return 2.0 * ( *s_ )( n_ ) - ( *S_ )( n_ ) ;
    }
  } else if( h_ == QCD::Flavor::PRp ) {
    switch( q ){
    case QCD::Flavor::d:    return (*D_)(n_) - (*dbar_)(n_);
    case QCD::Flavor::dbar: return (*dbar_)(n_);
    case QCD::Flavor::s:
    case QCD::Flavor::sbar: return 0.5 * ( *S_ )( n_ ) ;
    case QCD::Flavor::dv:   return (*D_)(n_) - 2.0 *(*dbar_)(n_);
    case QCD::Flavor::sv:   return complex< double >( 0.0, 0.0 );
    }
  } else if( h_ == QCD::Flavor::REp ) {
    switch( q ){
    case QCD::Flavor::d:
    case QCD::Flavor::s:    return (*U_)( n_ ) - (*ubar_)( n_ );
    case QCD::Flavor::dbar: 
    case QCD::Flavor::sbar: return (*ubar_)( n_ );
    case QCD::Flavor::dv:   
    case QCD::Flavor::sv:   return (*U_)( n_ ) - 2.0 * (*ubar_)( n_ );
    }
  }
  return complex< double >( 0.0, 0.0 );
}


complex< double > DSSFFParam::q( const QCD::Flavor::TYPE& q ) {
  if( h_ == QCD::Flavor::REp ) {
    switch( q ){
    case QCD::Flavor::a0: return 3.0 * (*U_)( n_ );
    case QCD::Flavor::a3: return 0.0;
    case QCD::Flavor::a8: return 0.0;
    } 
  } else {
    switch( q ){
    case QCD::Flavor::a0: return (*U_)( n_ ) + (*D_)( n_ ) + (*S_)( n_ );
    case QCD::Flavor::a3: return (*U_)( n_ ) - (*D_)( n_ );
    case QCD::Flavor::a8: return (*U_)( n_ ) + (*D_)( n_ ) -2.0 * (*S_)( n_ );
    }
  }
  return complex< double >( 0.0, 0.0 );
}

