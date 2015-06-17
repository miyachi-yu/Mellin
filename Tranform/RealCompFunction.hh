#ifndef _Tranform_RealCompFunction_hh_
#define _Tranform_RealCompFunction_hh_

#include "RealFunction.hh"
#include "ComplexFunction.hh"

namespace Transform {

  class RealCompFunction: public RealFunction, public ComplexFunction{
  public:
    RealCompFunction():RealFunction(),ComplexFunction(){};
    virtual ~RealCompFunction(){};

    virtual double operator() ( const double& x ) = 0 ;
    virtual double operator() ( const double& x, const double& z )
    { return 0.0; }

    virtual std::complex< double > operator() 
      ( const std::complex< double >& z ) = 0 ;
    virtual std::complex< double > operator() 
      ( const std::complex< double >& n, const std::complex< double >& m  ) 
    { return std::complex< double >( 0.0, 0.0 ); };

  };

};

#endif //  _Tranform_RealCompFunction_hh_
