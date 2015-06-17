// $Id: F1Param.hh,v 1.4 2008/05/23 02:08:01 miyachi Exp $
/*!
  \file  F1Param.hh
  \brief Class definition of parameterization of F1 structure function
 */
#ifndef _F1Param_hh_
#define _F1Param_hh_

#include <Tranform/RealFunction.hh>

#include "PhotoAbsorbXsec.hh"
#include "F2ParamBase.hh"
#include "RParam.hh"

namespace StrFncParam {

  /*!
    \class F1Param  F1Param.hh  "StrFncParam/F1Param.hh"
    \brief class for F1 structure function parameterization
    
    Instead of use parameterization of F2 and R, parameterization of
    transvers component of photo-absorption cross section, sigmaT, is used.
    The relation of F1 and sigmaT is given as
    
    sigmaT = (4 pi alpha (hbar c)^2 ) / Q^2 / ( 1 - x ) * 2x * F1
    
  */
  class F1Param : public Transform::RealFunction {
  public:
    
    /*!
      \par [in] xsec is transverse component of photo-absorption cross section
      \brief a constructor
    */
    F1Param( PhotoAbsorbXsec& xsec );
    
    //! construct with F2 and R parameterization
    F1Param( F2ParamBase& f2, RParam& r );
    
    virtual ~F1Param(); //!< a destructor
    
    /*!
      \par [in] x is a Bjorken x
      \return F1( x )
    */
    virtual double operator()( const double& x );
    
    /*!
      \par [in] x is a Bjorken x
      \par [in] q2 is a QCD scaling variable
      \return F1( x, q2 )
    */
    virtual double operator()( const double& x, const double& q2 );
    
    virtual std::string name() const { return "F1(x,Q^2)"; } 
    
  protected:
    PhotoAbsorbXsec* sigmaT_;
    F2ParamBase *f2_;
    RParam *r_;
    double q2_;
    
  };
  
};

#endif // _F1Param_hh_
