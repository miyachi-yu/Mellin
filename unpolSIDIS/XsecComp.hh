/*!
  \brief   class definition of Xsection Components for unpol SIDIS

  This is a class definition file for Xsection components for the 
  unpolarized structure functions in SIDIS.

  $Id: XsecComp.hh,v 1.7 2008/11/13 09:06:22 miyachi Exp $
 */
#ifndef _unpolSIDIS_XsecComp_hh_
#define _unpolSIDIS_XsecComp_hh_

#include <Utility/Arguments.hh>
#include <Xsec/Charge.hh>
#include <Xsec/XsecComp.hh>
#include <unpolPDF/Evolution.hh>
#include <unpolPDF/CTEQ6pdf.hh>
#include <Fragmentation/Evolution.hh>

namespace unpolSIDIS {
  
  /*!
    \brief  Xsection Components for unpol SIDIS
  */
  class XsecComp : public Xsection::XsecComp {
  public:
    
    /*!
      \param[in] args is a reference Utility::Arguments object
      \param[in] n is a target type
      \param[in] h is a final state hadron type
      \brief  a constructur with Arguments

      This requires options in the argument list, 

    */
    XsecComp( Utility::Arguments& args,
	      const Xsection::Charge::TARGET& n,
	      const QCD::Flavor::HADRON& h = QCD::Flavor::PIp ) throw( int ) ;
    
    virtual ~XsecComp();              //!< a destructor
    
    /*!
      \param[in] pdf is a pointer to a unpolPDF::Evo or unpolPDF::CTEQ6pdf object.
      \param[in] ff  is a pointer to a Fragmentation::Evo object
      \param[in] n   is a target nucleon type
      \param[in] h   is a final state hadron type
    */
    XsecComp( Evolution::KernelBase *pdf, 
	      Evolution::Kernel *ff,
	      const Xsection::Charge::TARGET& n = Xsection::Charge::p,
	      const QCD::Flavor::HADRON& h = QCD::Flavor::PIp ) 
      throw( int );
    
    virtual void update();            //!< when will it be called??

  private:
    
    //! create CoeffFunc object
    void coefficients( const Xsection::Charge::TARGET& n );
    
  };
  
};

#endif //  _unpolSIDIS_XsecComp_hh_
