// $Id: XsecComp.hh,v 1.8 2009/02/13 12:23:41 miyachi Exp $
/*!
  \file   XsecComp.hh
  \brief  class definition for xsection component for polarized SIDIS
 */
#ifndef _polSIDIS_XsecComp_hh_
#define _polSIDIS_XsecComp_hh_

#include <Utility/Arguments.hh>
#include <Xsec/Charge.hh>
#include <Xsec/XsecComp.hh>
#include <polPDF/Evolution.hh>
#include <Fragmentation/Evolution.hh>

namespace polSIDIS {
  
  /*!
    \brief  xsection component handling class for polarized SIDIS
   */
  class XsecComp : public Xsection::XsecComp {
  public:
    
    /*!
      \param[in] arg is a reference to an Argument object
      \param[in] n is a target nucleon type
      \param[in] h is a final state hadron type
      \brief a constructor with arguments object
      
      This try to construct polSIDIS::XsecComp object with information
      given in the argument list. The evolution kernel objects, PDF and 
      FF, will be created, if there have not been.
      XsecComp shares these PDF and FF objects.
      
    */
    XsecComp( Utility::Arguments& args,
	      const Xsection::Charge::TARGET& n = Xsection::Charge::p,
	      const QCD::Flavor::HADRON& h = QCD::Flavor::PIp ) 
      throw( int ) ;
    
    /*!
      \param[in] pdf is a pointer to a polPDF::Evo object.
      \param[in] ff  is a pointer to a Fragmentation::Evo object
      \param[in] n   is a target nucleon type
      \param[in] h   is a final state hadron type
    */
    XsecComp( Evolution::Kernel *pdf, 
	      Evolution::Kernel *ff,
	      const Xsection::Charge::TARGET& n = Xsection::Charge::p,
	      const QCD::Flavor::HADRON& h = QCD::Flavor::PIp ) 
      throw( int );
    
    virtual ~XsecComp();          //!< a destructor
    
    virtual void update();        //!< update
    
  private:
    
    //! create CoeffFunc object
    void coefficients( const Xsection::Charge::TARGET& n );
    
  };
  
};

#endif //  _polSIDIS_XsecComp_hh_
