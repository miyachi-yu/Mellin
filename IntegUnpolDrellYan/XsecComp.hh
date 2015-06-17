#ifndef _IntegUnpolDrellYan_XsecComp_hh_
#define _IntegUnpolDrellYan_XsecComp_hh_

#include <Utility/Arguments.hh>
#include <IntegXsec/Charge.hh>
#include <IntegXsec/XsecIntegrand.hh>

namespace IntegUnpolDrellYan {
  
  class XsecComp : public IntegXsec::XsecIntegrand {
  public:
    
    XsecComp( Utility::Arguments& args, const IntegXsec::Charge::TARGET& n,
	      const QCD::Flavor::HADRON& h,
              const int& low = 4, const int& high = 6,
              const double& precision = 1.0E-4, const double& length = 10.0,
              const double& offset = 1.8, const double& angle = 0.6 ) 
      throw( int ) ;

    XsecComp( Xsection::XsecCompBase* xc, 
              Utility::Arguments& args, const IntegXsec::Charge::TARGET& n )
      throw( int ) ;
    
    virtual ~XsecComp();
    
    virtual void update();
    
  private:
    
  };
  
};

#endif //  _IntegUnpolDrellYan_XsecComp_hh_
