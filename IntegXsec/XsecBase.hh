#ifndef _IntegXsec_XsecBase_hh_
#define _IntegXsec_XsecBase_hh_

#include <Tranform/RealFunction.hh>
#include <Tranform/IntegFunc.hh>
#include <Tranform/DataCacheInteg.hh>
#include <Tranform/DataCacheInteg2.hh>
#include <QCD/Scheme.hh>
#include <QCD/Flavor.hh>
#include <Evolution/PDFx.hh>
#include <Xsec/CKernelBase.hh>

namespace IntegXsec {
  
  class XsecIntegrand;
  
  class XsecBase: public Xsection::CKernelBase, 
		  public Transform::RealFunction {
  public:
    
    XsecBase();
    virtual ~XsecBase(){};

    virtual double operator()( const double& x, const double& z ) = 0;    
    virtual double operator()( const double& x ) = 0;
  
    void xsecInteg( XsecIntegrand* XC ) { XInteg_ = XC; }     
    XsecIntegrand* xsecInteg() { return XInteg_; }
    
    //! set moments n and m
    virtual void setMoment( const std::complex< double >& N ) = 0;
    
    virtual void setMoment( const std::complex< double >& N, 
                            const std::complex< double >& M ) = 0;

    void setQ2( const double& q2 );
    double q2();

    double nf();
    QCD::ChargeBase* charge();

    void constNf( const bool& frag );

    QCD::Alpha& alpha();
    
    void setOrder( const QCD::Scheme::ORDER& order );
    QCD::Scheme::ORDER order();
    //    QCD::Scheme::ORDER& order();
    
    virtual void update();
    
    friend std::ostream& operator<<( std::ostream& os, XsecBase& xs );
    
    void setIndiv( const QCD::Flavor::PARTON& q );
    void setIndiv( const bool& status = true );

    int getncount(){ return ncount_; }

  protected:

    int ncount_;

    double X_;    //!< kinematic x
    double Z_;    //!< kinematic z

    int acti_;
    int actj_; 
    std::map< int, int > actwc1_;
    std::map< int, std::map< int, int > > actwc2_;
    // for proper & efficient CKernel calc in n space kernel

    std::vector< std::vector< Transform::RealFunction* > >& vpdfx();

    //! memory resident cache 
    //! stock non zero components only
    Transform::DataCacheInteg  cache_;
    Transform::DataCacheInteg2  cache2_;
    std::vector< std::complex< double > > cdata_;
    
    virtual int getWCdim1(){ return 2; }; // ( qq, gg ) for 1 dim like DIS
    virtual int getWCdim2(){ return 3; };// ( qq, gq, qg ) for 1 dim like SIDIS

    virtual bool setActflav( const int& i, const int& j ) = 0;
    // get proper & efficient flavors for CKernel calc ( acti_, actj )

  private:

    //!< a pointer to XsecIntegrand object
    XsecIntegrand* XInteg_;  

    bool constNf_;

  };
  
};

#endif //  _IntegXsec_XsecBase_hh_
