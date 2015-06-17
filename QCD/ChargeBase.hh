// $Id: ChargeBase.hh,v 1.8 2009/06/18 13:24:48 imazu Exp $
/*!
  \file   ChargeBase.hh
  \brief  pure abstract class definition for Charge handling
 */
#ifndef _ChargeBase_hh_
#define _ChargeBase_hh_

#include <Tranform/RealFunction.hh>
#include "Flavor.hh"

namespace QCD {
  
  /*!
    \brief a pure abstract class for Charge handling
  */
  class ChargeBase : public Transform::RealFunction {
  public:
    
    //! a destructor
    virtual ~ChargeBase(){}
    
    //! calculate f(x) at the given x
    virtual double operator() ( const double& x ) { return 0.0; };
    
    //! get function name 
    virtual std::string name() const { return "charge(flavor)"; } 
    
    /*!
      \param[in] q is a parton type
      \brief  return charge at present scale for the given parton type
    */
    virtual double operator()( const Flavor::PARTON& q ) = 0;

    //! for a process like W production ( flavor non-diagonal )
    virtual double operator()( const Flavor::PARTON& q1,  
                               const Flavor::PARTON& q2 ){ return 0.0; }

    //! get bare charge
    virtual double bare( const Flavor::PARTON& q ) = 0; 

    //! for a process like W production ( flavor non-diagonal )
    virtual double bare( const QCD::Flavor::PARTON& q1, 
                         const QCD::Flavor::PARTON& q2 ){ return 0.0; }

    //! get bare single charge
    virtual double single( const Flavor::PARTON& q ){ return 0.0; } 
    
  private:
    
  };
}

#endif //  _ChargeBase_hh_
