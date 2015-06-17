/*!
  \file  F2ParamBase.hh
  \brief Definition of F2 Parameterization
  

*/
#ifndef _F2ParamBase_hh_
#define _F2ParamBase_hh_

#include <Tranform/RealFunction.hh>

namespace StrFncParam {
  
  /*!
    \class F2ParamBase  F2ParamBase.hh  "StrFncParam/F2ParamBase.hh"
    \brief class for F2 structure function parameterization
  */
  class F2ParamBase : public Transform::RealFunction {
  public:
    
    /*!
      \par [in] xsec is a photo-absorption cross section
      \par [in] r is a cross section ratio
    */
    F2ParamBase(); 
    virtual ~F2ParamBase();                           //!< a destructor
    
    /*!
      \par [in] x is Bjorken x
      \return   R(x)
    */
    virtual double operator() ( const double& x );
    
    /*!
      \par [in] x is Bjorken x
      \par [in] Q2 is a scaling parameter
      \return   R(x,Q2)
    */
    virtual double operator() ( const double& x, const double& Q2 );
    
    //! set q2 value
    virtual void q2( const double& v ) { q2_ = v; }
    
    virtual std::string name() const { return "F2(x,Q^2)"; } 
    
  protected:
    double q2_;
    
    virtual double eval( const double& x ) = 0;
    
  };
  
};
#endif // _F2ParamBase_hh_
