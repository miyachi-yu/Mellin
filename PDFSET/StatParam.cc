// $Id: StatParam.cc,v 1.2 2009/07/29 08:59:48 miyachi Exp $
/*!
  \file  StatParam.cc
  \brief class implementation of Statistical Model PDF parameterization
 */
#include "StatParam.hh"
#include "PdfStat.hh"

#include <sstream>
#include <cstdlib>
#include <iomanip>

extern "C" {
#include <libgen.h>
}

using namespace std;
using namespace PDFSET;

StatParam::StatParam() : 
  QCD::PdfParametrization()
{
}

StatParam::StatParam( StatParam& conf ) : 
  QCD::PdfParametrization( conf )
{
}

StatParam::StatParam( ConfigStat& conf ) : 
  QCD::PdfParametrization()
{
  this->path(  conf.path()  );
  this->name(  conf.name()  );
  this->scale( conf.scale() );
  this->pdfs( conf.pdfs() );
}

StatParam::~StatParam() {
}

complex< double > StatParam::q( const QCD::Flavor::PARTON& q ) {
  
  static double c3 = 1.0 / 3.0;
  
  switch( q ){
    
  case QCD::Flavor::g : return (*g_)( n_ );

  case QCD::Flavor::u : return (*u_)( n_ );
  case QCD::Flavor::d : return (*d_)( n_ );
  case QCD::Flavor::s : return (*s_)( n_ );

  case QCD::Flavor::ubar : return (*ubar_)( n_ );
  case QCD::Flavor::dbar : return (*dbar_)( n_ );
  case QCD::Flavor::sbar : return (*sbar_)( n_ );
    
  case QCD::Flavor::U : return (*u_)( n_ ) + (*ubar_)( n_ );
  case QCD::Flavor::D : return (*d_)( n_ ) + (*dbar_)( n_ );
  case QCD::Flavor::S : return (*s_)( n_ ) + (*sbar_)( n_ );
  case QCD::Flavor::uv: return (*u_)( n_ ) - (*ubar_)( n_ );
  case QCD::Flavor::dv: return (*d_)( n_ ) - (*dbar_)( n_ );
  case QCD::Flavor::sv: return (*s_)( n_ ) - (*sbar_)( n_ );
    
  case QCD::Flavor::uns: return this->q( QCD::Flavor::U ) - c3 * this->q( QCD::Flavor::a0 );
  case QCD::Flavor::dns: return this->q( QCD::Flavor::D ) - c3 * this->q( QCD::Flavor::a0 );
  case QCD::Flavor::sns: return this->q( QCD::Flavor::S ) - c3 * this->q( QCD::Flavor::a0 );
    
  }    
  
  return complex< double >( 0.0, 0.0 );
}

complex< double > StatParam::q( const QCD::Flavor::TYPE& q ) {
  
  switch( q ){
  case QCD::Flavor::a0:
    return
      this->q( QCD::Flavor::U ) +
      this->q( QCD::Flavor::D ) +
      this->q( QCD::Flavor::S );

  case QCD::Flavor::a3:
    return 
      this->q( QCD::Flavor::U ) -
      this->q( QCD::Flavor::D );

  case QCD::Flavor::a8:
    return
      this->q( QCD::Flavor::U ) +
      this->q( QCD::Flavor::D ) -
      2.0 * this->q( QCD::Flavor::S );

  case QCD::Flavor::a3m:
    return
      this->q( QCD::Flavor::uv ) - this->q( QCD::Flavor::dv );

  case QCD::Flavor::a8m: 
    return
      this->q( QCD::Flavor::uv ) + this->q( QCD::Flavor::dv ) - 
      3.0 *  this->q( QCD::Flavor::sv );
  } 
  
  return complex< double >( 0.0, 0.0 );
}

bool StatParam::check(){
  
  this->assign();
  
  // gluon distribution is always required....
  if( ! g_ ){
    cerr << __FILE__ << ":" << __LINE__ << "\tgluon is missing" << endl;
    return false;
  }
  
  if( ! u_ ){
    cerr << __FILE__ << ":" << __LINE__ << "\tu is missing" << endl;
    return false;
  }
  
  if( ! d_ ){
    cerr << __FILE__ << ":" << __LINE__ << "\td is missing" << endl;
    return false;
  }
  
  if( ! s_ ){
    cerr << __FILE__ << ":" << __LINE__ << "\ts is missing" << endl;
    return false;
  }
  
  if( ! ubar_ ){
    cerr << __FILE__ << ":" << __LINE__ << "\tubar is missing" << endl;
    return false;
  }
  
  if( ! dbar_ ){
    cerr << __FILE__ << ":" << __LINE__ << "\tdbar is missing" << endl;
    return false;
  }
  
  if( ! sbar_ ){
    cerr << __FILE__ << ":" << __LINE__ << "\tsbar is missing" << endl;
    return false;
  }

  return true;
}

StatParam& StatParam::operator<<( ConfigStat& conf ){
  this->path(  conf.path()  );
  this->name(  conf.name()  );
  this->scale( conf.scale() );
  this->pdfs(  conf.pdfs()  );
  return *this;
}
