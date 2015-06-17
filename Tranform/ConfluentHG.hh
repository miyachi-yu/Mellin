// $Id: ConfluentHG.hh,v 1.4 2008/07/29 08:59:10 miyachi Exp $
/*!
  \file   ConfluentHG.hh
  \brief  Class definition of Confluent Hyper-Geometric function
 */
#ifndef _ConfluentHG_hh_
#define _ConfluentHG_hh_

#include "ComplexFunction.hh"
#include "IntegFunc.hh"
#include "CGamma.hh"

namespace Transform {
  
  /*!
    \brief  class for Confluent Hyper-Geometric function

    Confluent Hyper-Geometric function for the first kind is given
    in integration presentation as:
    \f[
    _{1}F_{1}(a;b;z) =
    {\Gamma(b) \over \Gamma(b-a) \Gamma(a)}
    \int_{0}^{1} e^{zt} t^{a-1} (1-t)^{b-a-1} {\rm d}t
    \f]
    
    \par Relation with generalized Laguerre polynominal
    Confluent HyperGeometric function has relation with Laguerre polynominal
    as:
    \f[
    L_{n}^{m}( x ) = 
    { (m + 1)! \over m! n! }~ {1}F_{1}( -n; m+1; x)
    \f]
    This relation is used in Laguerre-Gauss Quadurature intergration.
    
    see 
    http://mathworld.wolfram.com/ConfluentHypergeometricFunctionoftheFirstKind.html

  */
  class ConfluentHG : public ComplexFunction, public IntegFunc {
    
  public:
    //! constructor
    ConfluentHG( const std::complex< double >& a, 
		 const std::complex< double >& b );
    virtual ~ConfluentHG();  //!< a destructor
    
    /*!
      \param[in] a is a parameter \f$ a \f$
      \param[in] a is a parameter \f$ b \f$
      \brief set parameters \f$ a \f$ and \f$ b \f$ in \f$ {1}F_{1}(a;b;z) \f$
      
      This will set parameters \f$ a \f$ and \f$ b \f$ 
      in \f$ {1}F_{1}(a;b;z) \f$. The cmoplex Gauss functions and 
      the integrand will be modified.
      
    */
    virtual void set( const std::complex< double >& a, 
		      const std::complex< double >& b );
    
    /*!
      \param[in] z is a complex value \f$ z \f$
      \brief calculate \f$ {1}F_{1}(a;b;z) \f$
    */
    virtual std::complex< double > 
    operator() ( const std::complex< double >& z );
    
    /*!
      \brief complex integrand function

      This is integrand in Confluent HyperGeometric function. 
      The functional form is
      \f[
      e^{zt} t^{a-1} (1-t)^{b-a-1} {\rm d}t
      \f]
    */
    class Integrand : public ComplexFunction {
    public:
      
      Integrand( const std::complex< double >& a, 
		 const std::complex< double >& b );  //!< a constructor
      
      virtual ~Integrand();                           //!< a destructor
      
      //! calculate integrand value
      virtual std::complex< double >
      operator() ( const std::complex< double >& t ){ 
	return std::complex< double >( 0.0, 0.0 ); 
      }
      
      //! calculate integrand value
      virtual std::complex< double >operator() ( const double& t );

      //! set a and b
      virtual void set( const std::complex< double >& a, 
			const std::complex< double >& b ) {
	a_ = a; b_ = b;
      }
      
      //! set z
      virtual void set( const std::complex< double >& z ){ z_ = z; }
      
    private:
      std::complex< double > a_;
      std::complex< double > b_;
      std::complex< double > z_;
      
    };
    
  private:
    
    CGamma gamma_;             //!< \f$ \Gamma( z ) \f$
    
    //!< \f$ { \Gamma( b ) \over \Gamma( b - a ) \Gamma( a ) } \f$
    std::complex< double > c_; 
    Integrand integrand_;      //!< integrand
    std::complex< double > a_;
    std::complex< double > b_;
    
  };
  
}

#endif // _ConfluentHG_hh_
