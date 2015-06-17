#ifndef _IntegPolVBoson_Charge_hh_
#define _IntegPolVBoson_Charge_hh_

#include <QCD/Flavor.hh>
#include <IntegXsec/Charge.hh>

namespace IntegPolVBoson {
  
  class Charge : public IntegXsec::Charge {
    
  public:
    
    Charge( IntegXsec::CKernel* coeff, 
	    const IntegXsec::Charge::TARGET& n = p ); //!< a constructor

    virtual ~Charge();             //!< a destructor
    
    //! get bare charge
    virtual double bare( const QCD::Flavor::PARTON& q );

    //! for W production
    virtual double bare( const QCD::Flavor::PARTON& q1,
			 const QCD::Flavor::PARTON& q2 );

    /*    Physics constans     */
    static const double CKMMX[ 3 ][ 3 ]; // (absolute) CKM Matrix for W process
    static const double EVC[ 3 ];  //!< A vector current charge
    static const double WAC[ 3 ];  //!< Z axial  current charge
    static double WVC[ 3 ];        //!< Z vector current charge

    static const double SWVA;    //!< Weinberg Angles

  private:

    void initialize();
    
  };
  
};

#endif //  _IntegPolVBoson_Charge_hh_
