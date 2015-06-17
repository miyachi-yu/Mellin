#ifndef _polDIS_CoeffFunc_hh_
#define _polDIS_CoeffFunc_hh_

#include <Utility/Arguments.hh>
#include <Xsec/Charge.hh>
#include <Xsec/CKernel.hh>

namespace polDIS {
  
  class CoeffFunc: public Xsection::CKernel {
  public:
    
    CoeffFunc( Utility::Arguments& args,
	       const Xsection::Charge::TARGET& n );
    
    virtual ~CoeffFunc();
    
  };
  
};

#endif //  _polDIS_CoeffFunc_hh_
