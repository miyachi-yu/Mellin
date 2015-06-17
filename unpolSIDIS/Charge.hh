#ifndef _unpolSIDIS_Charge_hh_
#define _unpolSIDIS_Charge_hh_

#include <QCD/Flavor.hh>
#include <Xsec/Charge.hh>

namespace unpolSIDIS {
  
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

#endif //  _unpolSIDIS_Charge_hh_
