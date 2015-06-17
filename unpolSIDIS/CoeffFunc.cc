#include "CoeffFunc.hh"

#include "Charge.hh"

using namespace std;
using namespace Utility;
using namespace unpolSIDIS;

CoeffFunc::CoeffFunc( Arguments& args, 
		      const Xsection::Charge::TARGET& n ) :
  CKernel( args )
{
  this->charge_     = new Charge( this, n );
  this->Nf_qfactor_ = 1.0;
}

CoeffFunc::~CoeffFunc(){
  delete this->charge_;
}

