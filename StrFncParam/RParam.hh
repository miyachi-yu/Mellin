// $Id: RParam.hh,v 1.4 2008/05/23 02:08:01 miyachi Exp $
/*!
  \file  RParam.hh
  \brief Definition of R structure function parameterization class
 */
#ifndef _RParam_hh_
#define _RParam_hh_

#include <Tranform/RealFunction.hh>

namespace StrFncParam {

  /*!
    \class RParam RParam.hh "StrFncParam/RParam.hh"
    \brief R structure function parameterization class
  */
  class RParam : public Transform::RealFunction {
  public:
    
    RParam();           //!< a default constructor
    virtual ~RParam();  //!< a destructor
    
    /*!
      \par [in] x is Bjorken x
      \return  R(x)
    */
    virtual double operator()( const double& x ) = 0 ;
    
    /*!
      \par [in] x is Bjorken x
      \par [in] q2 is QCD scaling variable
      \return  R(x,Q2)
    */
    virtual double operator()( const double& x, const double& q2 ) = 0;
    
    /*!
      \par [in] x is Bjorken x
      \return  dR(x)
    */
    virtual double error( const double& x ) = 0;
    
    /*!
      \par [in] x is Bjorken x
      \par [in] q2 is QCD scaling variable
      \return  dR(x,Q2)
    */
    virtual double error( const double& x, const double& q2 ) = 0;
    
  protected:
    double q2_;
    
  };
};

#endif // _RParam_hh_
