// $Id: StrFunc.hh,v 1.5 2008/08/21 11:34:40 miyachi Exp $
/*!
  \brief class definition of unpolarized structure functions
*/
#ifndef _unpolDIS_StrFunc_hh_
#define _unpolDIS_StrFunc_hh_

#include <iostream>
#include <vector>

#include <Utility/Arguments.hh>
#include <Evolution/KernelBase.hh>
#include <Xsec/Xsec.hh>
#include <Xsec/Charge.hh>

namespace unpolDIS {
  
  /*!
    \brief  unpolarized strucuture function class
  */
  class StrFunc : public Xsection::Xsec {
  public:
    
    /*!
      \param[in] args is a reference to Arguments object
      \param[in] n is a target type
      \brief     a constructour with Arguments object
    */
    StrFunc( Utility::Arguments& arg, 
	     const Xsection::Charge::TARGET& n = Xsection::Charge::p ) 
      throw( int ) ;
    
    /*!
      \param[in] kernel is either unpolPDF::Evo or unpolPDF::CTEQ6pdf
      \param[in] n is a target type
      \brief     a constructour with evolution kernel
    */
    StrFunc( Evolution::KernelBase* kernel,
	     const Xsection::Charge::TARGET& n = Xsection::Charge::p ) 
      throw( int ) ;

    virtual ~StrFunc();                          //!< a destructor
    
  private:
    
  };
  
};

#endif // _unpolDIS_StrFunc_hh_

