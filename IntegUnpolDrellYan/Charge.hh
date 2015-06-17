#ifndef _IntegUnpolDrellYan_Charge_hh_
#define _IntegUnpolDrellYan_Charge_hh_

#include <QCD/Flavor.hh>
#include <IntegXsec/Charge.hh>

namespace IntegUnpolDrellYan {
  
  class Charge : public IntegXsec::Charge {
    
  public:
    
    Charge( IntegXsec::CKernel* coeff, 
	    const IntegXsec::Charge::TARGET& n = p ); //!< a constructor

    virtual ~Charge();             //!< a destructor
    
    //! get bare charge
    virtual double bare( const QCD::Flavor::PARTON& q );
    
  };
  
};

#endif //  _IntegUnpolDrellYan_Charge_hh_
