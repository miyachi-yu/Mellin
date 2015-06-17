// $Id: ConfluentHG2.hh,v 1.3 2008/08/01 09:55:09 miyachi Exp $
/*!
  \brief class definition of Confluent Hypergeometric function 2nd kind
 */
#ifndef _Transform_ConfluentHG2_hh_
#define _Transform_ConfluentHG2_hh_

#include "IntegFunc.hh"
#include "ComplexFunction.hh"
#include "CGamma.hh"

namespace Transform {

  /*!
    \brief Confluent Hypergeometric function 2nd kind

    Confluent hypergeometric function 2nd kind is given in
    http://mathworld.wolfram.com/ConfluentHypergeometricFunctionoftheSecondKind.html
    
    \par integral representation 
    \f[
    U( a, b, z ) =
    { 1 \over \Gamma( a ) }
    \int_0^{\inf} e^{-zt} t^{a-1} (1+t)^{b-a-1} {\rm d}t
    \f]
 
  */
  class ConfluentHG2 : public ComplexFunction, public IntegFunc {
  public:

    //! a constructor
    ConfluentHG2( const std::complex< double >& a,
		  const std::complex< double >& b );            

    virtual ~ConfluentHG2(); //!< a destructor
    
    virtual std::complex< double >
    operator()( const std::complex< double >& z );
    
    void set( const std::complex< double >& a, 
	      const std::complex< double >& b ); //!< set a and b

    class Integrand : public ComplexFunction {
    public:
      
      Integrand( const std::complex< double >& a,
		 const std::complex< double >& b ) :
	a_( a ), b_( b ), z_( 0.0, 0.0 ) {}
      
      virtual ~Integrand() {}
      
      virtual std::complex< double >
      operator()( const std::complex< double >& z ) {
	return std::complex< double >( 0.0, 0.0 );
      } 
      
      virtual std::complex< double > operator()( const double& t );
      
      //! set a and b
      void set( const std::complex< double >& a,
		const std::complex< double >& b ) { a_ = a; b_ = b; }
      
      Integrand& z( const std::complex< double >& z ) { z_ = z; return *this; }
      
    private:
      std::complex< double > a_;
      std::complex< double > b_;
      std::complex< double > z_;

    };
    
  private:
    std::complex< double > a_;
    std::complex< double > b_;
    std::complex< double > ga_;
    CGamma gamma_;
    Integrand integrand_;
    
  };
  
}
#endif // _Transform_ConfluentHG2_hh_
