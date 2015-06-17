#ifndef _IntegUnpolDrellYan_nXsecComp_hh_
#define _IntegUnpolDrellYan_nXsecComp_hh_

#include <Utility/Arguments.hh>
#include <IntegXsec/Charge.hh>
#include <Xsec/XsecComp.hh>

namespace IntegUnpolDrellYan {
  
  class nXsecComp : public Xsection::XsecComp {
  public:
    
    nXsecComp( Utility::Arguments& args,
	       const IntegXsec::Charge::TARGET& n,
	       const QCD::Flavor::HADRON& h = QCD::Flavor::PRp ) throw( int ) ;
    
    virtual ~nXsecComp();              //!< a destructor
    
    nXsecComp( Evolution::KernelBase *pdf, 
	       const IntegXsec::Charge::TARGET& n = IntegXsec::Charge::p,
	       const QCD::Flavor::HADRON& h = QCD::Flavor::PRp ) 
      throw( int );
    
    virtual void update();            //!< when will it be called??

  private:
    
    //! create CoeffFunc object
    void coefficients( const IntegXsec::Charge::TARGET& n );
    
  };
  
};

#endif //  _IntegUnpolDrellYan_nXsecComp_hh_
