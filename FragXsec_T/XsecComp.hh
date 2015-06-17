// $Id: XsecComp.hh,v 1.10 2008/11/13 09:06:21 miyachi Exp $
/*!
  \file    XsecComp.hh
  \version $Revision: 1.10 $
  \author  $Author: miyachi $
  \date    $Date: 2008/11/13 09:06:21 $
  \brief   Class definition for cross section components for h production
 */
#ifndef _FragXsec_T_XsecComp_hh_
#define _FragXsec_T_XsecComp_hh_

#include <QCD/Flavor.hh>
#include <Utility/Arguments.hh>
#include <Xsec/XsecComp.hh>
#include <Evolution/Kernel.hh>
#include <Fragmentation/Evolution.hh>

namespace FragXsec_T {
  
  /*!
    \brief xsection component class for FragXsec_T
  */
  class XsecComp : public Xsection::XsecComp {
  public:
    
    /*!
      \param[in] args is a reference to Arguments object
      \param[in] h is a final state hadron type
      \brief a constructor with arguments
    */
    XsecComp( Utility::Arguments& args,
	      const QCD::Flavor::HADRON& h ) throw( int ) ;
    
    /*!
      \param[in] kernel is a pointer to Evolution::Kernel object
      \param[in] h is a final state hadron type
      \brief  a constructor with FF object
    */
    XsecComp( Evolution::Kernel *kernel,
	      const QCD::Flavor::HADRON& h ) throw( int ) ;
    
    virtual ~XsecComp();       //!< a destructor
    virtual void update();     //!< update method
    
  private:
    void coefficient();        //!< create coefficient function
  };
  
};

#endif //  _FragXsec_T_XsecComp_hh_
