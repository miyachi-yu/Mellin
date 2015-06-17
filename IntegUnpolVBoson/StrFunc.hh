#ifndef _IntegUnpolVBoson_StrFunc_hh_
#define _IntegUnpolVBoson_StrFunc_hh_

#include <iostream>
#include <vector>

#include <Utility/Arguments.hh>
#include <IntegXsec/xzXsec.hh>
#include <IntegXsec/Charge.hh>

namespace Xsection{
  class XsecCompBase;
}

namespace IntegUnpolVBoson {
  
  class StrFunc : public IntegXsec::xzXsec {
  public:
    
    StrFunc( Utility::Arguments& args, 
	     const IntegXsec::Charge::TARGET& n = IntegXsec::Charge::p,	     
	     const QCD::Flavor::HADRON& h = QCD::Flavor::PRp,
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

  protected:

    virtual double charge( const int& i, const int& j );   
    // get charge from CKernel for appropriate flavor

    virtual bool setActflav( const int& i, const int& j );
    // get proper & efficient flavors for CKernel calc ( acti_, actj )
    
  };
  
};

#endif // _IntegUnpolVBoson_StrFunc_hh_

