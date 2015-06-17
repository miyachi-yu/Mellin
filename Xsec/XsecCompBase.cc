#include "XsecCompBase.hh"
#include "CKernelBase.hh"

#include <iomanip>
#include <cstdlib>

using namespace std;
using namespace Transform;
using namespace QCD;
using namespace Evolution;
using namespace Xsection;

XsecCompBase::XsecCompBase() : vevo_( 0 ), vpdf_( 0 ), coeff_( NULL ){
}

XsecCompBase::~XsecCompBase(){}

