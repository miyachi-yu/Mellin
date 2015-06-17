#ifndef _polSIDIS_COFbase_hh_
#define _polSIDIS_COFbase_hh_

#include <Xsec/Coefficient.hh>

namespace polSIDIS {
  
  class COFbase : public Xsection::Coefficient {
  public:
    COFbase();
    virtual ~COFbase();
    
    virtual std::complex< double >
    operator()( const std::complex< double >& z ){ 
      return (*this)( z, std::complex< double >( 0.0, 0.0 ) );
    };
    
    virtual std::complex< double >
    operator()( const std::complex< double >& m,
		const std::complex< double >& n ) = 0;
    
  protected:
    
  };
  
};


#endif //  _polSIDIS_COFbase_hh_
