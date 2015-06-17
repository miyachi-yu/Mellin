#ifndef _IntegXsec_ConvIntegrand_hh_
#define _IntegXsec_ConvIntegrand_hh_

#include <Tranform/RealFunction.hh>
#include <Tranform/ComplexFunction.hh>
#include "ConvIntegPDF.hh"
#include "DeltaFunc.hh"
#include "PlusFunc.hh"
#include "LogPlusFunc.hh"

namespace IntegXsec{

  class IntegXsecComp;
  
  class ConvIntegrand: public Transform::RealFunction, 
		       public Transform::ComplexFunction {
  public:
    ConvIntegrand( IntegXsecComp* xc );
    ~ConvIntegrand();
    
    //    /*
    virtual double operator()( const double& x ) = 0;
    virtual double operator()( const double& x, const double& z ) = 0;

    virtual std::complex< double > operator()
      ( const std::complex< double >& n ) = 0;
    virtual std::complex< double > operator()
      ( const std::complex< double >& n, const std::complex< double >& m ) = 0;
    //    */
    
    /*
    virtual double operator()( const double& x );
    virtual double operator()( const double& x, const double& z );

    virtual std::complex< double > operator()
      ( const std::complex< double >& n );
    virtual std::complex< double > operator()
      ( const std::complex< double >& n, const std::complex< double >& m );
    */

    virtual double rescale( const int& i = 0 ) throw( int );

    void setConvIntfunc( std::vector< ConvIntegPDF* > g );
    
    double dim(){ return dim_; }
    
    ConvIntegrand& setxPDF( std::vector< Transform::RealFunction* > vf );
    ConvIntegrand& setxPDF
    ( Transform::RealFunction* f, Transform::RealFunction* g = NULL );

    ConvIntegrand& setcPDF( std::vector< Transform::ComplexFunction* > vf );
    ConvIntegrand& setcPDF
    ( Transform::ComplexFunction* f, Transform::ComplexFunction* g = NULL );

    bool xpdfcheck();
    bool cpdfcheck();
    
  protected:
    std::vector< ConvIntegPDF* > g_;
    std::vector< DeltaFunc* > delta_;
    std::vector< PlusFunc* > plusf_;
    std::vector< LogPlusFunc* > logplusf_;
    
    IntegXsecComp* xc_;
    int dim_;

  public:
    std::vector< ConvIntegPDF* > getvConvIntegPDF(){ return g_; }
    std::vector< DeltaFunc* > getvDeltaFunc(){ return delta_; }
    std::vector< PlusFunc* > getvPlusFunc(){ return plusf_; }
    std::vector< LogPlusFunc* > getvLogPlusFunc(){ return logplusf_; }

  };
};

#endif //  _IntegXsec_ConvIntegrand_hh_
