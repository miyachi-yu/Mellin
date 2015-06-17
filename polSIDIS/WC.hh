#ifndef _polSIDIS_WC_hh_
#define _polSIDIS_WC_hh_

#include <Xsec/XsecComp.hh>
#include <Xsec/Wilsons.hh>
#include <Xsec/Coefficient.hh>

namespace polSIDIS {
  
  class WC : public Xsection::Wilsons {
  public:
    
    WC( Xsection::XsecComp* xc );
    virtual ~WC();
    
  private:
    
    Xsection::Coefficient* qqlo ;
    Xsection::Coefficient* qqnlo;
    Xsection::Coefficient* gqnlo;
    Xsection::Coefficient* qgnlo;
    Xsection::Coefficient* cnull;

  };
  
};

#endif //  _polSIDIS_WC_hh_
