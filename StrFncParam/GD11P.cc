// $Id: GD11P.cc,v 1.3 2008/05/20 00:44:19 miyachi Exp $
/*!
  \file    GD11P.cc
  \version $Revision: 1.3 $
  \date    $Date: 2008/05/20 00:44:19 $
  \author  $Author: miyachi $
  \brief   Implementation of GD11P photon absorption cross section parameterization
*/
#include "GD11P.hh"

using namespace std;
using namespace StrFncParam;

GD11P::GD11P() : 
  ALLM91()
{
  
  m02  =  0.5063;
  mp2  = 34.75;
  mr2  =  0.03190;
  q02  =  1.374;
  lm2  =  0.06527;
  
  ap[0] = -0.11895;
  ap[1] = -0.4783;
  ap[2] =  1.353;

  bp[0] = -1.0833;
  bp[1] =  2.656;
  bp[2] =  1.771;

  cp[0] =  0.3638;
  cp[1] =  0.1211;
  cp[2] =  1.166;
  
  ar[0] =  0.3425;
  ar[1] =  1.0603;
  ar[2] =  0.5164;
  
  br[0] =-10.408;
  br[1] = 14.857;
  br[2] =  0.07739;
  
  cr[0] =  1.3633;
  cr[1] =  2.256;
  cr[2] =  2.209;
  
}

GD11P::~GD11P() {
}


