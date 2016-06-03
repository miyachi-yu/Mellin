// $Id: CPolyLog.hh,v 1.7 2008/12/02 08:13:16 miyachi Exp $
/*!
  \file    CPolyLog.hh
  \version $Revision: 1.7 $
  \date    $Date: 2008/12/02 08:13:16 $
  \brief   Definition of CPolyLog special function class
 */
#ifndef _CPolyLog_hh_
#define _CPolyLog_hh_

#include "RealFunction.hh"
#include "ComplexFunction.hh"
#include "IntegFunc.hh"
#include "DbKey.hh"
#include "CGamma.hh"
#include "DataCache2.hh"

#include <Utility/LookupTable.hh>

namespace Transform {
  
  /*!
    \class  CPolyLog  CPolyLog.hh "Tranform/CPolyLog.hh"
    \brief  Class for PolyLogarithmic function, CPolyLog( z ).

    Li_s (z) can be represent with complete Bose/Fermi distribution.
    
  */
  class CPolyLog : public IntegFunc, public ComplexFunction {
  public:
    
    /*!
      \brief  Integrand of CPolyLog
    */
    class Integrand : public ComplexFunction {
    public:
      Integrand() : bose_( true ), sIsPosi_( true ), z_( 0.0 ), s_( 0.0 ) {}
      virtual ~Integrand(){}                   //!< a destructor
      std::complex< double >
      operator()( const std::complex< double >& x );    //!< Bose/Fermi function
      bool bose_;
      bool sIsPosi_;
      std::complex< double > z_;
      std::complex< double > s_;
    };
    
    CPolyLog();                                //!< a defatult constructor
    virtual ~CPolyLog(){}                      //!< a destructor
    std::complex< double >
    operator()( const std::complex< double >& z );      //!< get CPolyLog( z )
    
    //! get CPolyLog( s, z )
    std::complex< double >
    operator()( const std::complex< double >& s,
		const std::complex< double >& z ) {
      return ( this->set( s ) )( z );
    }

    
    CPolyLog& set( const std::complex< double >& s );
    
  private:
    Integrand    func_;       //!< integrand function

    static CGamma G_;

    std::complex< double > normal( const std::complex< double >& z );
    std::complex< double > posiS( const std::complex< double >& z );
    std::complex< double > negaS( const std::complex< double >& z );
    std::complex< double > ints( const std::complex< double >& z );
    
    /*
    // These members are for LookupTable cache
    static DbKey key;                         //!< key for cache
    static std::complex< double > data;       //!< data for cache
    static Utility::LookupTable< DbKey, std::complex< double >  > cache; //!< cache buffer
    static bool cacheReady;   //!< if cache is ready or not
    static void initCache();  //!< initialize cache
    static void closeCache(); //!< close cache file
    */
    
    DataCache2 cache_;
    double q2_;

    bool sIsReal_;
    bool sIsInt_;
    
  };
};

#endif //_CPolyLog_hh_
