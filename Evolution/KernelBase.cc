#include "KernelBase.hh"

#include <iostream>
#include <Utility/Arguments.hh>

using namespace std;
using namespace Utility;
using namespace Transform;
using namespace QCD;
using namespace Evolution;

KernelBase::KernelBase() :
  alpha_( Arguments::ref() ),
  cache_(), 
  cdata_( 0 ),
  n_( 2.0, 0.0 ),
  calcerr_( false )
{
}

KernelBase::~KernelBase() {
}

void KernelBase::setOrder( const Scheme::ORDER& order ){
  if( this->alpha().scheme().order() == order ) return ;
  this->alpha().scheme().order() = order ;
  cache_.clear();
}

namespace Evolution {
  
  ostream& operator<<( ostream& os, KernelBase& k ){
    
    os << "*******************************************************" << endl;
    os << "*           KernelBase object information             *" << endl;
    os << "*******************************************************" << endl;
    
    return os;
  }
}
