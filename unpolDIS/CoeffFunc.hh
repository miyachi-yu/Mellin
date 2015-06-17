#ifndef _unpolDIS_CoeffFunc_hh_
#define _unpolDIS_CoeffFunc_hh_

#include <Utility/Arguments.hh>
#include <Xsec/CKernel.hh>

#include "StrFunc.hh"

namespace unpolDIS {
  
  class CoeffFunc: public Xsection::CKernel {
  public:
    
    CoeffFunc( Utility::Arguments& args, 
	       const Xsection::Charge::TARGET& n );

    virtual ~CoeffFunc();
    
  };
  
};

#endif //  _unpolDIS_CoeffFunc_hh_
