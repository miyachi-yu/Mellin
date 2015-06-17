#include "CoeffFunc.hh"
#include "Charge.hh"
#include "WC.hh"

using namespace std;
using namespace Utility;
using namespace FragXsec_T;

CoeffFunc::CoeffFunc( Arguments& args ) :
  Xsection::CKernel( args )
{
  this->charge_     = new Charge( this );
  this->Nf_qfactor_ = 0.5; // = 1.0 / 2.0;
}

CoeffFunc::~CoeffFunc(){
  delete this->charge_;
}
