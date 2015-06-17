// $Id: GridIntegration.cc,v 1.19 2009/07/28 05:44:48 miyachi Exp $
/*!
  \file    GridIntegration.cc
  \version $Revision: 1.19 $
  \date    $Date: 2009/07/28 05:44:48 $
  \author  $Author: miyachi $
  \brief   class implementation of integrand of based on the grid.
*/
#include "GridIntegration.hh"

#include <iostream>
#include <iomanip>

using namespace std;
using namespace Transform;

int GridIntegration::maxDepth_ = 15;
//int GridIntegration::maxDepth_ = 10;

GridIntegration::GridIntegration( Integration *method1, Integration *method2,
				  int n, double precision )
  : nGridX_( n ), nGridY_( n ), method1_( method1 ), method2_( method2 ), 
    precision_( precision ), depth_( 0 )
{
  this->sort_methods();
}

GridIntegration::~GridIntegration(){
}

void GridIntegration::integ_( const double& x, const double& dx, 
			      RealFunction& f, 
			      vector< double >& v1,
			      vector< double >& v2 ){
  double a( x ), b;
  for( int i = 0; i < nGridX_; i++ ){
    b = a + dx;
    v1[ i ] = method1_->integral( a, b, f );
    if( method2_ ) v2[ i ] = method2_->integral( a, b, f );
    a = b;
  }
}

void GridIntegration::integ_( const double& x, const double& dx, 
			      const double& z, const double& dz, 
			      RealFunction& f, 
			      vector< vector< double > >& v1,
			      vector< vector< double > >& v2 ) {
  double a1( x ), a2;
  for( int i = 0; i < nGridX_; i++ ){
    a2 = a1 + dx;
    double b1( z ), b2;
    for( int j = 0; j < nGridY_; j++ ){
      b2 = b1 + dz;
      v1[ i ][ j ] = method1_->integral( a1, a2, b1, b2, f );
      if( method2_ ) v2[ i ][ j ] = method2_->integral( a1, a2, b1, b2, f );
      b1 = b2;
    }
    a1 = a2;
  }
}

void GridIntegration::cinteg_( const double& x, const double& dx, 
			       ComplexFunction& f, 
			       vector< complex< double > >& v1,
			       vector< complex< double > >& v2 ){
  double a1( x ), a2;
  for( int i = 0; i < nGridX_; i++ ){
    a2 = a1 + dx;
    v1[ i ] = method1_->cintegral( a1, a2, f );
    if( method2_ ) v2[ i ] = method2_->cintegral( a1, a2, f );
    a1 = a2;
  }
}


void GridIntegration::cinteg_( const double& x, const double& dx, 
			       const double& z, const double& dz, 
			       ComplexFunction& f, 
			       vector< vector< complex< double > > >& v1,
			       vector< vector< complex< double > > >& v2 ){
  double a1( x ), a2;
  for( int i = 0; i < nGridX_; i++ ){
    a2 = a1 + dx;
    double b1( z ), b2;
    for( int j = 0; j < nGridY_; j++ ){
      b2 = b1 + dz;
      v1[ i ][ j ] = method1_->cintegral( a1, a2, b1, b2, f );
      if( method2_ ) v2[ i ][ j ] = method2_->cintegral( a1, a2, b1, b2, f );
      b1 = b2;
    }
    a1 = a2;
  }
}

bool GridIntegration::acceptable( const double& v1, const double& v2 ){
  return this->acceptable( v1, v2, 0.5 * ( fabs( v1 ) + fabs( v2 ) ) );
}

bool GridIntegration::acceptable( const double& v1, const double& v2, 
				  const double& ref ){
  double limit = precision_ * fabs( ref );
  double diff  = fabs( v1 - v2 );
  return ( limit < 1.0E-15 || diff < limit );
}

bool GridIntegration::acceptable( const complex< double >& v1, 
				  const complex< double >& v2 ){
  return this->acceptable( v1, v2, 0.5 * ( abs( v1 ) + abs( v2 ) ) );
}

bool GridIntegration::acceptable( const complex< double >& v1, 
				  const complex< double >& v2,
				  const complex< double >& ref ){
  double limit = precision_ * abs( ref );
  double diff  = abs( v1 - v2 );
  return ( limit < 1.0E-15 || diff < limit );
}

double GridIntegration::integral( const double& min, const double& max, 
				  RealFunction& f ) {
  
  // no grid or no integration method
  if( nGridX_ < 1 || method1_ == NULL ) return 0.0;
  
  double dstep = ( max - min ) / nGridX_;    // calculate grid size
  
  vector< double > v1( nGridX_, 0.0 );    // for method 1
  vector< double > v2( nGridX_, 0.0 );    // for method 2
  this->integ_( min, dstep, f, v1, v2 ); // calculate integral
  
  double v1sum = this->vectorSum( v1 );
  double v2sum = this->vectorSum( v2 );

  if( depth_ > maxDepth_ || method2_ == NULL   ||
      this->acceptable( v1sum, v2sum ) ) return v1sum;
  
  double x1;
  for( int i = 0; i < nGridX_; i++ ) {
    if( this->acceptable( v1[ i ], v2[ i ], v1sum ) ) continue;
    x1 = min + i * dstep;     // re-calculation in sub-grid i
    depth_++; 
    ( v1sum -= v1[ i ] ) += ( v1[ i ] = this->integral( x1, x1 + dstep, f ) );
    depth_--;
  }
  return this->vectorSum( v1 );
}

