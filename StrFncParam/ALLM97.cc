// $Id: ALLM97.cc,v 1.3 2008/05/20 00:44:19 miyachi Exp $
/*!
  \file    ALLM97.cc
  \version $Revision: 1.3 $
  \date    $Date: 2008/05/20 00:44:19 $
  \author  $Author: miyachi $
  \brief   Implementation of ALLM97 photon absorption cross section parameterization
*/
#include "ALLM97.hh"

using namespace std;
using namespace StrFncParam;

ALLM97::ALLM97() : 
  ALLM91()
{
  
  m02  =  0.31985;
  mp2  = 49.457;
  mr2  =  0.15052;
  q02  =  0.52544;
  lm2  =  0.06527;
  
  cp[0] =  0.28067;
  cp[1] =  0.22291;
  cp[2] =  2.1979;
  
  ap[0] = -0.0808;
  ap[1] = -0.44812;
  ap[2] =  1.1709;

  bp[0] =  0.36292;
  bp[1] =  1.8917;
  bp[2] =  1.8439;

  cr[0] =  0.80107;
  cr[1] =  0.97307;
  cr[2] =  3.4942;
  
  ar[0] =  0.58400;
  ar[1] =  0.37888;
  ar[2] =  2.6063;
  
  br[0] =  0.01147;
  br[1] =  3.7582;
  br[2] =  0.49338;
  
}

ALLM97::~ALLM97() {
}

