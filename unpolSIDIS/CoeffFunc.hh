#ifndef _unpolSIDIS_CoeffFunc_hh_
#define _unpolSIDIS_CoeffFunc_hh_

#include <Utility/Arguments.hh>
#include <Xsec/Charge.hh>
#include <Xsec/CKernel.hh>

namespace unpolSIDIS {
  
  class CoeffFunc: public Xsection::CKernel {
  public:
    
    CoeffFunc( Utility::Arguments& args, 
	       const Xsection::Charge::TARGET& n );
    
    virtual ~CoeffFunc();
    
  };
  
};

#endif //  _unpolSIDIS_CoeffFunc_hh_
