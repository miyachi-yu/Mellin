#ifndef _IntegXsec_CKernel_hh_
#define _IntegXsec_CKernel_hh_

#include <iostream>

#include <Tranform/RealFunction.hh>
#include <Tranform/ComplexFunction.hh>
#include <Utility/Arguments.hh>
#include <QCD/ChargeBase.hh>
#include <QCD/Alpha.hh>
#include <QCD/Scheme.hh>
#include <Xsec/CKernelBase.hh>

namespace IntegXsec {
  
  class IntegXsecComp;
  class Wilsons;
  
  class CKernel: public Xsection::CKernelBase {
    
  public:
    
    CKernel( Utility::Arguments& args, IntegXsec::IntegXsecComp* xc );
    virtual ~CKernel();
    
    double nf() { return Nf_; }
    QCD::ChargeBase* charge() { return charge_; }
    
    virtual void setQ2( const double& q2 );
    double q2() { return q2_; }

    void constNf( const bool& frag ){ constNf_ = frag; } 

    void setpp_judge( bool judge ){ pp_judge_ = judge; }
    bool getpp_judge(){ return pp_judge_ ; }
    
    QCD::Alpha& alpha() { return alpha_; }
    
    void setMoment( const double& X );    
    void setMoment( const double& X, const double& Z );

    void setMoment( const std::complex< double >& N );    
    void setMoment( const std::complex< double >& N, 
		    const std::complex< double >& M );
    
    void setNF_Qfac( const double& nqf ){ Nf_qfactor_ = nqf; }
    
    std::vector< std::vector< double > >& vCoeffr()
    { return vcoeffr_; }
    
    double alpha_pi_plus_1() { return alpha_pi_1_; }
    double alpha_pi_2() { return alpha_pi_2_; }
    
    friend std::ostream& operator<<( std::ostream& os, CKernel& ck );
    
    std::vector< double >& vchar(){ return vchar_; };
    std::vector< std::vector< double > >& vvchar(){ return vvchar_; };

    IntegXsecComp* getIntXsecC(){ return xc_; }
    double getX(){ return X_; }
    double getZ(){ return Z_; }

    QCD::Scheme::ORDER getOrder(){ return order_; }

    void setflavors( const int& i, const int& j ){ i_ = i; j_ = j; }

    void wilsons( IntegXsec::Wilsons* wc ) { cf_ = wc; } 
    //!< set wilson coefficients
    IntegXsec::Wilsons* wilsons() { return cf_; }

  protected:

    void setQ2Vir( const double& q2 );
    //!< 2 step setQ2 function for vector boson production

    QCD::ChargeBase*  charge_;
    double       Nf_qfactor_; 

    int i_; int j_;

  private:

    virtual void update();  // for integ calc
    virtual void updatec(); // for complex calc
    virtual void update2(){};

  private:
    
    IntegXsecComp* xc_;

    double q2_;               
    QCD::Alpha alpha_;        
    QCD::Scheme::ORDER order_;
    double Nf_;
    double alpha_pi_2_;           //!< 0.5 * alpha( Q2 ) / M_PI
    double alpha_pi_1_;           //!< 1.0 + alpha( Q2 ) / M_PI

    bool constNf_;
    bool pp_judge_;
    
    double X_;    //!< moment 1
    double Z_;    //!< momnet 2
    
    std::vector< std::vector< double > > vcoeffr_;
    
    IntegXsec::Wilsons*     cf_;   //!< wilson coefficients collection

    std::vector< double > vchar_;
    std::vector< std::vector< double > > vvchar_;
    
  };

};

#endif //  _IntegXsec_CKernel_hh_
