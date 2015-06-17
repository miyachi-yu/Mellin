// $Id: F2nSMC.hh,v 1.2 2008/05/20 02:25:33 miyachi Exp $
/*!
  \file   F2nSMC.hh
  \brief  class definition of F2n SMC parameterization
 */
#ifndef _F2nSMC_hh_
#define _F2nSMC_hh_

#include "F2pSMC.hh"
#include "F2dSMC.hh"

namespace StrFncParam {

  /*!
    \class F2nSMC
    \brief F2n SMC parameterization class
  */
  class F2nSMC : public F2ParamBase {
  public:
    
    F2nSMC();          //!< a default constructor
    virtual ~F2nSMC(); //!< a destructor
    
    //! evaluate F2 value with the given x and current Q2
    virtual double eval( const double& x );
    
    //! set q2 value
    virtual void q2( const double& v );
    
  private:
    
    F2pSMC f2p_;
    F2dSMC f2d_;
    
  };

};
#endif // _F2nSMC_hh_

