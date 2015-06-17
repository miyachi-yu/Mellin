// $Id: F2nSMC.cc,v 1.2 2008/05/20 02:25:33 miyachi Exp $
/*!
  \brief class implementation of F2n SMC parameterization
 */
#include "F2nSMC.hh"

using namespace std;
using namespace StrFncParam;

F2nSMC::F2nSMC() : 
  F2ParamBase(), 
  f2p_(), f2d_() {
}

F2nSMC::~F2nSMC() {
}

double F2nSMC::eval( const double& x ) {
  return 2.0 * f2d_( x ) - f2p_( x );
}

void F2nSMC::q2( const double& v ){
  q2_ = v;
  f2d_.q2( v );
  f2p_.q2( v );
}
