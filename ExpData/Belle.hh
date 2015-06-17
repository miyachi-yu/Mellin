#ifndef _Belle_hh_
#define _Belle_hh_

#include "DataSet.hh"

namespace Exp {
  namespace Belle {
    DataSet pion( const Exp::HADRON& h = Exp::PIpm ) throw( int );
    DataSet kaon( const Exp::HADRON& h = Exp::Kpm ) throw( int );
  };
};

#endif // _Belle_hh_
