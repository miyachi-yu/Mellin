/*!
  \brief class definition of Laguerre Quadrature Integration

  This is a class definition file for Laguerre Quadrature Integration.

  $Id: LaguerreQuadrature.hh,v 1.1 2008/07/29 08:59:10 miyachi Exp $

 */
#ifndef _Transform_LaguerreQuadrature_hh_
#define _Transform_LaguerreQuadrature_hh_

#include "Integration.hh"
#include "Laguerre.hh"

namespace Transform {
  
  /*!
    \brief  Generalized Laguerre quadrature integration

    This is a class for the generalized Laguerre quadrature integration.
    The Laguerre quadrature is a integral equation with a weight function:
    \f[
    \int_{0}^{\infty} {\rm d}x x^{\alpha} e^{-x} f( x ) \simeq 
    \sum_i^{n} w_i \cdot f( x_i ),
    \f]
    where \f$ x_i \f$ is an absissa of the generalized Laguerre
    function \f$ L_{n}^{\alpha}(x)\f$, and the weigth is obtained as
    \f[
    w_i = 
    { \Gamma( n + \alpha ) x_i
    \over
    n ! ( n + \alpha ) \left[ L_{n-1}^{\alpha}(x_i)\right]^2
    }
    \f]
    see http://mathworld.wolfram.com/Laguerre-GaussQuadrature.html
    
    \par Implementation
    It performs to calculate \f$ \sum_i^{n} w_i f( x_i )\f$, so 
    that the return value is a integral with the weight function
    \f$ w( x ) = x^{\alpha} e^{-x} \f$.

  */
  class LaguerreQuadrature : public Integration {
  public:
    
    /*!
      \param[in]  n is a number of nodes for integration
      \param[in]  alpha is a power in the weight function, \f$ \alpha \f$
      \brief      construct \f$ L_{n}^{\alpha}( x )\f$
    */
    LaguerreQuadrature( const int& n, const double& alpha = 0.0 );
    virtual ~LaguerreQuadrature();                  //!< a destructor
    
    /*!
      \brief perform Lagueree Quadratrue integration
      
      Alghtogh there are arguments, min and max, 
      the integration will be performed from 0 to \f$ \infty \f$,
      as a definiton of Laguerre integration.
      It may be worth to implement integration region, somehow....
      
    */
    virtual double integral( const double& min, const double& max, 
			     RealFunction& f );
    
    //! integrate the given function with Gass Legendre method
    virtual std::complex< double > 
    cintegral( const double& min, const double& max, 
	       ComplexFunction& f );
    
    //! integrate in 2D the given function with Gass Legendre method
    virtual double integral( const double& xmin, const double& xmax, 
			     const double& zmin, const double& zmax, 
			     RealFunction& f );
    
    //! integrate in 2D the given function with Gass Legendre method
    virtual std::complex< double > 
    cintegral( const double& xmin, const double& xmax, 
	       const double& zmin, const double& zmax, 
	       ComplexFunction& f );
    
  private:
    Laguerre lag_;
    
  };
  
}

#endif // _Transform_LaguerreQuadrature_hh_
