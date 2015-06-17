// $Id: MCInt.cc,v 1.1 2009/07/29 03:23:14 miyachi Exp $
/*!
  \file     MCInt.cc
  \date     $Date: 2009/07/29 03:23:14 $
  \version  $Revision: 1.1 $
  \author   $Author: miyachi $
  \brief    Implementation of Trapezoid approximation integration class
 */
#include "MCInt.hh"

#include <iostream>
#include <cstdlib>
#include <cmath>
#include <algorithm>

using namespace std;
using namespace Transform;


MCInt::MCInt( const int& n ) :
  n_( n )
{
  // srandom( ); // set seed
}

MCInt::~MCInt(){
}

double MCInt::integral( const double& min, const double& max, 
			RealFunction& f ) {
  double rval( 0.0 );
  double dx = max - min;
  for( int i = 0; i < n_; i++ ) rval += f( min + dx * random() / RAND_MAX );
  return rval * dx / n_;
}

complex< double > MCInt::cintegral( const double& min, const double& max, 
				    ComplexFunction& f ) {
  complex< double > rval( 0.0, 0.0 );
  double dx = max - min;
  for( int i = 0; i < n_; i++ ) rval += f( min + dx * random() / RAND_MAX );
  return rval * ( dx / n_ );
}

double MCInt::integral( const double& xmin, const double& xmax, 
			const double& zmin, const double& zmax, 
			RealFunction& f ){
  double rval( 0.0 );
  double dx = xmax - xmin;
  double dz = zmax - zmin;
  for( int i = 0; i < n_; i++ ) 
    rval += f( xmin + dx * random() / RAND_MAX, 
	       zmin + dz * random() / RAND_MAX );
  return rval * dx * dz / n_;
}

complex< double > MCInt::cintegral( const double& xmin, const double& xmax, 
				    const double& zmin, const double& zmax, 
				    ComplexFunction& f ){
  complex< double > rval( 0.0, 0.0 );
  double dx = xmax - xmin;
  double dz = zmax - zmin;
  for( int i = 0; i < n_; i++ ) 
    rval += f( xmin + dx * random() / RAND_MAX, 
	       zmin + dz * random() / RAND_MAX );
  return rval * ( dx * dz / n_ );
}
    
