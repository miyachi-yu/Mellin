#ifndef _IntegUnpolSIDIS_nXsecComp_hh_
#define _IntegUnpolSIDIS_nXsecComp_hh_

#include <Utility/Arguments.hh>
#include <unpolPDF/Evolution.hh>
#include <unpolPDF/CTEQ6pdf.hh>
#include <Fragmentation/Evolution.hh>
#include <IntegXsec/Charge.hh>
#include <Xsec/XsecComp.hh>

namespace IntegUnpolSIDIS {
  
  /*!
    \brief  Xsection Components for unpol SIDIS
  */
  class nXsecComp : public Xsection::XsecComp {
  public:
    
    /*!
      \param[in] args is a reference Utility::Arguments object
      \param[in] n is a target type
      \param[in] h is a final state hadron type
      \brief  a constructur with Arguments

      This requires options in the argument list, 

    */
    nXsecComp( Utility::Arguments& args,
	      const IntegXsec::Charge::TARGET& n,
	      const QCD::Flavor::HADRON& h = QCD::Flavor::PIp ) throw( int ) ;
    
    virtual ~nXsecComp();              //!< a destructor
    
    /*!
      \param[in] pdf is a pointer to a unpolPDF::Evo or unpolPDF::CTEQ6pdf xobject.
      \param[in] ff  is a pointer to a Fragmentation::Evo object
      \param[in] n   is a target nucleon type
      \param[in] h   is a final state hadron type
    */
    nXsecComp( Evolution::KernelBase *pdf, 
	      Evolution::Kernel *ff,
	      const IntegXsec::Charge::TARGET& n = IntegXsec::Charge::p,
	      const QCD::Flavor::HADRON& h = QCD::Flavor::PIp ) 
      throw( int );
    
    virtual void update();            //!< when will it be called??

  private:
    
    //! create CoeffFunc object
    void coefficients( const IntegXsec::Charge::TARGET& n );
    
  };
  
};

#endif //  _IntegUnpolSIDIS_nXsecComp_hh_
