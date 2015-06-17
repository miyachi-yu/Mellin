// $Id: F2dSMC.hh,v 1.2 2008/05/20 00:44:19 miyachi Exp $
/*!
  \brief class definition of F2d SMC parameterization
 */
#ifndef _F2dSMC_hh_
#define _F2dSMC_hh_

#include "F2pSMC.hh"
#include <vector>

namespace StrFncParam {
  
  /*!
    \brief class for F2d SMC parameterization
   */
  class F2dSMC : public F2pSMC {
  public:
    
    F2dSMC();
  virtual ~F2dSMC();
    
  };
};

#endif // _F2dSMC_hh_
