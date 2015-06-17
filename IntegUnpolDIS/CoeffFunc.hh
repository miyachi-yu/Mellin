#ifndef _IntegUnpolDIS_CoeffFunc_hh_
#define _IntegUnpolDIS_CoeffFunc_hh_

#include <Utility/Arguments.hh>
#include <IntegXsec/CKernel.hh>
#include <IntegXsec/IntegXsecComp.hh>

#include "StrFunc.hh"

namespace IntegUnpolDIS {
  
  class CoeffFunc: public IntegXsec::CKernel {
  public:
    
    CoeffFunc( Utility::Arguments& args, IntegXsec::IntegXsecComp* xc,
	       const IntegXsec::Charge::TARGET& n );

    virtual ~CoeffFunc();
    
  };
  
};

#endif //  _IntegUnpolDIS_CoeffFunc_hh_
