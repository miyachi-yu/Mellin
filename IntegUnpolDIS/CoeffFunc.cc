#include "CoeffFunc.hh"

using namespace std;
using namespace Utility;
using namespace IntegUnpolDIS;

CoeffFunc::CoeffFunc( Arguments& args, IntegXsec::IntegXsecComp* xc, 
		      const IntegXsec::Charge::TARGET& n ) :
  IntegXsec::CKernel( args, xc )
{
  this->charge_     = new  IntegXsec::Charge( this, n );
  this->Nf_qfactor_ = 1.0;
}

CoeffFunc::~CoeffFunc(){
  delete this->charge_;
}

