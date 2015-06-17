#include "CoeffFunc.hh"
#include "Charge.hh"

using namespace std;
using namespace Utility;
using namespace IntegFragXsec;

CoeffFunc::CoeffFunc( Arguments& args, IntegXsec::IntegXsecComp* xc ) :
  IntegXsec::CKernel( args, xc )
{
  this->charge_     = new Charge( this );
  this->Nf_qfactor_ = 0.5; // = 1.0 / 2.0;
}

CoeffFunc::~CoeffFunc(){
  delete this->charge_;
}

