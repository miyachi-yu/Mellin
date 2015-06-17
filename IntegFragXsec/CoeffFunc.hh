#ifndef _IntegFragXsec_CoeffFunc_hh_
#define _IntegFragXsec_CoeffFunc_hh_

#include <Utility/Arguments.hh>
#include <IntegXsec/CKernel.hh>
#include <IntegXsec/IntegXsecComp.hh>

#include "StrFunc.hh"

namespace IntegFragXsec {
  
  class CoeffFunc: public IntegXsec::CKernel {
  public:
    
    CoeffFunc( Utility::Arguments& args, IntegXsec::IntegXsecComp* xc );

    virtual ~CoeffFunc();
    
  };
  
};

#endif //  _IntegFragXsec_CoeffFunc_hh_
