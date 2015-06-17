#ifndef _HyperGeomInt_hh_
#define _HyperGeomInt_hh_

#include "IntegFunc.hh"
#include "ComplexFunction.hh"
#include "HyperGeom.hh"
#include "Li2Int2.hh"

#include <Utility/LookupTable.hh>

namespace Transform {

  /*!
    \class HyperGeomInt HyperGeomInt.hh "Tranform/HyperGeomInt.hh"
    \brief class to handle integration appeared in S~_{1,2} function.
  */
  class HyperGeomInt : public IntegFunc, public ComplexFunction {
  public:
    
    class Integrand : public ComplexFunction {
    public:
      virtual ~Integrand(){}
      std::complex< double > operator()( const std::complex< double >& n );
      std::complex< double > operator()( const double& x );
      ComplexFunction& n( const std::complex< double >& n ) {
	n_ = n; return *this;
      }
    private:
      HyperGeom    hyper_;
      std::complex< double > n_;
      Li2          li2_;
    };
    
    HyperGeomInt();
    virtual ~HyperGeomInt() {}
    std::complex< double > operator()( const std::complex< double >& n );
    
  private:
    Integrand    func_;
    
    static CKey key;
    static std::complex< double > data;
    static Utility::LookupTable< CKey, std::complex< double > > cache;
    static bool cacheReady;
    static void initCache();
    static void closeCache();
    
    Li2Int2  li2int2_;
    
  };
};
#endif // _HyperGeomInt_hh_
  
