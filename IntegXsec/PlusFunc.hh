#ifndef _IntegXsec_PlusFunc_hh_
#define _IntegXsec_PlusFunc_hh_

#include <cmath>
#include "ConvIntegPDF.hh"
#include "DeltaFunc.hh"

namespace IntegXsec{

  class PlusFunc : public Transform::RealFunction,
		   public Transform::ComplexFunction {
  public:
    PlusFunc( ConvIntegPDF* xpdf, 
	      const std::vector< std::complex< double > >& param ):
      delta_( xpdf, param ), g_( xpdf ), y_( param ){};
    ~PlusFunc(){};
    
    // assume integ range y_[0] < x < 1.0 
    virtual double operator()( const double& x ){
      return ( (*g_)( x ) - (*g_)( 1.0 ) ) / ( 1.0 - x ) 
	+ log( 1.0 - y_[ 0 ].real() ) * delta_( x ); 
    }

    // assume integ range y_[0] < x < 1.0 
    virtual std::complex< double > operator()
      ( const std::complex< double >& x ){
      return ( (*g_)( x ) - (*g_)( std::complex< double >( 1.0, 0.0 ) ) ) / 
	( 1.0 - x ) + log( 1.0 - y_[ 0 ] ) * delta_( x ); 
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

#endif // _IntegXsec_PlusFunc_hh_
