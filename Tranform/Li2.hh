// $Id: Li2.hh,v 1.7 2008/12/02 08:13:16 miyachi Exp $
/*!
  \file    Li2.hh
  \version $Revision: 1.7 $
  \date    $Date: 2008/12/02 08:13:16 $
  \brief   Definition of Li2 special function class
 */
#ifndef _Li2_hh_
#define _Li2_hh_

#include "RealFunction.hh"
#include "ComplexFunction.hh"
#include "IntegFunc.hh"

#include <Utility/LookupTable.hh>

namespace Transform {
  
  /*!
    \class  Li2  Li2.hh "Tranform/Li2.hh"
    \brief  Class for dilogarithm function, Li2(x).
    
    This is an implementation of dilogarithm function, 
    Li_{2}( x ). Not as commonly used in the other analyses,
    the original definition
    \f[
    Li_{2}( x ) = \int_{0}^{x} { - ln( 1 - x ) \over x } {\rm d}x
    \f]
    in implemented, instead of using the approximation.

  */
  class Li2 : public IntegFunc, public RealFunction {
  public:
    
    /*!
      \brief  Integrand of Li2
    */
    class Integrand : public RealFunction {
    public:
      virtual ~Integrand(){}                   //!< a destructor
      double operator()( const double& x );   //!< - ln( 1 - x ) / x
    };
    
    Li2();                    //!< a defatult constructor
    virtual ~Li2(){}          //!< a destructor
    double operator()( const double& x ); //!< get Li2( x )
    
  private:
    Integrand    func_;       //!< integrand function
    
    // These members are for LookupTable cache
    static double key;        //!< key for cache
    static double data;       //!< data for cache
    static Utility::LookupTable< double, double > cache; //!< cache buffer
    static bool cacheReady;   //!< if cache is ready or not
    static void initCache();  //!< initialize cache
    static void closeCache(); //!< close cache file
    
  };
};

#endif //_Li2_hh_
