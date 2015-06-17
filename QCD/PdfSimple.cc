// $Id: PdfSimple.cc,v 1.20 2009/02/19 04:26:52 miyachi Exp $
/*!
  \file     PdfSimple.cc
  \version  $Revision: 1.20 $
  \author   $Author: miyachi $
  \brief    Implementation of PDF parameterization ( x^a*(1-x)^b*(1-c*x) )
 */
#include "PdfSimple.hh"

#include <iostream>
#include <iomanip>

using namespace std;
using namespace Transform;
using namespace QCD;

PdfSimple::PdfSimple() : 
  PdfBase( PdfBase::parton, 1, 5 ),
  b1_( 1.0, 1.0 ), b2_( 2.0, 1.0 ), b3_( 1.5, 1.0 ), 
  A_( 0.0, 0.0 ), a_( 0.0 ), b_( 0.0 ), c_( 0.0 ), d_( 0.0 )
{
}

PdfSimple::~PdfSimple() {
}

complex< double > PdfSimple::operator() ( const complex< double >& z ) {
  
  if( A_ == complex< double >( 0.0, 0.0 ) ) return A_;

  complex< double > v = b1_( z );
  if( c_ != 0.0 ) v -= c_ * b2_( z );
  if( d_ != 0.0 ) v += d_ * b3_( z );

  return A_ * v;
}

double PdfSimple::x( const double& x ){
  if( A_.real() == 0.0 ) return 0.0;
  return A_.real() * pow( x, a_ - 1.0 ) * pow( 1.0 - x, b_ ) * 
    ( 1.0 - c_ * x + d_ * sqrt( x ) );
}

void PdfSimple::update() {
  
  if( this->reference() ) this->reference()->update();
  
  if( this->getValue( 0 ) == 0.0 ) { 
    A_ = complex< double >( 0.0, 0.0 ) ;
    return;  
  }
  
  //  cout << setw(12) << this->name() << flush;
  //  for( int i = 0; i < 5; i++ )
  //    cout << setw(12) << this->getValue( i ) << flush;
  //  cout << endl;
  a_ = this->getValue( 1 );
  b_ = this->getValue( 2 );
  c_ = this->getValue( 3 );
  d_ = this->getValue( 4 );
  
  b1_.set( a_ - 1.0, b_ );
  complex< double > b00 = b1_( 1.0 );

  if( c_ != 0.0 ) {
    b2_.set( a_,  b_ );
    b00 -= c_ * b2_( 1.0 );
  }
  
  if( d_ != 0.0 ){
    b3_.set( a_ - 0.5, b_ );
    b00 += d_ * b3_( 1.0 );
  }
  
  A_ = ( b00 != complex< double >( 0.0, 0.0 ) ?
	 this->getValue( 0 ) / b00 : complex< double >( 0.0, 0.0 )  );
  
}
