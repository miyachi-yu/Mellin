// $Id: MSTWParam.cc,v 1.4 2009/07/29 08:59:48 miyachi Exp $
/*!
  \file  PdfParameterization.cc
  \brief class implementation of PDF parameterization
 */
#include "MSTWParam.hh"
#include "PdfMRST.hh"

#include <sstream>
#include <cstdlib>
#include <iomanip>

using namespace std;
using namespace PDFSET;

MSTWParam::MSTWParam() : 
  QCD::PdfParametrization()
{
}

MSTWParam::MSTWParam( MSTWParam& conf ) : 
  QCD::PdfParametrization( conf )
{
}

MSTWParam::MSTWParam( ConfigMRST& conf ) : 
  QCD::PdfParametrization()
{
  this->path(  conf.path()  );
  this->name(  conf.name()  );
  this->scale( conf.scale() );
  this->pdfs( conf.pdfs() );
}

MSTWParam::~MSTWParam() {
}

complex< double > MSTWParam::q( const QCD::Flavor::PARTON& q ) {
  
  // ---------------  gluon and heavy flavors --------------- //
  switch( q ){
    
  case QCD::Flavor::g : return (*g_)( n_ );
    
  case QCD::Flavor::uv: return (*uv_)( n_ );
  case QCD::Flavor::dv: return (*dv_)( n_ );
  case QCD::Flavor::sv: return (*sv_)( n_ );
    
  case QCD::Flavor::u: return (*uv_)( n_ ) + this->q( QCD::Flavor::ubar );
  case QCD::Flavor::d: return (*dv_)( n_ ) + this->q( QCD::Flavor::dbar );
    
  case QCD::Flavor::s:    return 0.5 * ( ( *S_)( n_ ) + (*sv_)( n_ ) );
  case QCD::Flavor::sbar: return 0.5 * ( ( *S_)( n_ ) - (*sv_)( n_ ) );
    
  case QCD::Flavor::U: return (*uv_)( n_ ) + 2.0 * this->q( QCD::Flavor::ubar );
  case QCD::Flavor::D: return (*dv_)( n_ ) + 2.0 * this->q( QCD::Flavor::dbar );
  case QCD::Flavor::S: return (*S_)( n_ );
    
  case QCD::Flavor::ubar: 
    return 0.25 * ( (*sea_)( n_ ) - (*S_)( n_ ) ) - 0.5 * (*delta_)( n_ );
  case QCD::Flavor::dbar: 
    return 0.25 * ( (*sea_)( n_ ) - (*S_)( n_ ) ) + 0.5 * (*delta_)( n_ ); 
    
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

complex< double > MSTWParam::q( const QCD::Flavor::TYPE& q ) {
  
  switch( q ){
  case QCD::Flavor::a0: 
    return (*uv_)( n_ ) + (*dv_)( n_ ) + (*sea_)( n_ );
    
  case QCD::Flavor::a3m:
    return (*uv_)( n_ ) - (*dv_)( n_ );
    
  case QCD::Flavor::a3:
    return (*uv_)( n_ ) - (*dv_)( n_ ) + 2.0 * ( *delta_ )( n_ );
    
  case QCD::Flavor::a8: 
    return (*uv_)( n_ ) + (*dv_)( n_ ) + 0.6 * (*sea_)( n_ );
    
  case QCD::Flavor::a8m: 
    return (*uv_)( n_ ) + (*dv_)( n_ ) + (*sea_)( n_ ) - 3.0 * (*S_)( n_ );
  } 
  
  return complex< double >( 0.0, 0.0 );
}


bool MSTWParam::check(){
  
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
  
  if( ! sv_ ){
    cerr << __FILE__ << ":" << __LINE__ << "\tsv is missing" << endl;
    return false;
  }
  
  if( ! S_ ){
    cerr << __FILE__ << ":" << __LINE__ << "\tS is missing" << endl;
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

MSTWParam& MSTWParam::operator<<( ConfigMRST& conf ){
  this->path(  conf.path()  );
  this->name(  conf.name()  );
  this->scale( conf.scale() );
  this->pdfs(  conf.pdfs()  );
  return *this;
}
