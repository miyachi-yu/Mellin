//$Id: SteepestDescent.hh,v 1.2 2008/07/17 05:06:29 miyachi Exp $
/*!
  \file   SteepestDescent.hh
  \brief  class definition for the steepest descent contour
 */
#ifndef __Transform_SteepestDescent_hh__
#define __Transform_SteepestDescent_hh__

#include "IntegPath.hh"

namespace Transform {
  
  /*!
    \brief  Mellin Inversion Integration path ( steest descent )
    
    This is a class for the steepest descent contour in Mellin 
    inversion discussed in  
    Nuclear Physics B 520 (1998) 263-278.
    This contour is parameterized as
    \f[
    z( t ) = c_{0} + c_{1} t^2 + \mathi t.
    \f]
    The differential is given
    \f[
    dz( t ) = 2  c_{1} t^2 + \mathi .
    \f]
    
    \par parameters in this contour
    Starting point must be determined numerically, 
    finding a minimum point of \f$ x^{-z} f(z) \f$ 
    along the real axis.

  */
  class SteepestDescent : public IntegPath {
  public:
    
    SteepestDescent();             //!< a default constructor
    virtual ~SteepestDescent();    //!< a destructor

    //! get a complex point on the path
    virtual std::complex< double > operator()( const double& t );
    
    //! get a differential on the path
    virtual std::complex< double > differential( const double& t );
    
    //! set parameters
    void set( const double& c0, const double& c1 ){
      c0_ = c0; c1_ = c1;
    }
    
  private:
    double c0_;  //!< a starting port
    double c1_;  //!< a coefficient of t^2
    
  };
    
}

#endif // __Transform_SteepestDescent_hh__
