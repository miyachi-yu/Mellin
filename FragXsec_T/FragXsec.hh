// $Id: FragXsec.hh,v 1.10 2008/10/28 09:12:26 miyachi Exp $
/*!
  \file    FragXsec.hh
  \version $Revision: 1.10 $
  \author  $Author: miyachi $
  \date    $Date: 2008/10/28 09:12:26 $
  \brief Class definition for cross section calculation for hadron production
 */
#ifndef _FragXsec_T_FragXsec_hh_
#define _FragXsec_T_FragXsec_hh_

#include <iostream>
#include <vector>

#include <QCD/Flavor.hh>
#include <Utility/Arguments.hh>
#include <Evolution/Kernel.hh>
#include <Xsec/Xsec.hh>

namespace FragXsec_T {
  
  /*!
    \brief cross-section handling class for hadron production e^+ e^
    
    Fragmentation function can be switched for pion and kaon with
    --FF and --FFK option in the arguments. 
    
  */
  class FragXsec : public Xsection::Xsec {
  public:
    
    /*!
      \param[in] arg is an Arguments object
      \param[in] h is a QCD::Flavor::HADEON
      \brief a constructor for a hadron type h
    */
    FragXsec( Utility::Arguments& arg, 
	      const QCD::Flavor::HADRON& h = QCD::Flavor::PIp ) throw( int ) ;
    
    /*!
      \param[in] kernel is a Evolution::Kernel object pointer
      \param[in] h is a QCD::Flavor::HADEON
      \brief a constructor with FF for a hadron type h
    */
    FragXsec( Evolution::Kernel *kernel, const QCD::Flavor::HADRON& h )
      throw( int ) ;
    
    virtual ~FragXsec();                          //!< a destructor
    
  private:
    
  };
  
};

#endif // _FragXsec_T_FragXsec_hh_

