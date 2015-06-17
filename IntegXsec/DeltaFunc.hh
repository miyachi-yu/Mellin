#ifndef _IntegXsec_DeltaFunc_hh_
#define _IntegXsec_DeltaFunc_hh_

#include <cmath>
#include "ConvIntegPDF.hh"

//#include <iostream>
//using namespace std;

namespace IntegXsec{
  
  class DeltaFunc : public Transform::RealFunction,
		    public Transform::ComplexFunction {
  public:
    DeltaFunc( ConvIntegPDF* xpdf, 
	       const std::vector< std::complex< double > >& param ):
    g_( xpdf ), y_( param ){};
    ~DeltaFunc(){};
    
    // assume integ range y_[0] < x < 1.0 and deltafunction = delta( 1.0 - x )
    virtual double operator()( const double& x ){
      //      cout << y_[ 0 ] << endl;
      return (*g_)( 1.0 ) / ( 1.0 - y_[ 0 ].real() );
    } 

    virtual std::complex< double > operator()
      ( const std::complex< double >& x ){
      //      cout << y_[ 0 ] << endl;
      return (*g_)( std::complex< double >( 1.0, 0.0 ) ) / ( 1.0 - y_[ 0 ] );
    }

  private:
    ConvIntegPDF* g_;
    const std::vector< std::complex< double > >& y_;

  public:
    ConvIntegPDF* getgfunc(){ return g_; } 
    std::vector< std::complex< double > > getparam(){ return y_; } 

  };
};

#endif // _IntegXsec_DeltaFunc_hh_
