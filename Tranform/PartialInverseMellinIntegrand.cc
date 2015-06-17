/*!
  \file    PartialInverseMellinIntegrand.cc
  \version $Revision: 1.7 $
  \date    $Date: 2008/07/09 03:11:03 $
  \author  $Author: miyachi $
  \brief   class implementation of integrand of the Mellin inversion
*/
#include "PartialInverseMellinIntegrand.hh"

#include <iomanip>
#include <iostream>
#include <cmath>

using namespace std;
using namespace Transform;

//	a default constructor 
PartialInverseMellinIntegrand::PartialInverseMellinIntegrand() : 
  g_( NULL ), 
  x1_( 0.0 ), x2_( 1.0 ), dx_( x2_ - x1_ ),
  c_( 1.8 ), e_( cos( 0.65 * M_PI ), sin( 0.65 * M_PI  ) ), mom_( 1.0 )
{ }


//	a constructor with a pointer to a ComplexFunction to be transform
PartialInverseMellinIntegrand::PartialInverseMellinIntegrand( ComplexFunction *g ) : 
  g_( g ),
  x1_( 0.0 ), x2_( 1.0 ), dx_( x2_ - x1_ ),
  c_( 1.8 ), e_( cos( 0.65 * M_PI ), sin( 0.65 * M_PI  ) ), mom_( 1.0 )
{ }

//	a destructor
PartialInverseMellinIntegrand::~PartialInverseMellinIntegrand(){ }


//set parameter for changed integration variable   
void PartialInverseMellinIntegrand::setParameter( const double& c, 
						  const double&  phi ){
  c_ = c;
  e_ = complex< double >( cos( phi ), sin( phi ) );
}

//set a pointer to ComplexFunction 
PartialInverseMellinIntegrand& 
PartialInverseMellinIntegrand::set( ComplexFunction *g ) {
  g_ = g;
  return *this;
}

//set a real variable in the integrand 
PartialInverseMellinIntegrand& 
PartialInverseMellinIntegrand::set( const double& x1, const double& x2 ) {
  x1_ = x1; x2_ = x2;
  dx_ = x2_ - x1_ ;
  return *this;
}

/*
complex< double > 
PartialInverseMellinIntegrand::operator() ( const complex< double >& z ){
  return complex< double >( 0.0, 0.0 );
}
*/

//return a value to be integrated by PartialInverseMellin transformation 
double PartialInverseMellinIntegrand::operator() ( const double& r ){
  
  if( g_ == NULL ) return 0.0;
  
  complex< double > z = c_ + r * e_ ;
  complex< double > z1 = mom_ - z;
  return 
    imag( e_ * ( pow( x2_, z1 ) - pow( x1_, z1 ) ) / z1 / dx_
	  * ( *g_ )( z ) ) ; 
}


//-buck up------------------------------------

/*
//return a value to be integrated by PartialInverseMellin transformation 
double PartialInverseMellinIntegrand::operator() ( double x ){
  return 0.0;
  }
  
  complex< double > PartialInverseMellinIntegrand::operator() ( complex< double > z ){

  return pow( x, z_ - complex< double >( 1.0, 0.0 ) ) * (*f_)( x );
}


*/


