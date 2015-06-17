// $Id: BBParam.cc,v 1.2 2009/07/29 08:59:48 miyachi Exp $
/*!
  \file  PdfParameterization.cc
  \brief class implementation of PDF parameterization
 */
#include "BBParam.hh"

#include <sstream>
#include <cstdlib>
#include <iomanip>

extern "C" {
#include <libgen.h>
}

using namespace std;
using namespace PDFSET;

BBParam::BBParam() : 
  QCD::PdfParametrization()
{
}

BBParam::BBParam( BBParam& conf ) : 
  QCD::PdfParametrization( conf )
{
}

BBParam::BBParam( QCD::ConfigPdfBase& conf ) : 
  QCD::PdfParametrization()
{
  this->path(  conf.path()  );
  this->name(  conf.name()  );
  this->scale( conf.scale() );
  this->pdfs( conf.pdfs() );
}

BBParam::~BBParam() {
}

complex< double > BBParam::q( const QCD::Flavor::PARTON& q ) {
  
  static double c3 = 1.0 / 3.0;

  switch( q ){
    
  case QCD::Flavor::g : return (*g_)( n_ );
    
  case QCD::Flavor::U : return (*uv_)( n_ ) + 2.0 * (*ubar_)( n_ );
  case QCD::Flavor::D : return (*dv_)( n_ ) + 2.0 * (*dbar_)( n_ );
  case QCD::Flavor::S : return (*s_)( n_ ) + (*sbar_)( n_ );
    
  case QCD::Flavor::u:  return (*uv_)( n_ ) + (*ubar_)( n_ );
  case QCD::Flavor::d:  return (*dv_)( n_ ) + (*dbar_)( n_ );
  case QCD::Flavor::s:  return (*s_)( n_ );
    
  case QCD::Flavor::uv: return (*uv_)( n_ );
  case QCD::Flavor::dv: return (*dv_)( n_ );
  case QCD::Flavor::sv: return complex< double >( 0.0, 0.0 );
    
  case QCD::Flavor::ubar: return (*ubar_)( n_ );
  case QCD::Flavor::dbar: return (*dbar_)( n_ );
  case QCD::Flavor::sbar: return (*sbar_)( n_ );
    
  case QCD::Flavor::uns: 
    return (*uv_)( n_ ) + 2.0 * (*ubar_)( n_ ) - c3 * this->q( QCD::Flavor::a0 );
  case QCD::Flavor::dns: 
    return (*dv_)( n_ ) + 2.0 * (*dbar_)( n_ ) - c3 * this->q( QCD::Flavor::a0 );
  case QCD::Flavor::sns: 
    return (*s_)( n_ ) + (*sbar_)( n_ ) - c3 * this->q( QCD::Flavor::a0 );
    
  }    
  
  return complex< double >( 0.0, 0.0 );
}

complex< double > BBParam::q( const QCD::Flavor::TYPE& q ) {
  
  complex< double > U = (*uv_)( n_ ) + 2.0 * (*ubar_)( n_ );
  complex< double > D = (*dv_)( n_ ) + 2.0 * (*dbar_)( n_ );
  complex< double > S = (*s_)( n_ ) + (*sbar_)( n_ );
  
  switch( q ){
  case QCD::Flavor::a0: return U + D + S;
  case QCD::Flavor::a3: return U - D;
  case QCD::Flavor::a8: return U + D - 2.0 * S;
  case QCD::Flavor::a3m:
    return this->q( QCD::Flavor::uv ) - this->q( QCD::Flavor::dv );
  case QCD::Flavor::a8m: 
    return this->q( QCD::Flavor::uv ) + this->q( QCD::Flavor::dv );
  } 
  
  return complex< double >( 0.0, 0.0 );
}

bool BBParam::check(){
  
  this->assign();
  
  // gluon distribution is always required....
  if( ! g_ ){
    cerr << __FILE__ << ":" << __LINE__ << "\tgluon is missing" << endl;
    return false;
  }
  
  if( ! uv_ ){
    cerr << __FILE__ << ":" << __LINE__ << "\tuv is missing" << endl;
    return false;
  }
  
  if( ! dv_ ){
    cerr << __FILE__ << ":" << __LINE__ << "\tdv is missing" << endl;
    return false;
  }
  
  if( ! s_ ){
    cerr << __FILE__ << ":" << __LINE__ << "\ts is missing" << endl;
    return false;
  }
  
  if( ! ubar_ ){
    cerr << __FILE__ << ":" << __LINE__ << "\tu-bar is missing" << endl;
    return false;
  }
  
  if( ! dbar_ ){
    cerr << __FILE__ << ":" << __LINE__ << "\td-bar is missing" << endl;
    return false;
  }
  
  if( ! sbar_ ){
    cerr << __FILE__ << ":" << __LINE__ << "\ts-bar is missing" << endl;
    return false;
  }
  
  return true;
}

BBParam& BBParam::operator<<( QCD::ConfigPdfBase& conf ){
  this->path(  conf.path()  );
  this->name(  conf.name()  );
  this->scale( conf.scale() );
  this->pdfs(  conf.pdfs()  );
  return *this;
}
