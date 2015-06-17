#ifndef _IntegUnpolDIS_nXsecComp_hh_
#define _IntegUnpolDIS_nXsecComp_hh_

#include <Utility/Arguments.hh>
#include <unpolPDF/Evolution.hh>
#include <unpolPDF/CTEQ6pdf.hh>
#include <IntegXsec/Charge.hh>
#include <Xsec/XsecComp.hh>

namespace IntegUnpolDIS {
  
  /*!
    \brief  unpolarized structure function class
  */
  class nXsecComp : public Xsection::XsecComp {
  public:
    
    /*!
      \param[in] args is a reference to Arguments object
      \param[in] n is a target type
      \brief     a constructour with Arguments object
    */
    nXsecComp( Utility::Arguments& args,
	       const IntegXsec::Charge::TARGET& n ) throw( int ) ;
    
    /*!
      \param[in] kernel is either unpolPDF::Evo or unpolPDF::CTEQ6pdf
      \param[in] n is a target type
      \brief     a constructour with evolution kernel
    */
    nXsecComp( Evolution::KernelBase *kernel,
	       const IntegXsec::Charge::TARGET& n ) throw( int ) ;
    
    virtual ~nXsecComp();   //!< a destructor
    
    virtual void update(); //!< update method, but when is it called?
    
  private:
    
    //! create CoeffFunc object
    void coefficients( const IntegXsec::Charge::TARGET& n );

  };
  
};

#endif //  _IntegUnpolDIS_nXsecComp_hh_
