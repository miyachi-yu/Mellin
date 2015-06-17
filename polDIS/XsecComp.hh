// $Id: XsecComp.hh,v 1.10 2009/02/13 12:23:41 miyachi Exp $
/*!
  \file    XsecComp.hh
  \version $Revision: 1.10 $
  \author  $Author: miyachi $
  \date    $Date: 2009/02/13 12:23:41 $
  \brief   Class definition for cross section components for h production
 */
#ifndef _polDIS_XsecComp_hh_
#define _polDIS_XsecComp_hh_

#include <Utility/Arguments.hh>
#include <polPDF/Evolution.hh>
#include <Xsec/Charge.hh>
#include <Xsec/XsecComp.hh>

namespace polDIS {
  
  /*!
    \class XsecComp
    \brief Xsection compornent handling class for polDIS
   */
  class XsecComp : public Xsection::XsecComp {
  public:
    
    //! a constructor
    XsecComp( Utility::Arguments& args,
	      const Xsection::Charge::TARGET& n ) throw( int ) ;
    
    /*!
      \param[in] evo is a polPDF::Evo pointer
      \param[in] n is a TARGET nucleon type
      \brief a constructor with a polPDF::Evo object

      In order to share an existed evolution kernel among 
      Xsec objects, this constructor was prepared.
    */
    XsecComp( Evolution::Kernel *evo, 
	      const Xsection::Charge::TARGET& n ) throw( int ) ;
    
    virtual ~XsecComp();  //!< a destructor
    
    virtual void update(); //!< update coefficient functions
    
  private:
    
    //! create CoeffFunc object
    void coefficients( const Xsection::Charge::TARGET& n );
    
  };
  
};

#endif //  _polDIS_XsecComp_hh_
