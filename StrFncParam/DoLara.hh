// $Id: DoLara.hh,v 1.2 2008/05/20 00:44:19 miyachi Exp $
/*!
  \file  DoLara.hh
  \brief class definition of photo-abs xsec param by Dominik and Lara
 */
#ifndef _DoLara_hh_
#define _DoLara_hh_

#include "ALLM91.hh"

namespace StrFncParam {
  /*!
    \brief class for photo-abs xsec param by Dominik and Lara
   */
  class DoLara : public ALLM91 {
  public:
    
    DoLara();                             //!< a default constructor
    virtual ~DoLara();                    //!< a destuctor
    
    virtual std::string name() const { return "DOLARA(W^2,Q^2)"; } 
    
  private:
    
  };
  
};
#endif // _DoLara_hh_
