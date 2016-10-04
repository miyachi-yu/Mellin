// $Id: GD07P.cc,v 1.3 2008/05/20 00:44:19 miyachi Exp $
/*!
  \file    GD07P.cc
  \version $Revision: 1.3 $
  \date    $Date: 2008/05/20 00:44:19 $
  \author  $Author: miyachi $
  \brief   Implementation of GD07P photon absorption cross section parameterization
*/
#include "GD07P.hh"

using namespace std;
using namespace StrFncParam;

GD07P::GD07P() : 
  ALLM91()
{
  
  m02  =  0.454;
  mp2  = 30.7;
  mr2  =  0.117;
  q02  =  1.15;
  lm2  =  0.06527;
  
  ap[0] = -0.105;
  ap[1] = -0.495;
  ap[2] =  1.29;

  bp[0] = -1.42;
  bp[1] =  4.51;
  bp[2] =  0.551;

  cp[0] =  0.339;
  cp[1] =  0.127;
  cp[2] =  1.16;
  
  ar[0] =  0.374;
  ar[1] =  0.998;
  ar[2] =  0.745;
  
  br[0] =  2.71;
  br[1] =  1.83;
  br[2] =  1.26;
  
  cr[0] =  0.838;
  cr[1] =  2.36;
  cr[2] =  1.77;
  
}

GD07P::~GD07P() {
}

