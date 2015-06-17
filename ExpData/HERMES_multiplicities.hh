#ifndef _HERMES_multiplicities_hh_
#define _HERMES_multiplicities_hh_

#include "DataSet.hh"

namespace Exp {
  namespace HERMES_multiplicities {
    DataSet multiplicities( const Exp::TARGET& t = Exp::PRO, 
			    const Exp::HADRON& h = Exp::PIp ) throw( int );
  };
};

#endif // _HERMES_multiplicities_hh_
