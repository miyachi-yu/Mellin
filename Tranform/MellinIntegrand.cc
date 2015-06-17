// $Id: MellinIntegrand.cc,v 1.4 2009/06/04 09:22:22 miyachi Exp $
/*!
  \file    MellinIntegrand.cc
  \version $Revision: 1.4 $
  \date    $Date: 2009/06/04 09:22:22 $
  \author  $Author: miyachi $
  \brief   class implementation of integrand of the Mellin transform
 */
#include "MellinIntegrand.hh"
using namespace std;
using namespace Transform;

MellinIntegrand::MellinIntegrand() : f_( NULL ), n_( 1.0, 0.0 ), m_( 1.0, 0.0 )
{
}

MellinIntegrand::MellinIntegrand( RealFunction *f ) : 
  f_( f ), n_( 1.0, 0.0 ), m_( 1.0, 0.0 )
{
}

MellinIntegrand::~MellinIntegrand(){
}

MellinIntegrand& MellinIntegrand::set( RealFunction *f ) {
  f_ = f;
  return *this;
}

MellinIntegrand& MellinIntegrand::set( const complex< double >& z ) {
  n_ = z;
  return *this;
}

MellinIntegrand& MellinIntegrand::set( const complex< double >& n, 
				       const complex< double >& m ) {
  n_ = n;
  m_ = m;
  return *this;
}

complex< double > MellinIntegrand::operator() ( const complex< double >& z ){
  return complex< double >( 0.0, 0.0 );
}

complex< double > MellinIntegrand::operator() ( const complex< double >& n,
						const complex< double >& m ){
  return complex< double >( 0.0, 0.0 );
}

complex< double > MellinIntegrand::operator() ( const double& x ){
  if( f_ == NULL ) return complex< double >( 0.0, 0.0 );
  return pow( x, n_ - complex< double >( 1.0, 0.0 ) ) * (*f_)( x );
}

complex< double > MellinIntegrand::operator() ( const double& x,
						const double& z ){
  if( f_ == NULL ) return complex< double >( 0.0, 0.0 );
  return 
    pow( x, n_ - complex< double >( 1.0, 0.0 ) ) * 
    pow( z, m_ - complex< double >( 1.0, 0.0 ) ) * 
    (*f_)( x, z );
}

