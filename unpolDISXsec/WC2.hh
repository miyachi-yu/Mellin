#ifndef _unpolDISXsec_WC2_hh_
#define _unpolDISXsec_WC2_hh_

#include <Xsec/XsecComp.hh>
#include <Xsec/Wilsons.hh>
#include <Xsec/Coefficient.hh>

namespace unpolDISXsec {
  
  class WC2 : public Xsection::Wilsons {
  public:
    
    WC2( Xsection::XsecComp* xc );
    virtual ~WC2();
    
  private:
    
    Xsection::Coefficient* qqlo ;
    Xsection::Coefficient* qqnlo;
    Xsection::Coefficient* ggnlo;
    Xsection::Coefficient* cnull;

  };
  
};

#endif //  _unpolDISXsec_WC2_hh_
