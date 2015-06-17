#ifndef _polSIDIS_CoeffFunc_hh_
#define _polSIDIS_CoeffFunc_hh_

#include <Utility/Arguments.hh>
#include <Xsec/Charge.hh>
#include <Xsec/CKernel.hh>

namespace polSIDIS {
  
  class CoeffFunc: public Xsection::CKernel {
  public:
    
    CoeffFunc( Utility::Arguments& args, 
	       const Xsection::Charge::TARGET& n );

    virtual ~CoeffFunc();
    
  };
  
};

#endif //  _polSIDIS_CoeffFunc_hh_
