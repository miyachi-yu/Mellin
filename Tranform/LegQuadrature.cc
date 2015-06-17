// $Id: LegQuadrature.cc,v 1.12 2009/07/28 05:44:48 miyachi Exp $
/*!
  \file      LegQuadrature.cc
  \version   $Revison:$
  \author    $Author: miyachi $
  \date      $Date: 2009/07/28 05:44:48 $
  \brief     Implementation of Legendre-Gauss integration class
  
  $Log: LegQuadrature.cc,v $
  Revision 1.12  2009/07/28 05:44:48  miyachi
  Big update...... PDF parameterization sets are moved to PDFSET subdirectory

  Revision 1.11  2009-01-30 15:18:11  miyachi
  *** empty log message ***

  Revision 1.10  2008-10-23 04:50:50  miyachi
  *** empty log message ***

  Revision 1.9  2008-08-28 10:13:49  imazu
  *** empty log message ***

  Revision 1.8  2008-08-25 09:47:14  miyachi
  *** empty log message ***

  Revision 1.7  2008-08-19 03:47:10  miyachi
  clean up, a bit....

  Revision 1.6  2008-07-24 21:28:11  miyachi
  Double inversion is implemented in also InverseMellin, in bit different way. Acoordingly, the integration classes were modified...

  Revision 1.5  2008-07-17 07:24:25  imazu
  short changes

  Revision 1.4  2008/07/09 06:08:49  miyachi
  further test of Confluent HyperGeometric function with Error function.... see confluent.cc

  Revision 1.3  2008/05/23 02:08:02  miyachi
  Huge update. Now each library has own namespace. One has to explcitly specify the namespace.

  Revision 1.2  2008-05-09 06:51:34  miyachi
  Does it help the infinit loop?

  Revision 1.1  2007/04/19 10:01:22  miyachi
  First release of libTransform.so

  Revision 1.10  2007/01/19 02:12:41  miyachi
  Add another version of GaussLegendre integration.

  Revision 1.9  2007/01/10 03:26:29  miyachi
  Remove ^M character inserted in the original code. (PC)

  Revision 1.8  2007/01/10 02:16:46  miyachi
  The majour differences are:

  - Modify the algorythm of precision checking used in TrapInt class.
    Now it check the difference between trapezoid approximation and
    simple mid-point integration.

  - Add comments for doxygen. Including some CVS tag in the file comment.

 */
#include "LegQuadrature.hh"

#include <iostream>
#include <cmath>

using namespace std;
using namespace Transform;

double LegQuadrature::torrelance = 1.0e-15;

double LegQuadrature::calcX( const int& i, const int& n ){
  
  static int jmax = 50;
  
  //  initial values for Newton's method
  double xk0 = cos( M_PI * ( i + 0.75 )/( n + 0.5 ) );
  double pn  =  Pn_Legendre( n, xk0 );
  double dpn = dPn_Legendre( n, xk0 );
  
  int j = 0;
  while( fabs( pn / dpn ) > torrelance && j < jmax ){
    xk0 -= pn / dpn;
    pn   =  Pn_Legendre( n, xk0 );
    dpn  = dPn_Legendre( n, xk0 );
    j++;
  }
  
  if( j == jmax ){
    std::cout << "i = " << i << " : not convergent." 
	      << std::endl;
  }
  
  return xk0;
}

double LegQuadrature::Pn_Legendre( const int& n, const double& x ) {
  if( n < 0 )  return 0.0;
  if( n == 0 ) return 1.0;
  if( n == 1 ) return x;
  double ninv = 1.0 / n;
  return 
    ( 2.0 - ninv  ) * x * Pn_Legendre( n - 1, x ) - 
    ( 1.0 - ninv  ) *     Pn_Legendre( n - 2, x );
}

double LegQuadrature::dPn_Legendre( const int& n, const double& x ){
  if( ( ! ( n > 1 ) ) || x == 1.0 ) return 0.0;
  
  double pn   = Pn_Legendre(     n, x );
  double pn_1 = Pn_Legendre( n - 1, x );
  return n * ( pn_1 - x * pn ) / ( 1.0 - x * x );
}

vector< double > LegQuadrature::X( const int& n ) {
  vector< double > xv( n, 0.0 );
  for( int i = 0; i < xv.size(); i++ ) xv[ i ] = LegQuadrature::calcX( i, xv.size() );
  return xv;
}

vector< double > LegQuadrature::Weight( const vector< double >& x ){
  vector< double > w( x.size(), 0.0 );
  int n = x.size();
  for( int i = 0; i < n; i++ ){
    w[ i ] = 2.0 * ( 1.0 - x[ i ] * x[ i ] ) ;
    w[ i ] /= pow( n * Pn_Legendre( n - 1, x[ i ] ), 2 );
  }
  return w;
}

LegQuadrature::LegQuadrature( const int& n ) :
  n_( n ), m_( n ), xk_(), yk_(), wxk_(), wyk_(), wk2_(),
  loginteg_( false ) {
  this->get_abscissa_weight();
}

