// $Id: ALLM97.hh,v 1.2 2008/05/20 00:44:19 miyachi Exp $
/*!
  \file    ALLM97.hh
  \version $Revision: 1.2 $
  \date    $Date: 2008/05/20 00:44:19 $
  \author  $Author: miyachi $
  \brief   Definition of ALLM97 photon absorption cross section parameterization
*/
#ifndef _ALLM97_hh_
#define _ALLM97_hh_

#include "ALLM91.hh"

namespace StrFncParam {

  /*!
    \class ALLM97  ALLM97.hh "StrFncParam/ALLM97.hh"
    \brief class for ALLM97 photon absorption cross section parameterization
    
    ALLM97 virtual photon absorption cross section is implemented in ALLM97 class.
    The parameterization can be found in 
    H. Abramowicz and A. Levy, hep-ph/9712415.
    
  */
  class ALLM97 : public ALLM91 {
  public:
    
    ALLM97();                             //!< a default constructor
    virtual ~ALLM97();                    //!< a destuctor
    
    //! get function name 
    virtual std::string name() const { return "ALLM97(W2,Q2)"; } 
    
  private:
    
  };
  
};

#endif // _ALLM97_hh_
