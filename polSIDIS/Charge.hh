#ifndef _polSIDIS_Charge_hh_
#define _polSIDIS_Charge_hh_

#include <QCD/Flavor.hh>
#include <Xsec/Charge.hh>

namespace polSIDIS {

  class CoeffFunc;
  
  class Charge : public Xsection::Charge {

  public:

    Charge( CoeffFunc* coeff, const Charge::TARGET& n = p ); //!< a constructor
//	    const Xsection::Charge::TARGET& n = p ); //!< a constructor
    virtual ~Charge();             //!< a destructor

    //! get bare charge
    virtual double bare( const Flavor::PARTON& q );
    
  };
  
};

#endif //  _polSIDIS_Charge_hh_
