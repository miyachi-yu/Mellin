// $Id: Wilson.hh,v 1.6 2008/05/23 02:08:04 miyachi Exp $
/*!
  \file Willson.hh
 */
#ifndef _polDIS_Wilson_hh_
#define _polDIS_Wilson_hh_

#include <Xsec/Wilsons.hh>
#include <Xsec/Coefficient.hh>
#include <Xsec/XsecComp.hh>

namespace polDIS {
  
  class Wilson : public Xsection::Wilsons {
  public:
    Wilson( Xsection::XsecComp* xc ); //!< a constructor
    virtual ~Wilson();                //!< a destructor
    
  private:
    
    Xsection::Coefficient* qlo ;
    Xsection::Coefficient* qnlo;
    Xsection::Coefficient* gnlo;
    Xsection::Coefficient* cnull;
    
  };
  
};

#endif // _polDIS_Wilson_hh_


/*  old implementation
  DEF_ADIM( CQLO );
  DEF_ADIM( CGLO );
  DEF_ADIM( CQNLO );
  DEF_ADIM( CGNLO );
*/
