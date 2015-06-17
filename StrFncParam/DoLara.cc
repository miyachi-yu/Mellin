// $Id: DoLara.cc,v 1.3 2008/05/20 00:44:19 miyachi Exp $
/*!
  \file    DoLara.cc
  \version $Revision: 1.3 $
  \date    $Date: 2008/05/20 00:44:19 $
  \author  $Author: miyachi $
  \brief   Implementation of DoLara photon absorption cross section parameterization
*/
#include "DoLara.hh"

using namespace std;
using namespace StrFncParam;

DoLara::DoLara() : 
  ALLM91()
{
  
  m02  =  0.454;
  mp2  = 30.7;
  mr2  =  0.118;
  q02  =  1.13;
  lm2  =  0.06527;
  
  ap[0] = -0.105;
  ap[1] = -0.496;
  ap[2] =  1.31;

  bp[0] = -1.43;
  bp[1] =  4.50;
  bp[2] =  0.554;

  cp[0] =  0.339;
  cp[1] =  0.128;
  cp[2] =  1.17;
  
  ar[0] =  0.373;
  ar[1] =  0.994;
  ar[2] =  0.781;
  
  br[0] =  2.70;
  br[1] =  1.83;
  br[2] =  1.26;
  
  cr[0] =  0.837;
  cr[1] =  2.34;
  cr[2] =  1.79;
  
}

DoLara::~DoLara() {
}

