// $Id: Coefficient.cc,v 1.10 2009/04/24 09:33:34 imazu Exp $
/*!
  \file    Coefficient.hh
  \version $Revision: 1.10 $
  \author  $Author: imazu $
  \date    $Date: 2009/04/24 09:33:34 $
  \brief   class definition for Willson coefficient function
 */
#include "Coefficient.hh"
#include "XsecCompBase.hh"
#include <cmath>

using namespace std;
using namespace Transform;
using namespace Xsection;

GeneralHarmonic Xsection::Coefficient::s_;

Coefficient::Coefficient( ) : 
  xc_( NULL ),
  cache_(),
  cache2_(),
  data_( 1, complex< double >( 0.0, 0.0 ) )
{
}

Coefficient::~Coefficient( ) {
}

void Coefficient::set( Xsection::XsecCompBase* xc ){
  xc_  = xc;
}

double Coefficient::rescale( const int& i ){
  if( xc_ == NULL || i >= xc_->vevo().size() ) return 0.0;
  return log( xc_->q2() / xc_->vevo()[ i ]->Q2() );
}


complex< double > Coefficient::operator()( const complex< double >& n ){
  //  double Q2 = ( xc_ ? xc_->q2() : 1.0 );
  double Q2 = 1.0;
  if( cache_.cached( n, Q2 ) )  return cache_.cache( n, Q2 )[ 0 ];
  data_[ 0 ] = this->eval( n );
  cache_.cache( n, Q2, data_ );
  return data_[ 0 ] ;
}

complex< double > Coefficient::operator()( const complex< double >& n,
					   const complex< double >& m ){
  //  double Q2 = ( xc_ ? xc_->q2() : 1.0 );
  double Q2 = 1.0;
  if( cache2_.cached( n, m, Q2 ) ) return cache2_.cache( n, m, Q2 )[ 0 ];
  
  data_[ 0 ] = this->eval( n, m );
  cache2_.cache( n, m, Q2, data_ );
  return data_[ 0 ] ;
}

complex< double > Coefficient::eval( const complex< double >& n ) {
  return complex< double >( 0.0, 0.0 );
}

complex< double > Coefficient::eval( const complex< double >& n,
				     const complex< double >& m ) {
  return complex< double >( 0.0, 0.0 );
}

