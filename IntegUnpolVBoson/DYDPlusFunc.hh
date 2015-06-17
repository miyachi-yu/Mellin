#ifndef _IntegUnpolVBoson_DYDPlusFunc_hh_
#define _IntegUnpolVBoson_DYDPlusFunc_hh_

#include <Tranform/RealFunction.hh>
#include <cmath>
#include <IntegXsec/ConvIntegPDF.hh>

namespace IntegUnpolVBoson{

  class DYDPlusFunc{
  public:
    DYDPlusFunc( std::vector< IntegXsec::ConvIntegPDF* > xpdf, 
		 const std::vector< std::vector< std::complex< double > > >&  
		 param ): 
      g_( xpdf ), y_( param ){};
    ~DYDPlusFunc(){};
    
    virtual double operator()( Transform::RealFunction* f, 
			       const double& x, const double& z ){
      return ( 
	      (*f)( x, z ) * (*g_[ 0 ])( x ) * (*g_[ 1 ])( z )
	      - (*f)( 1.0, z ) * (*g_[ 0 ])( 1.0 ) / x * (*g_[ 1 ])( z )
	      - (*f)( x, 1.0 ) * (*g_[ 0 ])( x ) * (*g_[ 1 ])( 1.0 ) / z
	      + (*f)( 1.0, 1.0 ) 
	      * (*g_[ 0 ])( 1.0 ) / x * (*g_[ 1 ])( 1.0 ) / z
	      ) / ( 1.0 - x ) / ( 1.0 - z );
    } 

    virtual std::complex< double > operator()
      ( Transform::RealFunction* f, const std::complex< double >& x, 
	const std::complex< double >& z ){
      return ( 
	      (*f)( x.real(), z.real() ) * (*g_[ 0 ])( x ) * (*g_[ 1 ])( z )
	      - (*f)( 1.0, z.real() ) 
	      * (*g_[ 0 ])( std::complex< double >( 1.0, 0.0 ) ) / x 
	      * (*g_[ 1 ])( z )
	      - (*f)( x.real(), 1.0 ) 
	      * (*g_[ 0 ])( x ) 
	      * (*g_[ 1 ])( std::complex< double >( 1.0, 0.0 ) ) / z
	      + (*f)( 1.0, 1.0 ) 
	      * (*g_[ 0 ])( std::complex< double >( 1.0, 0.0 ) ) / x 
	      * (*g_[ 1 ])( std::complex< double >( 1.0, 0.0 ) ) / z
	      ) / ( 1.0 - x ) / ( 1.0 - z );
    } 
    
  private:
    std::vector< IntegXsec::ConvIntegPDF* > g_;
    const std::vector< std::vector< std::complex< double > > >& y_;

  public:
    std::vector< IntegXsec::ConvIntegPDF* > getgfunc(){ return g_; } 
    std::vector< std::vector< std::complex< double > > > getparam()
    { return y_; } 

  };
};

#endif // _IntegUnpolVBoson_DYDPlusFunc_hh_
