#ifndef _unpolDIS_WC_hh_
#define _unpolDIS_WC_hh_

#include <Xsec/XsecComp.hh>
#include <Xsec/Wilsons.hh>
#include <Xsec/Coefficient.hh>

namespace unpolDIS {
  
  class WC : public Xsection::Wilsons {
  public:
    
    WC( Xsection::XsecComp* xc );
    virtual ~WC();
    
  private:
    
    Xsection::Coefficient* qqlo ;
    Xsection::Coefficient* qqnlo;
    Xsection::Coefficient* ggnlo;
    Xsection::Coefficient* cnull;

  };
  
};

#endif //  _unpolDIS_WC_hh_
