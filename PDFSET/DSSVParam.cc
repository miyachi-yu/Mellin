// $Id: DSSVParam.cc,v 1.2 2009/07/29 08:59:48 miyachi Exp $
/*!
  \file  PdfParameterization.cc
  \brief class implementation of PDF parameterization
 */
#include "DSSVParam.hh"
#include "PdfDSSV.hh"

#include <sstream>
#include <cstdlib>
#include <iomanip>

extern "C" {
#include <libgen.h>
}

using namespace std;
using namespace PDFSET;

DSSVParam::DSSVParam() : 
  QCD::PdfParametrization()
{
}

DSSVParam::DSSVParam( DSSVParam& conf ) : 
  QCD::PdfParametrization( conf )
{
}

DSSVParam::DSSVParam( ConfigDSSV& conf ) : 
  QCD::PdfParametrization()
{
  this->path(  conf.path()  );
  this->name(  conf.name()  );
  this->scale( conf.scale() );
  this->pdfs( conf.pdfs() );
}

DSSVParam::~DSSVParam() {
}

complex< double > DSSVParam::q( const QCD::Flavor::PARTON& q ) {
  
  static double c3 = 1.0 / 3.0;
  
  switch( q ){
    
  case QCD::Flavor::g : return (*g_)( n_ );
    
  case QCD::Flavor::U : return (*U_)( n_ );
  case QCD::Flavor::D : return (*D_)( n_ );
  case QCD::Flavor::S : return (*S_)( n_ );

    /*
      If anti-quark distributions are not defined,
      take a half of S as a symetric sea for anti-quark.
     */
    
  case QCD::Flavor::u:  
    return ( ubar_ ? 
	     (*U_)( n_ ) - (*ubar_)( n_ ) :
	     (*U_)( n_ ) - 0.5 * ( *S_ )( n_ ) );
    
  case QCD::Flavor::d:  
    return ( dbar_ ? 
	     (*D_)( n_ ) - (*dbar_)( n_ ) :
	     (*D_)( n_ ) - 0.5 * ( *S_ )( n_ ) );
    
  case QCD::Flavor::s:  
    return ( sbar_ ? 
	     (*S_)( n_ ) - (*sbar_)( n_ ) : 
	     0.5 * (*S_)( n_ ) );
    
    /*
      If anti-quark distributions are not defined,
      take a half of S as a symetric sea for anti-quark.
      So uv and dv become just as difference to S.
     */
    
  case QCD::Flavor::uv: 
    return ( ubar_ ?
	     (*U_)( n_ ) - 2.0 * (*ubar_)( n_ ) :
	     (*U_)( n_ ) - (*S_)( n_ ) );
    
  case QCD::Flavor::dv: 
    return ( dbar_ ?
	     (*D_)( n_ ) - 2.0 * (*dbar_)( n_ ) :
	     (*D_)( n_ ) - (*S_)( n_ ) );
	     
  case QCD::Flavor::sv: 
    return ( sbar_ ? 
	     (*S_)( n_ ) - 2.0 * (*sbar_)( n_ ) :
	     complex< double >( 0.0, 0.0 ) );
    
  case QCD::Flavor::ubar: return ( ubar_ ? (*ubar_)( n_ ) : 0.5 * (*S_)( n_ ) );
  case QCD::Flavor::dbar: return ( dbar_ ? (*dbar_)( n_ ) : 0.5 * (*S_)( n_ ) );
  case QCD::Flavor::sbar: return ( sbar_ ? (*sbar_)( n_ ) : 0.5 * (*S_)( n_ ) );
    
  case QCD::Flavor::uns: return (*U_)( n_ ) - c3 * this->q( QCD::Flavor::a0 );
  case QCD::Flavor::dns: return (*D_)( n_ ) - c3 * this->q( QCD::Flavor::a0 );
  case QCD::Flavor::sns: return (*S_)( n_ ) - c3 * this->q( QCD::Flavor::a0 );
    
  }    
  
  return complex< double >( 0.0, 0.0 );
}

complex< double > DSSVParam::q( const QCD::Flavor::TYPE& q ) {
  
  switch( q ){
  case QCD::Flavor::a0: return (*U_)( n_ ) + (*D_)( n_ ) + (*S_)( n_ );
  case QCD::Flavor::a3: return (*U_)( n_ ) - (*D_)( n_ );
  case QCD::Flavor::a8: return (*U_)( n_ ) + (*D_)( n_ ) - 2.0 * (*S_)( n_ );
  case QCD::Flavor::a3m:
    return this->q( QCD::Flavor::uv ) - this->q( QCD::Flavor::dv );
  case QCD::Flavor::a8m: 
    return this->q( QCD::Flavor::uv ) + this->q( QCD::Flavor::dv ) - 
      3.0 *  this->q( QCD::Flavor::sv );
  } 
  
  return complex< double >( 0.0, 0.0 );
}

bool DSSVParam::check(){
  
  this->assign();
  
  // gluon distribution is always required....
  if( ! g_ ){
    cerr << __FILE__ << ":" << __LINE__ << "\tgluon is missing" << endl;
    return false;
  }
  
  if( ! U_ ){
    cerr << __FILE__ << ":" << __LINE__ << "\tU is missing" << endl;
    return false;
  }
  
  if( ! D_ ){
    cerr << __FILE__ << ":" << __LINE__ << "\tD is missing" << endl;
    return false;
  }
  
  if( ! S_ ){
    cerr << __FILE__ << ":" << __LINE__ << "\tS is missing" << endl;
    return false;
  }

  if( ! a0_ ){
    cerr << __FILE__ << ":" << __LINE__ << "\ta0 is missing" << endl;
    return false;
  }
  
  if( ! a3_ ){
    cerr << __FILE__ << ":" << __LINE__ << "\ta3 is missing" << endl;
    return false;
  }
  
  if( ! a8_ ){
    cerr << __FILE__ << ":" << __LINE__ << "\ta8 is missing" << endl;
    return false;
  }
  
  return true;
}

DSSVParam& DSSVParam::operator<<( ConfigDSSV& conf ){
  this->path(  conf.path()  );
  this->name(  conf.name()  );
  this->scale( conf.scale() );
  this->pdfs(  conf.pdfs()  );
  return *this;
}
