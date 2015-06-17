#ifndef _Colors_hh_
#define _Colors_hh_

#include <QCD/Flavor.hh>

namespace  Colors {
  int id( const QCD::Flavor::PARTON& p );
  int id( const QCD::Flavor::TYPE& p );
};

#endif // _Colors_hh_
