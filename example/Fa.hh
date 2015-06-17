#ifndef _Fa_hh_
#define _Fa_hh_

#include "Tranform/RealFunction.hh"

/*!
  \class Fa
  \brief f(x) = x^alpha * ( 1 - x )^beta

  This is a test RealFunction chaild class.  The function
  f(x) = x^alpha * ( 1 - x )^beta
  is Mellin transformed to Beta( beta + 1, alpha + z ) function,
  which is implemented as CBeta.

 */
class Fa : public Transform::RealFunction {
public:
  Fa( double a, double b );
  virtual ~Fa();
  virtual double operator() ( const double& x );
  
private:
  double a_;
  double b_;
};

#endif //  _Fa_hh_
