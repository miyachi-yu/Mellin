// $Id: CGamma.hh,v 1.8 2008/12/02 08:13:16 miyachi Exp $
/*!
  \file    CGamma.hh
  \version $Revision: 1.8 $
  \author  $Author: miyachi $
  \brief   Complex Gamma Function Class (Definition)
  
  This is C++ implementation of Gamma function. The code was generated
  absed on the 
  
  Algorithms and coefficient values from "Computation of Special
  Functions", Zhang and Jin, John Wiley and Sons, 1996.
  
  The algorithm was largely modified, keeping the basic structure.
  
*/
#ifndef _CGamma_hh_
#define _CGamma_hh_

#include "ComplexFunction.hh"
#include "CLogGamma.hh"
#include "IntegFunc.hh"
#include "CKey.hh"
#include "DataCache.hh"

#include <Utility/LookupTable.hh>

namespace Transform {
  
  /*!
    \class   CGamma    CGamma.hh    "Tranform/CGamma.hh"
    \brief   This is a class to calculate complex Gamma function.
    
    The basic routine is contained in CLogGamma class. This class
    just use CLogGamma object to calculate log( Gamma(z) ), then
    convert to Gamma function using exp() function implemented in
    complex package.

    \par functional equation
    Gamma function satisfies the following functional equations.
    \f[
    \Gamma( 1 + z ) = z \Gamma( z )
    \f]
    \f[
    \Gamma( 1 - z ) = -z \Gamma( -z )
    \f]
    This can be used to calculate Gamma function outside allowed
    region from the inside.

    
  */
  class CGamma : public ComplexFunction, public IntegFunc {
  public:
    
    CGamma();                  //!< a default constructor
    virtual ~CGamma();         //!< a destructor
    
    /*!
      \param[in] z is a complex variable 
      \return      a value of complex Gamma function at the given z
      \brief       calculate Gamma( z )
    */
    virtual std::complex< double > 
    operator() ( const std::complex< double >& z );
    
    //! get a reference to CLogGamma function
    CLogGamma& logGamma() { return lg; }
    
    /*!
      \brief Integrand for complex gamma function calculation.
      
      There are several integration expressions. Here we will try to use
      \f[
      \int_0^1 \left( \ln( 1 \over t ) \right)^{z - 1} {\rm d}t
      \f]
    */
    class Integrand : public ComplexFunction {
    public:
      Integrand() : z_( 0.0, 0.0 ) {} //!<< a default constructor
      virtual ~Integrand() {}  //!< a destructor
      virtual std::complex< double > operator() ( const std::complex< double >& z ) {
	return std::complex< double >( 0.0, 0.0 );
      }
      virtual std::complex< double > operator() ( const double& t );
      ComplexFunction& set( const std::complex< double >& z ) {
	z_ = z; return *this;
      }
    private:
      std::complex< double > z_;
    };
    
  private:
    CLogGamma lg;          //!< complex log gamma function
    Integrand integrand_;  //!< integrand
    
    static Utility::LookupTable< CKey, std::complex< double > >* cache; 
    static CKey key;                        //!< key  for cache
    static std::complex< double > data;     //!< data for cache
    static bool cacheReady;                 //!< flag for cache buffer
    static void initCache();                //!< initialize cache buffer
    static void closeCache();               //!< close cache buffer
    
    static DataCache dcache_;
    static std::vector< std::complex< double > > cdata_;
    
  };
};
#endif // _CGamma_hh_
