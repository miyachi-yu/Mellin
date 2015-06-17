#ifndef _HERMES_MultiQ2_hh_
#define _HERMES_MultiQ2_hh_

#include "DataSet.hh"

namespace Exp{
  namespace HERMES_MultiQ2 {
    
    std::vector< double > getkinev( const int& had, const double& zmin,
				    const double& zmax,
				    const double& q2min,
				    const double& q2max, const bool& zMulti = false );
    
    std::vector< double > getx( const double& q2 );
    
    DataSet Multi( const Exp::TARGET& t, const Exp::HADRON& h ) throw( int );
  }
}

#endif //  _HERMES_MultiQ2_hh_