double GridIntegration::integral( const double& xmin, const double& xmax, 
				  const double& zmin, const double& zmax, 
				  RealFunction& f ) {
  
  if( nGridX_ < 1 || nGridY_ < 1 || method1_ == NULL ) return 0.0;
  
  double dx = ( xmax - xmin ) / nGridX_; // x step size
  double dz = ( zmax - zmin ) / nGridY_; // z step size
  
  vector< vector< double > > v1( nGridX_, vector< double >( nGridY_, 0.0 ) );
  vector< vector< double > > v2( nGridX_, vector< double >( nGridY_, 0.0 ) );
  this->integ_( xmin, dx, zmin, dz, f, v1, v2 );
  
  double v1sum = this->vectorSum( v1 );
  double v2sum = this->vectorSum( v2 );
  if( depth_ > maxDepth_ || method2_ == NULL || 
      this->acceptable( v1sum, v2sum ) ) return v1sum;
  
  double x1, z1;
  for( int i = 0; i < nGridX_; i++ ) {
    for( int j = 0; j < nGridY_; j++ ) {
      if( this->acceptable( v1[ i ][ j ], v2[ i ][ j ], v1sum ) ) continue;
      x1 = xmin + i * dx; z1 = zmin + j * dz;
      depth_++; 
      ( v1sum -= v1[ i ][ j ] )
	+= ( v1[ i ][ j ] = this->integral( x1, x1 + dx, z1, z1 + dz, f ) );
      depth_--;
    }
  }
  return this->vectorSum( v1 );
}

complex< double > 
GridIntegration::cintegral( const double& min, const double& max, 
			    ComplexFunction& f ) {
  
  // no grid or no integration method
  if( nGridX_ < 1 || method1_ == NULL ) return 0.0;
  
  double dstep = ( max - min ) / nGridX_;    // calculate grid size
  
  vector< complex< double > > v1( nGridX_, complex< double >( 0.0, 0.0 ) );
  vector< complex< double > > v2( nGridX_, complex< double >( 0.0, 0.0 ) );
  this->cinteg_( min, dstep, f, v1, v2 );
  complex< double > v1sum = this->vectorSum( v1 );
  complex< double > v2sum = this->vectorSum( v2 );
  
  if( depth_ > maxDepth_ || method2_ == NULL ||
      this->acceptable( v1sum, v2sum ) ) return v1sum;
  
  double x1;
  for( int i = 0; i < nGridX_; i++ ) {
    if( this->acceptable( v1[ i ], v2[ i ], v1sum ) ) continue;
    // re-calculation in sub-grid i
    x1 = min + i * dstep;
    depth_++; 
    ( v1sum -= v1[ i ] ) += ( v1[ i ] = this->cintegral( x1, x1 + dstep, f ) );
    depth_--;
  }

  return this->vectorSum( v1 );
}

complex< double > 
GridIntegration::cintegral( const double& xmin, const double& xmax, 
			    const double& zmin, const double& zmax, 
			    ComplexFunction& f ) {
  
  if( nGridX_ < 1 || nGridY_ < 1 || method1_ == NULL ) return 0.0;
  
  double dx = ( xmax - xmin ) / nGridX_; // x step size
  double dz = ( zmax - zmin ) / nGridY_; // z step size
  
  vector< vector< complex< double > > > 
    v1( nGridX_, 
	vector< complex< double > >( nGridY_, complex< double >( 0.0, 0.0 ) ) );
  vector< vector< complex< double > > > 
    v2( nGridX_, 
	vector< complex< double > >( nGridY_, complex< double >( 0.0, 0.0 ) ) );
  
  this->cinteg_( xmin, dx, zmin, dz, f, v1, v2 );

  complex< double > v1sum = this->vectorSum( v1 );
  complex< double > v2sum = this->vectorSum( v2 );
  
  if( depth_ > maxDepth_ || method2_ == NULL   || 
      this->acceptable( v1sum, v2sum ) ) return v1sum; 
  
  double x1, x2, z1, z2;
  for( int i = 0; i < nGridX_; i++ ) {
    for( int j = 0; j < nGridY_; j++ ) {
      if( this->acceptable( v1[ i ][ j ], v2[ i ][ j ], v1sum ) ) continue;
      x1 = xmin + i * dx; z1 = zmin + j * dz;
      depth_++; 
      ( v1sum -= v1[ i ][ j ] ) += 
	( v1[ i ][ j ] = this->cintegral( x1, x1 + dx, z1, z1 + dz, f ) );
      depth_--;
    }
  }
  
  return this->vectorSum( v1 );
}

void GridIntegration::setmethod1( Integration* integ ) { 
  method1_ = integ; 
  this->sort_methods();
}

void GridIntegration::setmethod2( Integration* integ ) { 
  method2_ = integ; 
  this->sort_methods();
}

void GridIntegration::sort_methods(){
  if( ( method1_ == NULL ) && method2_ ){
    method1_ = method2_;
    method2_ = NULL;
  }
}

void GridIntegration::ngrid( const int& n ) { 
  this->ngrid( n, n );
}

void GridIntegration::ngrid( const int& n1, const int& n2 ) {
  nGridX_ = n1;
  nGridY_ = n2;
} 

