// $Id: XsecComp.hh,v 1.7 2008/11/13 09:06:22 miyachi Exp $
/*!
  \brief  class definition for unpolarized structure functions
 */
#ifndef _unpolDIS_XsecComp_hh_
#define _unpolDIS_XsecComp_hh_

#include <Utility/Arguments.hh>
#include <unpolPDF/Evolution.hh>
#include <unpolPDF/CTEQ6pdf.hh>
#include <Xsec/Charge.hh>
#include <Xsec/XsecComp.hh>

namespace unpolDIS {
  
  /*!
    \brief  unpolarized structure function class
  */
  class XsecComp : public Xsection::XsecComp {
  public:
    
    /*!
      \param[in] args is a reference to Arguments object
      \param[in] n is a target type
      \brief     a constructour with Arguments object
    */
    XsecComp( Utility::Arguments& args,
	      const Xsection::Charge::TARGET& n ) throw( int ) ;
    
    /*!
      \param[in] kernel is either unpolPDF::Evo or unpolPDF::CTEQ6pdf
      \param[in] n is a target type
      \brief     a constructour with evolution kernel
    */
    XsecComp( Evolution::KernelBase *kernel,
	      const Xsection::Charge::TARGET& n ) throw( int ) ;
    
    virtual ~XsecComp();   //!< a destructor
    
    virtual void update(); //!< update method, but when is it called?
    
  private:
    
    //! create CoeffFunc object
    void coefficients( const Xsection::Charge::TARGET& n );

  };
  
};

#endif //  _unpolDIS_XsecComp_hh_
