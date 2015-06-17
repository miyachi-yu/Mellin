#ifndef _IntegPolSIDIS_StrFunc_hh_
#define _IntegPolSIDIS_StrFunc_hh_

#include <iostream>
#include <vector>

#include <Utility/Arguments.hh>
#include <IntegXsec/xzXsec.hh>
#include <IntegXsec/Charge.hh>

namespace Xsection{
  class XsecCompBase;
}

namespace IntegPolSIDIS {
  
  class StrFunc : public IntegXsec::xzXsec {
  public:
    
    StrFunc( Utility::Arguments& args, 
	     const IntegXsec::Charge::TARGET& n = IntegXsec::Charge::p,	     
	     const QCD::Flavor::HADRON& h = QCD::Flavor::PIp,
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

#endif // _IntegPolSIDIS_StrFunc_hh_

