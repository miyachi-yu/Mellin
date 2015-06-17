#ifndef _IntegXsec_Wilsons_hh_
#define _IntegXsec_Wilsons_hh_

#include "ConvIntegrand.hh"
#include <vector>

namespace IntegXsec {
  
  class Wilsons : 
    public std::vector< std::vector< std::vector< ConvIntegrand* > > > 
  {
  public:
    
    Wilsons();                         //!< a default constructor
    virtual ~Wilsons();                //!< a destructor
    
  };
  
};
#endif //  _IntegXsec_Wilsons_hh_
