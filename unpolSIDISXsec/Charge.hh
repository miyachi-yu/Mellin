#ifndef _unpolSIDISXsec_Charge_hh_
#define _unpolSIDISXsec_Charge_hh_

#include <QCD/Flavor.hh>
#include <Xsec/Charge.hh>

namespace unpolSIDISXsec {
  
  class Charge : public Xsection::Charge {
    
  public:
    
    Charge( Xsection::CKernel* coeff, 
	    const Xsection::Charge::TARGET& n = p ); //!< a constructor
    //	    const Xsection::Charge::TARGET& n = p ); //!< a constructor

    virtual ~Charge();             //!< a destructor
    
    //! get bare charge
    virtual double bare( const QCD::Flavor::PARTON& q );
    
  };
  
};

#endif //  _unpolSIDISXsec_Charge_hh_
