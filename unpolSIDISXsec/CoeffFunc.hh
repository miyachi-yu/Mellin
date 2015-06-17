#ifndef _unpolSIDISXsec_CoeffFunc_hh_
#define _unpolSIDISXsec_CoeffFunc_hh_

#include <Utility/Arguments.hh>
#include <Xsec/Charge.hh>
#include <Xsec/CKernel.hh>
#include <Tranform/DataCache.hh>
#include <Tranform/DataCache2.hh>

namespace Xsection{
  class Wilsons;
}

namespace unpolSIDISXsec {
  
  class CoeffFunc: public Xsection::CKernel {
  public:
    
    CoeffFunc( Utility::Arguments& args, 
	       const Xsection::Charge::TARGET& n );
    virtual ~CoeffFunc();

    double getXsecfact(){ return Xsecfact_; }

    void setS( const double& s ){ S_ = s; }
    void setX( const double& x ){ X_ = x; }

    void wilsons2( Xsection::Wilsons* wc ) { cf_2 = wc; }
    Xsection::Wilsons* wilsons2() { return cf_2; }

    void setDirect( const bool& dir ){ direct_ = dir; }
    bool getDirect(){ return direct_; }

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

    Xsection::Wilsons* cf_2;

    double Xsecfact_;    

    //! Xsec parameters
    double X_; //! bjorken x
    double S_; //! mandelstam s ( GeV^2 )

    bool direct_;

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

#endif //  _unpolSIDISXsec_CoeffFunc_hh_
