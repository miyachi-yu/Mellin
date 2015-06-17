// $Id: StrFunc.hh,v 1.2 2009/02/13 12:23:41 miyachi Exp $
/*!
  \file     StrFunc.hh
  \version  $Revision: 1.2 $
  \author   $Author: miyachi $
  \date     $Date: 2009/02/13 12:23:41 $
  \brief    class definition of structure functions in polarized DIS
 */
#ifndef _polDIS_StrFunc_hh_
#define _polDIS_StrFunc_hh_

#include <Utility/Arguments.hh>
#include <Xsec/Charge.hh>
#include <Xsec/Xsec.hh>
#include <polPDF/Evolution.hh>

namespace polDIS {
  
  /*!
    \class StrFunc
    \brief Class for structure functions in polarized DIS
    
    Polarized structure functions are implemented in Xsec framework.
    At the moment, one can calculate only inclusive structure functions,
    g1p, g1n, and g1d.
    
    To do:   Include Fragmentation library for structure function for
    polarized SIDIS.
    
  */
  class StrFunc : public Xsection::Xsec {
  public:
    
    //! a constructor
    StrFunc( Utility::Arguments& args, 
	     const Xsection::Charge::TARGET& n = Xsection::Charge::p )
      throw( int ) ;
    
    //! a constructor with a polPDF::Evo object
    StrFunc( Evolution::Kernel *evo,
	     const Xsection::Charge::TARGET& n = Xsection::Charge::p )
      throw( int ) ;
    
    virtual ~StrFunc(); //!< a destructor
    
  private:
  };
  
};

#endif //  _polDIS_StrFunc_hh_
