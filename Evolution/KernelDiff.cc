// $Id: KernelDiff.cc,v 1.1 2009/02/13 12:23:40 miyachi Exp $
/*!

  \brief Class definition of Evolution based on multiple parameterization

// KernelDiff.cc
//  
// Made by Yoshiyuki Miyachi
// Login   <miyachi@yoshiyuki-desktop>
// 
// Started on  Sun Oct 26 16:05:39 2008 Yoshiyuki Miyachi
// Started on  Sun Oct 26 16:05:39 2008 Yoshiyuki Miyachi
//
*/

#include <iostream>
#include <iomanip>

#include "KernelDiff.hh"
#include <Utility/Arguments.hh>

using namespace std;
using namespace Evolution;

KernelDiff::KernelDiff( Kernel* target, Kernel* base ) throw( int ) : 
  Kernel( Utility::Arguments::ref() ), 
  params_()
{
  
  if( target == NULL || base == NULL ){
    ARG_ERR << "target and base Kernel must not be NULL" << endl;
    throw( 1 );
  }

  // --------------------------------------------------------------------
  //     alpha_s
  if( target->alpha() != base->alpha() ) {
    ARG_ERR << "alpha in target and base must be same" << endl;
    throw( 2 );
  }
  this->alpha() = base->alpha();
  
  // --------------------------------------------------------------------
  //     Nf handling
  if( target->constNf() != base->constNf() ){
    ARG_ERR << "Nf mode in target and base must be same" << endl;
    throw( 3 );
  }
  this->constNf( base->constNf() );      // assign constNf mode
  
  // --------------------------------------------------------------------
  //     initial distribution
  try {
    params_.set( target->initialDistribution(), base->initialDistribution() );
  }
  catch( int error ){
    ARG_ERR << "catch error(" << error << ")" << endl;
    throw( error );
  }
  
  // set initial distribution
  this->initialDistribution( &params_ ); 
  
  // Set anomalous dimension using one of the evolution kernel
  this->anomalousDimension( base->anomalousDimension() );
  
  // reset value of N with 
  this->setN( base->n() );
  
  // clear stored cached data....
  this->clearCache();
}

KernelDiff::~KernelDiff(){
}

// ---------------------------------------------------------------- //
//                           Param Sum                              //
// ---------------------------------------------------------------- //
KernelDiff::ParamDiff::ParamDiff( ) : 
  QCD::PdfParametrization(), 
  p1_( NULL ), p2_( NULL )
{
  name_ = "diff";
}

KernelDiff::ParamDiff::~ParamDiff() {
}

void KernelDiff::ParamDiff::set( PdfParametrization* target, 
				 PdfParametrization* base ) throw( int ) {
  
  if( target == NULL || base == NULL ){
    ARG_ERR << "target and base parameterization must not be NULL" << endl;
    throw( 1 );
  }
  
  if( target->initialScale() != base->initialScale() ){
    ARG_ERR << "parameterizations must have same initial scale." << endl;
    ARG_ERR << "target: " << setw(10) << target->initialScale() << endl;
    ARG_ERR << "base:   " << setw(10) << base->initialScale() << endl;
    throw( 2 );
  }
  
  p1_ = target;
  p2_ = base;

}

complex< double > KernelDiff::ParamDiff::q( const QCD::Flavor::PARTON& q ){
  return 
    ( p1_ ? p1_->q( q ) : complex< double >( 0.0, 0.0 ) ) -
    ( p2_ ? p2_->q( q ) : complex< double >( 0.0, 0.0 ) )   ;
}

complex< double > KernelDiff::ParamDiff::q( const QCD::Flavor::TYPE& q ){
  return 
    ( p1_ ? p1_->q( q ) : complex< double >( 0.0, 0.0 ) ) -
    ( p2_ ? p2_->q( q ) : complex< double >( 0.0, 0.0 ) )   ;
}

void KernelDiff::ParamDiff::setN( const std::complex< double >& n ){
  if( p1_ ) p1_->setN( n );
  if( p2_ ) p2_->setN( n );
  n_ = n;
}
