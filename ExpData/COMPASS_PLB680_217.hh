#ifndef _COMPASS_PLB680_217_hh_
#define _COMPASS_PLB680_217_hh_

#include "DataSet.hh"

namespace Exp{
  namespace COMPASS_PLB680_217 {
    DataSet A1( const Exp::TARGET& t ) throw( int );
    DataSet A1h( const Exp::TARGET& t, const Exp::HADRON& h ) throw( int );
  }
}

#endif //  _COMPASS_PLB680_217_hh_
