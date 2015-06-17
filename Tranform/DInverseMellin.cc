// $Id: DInverseMellin.cc,v 1.9 2008/10/26 08:36:35 imazu Exp $
/*!
  \file  DInverseMelli.cc
  \brief class implementation of double Mellin Inversion
 */
#include "DInverseMellin.hh"
#include <iostream>
#include <cmath>
#include <Tranform/GridIntegration.hh>
#include <Tranform/LegQuadrature.hh>

using namespace std;
using namespace Transform;

//	a default constructor. 
DInverseMellin::DInverseMellin( ComplexFunction *g, Integration *inter ) :
  Transformer(), integrand_( g )
{
  this->integration( inter );
  this->upper( 10. );
  this->lower( 0.0 );
  this->integrand_.integration( this->integration() );
  this->integrand_.upper( this->upper() );
  this->integrand_.lower( this->lower() );
}

//a constructor only with integration method 
DInverseMellin::DInverseMellin( Integration *inter ) :
  Transformer(), integrand_( (ComplexFunction*) NULL )
{
  this->integration( inter );
  this->upper( 10. );
  this->lower( 0.0 );
  this->integrand_.integration( this->integration() );
  this->integrand_.upper( this->upper() );
  this->integrand_.lower( this->lower() );
}

//	a destructor 
DInverseMellin::~DInverseMellin(){
}

void DInverseMellin::upper( const double& v ){
  upper_ = v;
  this->integrand_.upper( this->upper() );
}

void DInverseMellin::upper( const vector< double >& v ){
  if( v.size() == 0 ) return;
  this->integrand_.upper( v[ 0 ] ); // x
  upper_ = v[ ( v.size() == 1 ? 0 : 1 ) ]; // z
}

double DInverseMellin::upper(){
  return upper_;
}

void DInverseMellin::setParameter( const std::vector< double >& c, 
				   const std::vector< double >& phi ){
  integrand_.setParameter( c, phi );
}	

void DInverseMellin::setMomParam( const std::vector< double >& mom ){
  integrand_.setMomParam( mom );
}	

double DInverseMellin::operator()( const std::vector< double >& x, 
				   const std::vector< double >& z ) {
  this->setxzparams( x, z );
  //  cout << integrand_.set( x, z ) << endl;
  return - 0.5 / M_PI / M_PI * this->integration()->integral
       ( this->lower(), this->upper(), integrand_.set( x, z ) );
}

//	set Complex function for Mellin transformation 
DInverseMellin& DInverseMellin::transform( ComplexFunction *g ){
  integrand_.set( g );
  return *this;
}
