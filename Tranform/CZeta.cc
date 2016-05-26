// $Id: CZeta.cc,v 1.10 2008/12/02 08:13:15 miyachi Exp $
/*!
  \file    CZeta.cc
  \version $Revision: 1.10 $
  \date    $Date: 2008/12/02 08:13:15 $
  \author  $Author: miyachi $
  \brief   Class implementation of complex zeta function.
 */

#include "CZeta.hh"
#include "Binomial.hh"

#include <cmath>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <complex>

using namespace std;
using namespace Transform;

CZeta::CZeta( ) : mode_( havil2003 )
{
}

complex< double > CZeta::operator()( const complex< double >& z ) {
  
  complex< double > coef = 1.0 / ( 1.0 - pow( 2.0, 1.0 - z ) );
  complex< double > v = this->val( z, 100 );
  v *= coef; 
  return v;
}

std::complex< double > CZeta::val( const std::complex< double >& z,
				   const int& n ){
  
  complex< double > v( 0.0, 0.0 );
  if( mode_ == havil2003 ){
    if( n < 0 ) return v;
    for( int k = 0; k < n + 1; k++ ) v += this->val( z, n, k );
    v /= pow( 2, n + 1 );
  } else {
    if( n == 0 ) return v;
    v = ( ( n - 1 ) % 2 == 0 ? 1.0 : -1.0 ) / pow( double( n ), z  );
  }
  
  return v + this->val( z, n - 1 );
}

std::complex< double > CZeta::val( const std::complex< double >& z,
				   const int& n, const int& k ){
  return ( k % 2 == 0 ? 1.0 : -1.0 ) * Binomial( n, k ) * pow( double( k + 1 ), -z );
}

