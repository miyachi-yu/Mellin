// $Id: Trapezoid.cc,v 1.5 2008/08/27 02:49:29 miyachi Exp $
/*!
  \file    Trapezoid.hh
  \version $Revision: 1.5 $
  \date    $Date: 2008/08/27 02:49:29 $
  \author  $Author: miyachi $
  \brief   class implementation of Trapezoid approximation
 */
#include "Trapezoid.hh"
using namespace std;
using namespace Transform;

Trapezoid::Trapezoid( const int& n ) : 
  n_( n ), s_( 0.0 ), cs_( 0.0, 0.0 )
{
}

Trapezoid::~Trapezoid() {
}

double Trapezoid::integral( const double& min, const double& max, 
			    RealFunction& f ) {
  for( int i = 1; i <= n_; i++ ) this->integral( min, max, f, i );
  return s_;
}

double Trapezoid::integral( const double& min, const double& max, 
			    RealFunction& f, const int& n ) {
  
  if( n == 1 ) return s_ = 0.5 * ( max - min ) * ( f( max ) + f( min ) );
  int it, j;
  for( it = 1, j = 1; j < n - 1; j++ ) it <<= 1;
  double tnm = it;
  double del = ( max - min ) / tnm;
  double x = min + 0.5 * del;
  double sum = 0.0;
  for( sum = 0.0, j = 1; j <= it; j++, x+=del ) sum += f( x );
  return s_ = 0.5 * ( s_ + ( max - min ) * sum / tnm );
}

complex< double > Trapezoid::cintegral( const double& min, const double& max, 
					ComplexFunction& f ) {
  for( int i = 1; i <= n_; i++ ) this->cintegral( min, max, f, i );
  return s_;
}

complex< double > 
Trapezoid::cintegral( const double& min, const double& max, 
		      ComplexFunction& f, const int& n ) {
  if( n == 1 ) return cs_ = ( f( max ) + f( min ) ) * ( max - min ) * 0.5 ;
  
  int it, j;
  for( it = 1, j = 1; j < n - 1; j++ ) it <<= 1;
  double tnm = it;
  double del = ( max - min ) / tnm;
  double x = min + 0.5 * del;
  complex< double > sum( 0.0, 0.0 );
  for( sum = 0.0, j = 1; j <= it; j++, x+=del ) sum += f( x );
  return cs_ = 0.5 * ( cs_ + ( max - min ) * sum / tnm );
}

double Trapezoid::integral( const double& xmin, const double& xmax, 
			    const double& zmin, const double& zmax, 
			    RealFunction& f ) {
  return s_ = 0.25 * ( xmax - xmin ) * ( zmax - zmin ) 
    * ( f( xmin, zmin ) + f( xmin, zmax ) + f( xmax, zmin ) + f( xmax, zmax ) );
  return s_;
}

complex< double > Trapezoid::cintegral( const double& xmin, const double& xmax, 
					const double& zmin, const double& zmax, 
					ComplexFunction& f ) {
  return cs_ = 0.25 * ( xmax - xmin ) * ( zmax - zmin ) 
    * ( f( xmin, zmin ) + f( xmin, zmax ) + f( xmax, zmin ) + f( xmax, zmax ) );
  return cs_;
}
