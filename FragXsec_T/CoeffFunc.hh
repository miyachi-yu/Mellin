// $Id: CoeffFunc.hh,v 1.2 2008/05/23 02:08:00 miyachi Exp $
/*!
  \file CoeffFunc.hh
 */
#ifndef _FragXsec_T_CoeffFunc_hh_
#define _FragXsec_T_CoeffFunc_hh_

#include <Utility/Arguments.hh>
#include <Xsec/CKernel.hh>

namespace FragXsec_T {
  /*!
    \brief Coefficient function handling class for FragXsec_T
   */  
  class CoeffFunc: public Xsection::CKernel {
  public:
    
    CoeffFunc( Utility::Arguments& args ); //!< a constructor
    virtual ~CoeffFunc();                  //!< a destuctor
    
  };
  
};

#endif //  _FragXsec_T_CoeffFunc_hh_
