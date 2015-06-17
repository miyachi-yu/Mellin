// $Id: F2pSMC.hh,v 1.2 2008/05/20 00:44:19 miyachi Exp $
/*!
  \file    F2pSMC.hh
  \brief   class implementation for F2p SMC parameterization
 */
#ifndef _F2pSMC_hh_
#define _F2pSMC_hh_

#include "F2ParamBase.hh"
#include <vector>

namespace StrFncParam {
  
  /*!
    \class  F2p SMC parameterization
  */
  class F2pSMC : public F2ParamBase {
  public:
    
  F2pSMC();               //!< a default constructor
    virtual ~F2pSMC();      //!< a destructor
    
  protected:
  
    double lm2;
    double q02;

    std::vector< double > a;
    std::vector< double > b;
    std::vector< double > c;
    
    std::vector< double > au;
    std::vector< double > bu;
    std::vector< double > cu;
    
    std::vector< double > ad;
    std::vector< double > bd;
    std::vector< double > cd;
    
    virtual double eval( const double& x );
    
  private:
    
    double A( const double& x );
    double B( const double& x );
    double C( const double& x );
    
  };
};
#endif // _F2SMC_hh_
