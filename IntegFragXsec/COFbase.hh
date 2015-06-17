#ifndef _IntegFragXsec_COFbase_hh_
#define _IntegFragXsec_COFbase_hh_

#include <IntegXsec/ConvIntegrand.hh>
#include <IntegXsec/IntegXsecComp.hh>

namespace IntegFragXsec {
  
  class COFbase : public IntegXsec::ConvIntegrand {
  public:
    COFbase( IntegXsec::IntegXsecComp* xc );
                 //!< a default constructor
    virtual ~COFbase();        //!< a destructor

    virtual double operator()( const double& x ){ return 0.0; }
    virtual double operator()( const double& x, const double& z )
    { return 0.0; }

    virtual std::complex< double > operator()
      ( const std::complex< double >& n )
    { return std::complex< double >( 0.0, 0.0 ); }
    virtual std::complex< double > operator()
      ( const std::complex< double >& n, const std::complex< double >& m )
    { return std::complex< double >( 0.0, 0.0 ); }
    
  };
};

#endif //  _IntegFragXsec_COFbase_hh_
