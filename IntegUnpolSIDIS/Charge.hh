#ifndef _IntegUnpolSIDIS_Charge_hh_
#define _IntegUnpolSIDIS_Charge_hh_

#include <QCD/Flavor.hh>
#include <IntegXsec/Charge.hh>

namespace IntegUnpolSIDIS {
  
  class Charge : public IntegXsec::Charge {
    
  public:
    
    Charge( IntegXsec::CKernel* coeff, 
	    const IntegXsec::Charge::TARGET& n = p ); //!< a constructor

    virtual ~Charge();             //!< a destructor
    
    //! get bare charge
    virtual double bare( const QCD::Flavor::PARTON& q );
    
  };
  
};

#endif //  _IntegUnpolSIDIS_Charge_hh_
