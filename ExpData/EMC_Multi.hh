#ifndef _EMC_Multi_hh_
#define _EMC_Multi_hh_

#include "DataSet.hh"

namespace Exp{
  namespace EMC_Multi {
    DataSet Multi( const Exp::TARGET& t, const Exp::HADRON& h ) throw( int );
  }
}

#endif //  _EMC_Multi_hh_
