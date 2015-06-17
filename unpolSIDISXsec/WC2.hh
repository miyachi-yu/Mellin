#ifndef _unpolSIDISXsec_WC2_hh_
#define _unpolSIDISXsec_WC2_hh_

#include <Xsec/XsecComp.hh>
#include <Xsec/Wilsons.hh>
#include <Xsec/Coefficient.hh>

namespace unpolSIDISXsec {
  
  class WC2 : public Xsection::Wilsons {
  public:
    
    WC2( Xsection::XsecComp* xc );
    virtual ~WC2();
    
  private:
    
    Xsection::Coefficient* qqlo ;
    Xsection::Coefficient* qqnlo;
    Xsection::Coefficient* gqnlo;
    Xsection::Coefficient* qgnlo;
    Xsection::Coefficient* cnull;

  };
  
};

#endif //  _unpolSIDISXsec_WC2_hh_
