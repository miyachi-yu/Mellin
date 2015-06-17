#ifndef _unpolDISXsec_CoeffFunc_hh_
#define _unpolDISXsec_CoeffFunc_hh_

#include <Utility/Arguments.hh>
#include <Xsec/Charge.hh>
#include <Xsec/CKernel.hh>
#include <Tranform/DataCache.hh>
#include <Tranform/DataCache2.hh>

namespace Xsection{
  class Wilsons;
}

namespace unpolDISXsec {
  
  class CoeffFunc: public Xsection::CKernel {
  public:
    
    CoeffFunc( Utility::Arguments& args, 
	       const Xsection::Charge::TARGET& n );
    virtual ~CoeffFunc();

    double getXsecfact(){ return Xsecfact_; }

    void setS( const double& s ){ S_ = s; }
    void setX( const double& x ){ X_ = x; }
    void setX1X2( const double& x1, const double& x2 ) throw( int );

    void wilsons2( Xsection::Wilsons* wc ) { cf_2 = wc; }
    Xsection::Wilsons* wilsons2() { return cf_2; }

    void setDirect( const bool& dir ){ direct_ = dir; }
    bool getDirect(){ return direct_; }

    void setXmom( const bool& xmom ){ xmom_ = xmom; }
    bool& getXmom(){ return xmom_; }

    void setZmom( const bool& zmom ){ zmom_ = zmom; }
    bool& getZmom(){ return zmom_; }

    void setXZmom( const bool& xmom, const bool& zmom )
    { xmom_ = xmom; zmom_ = zmom; }

   //! set moments n and m ( overload for cache manipulation )
    virtual void setMoment( const std::complex< double >& N );
    
    virtual void setMoment( const std::complex< double >& N, 
                            const std::complex< double >& M );

    void usecc( const int& i );
    void usecc();

  private:
    virtual void update();
    virtual void update2();

    void applyY();

    std::complex< double > factor( const double& order );

    Xsection::Wilsons* cf_2;

    double Xsecfact_;    

    //! Xsec parameters
    double X_; //! bjorken x
    double S_; //! mandelstam s ( GeV^2 )

    double x1_; //! momoent param
    double x2_; //! momoent param x2 > x1

    bool direct_;
    bool xmom_;
    bool zmom_;

    //! for cache manipulation
    Transform::DataCache xcache_1_;
    Transform::DataCache xcache_2_;
    Transform::DataCache xcache_3_;
    Transform::DataCache2 xcache2_1_;
    Transform::DataCache2 xcache2_2_;
    Transform::DataCache2 xcache2_3_;
    std::vector< std::complex< double > > cdata_1_;
    std::vector< std::complex< double > > cdata_2_;
    std::vector< std::complex< double > > cdata_3_;

    std::vector< bool > usecc_;

  };
  
};

#endif //  _unpolDISXsec_CoeffFunc_hh_
