//
// xXsec.cc
//  
// Made by Yoshiyuki Miyachi
// Login   <miyachi@yoshiyuki-desktop>
// 
// Started on  Thu Oct 23 11:22:28 2008 Yoshiyuki Miyachi
// Started on  Thu Oct 23 11:22:28 2008 Yoshiyuki Miyachi
//

#include <iostream>
#include "xXsec.hh"
#include "StrFunc.hh"

using namespace std;
using namespace unpolSIDISXsec;

xXsec::xXsec( Xsection::Xsec* xc,
	      const int& low, const int& high, const double& precision,
	      const double& length, const double& offset, 
	      const double& rangle ) : 
  Xsection::xXsec( xc, low, high, precision, length, offset, rangle )
{
}

xXsec::xXsec( Xsection::Xsec* xc, Transform::Integration* integ ) :
  Xsection::xXsec( xc, integ )
{
}

xXsec::~xXsec(){
}



void xXsec::setxparams( const vector< double >& x ){
  if( x.size() == 0 ) return;
  this->setxparams( x[ 0 ] );
}

void xXsec::setxparams( const double& x1, const double& x2 ){
  StrFunc *func = dynamic_cast< StrFunc* >( this->moment() );
  if( func ) func->setX( x1 );
}

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

