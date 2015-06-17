/*!
  \file  F2Param.hh
  \brief Definition of F2 Parameterization
  

*/
#ifndef _F2Param_hh_
#define _F2Param_hh_

#include "F2ParamBase.hh"
#include "PhotoAbsorbXsec.hh"
#include "RParam.hh"

namespace StrFncParam {
  /*!
    \class F2Param  F2Param.hh  "StrFncParam/F2Param.hh"
    \brief class for F2 structure function parameterization
    
    Instead of parameterize F2 itself, it uses parameterization of
    transvers component of photo-absorption cross section, sigma_T
    and parameterization of cross section ration between longitudinal
    and transverse component.
    
  */
  class F2Param : public F2ParamBase {
  public:
    
    /*!
      \par [in] xsec is a photo-absorption cross section
      \par [in] r is a cross section ratio
    */
    F2Param( PhotoAbsorbXsec& xsec, RParam& r ); 
    
    virtual ~F2Param();                           //!< a destructor
    
  protected:
    
    virtual double eval( const double& x );
    
  private:
    PhotoAbsorbXsec& sigmaT_;
    RParam& r_;
    
  };
  
};

#endif // _F2Param_hh_
