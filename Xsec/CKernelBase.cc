#include "CKernelBase.hh"
#include "Wilsons.hh"

#include <iostream>
#include <iomanip>
#include <cmath>

using namespace std;
using namespace Utility;
using namespace QCD;
using namespace Xsection;

CKernelBase::CKernelBase() : 
  cf_( NULL ), 
  M_( complex< double >( 2.0, 0.0 ) ), 
  N_( complex< double >( 2.0, 0.0 ) ),
  vcoeff_( 13, vector< complex< double> >
	   ( 13, ( complex< double > )( 0.0, 0.0 ) ) ),
  vcoeff2_( 13, vector< complex< double> >
	   ( 13, ( complex< double > )( 0.0, 0.0 ) ) ),
  vcoeff3_( 13, vector< complex< double> >
	   ( 13, ( complex< double > )( 0.0, 0.0 ) ) )
{
}

CKernelBase::~CKernelBase(){
}

