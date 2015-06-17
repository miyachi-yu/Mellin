#ifndef _unpolDISXsec_WC_hh_
#define _unpolDISXsec_WC_hh_

#include <Xsec/XsecComp.hh>
#include <Xsec/Wilsons.hh>
#include <Xsec/Coefficient.hh>

namespace unpolDISXsec {
  
  class WC : public Xsection::Wilsons {
  public:
    
    WC( Xsection::XsecComp* xc );
    virtual ~WC();
    
    Xsection::Coefficient* getqqlo(){ return qqlo; }
    Xsection::Coefficient* getqqnlo(){ return qqnlo; }
    Xsection::Coefficient* getggnlo(){ return ggnlo; }
    Xsection::Coefficient* getcnull(){ return cnull; }

  private:
    
    Xsection::Coefficient* qqlo ;
    Xsection::Coefficient* qqnlo;
    Xsection::Coefficient* ggnlo;
    Xsection::Coefficient* cnull;

  };
  
};

#endif //  _unpolDISXsec_WC_hh_
