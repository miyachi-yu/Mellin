// $Id: FFSimple.cc,v 1.17 2010/02/22 09:41:46 miyachi Exp $
/*!
  \file     FFSimple.cc
  \version  $Revision: 1.17 $
  \author   $Author: miyachi $
  \brief    Implementation of PDF parameterization ( x^a*(1-x)^b*(1-c*x) )
 */
#include "FFSimple.hh"

#include <iostream>
#include <iomanip>

using namespace std;
using namespace Transform;
using namespace QCD;

FFSimple::FFSimple() : 
  PdfBase( PdfBase::parton, 1, 5 ),
  favored_( NULL ), disfavo_( NULL ),
  b1_( 1.0, 1.0 ), b2_( 2.0, 1.0 ), b3_( 1.0, 2.0 ), 
  b00_( 0.0, 0.0 ),
  A_( 0.0, 0.0 ),
  a_( 0.0 ), b_( 0.0 ), c_( 0.0 ), d_( 0.0 ),
  e_( 0.0 ), f_( 0.0 )
{
}

FFSimple::~FFSimple() {
}

complex< double > FFSimple::operator() ( const complex< double >& z ) {

  complex< double > tmp_( 0.0, 0.0 );

  if( A_ == complex< double >( 0.0, 0.0 ) ) return tmp_;
  
  tmp_ = b1_( z );
  if( c_ != 0.0 ) tmp_ += c_ * b2_( z );
  if( e_ != 0.0 ) tmp_ += e_ * b3_( z );

  return A_ * tmp_;
}

double FFSimple::x( const double& x ){
  
  if( A_.real() == 0.0 ) return 0.0;
  
  double tmp_( 1.0 );
  if( c_ != 0.0 ) tmp_ += c_ * pow( x, d_ );
  if( e_ != 0.0 ) tmp_ += e_ * pow( 1.0 - x, f_ );
  
  return A_.real() * pow( x, a_ ) * pow( 1.0 - x, b_ ) * tmp_;
}

void FFSimple::update() {
  
  if( this->reference() != NULL ) this->reference()->update();
  
  // copy to member variable
  a_ = this->getValue( 1 );
  b_ = this->getValue( 2 );
  
  b1_.set( a_, b_ );
  b00_ = b1_( 2.0 );
  
  if( ( c_ = this->getValue( 3 ) ) != 0.0 ){
    d_ = this->getValue( 4 );
    b2_.set( a_ + d_,  b_ );
    b00_ += c_ * b2_( 2.0 );
  }

  if( ( e_ = this->getValue( 5 ) ) != 0.0 ){
    f_ = this->getValue( 6 );
    b3_.set( a_,  b_ + f_ );
    b00_ += e_ * b3_( 2.0 );
  }

  // A_ assignment depending on the conditions
  if( b00_ == complex< double >( 0.0, 0.0 ) ){
    A_ = complex< double >( 0.0, 0.0 );
  } else {
    if( favored_ && disfavo_ ) {
      favored_->update();
      disfavo_->update();
      // update Parameter data
      (*this)[ 0 ].state( "fixed" );
      (*this)[ 0 ].error() = 0.0;
      (*this)[ 0 ].value() 
	= 0.5 * ( (*favored_)( 2.0 ) + (*disfavo_)( 2.0 ) ).real();
    
      A_ =  (*this)[ 0 ].value() / b00_;
    } else {
      A_ = this->getValue( 0 ) / b00_;
    }
  }

}
