#include "xXsec.hh"
#include "StrFunc.hh"
#include "XsecComp.hh"
#include "CoeffFunc.hh"
#include <Xsec/Xsec.hh>

using namespace std;
using namespace unpolDISXsec;

xXsec::xXsec( Xsection::Xsec* xc,
	      const int& low, const int& high, const double& precision,
	      const double& length, const double& offset, 
	      const double& rangle ) : 
  Xsection::xXsec( xc, low, high, precision, length, offset, rangle ),
  iminteg_( xc )
{
  CoeffFunc* coeff = dynamic_cast< CoeffFunc* >( xc->xsecComp()->coeff() );
  if( coeff ) iminteg_.setxzmom( coeff->getXmom(), coeff->getZmom() );
  this->setIMIntegrand( &iminteg_ );
}

xXsec::xXsec( Xsection::Xsec* xc, Transform::Integration* integ ) :
  Xsection::xXsec( xc, integ ),
  iminteg_( xc )
{
  CoeffFunc* coeff = dynamic_cast< CoeffFunc* >( xc->xsecComp()->coeff() );
  if( coeff ) iminteg_.setxzmom( coeff->getXmom(), coeff->getZmom() );
  this->setIMIntegrand( &iminteg_ );
}

xXsec::~xXsec(){
}

void xXsec::setxparams( const vector< double >& x ){

  if( x.size() == 1 ) { 
    this->setxparams( x[0] );
    return;
  }

  if( x.size() == 2 ) {
    this->setxparams( x[0], x[1] );
    return;
  }
  
}

void xXsec::setxparams( const double& x1, const double& x2 ){
  CoeffFunc* coeff = 
    dynamic_cast< CoeffFunc* >( this->moment()->xsecComp()->coeff() );
  
  ( x2 <= 0.0 ? coeff->setX( x1 ) : coeff->setX1X2( x1, x2 ) );
}

//calculate a value of M-1(f)(z) where M-1 means InerseMellin transform 
double xXsec::operator() ( const double& x ) {

  this->setxparams( x );  
  activefunc_->x( x );
  
  return this->integrate_();
}

double xXsec::operator() ( const double& x, const double& z ) {

  this->setxparams( x );  
  activefunc_->x( x );

  this->setzparams( z );
  activefunc_->z( z );

  static double c = - 0.5 / M_PI / M_PI;
  return c * this->integration()->integral( vlower_[0], vupper_[0], 
					    vlower_[1], vupper_[1],
					    integ_ );
}

double xXsec::operator()( const vector< double >& x,
			  const vector< double >& z ){
  if( x.size() == 0 ) return 0.0;
  if( z.size() == 0 ) (*this)( x );
  
  this->setxparams( x );  
  activefunc_->x( x );
  
  this->setzparams( z );
  activefunc_->z( z );
  
  static double c = - 0.5 / M_PI / M_PI;
  return c * this->integration()->integral( vlower_[0], vupper_[0], 
					    vlower_[1], vupper_[1],
					    integ_ );
}

double xXsec::operator()( const std::vector< double >& x ) {
  if( x.size() == 0 ) return 0.0;
  if( x.size() == 1 ) return (*this)( x[0] );

  this->setxparams( x );  
  activefunc_->x( x );
  return this->integrate_();
}

