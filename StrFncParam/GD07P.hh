// $Id: GD07P.hh,v 1.2 2008/05/20 00:44:19 miyachi Exp $
/*!
  \file  GD07P.hh
  \brief class definition of photo-abs xsec param by Dominik and Lara
 */
#ifndef _GD07P_hh_
#define _GD07P_hh_

#include "ALLM91.hh"

namespace StrFncParam {
  /*!
    \brief class for photo-abs xsec param by Dominik and Lara (GD07-P)

    Parameterization of photo-absorption cross section, known as GD07-P 
    (hep-ph/0708.3196)

   */
  class GD07P : public ALLM91 {
  public:
    
    GD07P();                             //!< a default constructor
    virtual ~GD07P();                    //!< a destuctor
    
    virtual std::string name() const { return "GD07P(W^2,Q^2)"; } 
    
  private:
    
  };
  
};
#endif // _GD07P_hh_
