// $Id: CPolyLog.cc,v 1.10 2008/12/02 08:13:15 miyachi Exp $
/*!
  \file    CPolyLog.cc
  \version $Revision: 1.10 $
  \date    $Date: 2008/12/02 08:13:15 $
  \author  $Author: miyachi $
  \brief   Class implementation of complex zeta function.
 */

#include "CPolyLog.hh"

#include <cmath>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <complex>

using namespace std;
using namespace Transform;

CPolyLog::CPolyLog( ) : n_( 1.0, 0.0 ), max_( 100 )
{
}

complex< double > CPolyLog::operator()( const complex< double >& z ) {
  return this->val( z, max_ );
}

std::complex< double > CPolyLog::val( const std::complex< double >& z,
				      const int& k ){
  if( k == 0 ) return complex< double >( 0.0, 0.0 );

  return pow( z, k ) / pow( double( k ), n_ )  + this->val( z, k - 1 );
}

