/*!
  \file  F2Param.cc
  \brief Implementation of F2 Parameterization class
*/
#include "F2Param.hh"

using namespace std;
using namespace StrFncParam;

F2Param::F2Param( PhotoAbsorbXsec& xsec, RParam& r ) :
  F2ParamBase(),
  sigmaT_( xsec ), r_( r )
{
}

F2Param::~F2Param(){
}

double F2Param::eval( const double& x ){
  
  double xtot = sigmaT_( sigmaT_.W2( x ), q2_ ) * ( 1.0 + r_( x, q2_ ) );
  return q2_ * q2_ * ( 1.0 - x ) /
    ( q2_ + 4.0 * PhotoAbsorbXsec::Mp2 * x * x ) / 
    PhotoAbsorbXsec::FineConstant * xtot ;
}
