// $Id: Rectangle.cc,v 1.3 2008/07/24 21:28:15 miyachi Exp $
/*!
  \file    Rectangle.cc
  \version $Revision: 1.3 $
  \date    $Date: 2008/07/24 21:28:15 $
  \author  $Author: miyachi $
  \brief   class implementation of Rectangle approximation
 */
#include "Rectangle.hh"

using namespace std;
using namespace Transform;

Rectangle::~Rectangle() {
}

double Rectangle::integral( const double& min, const double& max, 
			    RealFunction& f ) {
  return ( max - min ) * ( f( 0.5 * ( max + min ) ) );
}

complex< double > Rectangle::cintegral( const double& min, const double& max, 
					ComplexFunction& f ) {
  return ( max - min ) * ( f( 0.5 * ( max + min ) ) );
}

double Rectangle::integral( const double& xmin, const double& xmax, 
			    const double& zmin, const double& zmax, 
			    RealFunction& f ) {
  return ( xmax - xmin ) * ( zmax - zmin ) * 
    ( f( 0.5 * ( xmax + xmin ), 0.5 * ( zmax + zmin ) ) );
}

complex< double > Rectangle::cintegral( const double& xmin, const double& xmax, 
					const double& zmin, const double& zmax, 
					ComplexFunction& f ) {
  return ( xmax - xmin ) * ( zmax - zmin ) * 
    ( f( 0.5 * ( xmax + xmin ), 0.5 * ( zmax + zmin ) ) );
}
