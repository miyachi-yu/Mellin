// $Id: PdfStat.cc,v 1.1 2009/07/28 04:07:32 miyachi Exp $
/*!
  \file     PdfStat.cc
  \version  $Revision: 1.1 $
  \author   $Author: miyachi $
  \brief    Implementation of PDF parameterization ( x^a*(1-x)^b*(1-c*x) )
 */
#include "PdfStat.hh"
#include <QCD/Flavor.hh>
#include <QCD/PdfParametrization.hh>
#include <cmath>

using namespace std;
using namespace Transform;
using namespace PDFSET;

Transform::CGamma   PdfStat::Ga_;
Transform::CPolyLog PdfStat::Li_;
Transform::CZeta    PdfStat::ze_;

PdfStat::PdfStat() : 
  QCD::PdfBase( PdfBase::parton, 1, 9 ),
  A1_( 0.0 ), b1_( 0.0 ), A2_( 0.0 ), b2_( 0.0 ),
  Xp_( 0.0 ), Xm_( 0.0 ), xb_( 0.0 ),
  eXp_( 0.0, 0.0 ), eXm_( 0.0, 0.0 )
{
}

PdfStat::~PdfStat() {
}

complex< double > PdfStat::operator() ( const complex< double >& z ) {
  if( (QCD::Flavor::PARTON) this->id() == QCD::Flavor::g ) return this->G(z);
  
  complex< double > z1 = z + b1_;
  complex< double > z2 = z + b2_;
  
  return
    - A1_ * pow( xb_, z1 ) * Ga_( z1 ) * ( Xp_ * Li_( z1, eXp_ ) + Xm_ * Li_( z1, eXm_ ) )
    - A2_ * pow( xb_, z2 ) * Ga_( z2 ) * Li_( z2, complex< double >( -1.0, 0.0 ) );

}

complex< double > PdfStat::G( const complex< double >& z ){
  complex< double > z1 = z + b1_;
  return A1_ * pow( xb_, z1 ) * Ga_( z1 ) * ze_( z1 );
}

double PdfStat::G( const double& x ){
  return 0;
}

double PdfStat::x( const double& x ){
  if( (QCD::Flavor::PARTON) this->id() == QCD::Flavor::g ) return this->G(x);
  return 0;
}

void PdfStat::update() {
  
  if( this->parametrization() == NULL ) return;
  if( this->reference() ) this->reference()->update();
  
  A1_ = this->getValue( 0 );
  b1_ = this->getValue( 1 ) - 1.0;  // parameterization is given for xq
  A2_ = 2.0 * this->getValue( 2 );
  b2_ = this->getValue( 3 ) - 1.0;  // parameterization is given for xq
  Xp_ = this->getValue( 4 );
  Xm_ = this->getValue( 5 );
  xb_ = this->getValue( 6 );
  
  if( (QCD::Flavor::PARTON) this->id() == QCD::Flavor::g )
    return;
  
  eXp_ = -1.0 * exp( Xp_ / xb_ );
  eXm_ = -1.0 * exp( Xm_ / xb_ );
  
  if( this->id() < 0 ){
    Xp_ = 1.0 / Xp_;
    Xm_ = 1.0 / Xm_;
    eXp_ = 1.0 / eXp_;
    eXm_ = 1.0 / eXm_;
  }
  
}
