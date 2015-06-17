// $Id: F1Param.cc,v 1.3 2008/05/20 00:44:19 miyachi Exp $
/*!
  \file  F1Param.hh
  \brief implementation of parameterization of F1 structure function class
 */
#include "F1Param.hh"

using namespace std;
using namespace StrFncParam;

F1Param::F1Param( PhotoAbsorbXsec& xsec ) : 
  sigmaT_( &xsec ), f2_( NULL ), r_( NULL )
{
}

F1Param::F1Param( F2ParamBase& f2, RParam& r ) : 
  sigmaT_( NULL ), f2_( &f2 ), r_( &r )
{
}

F1Param::~F1Param() {
}

double F1Param::operator()( const double& x ) {
  double val = 0.0;
  if( sigmaT_ ){
    double w2 = sigmaT_->W2( x );
    val = 0.5 * (*sigmaT_)( w2, q2_ ) * q2_ * ( 1.0 - x ) / 
      PhotoAbsorbXsec::FineConstant / x;
  } else {
    val = (*f2_)( x, q2_ );
    val /= 2.0 * x * ( 1.0 + (*r_)( x, q2_ ) );
  }
  return val;
}

double F1Param::operator()( const double& x, const double& q2 ) {
  q2_ = q2;
  return (*this)( x );
}
