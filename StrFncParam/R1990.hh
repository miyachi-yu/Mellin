// $Id: R1990.hh,v 1.2 2008/05/20 00:44:19 miyachi Exp $
/*!
  \file    R1990.hh
  \brief   Class definition of R1990 parameterization
 */
#ifndef _R1990_hh_
#define _R1990_hh_

#include "RParam.hh"
#include <vector>

namespace StrFncParam {
  
  /*!
    \class R1990 R1990.hh "StrFncParam/R19909.hh"
    \brief class for R structure function parameterization (R1990)
    
    This is implementation of R1990 parameterization, 
    L. W. Whitlow, Phys. Lett. B250 (1990) 193.
    
  */
  class R1990 : public RParam {
  public:
    R1990();                 //!< a default constructor
    virtual ~R1990();        //!< a destructor
    
    /*!
      \par [in] x is Bjorken x
      \return  R(x)
    */
    virtual double operator()( const double& x );
    
    /*!
      \par [in] x is Bjorken x
      \par [in] q2 is QCD scaling variable
      \return  R(x,Q2)
    */
    virtual double operator()( const double& x, const double& q2 );
    
    /*!
      \par [in] x is Bjorken x
      \return  dR(x)
    */
    virtual double error( const double& x );
    
    /*!
      \par [in] x is Bjorken x
      \par [in] q2 is QCD scaling variable
      \return  dR(x,Q2)
    */
    virtual double error( const double& x, const double& q2 );
    
    //!< evaluation of R1990 parameterization
    virtual double eval( const double& x );
    
    //!< uncertainty evaluation of R1990 parameterization
    virtual double evalError( const double& x );
    
  protected:
    double theta( const double& x );  //!< Theta(x,Q2) function
    double lm2;                       //!< squre of Lambda
    std::vector< double > b;                      //!< fit parameters
    
  };

};

#endif // _R1990_hh_
