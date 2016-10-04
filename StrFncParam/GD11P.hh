// $Id: GD11P.hh,v 1.2 2008/05/20 00:44:19 miyachi Exp $
/*!
  \file  GD11P.hh
  \brief class definition of photo-abs xsec param by Dominik and Lara
 */
#ifndef _GD11P_hh_
#define _GD11P_hh_

#include "ALLM91.hh"

namespace StrFncParam {
  /*!
    \brief class for photo-abs xsec param by Dominik and Lara (GD11-P)

    Parameterization of photo-absorption cross section, known as GD11-P 
    (JHEP 05 (2011) 126)

   */
  class GD11P : public ALLM91 {
  public:
    
    GD11P();                             //!< a default constructor
    virtual ~GD11P();                    //!< a destuctor
    
    virtual std::string name() const { return "GD11P(W^2,Q^2)"; } 
    
  private:
    
  };
  
};
#endif // _GD11P_hh_
