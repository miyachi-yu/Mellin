#include "CoeffFunc.hh"

using namespace std;
using namespace polDIS;

CoeffFunc::CoeffFunc( Utility::Arguments& args,
		      const Xsection::Charge::TARGET& n ) :
  CKernel( args )
{
  this->charge_     = new Xsection::Charge( this, n );
  this->Nf_qfactor_ = 1.0;
}

CoeffFunc::~CoeffFunc(){
  delete this->charge_;
}

