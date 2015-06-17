#ifndef _IntegXsec_LogPlusFunc_hh_
#define _IntegXsec_LogPlusFunc_hh_

#include "DeltaFunc.hh"
#include <Tranform/RealFunction.hh>
#include <cmath>
#include <complex>

namespace IntegXsec{
  
  class LogPlusFunc : public Transform::RealFunction,
		      public Transform::ComplexFunction {
  public:
    LogPlusFunc( ConvIntegPDF* xpdf, 
		 const std::vector< std::complex< double > >& param ):
      delta_( xpdf, param ), g_( xpdf ), y_( param ){};
    ~LogPlusFunc(){};
    
    // assume integ range y_[0] < x < 1.0 
    virtual double operator()( const double& x ){
      return ( (*g_)( x ) - (*g_)( 1.0 ) ) 
	* log( 1.0 - x ) / ( 1.0 - x ) 
	+ 0.5 * pow( log( 1.0 - y_[ 0 ].real() ), 2 ) * delta_( x ); 
    } 

    virtual std::complex< double > operator()
      ( const std::complex< double >& x ){
      return ( (*g_)( x ) - (*g_)( std::complex< double >( 1.0, 0.0 ) ) )
	* log( 1.0 - x ) / ( 1.0 - x )
	+ 0.5 * pow( log( 1.0 - y_[ 0 ] ), 2 ) * delta_( x ); 
    } 
    
  private:
    ConvIntegPDF* g_;
    const std::vector< std::complex< double > >& y_;
    DeltaFunc delta_;

  public:
    ConvIntegPDF* getgfunc(){ return g_; } 
    std::vector< std::complex< double > > getparam(){ return y_; } 
    DeltaFunc* getdelta(){ return &delta_; }

  };
};

#endif // _IntegXsec_LogPlusFunc_hh_
