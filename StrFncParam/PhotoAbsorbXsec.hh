// $Id: PhotoAbsorbXsec.hh,v 1.3 2008/05/23 02:08:01 miyachi Exp $
/*!
  \file  PhotoAbsorbXsec.hh
  \brief Definition of photo absorption cross section sigma_T class
 */
#ifndef _PhotoAbsorbXsec_hh_
#define _PhotoAbsorbXsec_hh_

#include <Tranform/RealFunction.hh>

namespace StrFncParam {
  
  /*!
    \class PhotoAbsorbXsec PhotoAbsorbXsec.hh "StrFncParam/PhotoAbsorbXsec.hh"
    \brief Photo-absorption cross section sigma_T implementation
    
    This is a base class for implementation of virtual/real photon absorption
    cross section, sigma_T( gamma* p ).
    
  */
  class PhotoAbsorbXsec : public Transform::RealFunction {
  public:
    
    static double FineConstant; //!< 4 pi^2 alpha * (hbar c)^2
    static double Mp2;          //!< nucleon mass sqare
    
    PhotoAbsorbXsec();
    virtual ~PhotoAbsorbXsec();
    
    /*!
      \param[in] W2 is a square of center of mass energy
      \brief   return sigma(gamma* p)( W2 )
    */
    virtual double operator() ( const double& W2 ) = 0 ; 
    
    /*!
      \param[in] W2 is a square of center of mass energy
      \param[in] Q2 is a QCD scale
      \brief  return sigma(gamma* p)( W2, Q2 )
    */
    virtual double operator() ( const double& W2, const double& Q2 ) = 0 ;

    //! calculate W2 from x
    virtual double W2( const double& x );
    
    //! calculate x from W2 and mass
    virtual double x( const double& W2, const double& m2 = 0.0 ) ;
    
  protected:
    double q2_;
    
  };
};

#endif //  _PhotoAbsorbXsec_hh_
