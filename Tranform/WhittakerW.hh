#ifndef _Transform_WhittakerW_hh_
#define _Transform_WhittakerW_hh_

#include "ComplexFunction.hh"
#include "ConfluentHG2.hh"

namespace Transform {
  class WhittakerW : public ComplexFunction {
  public:
    WhittakerW( const double& k, const double& m );
    virtual ~WhittakerW();
    
    virtual std::complex< double >
    operator()( const std::complex< double >& z );

    virtual void set( const double& k, const double& m );

  private:
    ConfluentHG2 U_;
    double k_;
    double m_;
    
  };
}

#endif // _Transform_WhittakerW_hh_
