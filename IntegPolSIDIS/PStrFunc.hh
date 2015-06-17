#ifndef _IntegPolSIDIS_PStrFunc_hh_
#define _IntegPolSIDIS_PStrFunc_hh_

#include "StrFunc.hh"
#include "PartialInteg.hh"

namespace IntegPolSIDIS {
  
  class PStrFunc : public StrFunc, public PartialInteg {
  public:
    
    PStrFunc( Xsection::XsecCompBase* xc, Utility::Arguments& arg, 
             const IntegXsec::Charge::TARGET& n = IntegXsec::Charge::p,      
             const int& low = 4, const int& high = 6,
             const double& precision = 1.0E-4 )
      throw( int ) ;
    
    virtual ~PStrFunc(){};                          //!< a destructor

    virtual void setMoment( const std::complex< double >& N, 
                            const std::complex< double >& M );

    void setx_zPI( const double& x );
    void setz_xPI( const double& z );
    
  private:
    std::complex< double > nxz_;

  protected:

    virtual std::complex< double > getCvalue( const int& i, const int& j );
    // get proper CKernel calc result for the flavor
    
  };
  
};

#endif // _IntegPolSIDIS_PStrFunc_hh_
