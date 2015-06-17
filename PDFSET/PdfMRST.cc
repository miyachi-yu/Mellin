// $Id: PdfMRST.cc,v 1.1 2009/07/28 04:07:32 miyachi Exp $
/*!
  \file     PdfMRST.cc
  \version  $Revision: 1.1 $
  \author   $Author: miyachi $
  \brief    Implementation of PDF parameterization ( x^a*(1-x)^b*(1-c*x) )
 */
#include "PdfMRST.hh"

using namespace std;
using namespace Transform;
using namespace PDFSET;

PdfMRST::PdfMRST() : 
  QCD::PdfBase( PdfBase::parton, 1, 5 ),
  b1_( 1.0, 1.0 ), b2_( 2.0, 1.0 ), b3_( 1.0, 1.0 ), b4_( 1.0, 1.0 ),
  A_( 0.0 ), a_( 0.0 ), b_( 0.0 ), c_( 0.0 ), d_( 0.0 ),
  e_( 0.0 ), f_( 0.0 ), g_( 0.0 )
{
}

PdfMRST::~PdfMRST() {
}

complex< double > PdfMRST::operator() ( const complex< double >& z ) {
  return 
    ( A_ != 0.0 ? A_ * ( b1_( z ) 
			 +( c_ != 0.0 ? c_ * b2_( z ) : 0.0 )
			 +( d_ != 0.0 ? d_ * b3_( z ) : 0.0 ) ) :
      complex< double >( 0.0, 0.0 ) ) +
    ( e_ != 0.0 ? e_ * b4_( z ) : complex< double >( 0.0, 0.0 ) );
}

double PdfMRST::x( const double& x ){
  
  if( A_ == 0.0 ) return 0.0;
  double v = 1.0;
  if( c_ != 0.0 ) v += c_ * ( this->name() != "DELTA" ? pow( x, 0.5 ) : x );
  if( d_ != 0.0 ) v += d_ * ( this->name() != "DELTA" ? x : x * x );
  
  return 
    A_ * pow( x, a_ - 1.0 ) * pow( 1.0 - x, b_ ) * v
    + ( e_ != 0.0 ? e_ * pow( x, f_ ) * pow( 1.0 - x, g_ ) : 0.0 );
  
  ;
  
}

void PdfMRST::update() {
  
  if( ( A_ = this->getValue( 0 ) ) == 0.0 ) return;
  
  a_ = this->getValue( 1 );
  b_ = this->getValue( 2 );
  c_ = this->getValue( 3 );
  d_ = this->getValue( 4 );
  e_ = this->getValue( 5 );
  f_ = this->getValue( 6 );
  g_ = this->getValue( 7 );
  
  b1_.set( a_ - 1.0, b_ );
  
  if( this->name() == "DELTA" ){
    b2_.set( a_,       b_ );
    b3_.set( a_ + 1.0, b_ );
  } else {
    b2_.set( a_ - 0.5, b_ );
    b3_.set( a_,       b_ );
  }
  
  if( e_ != 0.0 ) b4_.set( f_ - 1.0, g_ );
  
}
