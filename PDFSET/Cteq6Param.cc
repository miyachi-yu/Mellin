// $Id: Cteq6Param.cc,v 1.1 2009/07/29 08:59:48 miyachi Exp $
/*!
  \file  PdfParameterization.cc
  \brief class implementation of PDF parameterization
 */
#include "Cteq6Param.hh"
#include "PdfCteq6.hh"

#include <sstream>
#include <cstdlib>
#include <iomanip>

extern "C" {
#include <libgen.h>
}

using namespace std;
using namespace PDFSET;

Cteq6Param::Cteq6Param() : 
  QCD::PdfParametrization()
{
}

Cteq6Param::Cteq6Param( Cteq6Param& conf ) : 
  QCD::PdfParametrization( conf )
{
}

Cteq6Param::Cteq6Param( ConfigCteq6& conf ) : 
  QCD::PdfParametrization()
{
  this->path(  conf.path()  );
  this->name(  conf.name()  );
  this->scale( conf.scale() );
  this->pdfs( conf.pdfs()   );
}

Cteq6Param::~Cteq6Param() {
}

complex< double > Cteq6Param::q( const QCD::Flavor::PARTON& q ) {
  
  if( q == QCD::Flavor::dbar ){
    complex< double > r = (*duratio_)( n_ );
    return (*dusum_)( n_ ) * r / ( r + 1.0 );
  }
  
  switch( q ){
  case QCD::Flavor::g : return (*g_)( n_ );
    
  case QCD::Flavor::uv: return (*uv_)( n_ );
  case QCD::Flavor::dv: return (*dv_)( n_ );
  case QCD::Flavor::sv: return complex< double >( 0.0, 0.0 );
    
  case QCD::Flavor::s:
  case QCD::Flavor::sbar: return (*s_)( n_ );
  case QCD::Flavor::S: return 2.0 * (*s_)( n_ );
    
  case QCD::Flavor::ubar : return (*dusum_)( n_ ) / ( 1.0 + (*duratio_)( n_ ) );

  case QCD::Flavor::u: return (*uv_)( n_ ) + this->q( QCD::Flavor::ubar );
  case QCD::Flavor::U: return (*uv_)( n_ ) + 2.0 * this->q( QCD::Flavor::ubar );
    
  case QCD::Flavor::d: return (*dv_)( n_ ) + this->q( QCD::Flavor::dbar );
  case QCD::Flavor::D: return (*dv_)( n_ ) + 2.0 * this->q( QCD::Flavor::dbar );
    
  case QCD::Flavor::uns: return this->q( QCD::Flavor::U ) - this->q( QCD::Flavor::a0 ) / 3.0;
  case QCD::Flavor::dns: return this->q( QCD::Flavor::D ) - this->q( QCD::Flavor::a0 ) / 3.0;
  case QCD::Flavor::sns: return this->q( QCD::Flavor::S ) - this->q( QCD::Flavor::a0 ) / 3.0;
    
  }
  
  return complex< double >( 0.0, 0.0 );
}

complex< double > Cteq6Param::q( const QCD::Flavor::TYPE& q ) {
  
  if( q == QCD::Flavor::a0 ){
    return 
      (*uv_)( n_ ) + (*dv_)( n_ ) + 2.0 * ( (*dusum_)( n_ ) + (*s_)( n_ ) );
  }
  
  if( q == QCD::Flavor::a3m ) return (*uv_)( n_ ) - (*dv_)( n_ );
  
  if( q == QCD::Flavor::a8m ) return (*uv_)( n_ ) + (*dv_)( n_ );
  
  if( q == QCD::Flavor::DUSUM ) return (*dusum_)( n_ );

  if( q == QCD::Flavor::DURATIO ) return (*duratio_)( n_ );
  
  if( q == QCD::Flavor::a8 )
    return (*uv_)( n_ ) + (*dv_)( n_ ) + 2.0 * (*dusum_)( n_ )
      - 4.0 * (*s_)( n_ );
  
  if( q == QCD::Flavor::a3 ){
    complex< double > r = (*duratio_)( n_ );
    return 
      (*uv_)( n_ ) - (*dv_)( n_ ) 
      + 2.0 * ( 1.0 - r ) / ( 1.0 + r ) * (*dusum_)( n_ );
  } 
  
  return complex< double >( 0.0, 0.0 );
}


bool Cteq6Param::check(){
  
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
  
  if( ! dusum_ ){
    cerr << __FILE__ << ":" << __LINE__ << "\tduSum is missing" << endl;
    return false;
  }
  
  if( ! duratio_ ){
    cerr << __FILE__ << ":" << __LINE__ << "\tduRatio is missing" << endl;
    return false;
  }
  
  return true;
}

Cteq6Param& Cteq6Param::operator<<( ConfigCteq6& conf ){
  this->path(  conf.path()  );
  this->name(  conf.name()  );
  this->scale( conf.scale() );
  this->pdfs(  conf.pdfs()  );
  return *this;
}
