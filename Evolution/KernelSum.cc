// $Id: KernelSum.cc,v 1.9 2008/11/11 05:49:33 miyachi Exp $
/*!

  \brief Class definition of Evolution based on multiple parameterization

// KernelSum.cc
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

#include "KernelSum.hh"
#include <Utility/Arguments.hh>

using namespace std;
using namespace Evolution;

KernelSum::KernelSum() : 
  Kernel( Utility::Arguments::ref() ), vector< Kernel* >( 0 ),
  params_()
{
}

KernelSum::~KernelSum(){
}

void KernelSum::add( Kernel* k ) throw( int ) {
  
  // alpha_Mz
  QCD::Alpha& myAs = this->alpha();
  QCD::Alpha& as   = k->alpha();
  
  if( this->size() == 0 ){
    
    myAs = as;                          // assign alpha_s
    this->constNf( k->constNf() );      // assign constNf mode
    
  } else {
    
    if( myAs != as ){
      ARG_ERR << "The given kernel has different alpha_s object."  << endl;
      ARG_ERR << "Kernels should have same property...." << endl;
      throw( 1 );
    } 
    
    if( this->constNf() != k->constNf() ){
      ARG_ERR << "The given kernel has different const NF flag"  << endl;
      ARG_ERR << "Kernels should have same property...." << endl;
      throw( 2 );
    }
    
  }
  
  try {
    // add parameterization to the container object
    params_.add( k->initialDistribution() );
    if( this->initialDistribution() == NULL ) 
      this->initialDistribution( &params_ ); // set initial distribution 
  }
  
  catch( int error ){
    ARG_ERR << "catch error (" << error << ")" << endl;
    throw( error );
  }

  // set the given kernel 
  this->push_back( k );
  
  // Set anomalous dimension using one of the evolution kernel
  this->anomalousDimension( k->anomalousDimension() );
  
  // reset value of N with 
  this->setN( k->n() );
  
  // clear stored cached data....
  this->clearCache();
}

// ---------------------------------------------------------------- //
//                           Param Sum                              //
// ---------------------------------------------------------------- //
KernelSum::ParamSum::ParamSum() : 
  QCD::PdfParametrization(), 
  vector< QCD::PdfParametrization* >( 0 )
{
  name_ = "sum";
}

KernelSum::ParamSum::~ParamSum() {
}

complex< double > KernelSum::ParamSum::q( const QCD::Flavor::PARTON& q ){
  complex< double > val( 0.0, 0.0 );
  for( int i = 0; i < this->size(); i++ ) val += (*this)[ i ]->q( q );
  return val;
}

complex< double > KernelSum::ParamSum::q( const QCD::Flavor::TYPE& q ){
  complex< double > val( 0.0, 0.0 );
  for( int i = 0; i < this->size(); i++ ) val += (*this)[ i ]->q( q );
  return val;
}

void KernelSum::ParamSum::add( QCD::PdfParametrization *pdf ) throw( int ) {
  
  if( this->size() == 0 ){
    scale_ = pdf->initialScale();
  } else {
    if( scale_ != pdf->initialScale() ){
      ARG_ERR << "Initial distribution has diffrent initial scale." << endl;
      ARG_ERR << "Please check your initial distribution file." << endl;
      throw( 1 );
    }
  }
  
  this->push_back( pdf );
  this->pdfs().insert( this->pdfs().end(), 
		       pdf->pdfs().begin(), pdf->pdfs().end() );
}

void KernelSum::ParamSum::setN( const std::complex< double >& n ){
  for( int i = 0; i < this->size(); i++ ) (*this)[ i ]->setN( n );
  n_ = n;
}
