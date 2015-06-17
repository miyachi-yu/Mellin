/*!
  \brief  Class definition of Structure function in unpol SIDIS

  
  $Id: StrFunc.hh,v 1.7 2008/11/20 09:50:52 miyachi Exp $
 */
#ifndef _unpolSIDIS_StrFunc_hh_
#define _unpolSIDIS_StrFunc_hh_

#include <Utility/Arguments.hh>
#include <unpolPDF/Evolution.hh>
#include <Fragmentation/Evolution.hh>
#include <Xsec/Xsec.hh>
#include <Xsec/Charge.hh>

namespace unpolSIDIS {
  
  /*!
    \brief  class for unpolaried structure funciton in SIDIS
  */
  class StrFunc : public Xsection::Xsec {
  public:
    
    /*!
      \param[in] arg is reference to the Utility::Argument object
      \param[in] n is target nucleon type
      \param[in] h is a fragmenting hadron type
      \brief  a constructor with Argumetns object
    */
    StrFunc( Utility::Arguments& arg, 
	     const Xsection::Charge::TARGET& n = Xsection::Charge::p,
	     const QCD::Flavor::HADRON& h = QCD::Flavor::PIp ) 
      throw( int ) ;
    
    /*!
      \param[in] pdf is a pointer to a unpolPDF::Evo or unpolPDF::CTEQ6pdf object
      \param[in] ff is a pointer to a Evolution::Kernel object
      \param[in] n is a target nucleon type
      \param[in] h is a fragmenting hadron type
    */
    StrFunc( Evolution::KernelBase *pdf,
	     Evolution::Kernel *ff,
	     const Xsection::Charge::TARGET& n = Xsection::Charge::p,
	     const QCD::Flavor::HADRON& h = QCD::Flavor::PIp ) 
      throw( int ) ;
    
    virtual ~StrFunc();                          //!< a destructor
    
    virtual std::complex< double > operator()
    ( const std::complex< double >& n, const std::complex< double >& m );
    
  private:
    
  };
  
};
#endif // _unpolSIDIS_StrFunc_hh_

