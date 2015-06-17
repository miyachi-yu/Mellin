#ifndef _IntegPolDIS_CoeffFunc_hh_
#define _IntegPolDIS_CoeffFunc_hh_

#include <Utility/Arguments.hh>
#include <IntegXsec/CKernel.hh>
#include <IntegXsec/IntegXsecComp.hh>

#include "StrFunc.hh"

namespace IntegPolDIS {
  
  class CoeffFunc: public IntegXsec::CKernel {
  public:
    
    CoeffFunc( Utility::Arguments& args, IntegXsec::IntegXsecComp* xc,
	       const IntegXsec::Charge::TARGET& n );

    virtual ~CoeffFunc();
    
  };
  
};

#endif //  _IntegPolDIS_CoeffFunc_hh_
