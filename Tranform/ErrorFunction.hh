// $Id: ErrorFunction.hh,v 1.1 2008/07/09 06:08:49 miyachi Exp $
#ifndef _ErrorFunction_hh_
#define _ErrorFunction_hh_

#include "IntegFunc.hh"

namespace Transform {
  
  /*!
    \brief  Error Function class
    
    see http://mathworld.wolfram.com/Erf.html 
   */
  class ErrorFunction : public RealFunction, public IntegFunc {
  public:
    ErrorFunction();
    virtual ~ErrorFunction() {}
    
    virtual double operator() ( const double& z );
    
    class Integrand : public RealFunction {
    public:
      Integrand(){}
      virtual ~Integrand(){}
      virtual double operator()( const double& x );
    };
    
  private:
    Integrand integrand_;
  };
  
}
#endif // _ErrorFunction_hh_
