/*!
  \file class definition for Romberg integration
 */
#ifndef _Transform_Romberg_hh_
#define _Transform_Romberg_hh_

#include "Integration.hh"
#include "Trapezoid.hh"

namespace Transform {
  
  /*!
    \brief  Romberg integration class

    This is a class implementation for Romberg integration,
    whose details can be found in Numerical Receipt in C.
    
  */
  class Romberg : public Integration {
  public:
    
    /*!
      \param[in] K is a number of points to extrapolate integration
      \param[in] precision is a required integration relative precision
    */
    Romberg( const int& K = 6, const double& precision = 1.0E-6 );
    virtual ~Romberg();                                   //!< a destructor
    
    //! Integrate the given region with Trapezoid approxymeation
    virtual double integral( const double& min, const double& max, 
			     RealFunction& f ) ;
    
    //! Integrate the given region with Trapezoid approxymeation
    virtual std::complex< double > 
    cintegral( const double& min, const double& max, ComplexFunction& f );
    
    //! Integrate the given region with Trapezoid approxymeation
    virtual double integral( const double& xmin, const double& xmax, 
			     const double& zmin, const double& zmax, 
			     RealFunction& f ) {
      return 0.0;
    }
    
    //! Integrate the given region with Trapezoid approxymeation
    virtual std::complex< double > 
    cintegral( const double& xmin, const double& xmax, 
	       const double& zmin, const double& zmax, 
	       ComplexFunction& f ) {
      return std::complex< double >( 0.0, 0.0 );
    }
    
  private:
    Trapezoid trap_;  //!< Trapezoid integration
    int jmax_;        //!< maximum size of grid
    int K_;           //!< size of grid for extrapolation
    double eps_;      //!< required relative precision

  };
  
};

#endif // _Transform_Romberg_hh_
