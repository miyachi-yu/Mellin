#ifndef _IntegPolDrellYan_DYDPlusFunc_hh_
#define _IntegPolDrellYan_DYDPlusFunc_hh_

#include <IntegUnpolDrellYan/DYDPlusFunc.hh>

namespace IntegPolDrellYan{

  class DYDPlusFunc : public IntegUnpolDrellYan::DYDPlusFunc {
  public:
    DYDPlusFunc( std::vector< IntegXsec::ConvIntegPDF* > xpdf, 
		 const std::vector< std::vector< std::complex< double > > >& 
		 param ) : IntegUnpolDrellYan::DYDPlusFunc( xpdf, param ){};
    ~DYDPlusFunc(){};
    
  };
};

#endif // _IntegPolDrellYan_DYDPlusFunc_hh_
