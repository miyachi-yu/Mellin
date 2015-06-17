#ifndef _IntegUnpolDrellYan_DYPlusFunc_hh_
#define _IntegUnpolDrellYan_DYPlusFunc_hh_

#include <Tranform/RealFunction.hh>
#include <cmath>
#include <complex>
#include <IntegXsec/ConvIntegPDF.hh>

namespace IntegUnpolDrellYan{

  class DYPlusFunc{
  public:
    DYPlusFunc( IntegXsec::ConvIntegPDF* xpdf, 
		const std::vector< std::complex< double > >& param ): 
      g_( xpdf ), y_( param ){};
    ~DYPlusFunc(){};
    
    // assume integ range y_[0] < x < 1.0 
    virtual double operator()( const double& x ){
      return ( (*g_)( x ) - (*g_)( 1.0 ) / x ) / ( 1.0 - x ) ;
    } 

    virtual double operator()( Transform::RealFunction* f, 
			       const double& x, const double& z ){
      return ( (*f)( x, z ) * (*g_)( z ) - (*f)( x, 1.0 ) * (*g_)( 1.0 ) / z )
	/ ( 1.0 - z );
    } 

    // assume integ range y_[0] < x < 1.0 
    virtual std::complex< double > operator()
      ( const std::complex< double >& x ){
      return ( (*g_)( x ) - (*g_)( std::complex< double >( 1.0, 0.0 ) ) / x ) 
	/ ( 1.0 - x ) ;
    } 

    virtual std::complex< double > operator()
      ( Transform::RealFunction* f, 
	const std::complex< double >& x, 
	const std::complex< double >& z )
    {
      return ( (*f)( x.real(), z.real() ) * (*g_)( z ) 
	       - (*f)( x.real(), 1.0 ) 
	       * (*g_)( std::complex< double >( 1.0, 0.0 ) ) / z )
	/ ( 1.0 - z );
    } 
    
  private:
    IntegXsec::ConvIntegPDF* g_;
    const std::vector< std::complex< double > >& y_;

  public:
    IntegXsec::ConvIntegPDF* getgfunc(){ return g_; } 
    std::vector< std::complex< double > > getparam(){ return y_; } 

  };
};

#endif // _IntegUnpolDrellYan_DYPlusFunc_hh_
