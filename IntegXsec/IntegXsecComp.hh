#ifndef _IntegXsec_IntegXsecComp_hh_
#define _IntegXsec_IntegXsecComp_hh_

#include <iostream>

#include <QCD/Flavor.hh>
#include <Evolution/KernelBase.hh>
#include <Xsec/XsecCompBase.hh>

namespace Xsection {

  class CKernelBase;

}

namespace IntegXsec {
  
  class IntegXsecComp : public Xsection::XsecCompBase {
    
  public:
    
    IntegXsecComp();
    IntegXsecComp( Xsection::XsecCompBase* xc );
    virtual ~IntegXsecComp();
    
    double q2(){ return q2_; } 
    void setQ2( const double& q2 );

    void setOrder( const QCD::Scheme::ORDER& order );
    QCD::Scheme::ORDER order();
    
    Xsection::CKernelBase* coeff()
    { return dynamic_cast< Xsection::XsecCompBase* >( this )->coeff(); } 
    //!< get a CKernelBase pointer  
    void coeff( Xsection::CKernelBase* ck );   //!< set a CKernelBase pointer

    std::vector< std::vector< Transform::RealFunction* > >& vpdfx()
    { return vpdfx_; }

    std::vector< std::vector< double > >& vCoeffr(); 
    
    std::vector< Evolution::KernelBase* >& vvevo(){ return xc_->vevo(); }

    virtual void update() ;
    
    std::vector< Transform::RealFunction* >& rescaleFcn() 
    { return rescaleFcn_; }
    
    bool insert( Evolution::KernelBase *kernel, 
		 const QCD::Flavor::HADRON& hadron = QCD::Flavor::PIp );

    friend std::ostream& operator<<( std::ostream& os, IntegXsecComp& xc );

    std::vector< std::vector< std::complex< double > > >& vparam()
    { return vparam_; }
    // return reference vector to be changed depending on integ range change!

    //  protected:
    virtual void globalScale( const double& v ) = 0;
    virtual double getglobalScale() = 0;

    Xsection::XsecCompBase* getvevoXC(){ return xc_; }    

  private:
    
    double q2_;
    std::vector< std::vector< Transform::RealFunction* > > vpdfx_;
    std::vector< Transform::RealFunction* > rescaleFcn_; 

    Xsection::XsecCompBase* xc_;

    std::vector< std::vector< std::complex< double > > > vparam_;

    std::vector< Transform::RealFunction* > localPdfs_;

    // params for xPDF calc
    double length_, offset_, angle_, precision_;
    int low_, high_;

  public:
    void setIMparam( const double& length, const double& offset, 
		     const double& angle )
    { length_ = length; offset_ = offset; angle_ = angle ; }
    void setIMIntegparam( const int& low, const int& high,
			  const double& precision )
    { low_ = low; high_ = high; precision_ = precision; }
    
  };
  
};

#endif //  _IntegXsec_IntegXsecComp_hh_
