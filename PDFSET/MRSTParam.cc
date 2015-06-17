// $Id: MRSTParam.cc,v 1.2 2009/07/29 08:59:48 miyachi Exp $
/*!
  \file  PdfParameterization.cc
  \brief class implementation of PDF parameterization
 */
#include "MRSTParam.hh"
#include "PdfMRST.hh"

#include <sstream>
#include <cstdlib>
#include <iomanip>

extern "C" {
#include <libgen.h>
}

using namespace std;
using namespace PDFSET;

MRSTParam::MRSTParam() : 
  QCD::PdfParametrization()
{
}

MRSTParam::MRSTParam( MRSTParam& conf ) : 
  QCD::PdfParametrization( conf )
{
}

MRSTParam::MRSTParam( ConfigMRST& conf ) : 
  QCD::PdfParametrization()
{
  this->path(  conf.path()  );
  this->name(  conf.name()  );
  this->scale( conf.scale() );
  this->pdfs( conf.pdfs() );
}

MRSTParam::~MRSTParam() {
}

complex< double > MRSTParam::q( const QCD::Flavor::PARTON& q ) {
  
  // ---------------  gluon and heavy flavors --------------- //
  switch( q ){
  case QCD::Flavor::g : return (*g_)( n_ );
  case QCD::Flavor::uv: return (*uv_)( n_ );
  case QCD::Flavor::dv: return (*dv_)( n_ );
  case QCD::Flavor::u:  return this->q( QCD::Flavor::uv ) + this->q( QCD::Flavor::ubar );
  case QCD::Flavor::d:  return this->q( QCD::Flavor::dv ) + this->q( QCD::Flavor::dbar );
  case QCD::Flavor::s:
  case QCD::Flavor::sbar: return 0.1 * (*sea_)( n_ );
  case QCD::Flavor::ubar: 
    return 0.2 * (*sea_)( n_ ) - 0.5 * (*delta_)( n_ );
  case QCD::Flavor::dbar: 
    return 0.2 * (*sea_)( n_ ) + 0.5 * (*delta_)( n_ );
  }
  
  // ---------------- NS components ---------------- //
  switch( q ){
    
  case QCD::Flavor::uns: 
    return this->q( QCD::Flavor::u ) + this->q( QCD::Flavor::ubar ) 
      - this->q( QCD::Flavor::a0 ) / 3.0;
    
  case QCD::Flavor::dns:
    return this->q( QCD::Flavor::d ) + this->q( QCD::Flavor::dbar ) 
      - this->q( QCD::Flavor::a0 ) / 3.0;
    
  case QCD::Flavor::sns:
    return this->q( QCD::Flavor::s ) + this->q( QCD::Flavor::sbar ) 
      - this->q( QCD::Flavor::a0 ) / 3.0;
    
  }    
  
  return complex< double >( 0.0, 0.0 );
}

complex< double > MRSTParam::q( const QCD::Flavor::TYPE& q ) {
  
  switch( q ){
  case QCD::Flavor::a0: 
    return (*uv_)( n_ ) + (*dv_)( n_ ) + (*sea_)( n_ );
    
  case QCD::Flavor::a3m:
    return (*uv_)( n_ ) - (*dv_)( n_ );
    
  case QCD::Flavor::a3:
    return (*uv_)( n_ ) - (*dv_)( n_ ) - 2.0 * ( *delta_ )( n_ );
    
  case QCD::Flavor::a8: 
    return (*uv_)( n_ ) + (*dv_)( n_ ) + 0.6 * (*sea_)( n_ );
    
  case QCD::Flavor::a8m: 
    return (*uv_)( n_ ) + (*dv_)( n_ );
    
  } 
  
  return complex< double >( 0.0, 0.0 );
}


bool MRSTParam::check(){
  
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
  
  if( ! sea_ ){
    cerr << __FILE__ << ":" << __LINE__ << "\tSEA is missing" << endl;
    return false;
  }
  
  if( ! delta_ ){
    cerr << __FILE__ << ":" << __LINE__ << "\tDELTA is missing" << endl;
    return false;
  }
  
  return true;
}

MRSTParam& MRSTParam::operator<<( ConfigMRST& conf ){
  this->path(  conf.path()  );
  this->name(  conf.name()  );
  this->scale( conf.scale() );
  this->pdfs(  conf.pdfs()  );
  return *this;
}
