#ifndef _IntegPolVBoson_DYLogPlusFunc_hh_
#define _IntegPolVBoson_DYLogPlusFunc_hh_

#include <Tranform/RealFunction.hh>
#include <cmath>
#include <complex>
#include <IntegXsec/ConvIntegPDF.hh>

namespace IntegPolVBoson{
  
  class DYLogPlusFunc{
  public:
    DYLogPlusFunc( IntegXsec::ConvIntegPDF* xpdf, 
		   const std::vector< std::complex< double > >& param ): 
      g_( xpdf ), y_( param ){};
    ~DYLogPlusFunc(){};
    
    // assume integ range y_[0] < x < 1.0 
    virtual double operator()( const double& x ){
      return ( (*g_)( x ) - (*g_)( 1.0 ) / x ) 
	* log( 1.0 - x ) / ( 1.0 - x );
    } 

    // assume integ range y_[0] < x < 1.0 
    virtual std::complex< double > operator()
      ( const std::complex< double >& x ){
      return ( (*g_)( x ) - (*g_)( std::complex< double >( 1.0, 0.0 ) ) / x )
	* log( 1.0 - x ) / ( 1.0 - x );
    } 
    
  private:
    IntegXsec::ConvIntegPDF* g_;
    const std::vector< std::complex< double > >& y_;

  public:
    IntegXsec::ConvIntegPDF* getgfunc(){ return g_; } 
    std::vector< std::complex< double > > getparam(){ return y_; } 

  };
};

#endif // _IntegPolVBoson_DYLogPlusFunc_hh_
