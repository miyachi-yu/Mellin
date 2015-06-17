// $Id: DSSFF.cc,v 1.1 2009/07/28 04:07:31 miyachi Exp $
/*!
  \file     DSSFF.cc
  \version  $Revision: 1.1 $
  \author   $Author: miyachi $
  \brief    Implementation of PDF parameterization ( x^a*(1-x)^b*(1-c*x) )
 */
#include "DSSFF.hh"

using namespace std;
using namespace Transform;
using namespace PDFSET;

DSSFF::DSSFF() : 
  QCD::PdfBase( PdfBase::parton, 1, 5 ),
  b1_( 1.0, 1.0 ), b2_( 2.0, 1.0 ), 
  b10_( 0.0, 0.0 ),  b20_( 0.0, 0.0 ),  b00_( 0.0, 0.0 ),
  A_( 0.0, 0.0 ),
  a_( 0.0 ), b_( 0.0 ), c_( 0.0 ), d_( 0.0 )
{
}

DSSFF::~DSSFF() {
}

complex< double > DSSFF::operator() ( const complex< double >& z ) {
  return 
    ( A_ == complex< double >( 0.0, 0.0 ) ?
      complex< double >( 0.0, 0.0 ) : 
      A_ * ( c_ == 0.0 ? b1_( z ) : b1_( z ) + c_ * b2_( z ) ) );
}

double DSSFF::x( const double& x ){
  if( A_.real() == 0.0 ) return 0.0;
  return A_.real() * pow( x, a_ - 1.0 ) * pow( 1.0 - x, b_ ) * 
    ( 1.0 + c_ * pow( 1.0 - x, d_ ) );
}

void DSSFF::update() {
  
  if( this->reference() ) this->reference()->update();
  
  if( this->getValue( 0 ) == 0.0 ) {
    A_ = complex< double >( 0.0, 0.0 );
    return ;
  }
  
  a_ = this->getValue( 1 );
  b_ = this->getValue( 2 );
  c_ = this->getValue( 3 );
  d_ = this->getValue( 4 );
  
  b1_.set( a_, b_ );       // z^a ( 1 - z )^b
  b2_.set( a_, b_ + d_ );  // z^a ( 1 - z )^{ b + d }
  
  // calculate second moment 
  b10_ = b1_( 2.0 );
  b20_ = b2_( 2.0 );
  b00_ = b10_ + c_ * b20_; 
  
  A_ = ( b00_ != complex< double >( 0.0, 0.0 ) ?
	 this->getValue( 0 ) / b00_ : complex< double >( 0.0, 0.0 )  );
  
}
