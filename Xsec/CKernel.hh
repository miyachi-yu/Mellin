
// $Id: CKernel.hh,v 1.18 2009/04/16 10:31:32 imazu Exp $
/*!
  \file     CKernel.hh
  \version  $Revision: 1.18 $
  \date     $Date: 2009/04/16 10:31:32 $
  \author   $Author: imazu $
  \brief    class definition of .......
 */
#ifndef _Xsec_CKernel_hh_
#define _Xsec_CKernel_hh_

#include <iostream>

#include <Tranform/ComplexFunction.hh>
#include <Utility/Arguments.hh>
#include <QCD/ChargeBase.hh>
#include <QCD/Alpha.hh>
#include <QCD/Scheme.hh>
#include <Tranform/DataCache.hh>
#include <Tranform/DataCache2.hh>
#include "CKernelBase.hh"

namespace Evolution {
  class Kernel;
}

namespace Xsection {
  
  class Wilsons;
  
  /*!
    \class CKernl
    \brief This class manage Coefficient function ........
  */
  class CKernel: public CKernelBase {
    
  public:
    
    CKernel( Utility::Arguments& args = Utility::Arguments::ref() );
    //!< a constructor
    virtual ~CKernel();                       //!< a destructor

    QCD::ChargeBase* charge() { return charge_; }  //!< get ChargeBase pointer

    double nf(){ return Nf_; }               //!< get number of flavors
    
    void setQ2( const double& q2 );  //!< set hard scale Q2
    double q2(){ return q2_; }               //!< get Q2

    void constNf( const bool& frag ){ constNf_ = frag; } 

    QCD::Alpha& alpha() { return alpha_; }         
    //!< get a reference of Alpha
    
    // void wilsons( Wilsons* wc ) { cf_ = wc; } //!< set wilson coefficients
    // Wilsons* wilsons() { return cf_; }

    //! set moments n and m
    virtual void setMoment( const std::complex< double >& N );
    
    virtual void setMoment( const std::complex< double >& N, 
			    const std::complex< double >& M );
    
    void setNF_Qfac( const double& nqf ){ Nf_qfactor_ = nqf; }
    
    double alpha_pi_plus_1() { return alpha_pi_1_; }
    double alpha_pi_2() { return alpha_pi_2_; }
    
    std::vector< double >& vchar(){ return vchar_; };

    QCD::Scheme::ORDER getOrder(){ return order_; } //!< get order_

    friend std::ostream& operator<<( std::ostream& os, CKernel& ck );
    
  protected:

    QCD::ChargeBase*  charge_;    //!< parton charge calculator    
    double       Nf_qfactor_;     //!< factor to modify Nf
    
  private:

    double q2_;                   //!< hard scale     
    QCD::Alpha alpha_;            //!< value of alpha_s
    QCD::Scheme::ORDER order_;    //!< order of pQCD calculation
    double alpha_pi_2_;           //!< 0.5 * alpha( Q2 ) / M_PI
    double alpha_pi_1_;           //!< 1.0 + alpha( Q2 ) / M_PI
    double Nf_;

    bool constNf_;
    
    /*!
      \brief  update current value of coefficients
      
      This update value of coefficients needed for cross section calculation.

    */
    virtual void update();
    virtual void update2(){};
    
    //! charge information
    std::vector< double > vchar_;
    
    //! memory resident cache 
    Transform::DataCache cache_;
    Transform::DataCache2 cache2_;
    std::vector< std::complex< double > > cdata_;

  };

};

#endif //  _Xsec_CKernel_hh_
