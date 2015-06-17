// $Id: Li2Int.hh,v 1.6 2008/12/02 08:13:16 miyachi Exp $
/*!
  \file    Li2Int.hh
  \version $Revision: 1.6 $
  \date    $Date: 2008/12/02 08:13:16 $
  \brief   Definition of Integral function on Li2 ( case 1 )
 */
#ifndef _Li2Int_hh_
#define _Li2Int_hh_

#include "IntegFunc.hh"
#include "ComplexFunction.hh"
#include "Li2.hh"
#include "CKey.hh"

#include <Utility/LookupTable.hh>

namespace Transform {
  
  /*!
    \class Li2Int Li2Int.hh "Tranform/Li2Int.hh"
    \brief Integral function over dilogarithm function \f$ Li_{2}(x) \f$.
    
    This class implements a complex integral function,
    \f[
    f( z ) = \int_0^1 {\rm d}x x^{n-1} { Li_2(x) \over 1 + x  }
    \f]
  for dilogarthm function, class Li2 is used.
  */
  class Li2Int : public IntegFunc, public ComplexFunction {
  public:
    
    //! An integrad for Li2Int class
    class Integrand : public ComplexFunction {
    public:
      virtual ~Integrand(){} //!< a destructor
      //! a function operator
      std::complex< double > operator()( const std::complex< double >& z );

      //! set value n in power of x
      ComplexFunction& n( const std::complex< double >& n ) 
      { n_ = n; return *this;}
      
    private:
      Li2 li2_;                //!< dilogarithm function object
      std::complex< double > n_;    //!< a complex value n in power of x
    };
  
    Li2Int();                  //!< a default constructor
    virtual ~Li2Int(){}        //!< a destructor
    //! a function operator
    virtual std::complex< double > 
    operator()( const std::complex< double >& n );
    
  private:
    Integrand    fnc_;         //!< integrand
    
    // These are members related chache (LookupTable)
    static CKey key;                   //!< key for cache
    static std::complex< double > data;     //!< data for cache
    static Utility::LookupTable< CKey, std::complex< double > > cache; //!< cache buffer
    static bool cacheReady;            //!< flag if cache is ready or not
    static void initCache();           //!< initialize cache
    static void closeCache();          //!< close cache DB file
    
  };
};
#endif //  _Li2Int_hh_
