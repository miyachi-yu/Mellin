#ifndef _HyperGeom_hh_
#define _HyperGeom_hh_

#include "ComplexFunction.hh"
#include "Li2.hh"
#include "DataCache.hh"
#include "IntegFunc.hh"

#include <Utility/LookupTable.hh>

namespace Transform {
  
  /*!
    \class HyperGeom HyperGeom.hh "Tranform/HyperGeom.hh"
    \brief class for HyperGeometric function implementation.
    
    Hyper Geometeric Function is given in the integration form as
    \f[
    _{1}F_{2}( a, b; c; z )
    =
    { \Gamma( c ) \over \Gamma( b) \Gamma( c - b ) }
    \int_0^1 {\rm d}t
    { t^{b-1} ( 1 - t )^{c-b-1} \over ( 1 - tz )^a }
    \f]
    
    \par
    However, HyperGeom class is implemented for the special
    combination of
    \f[
    _{2}F_{1}( 1, n+1; n+2; z ) + _{2}F_{1}( 1, n+1; n+2; -z )
    =
    2 ( n+ 1 ) \int_{-\infty}^{0} {\rm d}u 
    { {\rm e}^{ ( \Re n + 1 ) u } \over 1 - {\rm e}^{2u} z^2 }
    {\rm e}^{ i( \Im n ) u}
    \f]
    
  */
  class HyperGeom : public IntegFunc, public ComplexFunction {
  public:
    
    /*!
      \brief integrand class for HyperGeom
    */
    class Integrand : public ComplexFunction {
    public:
      virtual ~Integrand(){}                    //!< a destructor
      //! complex function implementation
      std::complex< double > operator()( const std::complex< double >& n );
      //! complex function implementation
      std::complex< double > operator()( const double& x );
      //! set parameters
      ComplexFunction& set( const std::complex< double >& n, const double& z ) {
	n_ = n; z_ = z; return *this;
      }
    private:
      std::complex< double > n_;
      double            z_;
    };
    
    HyperGeom();                 //!< a default constructor
    virtual ~HyperGeom(){}       //!< a destructor
    //! complex function implementation
    std::complex< double > operator()( const std::complex< double >& n );
    //! complex function implementation
    std::complex< double > operator()( const double& z );
    //! set parameter
    ComplexFunction& n( const std::complex< double >& n ) {
      n_ = n; return *this;
    }
    
  private:
    Integrand    func_;         //!< integrand function
    std::complex< double > n_;       //!< parameter n

    // for file base cache 
    static DataCache::Grid key;    //!< key for cache
    static std::complex< double > data; //!< buffer for cache
    //! file base cache
    static Utility::LookupTable< DataCache::Grid, std::complex< double > > cache;

    static bool cacheReady;
    static void initCache();
    static void closeCache();
    
  };
};
#endif // _HyperGeom_hh_
