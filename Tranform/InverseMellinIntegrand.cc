/*!
  \file    InverseMellinIntegrand.cc
  \version $Revision: 1.19 $
  \date    $Date: 2009/05/12 05:37:58 $
  \author  $Author: miyachi $
  \brief   class implementation of integrand of the Mellin inversion
*/

#include "InverseMellinIntegrand.hh"

#include <iostream>

using namespace std;
using namespace Transform;

//InverseMellinIntegrand( ComplexFunction *f, InverseMellin* invm ) : 
InverseMellinIntegrand::InverseMellinIntegrand( ComplexFunction *f ) : 
  f_( f ), 
  x1_( 0.1 ), x2_( 0.0 ), dx_( 0.0 ), 
  z1_( 0.1 ), z2_( 0.0 ), dz_( 0.0 ),
  rf_( *this ),
  partialx_( false ), partialz_( false ), 
  momx_( 1.0 ), momz_( 1.0 ), 
  dim_( 1 ),  minimum_( 4.0 )
{
}

complex< double > 
InverseMellinIntegrand::operator()( const complex< double >& n ){
  if( f_ == NULL ) return complex< double >( 0.0, 0.0 );
  complex< double > val = (*f_)( n );
  if( ! partialx_ ) return pow( x1_, - n ) * val;
  complex< double > N = momx_ - n;
  return ( pow( x2_, N ) - pow( x1_, N ) ) / N / dx_ * val;
}

complex< double > 
InverseMellinIntegrand::operator()( const complex< double >& n,
				    const complex< double >& m ){
  
  if( f_ == NULL ) return complex< double >( 0.0, 0.0 );
  
  complex< double > weightx;
  if( partialx_ ){
    complex< double > nx = momx_ - n;
    weightx = ( pow( x2_, nx ) - pow( x1_, nx ) ) / nx / dx_ ;
  } else {
    weightx = pow( x1_, - n );
  }
  
  complex< double > weightz;
  if( partialz_ ){
    complex< double > nz = momz_ - m;
    weightz = ( pow( z2_, nz ) - pow( z1_, nz ) ) / nz / dz_ ;
  } else {
    weightz = pow( z1_, - m );
  }

  return weightx * weightz * (*f_)( n, m );
}

InverseMellinIntegrand& InverseMellinIntegrand::x( const double& x ) {
  x1_ = x; 
  partialx_ = false; 
  return *this;
}

//! set value of x
InverseMellinIntegrand& InverseMellinIntegrand::x( const double& x1,
						   const double& x2 ) { 
  x1_ = x1; 
  if( x1 != x2 ){
    x2_ = x2; 
    dx_ = x2_ - x1_;
    partialx_ = true; 
  } else {
    partialx_ = false; 
  }
  return *this; 
}

InverseMellinIntegrand& InverseMellinIntegrand::x( const vector< double >& x ){
  int n = x.size();
  
  if( n != 1 && n != 2 ) {
    cerr << __FILE__ << ":" << __LINE__ 
	 << "  x should contain one or two elements. but size is "
	 << n << endl;
    return *this;
  }
  
  x1_ = x[0];
  partialx_ = false; 
  
  if( n == 2 && x[ 0 ] != x[ 1 ] ){
    x2_ = x[1]; 
    partialx_ = true; 
    dx_ = x2_ - x1_;
  }
  
  return *this; 
}

InverseMellinIntegrand& InverseMellinIntegrand::z( const vector< double >& z ){
  int n = z.size();
  
  if( n != 1 && n != 2 ) {
    cerr << __FILE__ << ":" << __LINE__ 
	 << "  z should contain one or two elements. but size is "
	 << n << endl;
    return *this;
  }
  
  z1_ = z[0];
  partialz_ = false; 
  
  if( n == 2 && z[ 0 ] != z[ 1 ] ){
    z2_ = z[1]; 
    dz_ = z2_ - z1_;
    partialz_ = true; 
  }
  
  return *this; 
}


InverseMellinIntegrand& InverseMellinIntegrand::z( const double& z ){
  z1_ = z; 
  partialz_ = false; 
  return *this;
}

//! set value of x
InverseMellinIntegrand& InverseMellinIntegrand::z( const double& z1,
						   const double& z2 ) { 
  z1_ = z1; 
  if( z1 != z2 ){
    z2_ = z2; 
    dz_ = z2_ - z1_;
    partialz_ = true; 
  } else {
    partialz_ = false; 
  }
  return *this; 
}

double InverseMellinIntegrand::c0(){
  return this->minimum( minimum_, 0.01 );
  //  vector< double > x( 3, 0.0 );
  //  x[0] = 1.2;
  //  x[1] = 1.3;
  //  x[2] = 6.0;
  //  return this->minimum( x );
}

double InverseMellinIntegrand::c1( const double& c0 ){
  complex< double > z( c0, 0.0 );
  static complex< double > dz( 0.04, 0.0 );
  return 
    ( this->differential( z, 3, dz ) /
      this->differential( z, 2, dz ) ).real() / 6.0 ; 
}

double InverseMellinIntegrand::c2( const double& c0 ){
  complex< double > z( c0, 0.0 );
  static complex< double > dz( 0.04, 0.0 );
  return
    sqrt( 2.0 * ( (*this)( z ).real() / 
		  this->differential( z, 2, dz ) ).real() );
}


void InverseMellinIntegrand::setMomParam( const vector< double >& mom ){
  if( mom.size() == 0 ) return;
  momx_ = mom[ 0 ];
  if( mom.size() > 1 ) momz_ = mom[ 1 ];
  return;
}
