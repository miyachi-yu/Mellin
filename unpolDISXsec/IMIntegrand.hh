#ifndef _unpolDISXsec_IMIntegrand_hh_
#define _unpolDISXsec_IMIntegrand_hh_

#include <Tranform/InverseMellinIntegrand.hh>
#include <Tranform/InverseMellin.hh>

namespace Transform{
  class InverseMellin;
}

namespace unpolDISXsec {

    class IMIntegrand : public Transform::InverseMellinIntegrand {
    public:
      
      IMIntegrand( Transform::ComplexFunction *f );  //!< a constructor
      virtual ~IMIntegrand(){};                      //!< a destructor
      
      virtual 
      std::complex< double > operator()( const std::complex< double >& n );
      
      virtual 
      std::complex< double > operator()( const std::complex< double >& n,
					 const std::complex< double >& m );

      IMIntegrand& setxmom( const bool& xmom ){ xmom_ = xmom; return *this; }
      bool getxmom(){ return xmom_; }

      IMIntegrand& setzmom( const bool& zmom ){ zmom_ = zmom; return *this; }
      bool getzmom(){ return zmom_; }
      
      IMIntegrand& setxzmom( const bool& xmom, const bool& zmom )
      { xmom_ = xmom; zmom_ = zmom; return *this; }

    private:
      
      bool xmom_;
      bool zmom_;

    };

};
#endif // _unpolDISXsec_xXsec_hh_
