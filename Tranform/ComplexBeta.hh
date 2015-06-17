#ifndef _Transform_ComplexBeta_hh_
#define _Transform_ComplexBeta_hh_

#include "ComplexFunction.hh"
#include "IntegFunc.hh"

namespace Transform {
  
  /*!
   */
  class ComplexBeta : public ComplexFunction, public IntegFunc  {
  public:
    
    ComplexBeta( const double& alpha, const double& beta );
    ComplexBeta( const ComplexBeta& cb );
    ~ComplexBeta();
    
    ComplexBeta& operator=( const ComplexBeta& cb );

    void set( const double& a, const double& b );

    virtual std::complex< double > 
    operator()( const std::complex< double >& n );

    class Integrand : public ComplexFunction {
    public:
      
      virtual std::complex< double > 
      operator()( const std::complex< double >& z ) { 
	return std::complex< double >( 0.0, 0.0 );
      }
      
      virtual std::complex< double > operator()( const double& x );
      std::complex< double > n;
      std::complex< double > m;
    };
    
  private:
    Integrand integrand_;
    double a_;

  };
  
};

#endif // _Transform_ComplexBeta_hh_
