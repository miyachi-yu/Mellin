#ifndef _unpolSIDISXsec_WC_hh_
#define _unpolSIDISXsec_WC_hh_

#include <Xsec/XsecComp.hh>
#include <Xsec/Wilsons.hh>
#include <Xsec/Coefficient.hh>

namespace unpolSIDISXsec {
  
  class WC : public Xsection::Wilsons {
  public:
    
    WC( Xsection::XsecComp* xc );
    virtual ~WC();
    
    Xsection::Coefficient* getqqlo(){ return qqlo; }
    Xsection::Coefficient* getqqnlo(){ return qqnlo; }
    Xsection::Coefficient* getgqnlo(){ return gqnlo; }
    Xsection::Coefficient* getqgnlo(){ return qgnlo; }
    Xsection::Coefficient* getcnull(){ return cnull; }

  private:
    
    Xsection::Coefficient* qqlo ;
    Xsection::Coefficient* qqnlo;
    Xsection::Coefficient* gqnlo;
    Xsection::Coefficient* qgnlo;
    Xsection::Coefficient* cnull;

  };
  
};

#endif //  _unpolSIDISXsec_WC_hh_
