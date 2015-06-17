#include "CoeffFunc.hh"
#include <unpolSIDIS/Charge.hh>

using namespace std;
using namespace polSIDIS;

CoeffFunc::CoeffFunc( Utility::Arguments& args, 
		      const Xsection::Charge::TARGET& n ) :
  CKernel( args )
{
  this->charge_     = new unpolSIDIS::Charge( this, n );
  this->Nf_qfactor_ = 1.0;
}

CoeffFunc::~CoeffFunc(){
  delete this->charge_;
}

