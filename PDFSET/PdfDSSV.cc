// $Id: PdfDSSV.cc,v 1.1 2009/07/28 04:07:32 miyachi Exp $
/*!
  \file     PdfDSSV.cc
  \version  $Revision: 1.1 $
  \author   $Author: miyachi $
  \brief    Implementation of PDF parameterization ( x^a*(1-x)^b*(1-c*x) )
 */
#include "PdfDSSV.hh"
#include <QCD/Flavor.hh>
#include <QCD/PdfParametrization.hh>

using namespace std;
using namespace Transform;
using namespace PDFSET;

PdfDSSV::PdfDSSV() : 
  QCD::PdfBase( PdfBase::parton, 1, 5 ),
  b1_( 1.0, 1.0 ), b2_( 2.0, 1.0 ), b3_( 1.5, 1.0 ), 
  A_( 0.0, 0.0 ), a_( 0.0 ), b_( 0.0 ), c_( 0.0 ), d_( 0.0 )
{
}

PdfDSSV::~PdfDSSV() {
}

complex< double > PdfDSSV::operator() ( const complex< double >& z ) {
  
  if( A_ == complex< double >( 0.0, 0.0 ) ) return A_;
  
  complex< double > v =       b1_( z );
  if( c_ != 0.0 )   v += c_ * b2_( z );
  if( d_ != 0.0 )   v += d_ * b3_( z );
  
  return A_ * v;
}

double PdfDSSV::x( const double& x ){
  if( A_.real() == 0.0 ) return 0.0;
  return A_.real() *
    pow( x, a_ - 1.0 ) * pow( 1.0 - x, b_ ) * ( 1.0 + c_ * sqrt(x) + d_ * x );
}

void PdfDSSV::update() {
  
  static double c3 = 1.0 / 3.0;
  static double c6 = 1.0 / 6.0;
  
  if( this->parametrization() == NULL ) return;
  if( this->reference() ) this->reference()->update();
  
  // flavor SU(3) combination only define the first moment size
  if( this->type() == combo ) return ; 
  
  QCD::Flavor::PARTON pid = ( QCD::Flavor::PARTON ) this->id();

  double N = 0.0;
  if( pid != QCD::Flavor::U && pid != QCD::Flavor::D && pid != QCD::Flavor::S ){
    
    N = this->getValue( 0 );
    
  } else {
    
    double a0 = ( this->parametrization()->a0() ?
		  this->parametrization()->a0()->getValue( 0 ) : 0.0 );
    
    double a3 = ( this->parametrization()->a3() ?
		  this->parametrization()->a3()->getValue( 0 ) : 0.0 );
    
    double a8 = ( this->parametrization()->a8() ?
		  this->parametrization()->a8()->getValue( 0 ) : 0.0 );
    
    switch( pid ){
    case QCD::Flavor::U : N = c3 * a0 + 0.5 * a3 + c6 * a8; break;
    case QCD::Flavor::D : N = c3 * a0 - 0.5 * a3 + c6 * a8; break;
    case QCD::Flavor::S : N = c3 * ( a0 - a8 );             break;
    }
  }
  
  if( N == 0.0 ) { A_ = complex< double >( 0.0, 0.0 ); return ; }
  
  a_ = this->getValue( 1 );
  b_ = this->getValue( 2 );
  c_ = this->getValue( 3 );
  d_ = this->getValue( 4 );
  
  static complex< double > mom = complex< double >( 1.0, 0.0 );
  
  b1_.set( a_ - 1.0, b_ );
  complex< double > b00 = b1_( mom );
  
  if( c_ != 0.0 ){
    b2_.set( a_ - 0.5, b_ );
    b00 += c_ * b2_( mom );
  }
  
  if( d_ != 0.0 ){
    b3_.set( a_, b_ );
    b00 += d_ * b3_( mom );
  }
  
  A_ = ( b00 != complex< double >( 0.0, 0.0 ) ? 
	 N / b00 : complex< double >( 0.0, 0.0 ) );
}
