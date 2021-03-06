#ifndef _IntegUnpolDIS_StrFunc_hh_
#define _IntegUnpolDIS_StrFunc_hh_

#include <iostream>
#include <vector>

#include <Utility/Arguments.hh>
#include <IntegXsec/xXsec.hh>
#include <IntegXsec/Charge.hh>

namespace Xsection{
  class XsecCompBase;
}

namespace IntegUnpolDIS {
  
  class StrFunc : public IntegXsec::xXsec {
  public:
    
    StrFunc( Utility::Arguments& arg, 
	     const IntegXsec::Charge::TARGET& n = IntegXsec::Charge::p,	     
             const int& low = 4, const int& high = 6,
             const double& precision = 1.0E-4, const double& length = 10.0,
             const double& offset = 1.8, const double& angle = 0.6 )
      throw( int ) ;

    StrFunc( Xsection::XsecCompBase* xc, Utility::Arguments& arg, 
	     const IntegXsec::Charge::TARGET& n = IntegXsec::Charge::p,	     
             const int& low = 4, const int& high = 6,
             const double& precision = 1.0E-4 )
      throw( int ) ;
    
    virtual ~StrFunc();                          //!< a destructor
    
  private:
    
  };
  
};

#endif // _IntegUnpolDIS_StrFunc_hh_

