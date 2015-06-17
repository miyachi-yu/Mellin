#ifndef _IntegXsec_XsecIntegrand_hh_
#define _IntegXsec_XsecIntegrand_hh_

#include <Tranform/RealFunction.hh>
#include <QCD/Flavor.hh>
#include <complex>
#include "IntegXsecComp.hh"

namespace IntegXsec{
  
  class XsecIntegrand : public IntegXsecComp, public Transform::RealFunction,
			public Transform::ComplexFunction {
  public:
    XsecIntegrand();
    XsecIntegrand( Xsection::XsecCompBase* xc );
    virtual ~XsecIntegrand();    
    
    virtual double operator()( const double& x );
    virtual double operator()( const double& x, const double& z );
    virtual double operator()
    ( const double& x, const QCD::Flavor::PARTON& q );

    std::complex< double > operator()( const std::complex< double >& n );
    std::complex< double > operator()( const std::complex< double >& n, 
				       const std::complex< double >& m );

    void setIndiv( const bool& status = true ) { indiv_ = status; }
    void setIndiv( const QCD::Flavor::PARTON& q )
    { indiv_ = true; qtype_ = q; };

    //  protected:
    void globalScale( const double& v ) { gscale_ = v; } 
    double getglobalScale() { return gscale_; } 
    
    void setflavors( const int& i, const int& j );

  private:

    double gscale_;
    bool indiv_;
    QCD::Flavor::PARTON qtype_;

    int i_; int j_;
    
  };
};

#endif //  _IntegXsec_XsecIntegrand_hh_