LegQuadrature::LegQuadrature( const int& n, const int& m ) :
  n_( n ), m_( m ), xk_(), yk_(), wxk_(), wyk_(), wk2_(),
  loginteg_( false ) {
  this->get_abscissa_weight();
}

LegQuadrature::~LegQuadrature(){
}

void LegQuadrature::get_abscissa_weight() {
  this->setX();
  this->setWeight();
}

double LegQuadrature::setX(){
  xk_ = LegQuadrature::X( n_ );
  yk_ = LegQuadrature::X( m_ );
  return 0.0;
}

void LegQuadrature::setWeight() {
  
  wxk_ = LegQuadrature::Weight( xk_ );
  wyk_ = LegQuadrature::Weight( yk_ );
  
  wk2_.resize( wxk_.size() );
  
  for( int i = 0; i < wxk_.size(); i++ ) {
    wk2_[ i ].resize( wyk_.size(), 0.0 );
    for( int j = 0; j < wyk_.size(); j++ ) wk2_[ i ][ j ] = wxk_[ i ] * wyk_[ j ];
  }
}

double LegQuadrature::integral( const double& min, const double& max, 
				RealFunction& f  ){
  
  if( max == min ) return 0.0;
  
  if( loginteg_ ) return this->logintegral( min, max, f );
  
  double c2  = 0.5 * ( max - min );
  double hwx = fabs( c2 );
  double cx  = 0.5 * ( max + min );
  double s   = 0.0;
  for( int i=0; i < n_; i++ )
    s += wxk_[ i ] * f( this->convertX( xk_[ i ], cx, hwx ) ) ;
  
  return c2 * s;
}

complex< double > 
LegQuadrature::cintegral( const double& min, const double& max, 
			  ComplexFunction& f ) {
  
  if( max == min ) return complex< double >( 0.0, 0.0 );
  
  double c2  = 0.5 * ( max - min );
  double hwx = fabs( c2 );
  double cx  = 0.5 * ( max + min );
  
  complex< double > s( 0.0, 0.0 );
  
  for( int i=0; i < n_; i++ )
    s += wxk_[ i ] * f( this->convertX( xk_[ i ], cx, hwx ) ) ;
  
  return c2 * s;
}

double LegQuadrature::integral( const double& xmin, const double& xmax, 
				const double& zmin, const double& zmax, 
				RealFunction& f  ){
  
  if( xmax == xmin || zmax == zmin ) return 0.0;
  
  double dx = xmax - xmin;
  double dz = zmax - zmin;
  
  double hwx = 0.5 * fabs( dx );
  double cx  = 0.5 * ( xmax + xmin );
  
  double hwz = 0.5 * fabs( dz );
  double cz  = 0.5 * ( zmax + zmin );
  
  double c2 = 0.25 * dx * dz;
  
  double s  = 0.0;
  for( int i = 0; i < n_; i++ )
    for( int j = 0; j < m_; j++ )
      s += wk2_[ i ][ j ] * f( this->convertX( xk_[ i ], cx, hwx ),
			      this->convertX( yk_[ j ], cz, hwz ) );
  return c2 * s;
}

complex< double > 
LegQuadrature::cintegral( const double& xmin, const double& xmax, 
			  const double& zmin, const double& zmax, 
			  ComplexFunction& f ) {
  
  if( xmax == xmin || zmin == zmax ) return complex< double >( 0.0, 0.0 );
  
  double dx = xmax - xmin;
  double dz = zmax - zmin;
  
  double hwx = 0.5 * fabs( dx );
  double cx  = 0.5 * ( xmax + xmin );
  
  double hwz = 0.5 * fabs( dz );
  double cz  = 0.5 * ( zmax + zmin );
  
  double c2 = 0.25 * dx * dz;
  complex< double > s( 0.0, 0.0 );
  
  for( int i=0; i < n_; i++ )
    for( int j=0; j < m_; j++ )
      s += wk2_[ i ][ j ] * f( this->convertX( xk_[ i ], cx, hwx ),
			       this->convertX( yk_[ j ], cz, hwz ) ) ;
  
  return c2 * s;
}

void LegQuadrature::setngrid( const int& n ){
  n_ = n;
  this->get_abscissa_weight();
}

void LegQuadrature::setngrid( const int& n, const int& m ){
  n_ = n;
  m_ = m;
  this->get_abscissa_weight();
}

double LegQuadrature::logintegral( const double& min, 
				   const double& max, RealFunction& f  ){
  if( max == min ) return 0.0;
  
  double lmax = log( max );
  double lmin = log( min );
  
  double c2 = 0.5 * ( lmax - lmin );
  
  double s  = 0.0;
  for( int i=0; i < n_; i++ ){
    double expy = exp( this->convertX( xk_[ i ], lmin, lmax ) );
    s += c2 * wxk_[ i ] * expy * f( expy ) ;
  }
  return s;
}
