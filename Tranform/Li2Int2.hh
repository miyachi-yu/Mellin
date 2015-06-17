// $Id: Li2Int2.hh,v 1.6 2008/12/02 08:13:16 miyachi Exp $
/*!
  \file    Li2Int2.hh
  \version $Revision: 1.6 $
  \date    $Date: 2008/12/02 08:13:16 $
  \brief   Definition of Integral function on Li2 ( case 2 )
 */
#ifndef _Li2Int2_hh_
#define _Li2Int2_hh_

#include "IntegFunc.hh"
#include "ComplexFunction.hh"
#include "Li2.hh"
#include "CKey.hh"

#include <Utility/LookupTable.hh>

namespace Transform {
  
  /*!
    \class Li2Int2  Li2Int2.hh  "Tranform/Li2Int2.hh"
    \brief class for a complex integral function over Li2(x)
    
    This is class for a complex integral function which appears in
    \f$  \tilde{S}_{1,2}(n) \f$
    function as 
    \f[
    \int_0^1 dx (x^n - 1 )/(x - 1) Li_2(x).
    \f]

  */
  class Li2Int2 : public IntegFunc, public ComplexFunction {
  public:
    
    /*! 
      \brief Integrad complex function for Li2Int2
      
      This class privide a complex function
      
      ( x^n - 1 ) / ( x - 1 ) Li_{2}(x)
      
      where Li2 class is used for the dilogarithm function.
    */
    class Integrand : public ComplexFunction {
    public:
      virtual ~Integrand(){}                   //!< destructor
      //! a function operator
      std::complex< double > operator()( const std::complex< double >& z );
      //! set complex value n in a power of x
      ComplexFunction& n( const std::complex< double >& n ) { n_ = n; return *this;}
      
    private:
      Li2 li2_;                      //!< a dilogarithm function 
      std::complex< double > n_;          //!< complex variable in power of x
    };
    
    Li2Int2();                       //!< a default constructor
    virtual ~Li2Int2(){}             //!< a destructor
    //! function operator implementation
    virtual std::complex< double > operator()( const std::complex< double >& n );
    
  private:
    Integrand    fnc_;               //!< integrand function
    
    // these are members for LookupTable
    static CKey key;                 //!< key for cache
    static std::complex< double > data;   //!< data buffer for cache
    static Utility::LookupTable< CKey, std::complex< double > > cache; //!< cache buffer
    static bool cacheReady;          //!< ready flag for cache buffer
    static void initCache();         //!< initialize cache
    static void closeCache();        //!< close cache buffer
    
  };
};


#endif // _Li2Int2_hh_
