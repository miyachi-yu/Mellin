/*!
  \file  F2ParamBase.cc
  \brief Implementation of F2 Parameterization class
*/
#include "F2ParamBase.hh"

using namespace std;
using namespace StrFncParam;

F2ParamBase::F2ParamBase() :
  q2_( 1.0 )
{
}

F2ParamBase::~F2ParamBase(){
}

double F2ParamBase::operator() ( const double& x, const double& Q2 ){
  this->q2( Q2 );
  return (*this)( x );
}

double F2ParamBase::operator() ( const double& x ){
  return this->eval( x );
}
