// $Id: ALLM91.hh,v 1.3 2008/07/18 01:36:45 miyachi Exp $
/*!
  \file    ALLM91.hh
  \version $Revision: 1.3 $
  \date    $Date: 2008/07/18 01:36:45 $
  \author  $Author: miyachi $
  \brief   Definition of ALLM91 photon absorption cross section parameterization
*/
#ifndef _ALLM91_hh_
#define _ALLM91_hh_

#include "PhotoAbsorbXsec.hh"
#include <vector>

namespace StrFncParam {
  
  /*!
    \class ALLM91  ALLM91.hh "StrFncParam/ALLM91.hh"
    \brief class for ALLM91 photon absorption cross section parameterization
    
    ALLM91 virtual photon absorption cross section is implemented in ALLM91 class.
    The parameterization can be found in 
    "A Parameterization of sigma-T (gamma* p) above the resonance region Q**2>=0."
    , H. Abramowicz, E.M. Levin, and A. Levy, Phys. Lett. B465 (1991) 465-476.
  */
  class ALLM91 : public PhotoAbsorbXsec {
  public:
    
    ALLM91();                             //!< a default constructor
    virtual ~ALLM91();                    //!< a destuctor
    
    /*!
      \param[in] W2 is a square of center of mass energy
      \brief   return sigma(gamma* p)( W2 )
    */
    virtual double operator() ( const double& W2 ) ; 
    
    /*!
      \param[in] W2 is a square of center of mass energy
      \param[in] Q2 is a QCD scale
      \brief  return sigma(gamma* p)( W2, Q2 )
    */
    virtual double operator() ( const double& W2, const double& Q2 );
    
    //! get function name 
    virtual std::string name() const { return "ALLM91(W2,Q2)"; } 
    
    virtual double t();
    
  protected:
    
    class C1 : public RealFunction, public std::vector< double > {
    public:
      C1( ) ;
      virtual ~C1();
      virtual double operator() ( const double& t );
    };
    
    class C2 : public RealFunction, public std::vector< double > {
    public:
      C2( );
      virtual ~C2();
      virtual double operator() ( const double& t );
    };
    
    double m02;           //!< m_0^2( GeV^2 )
    double mp2;           //!< m_P^2( GeV^2 )
    double mr2;           //!< m_R^2( GeV^2 )
    double q02;           //!< Q_0^2( GeV^2 )
    double lm2;           //!< Lambda^2( GeV^2 )
    
    C1 cr;
    C1 br;
    C1 ar;
    
    C1 bp;
    C2 cp;
    C2 ap;
    
  protected:
    
    double t_;
    
  };

};

#endif // _ALLM91_hh_
