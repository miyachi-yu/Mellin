// $Id: MRST2001.cc,v 1.2 2009/02/04 08:24:24 miyachi Exp $
/*!
  \file      MRST2001.cc
  \version   $Revison:$
  \author    $Author: miyachi $
  \date      $Date: 2009/02/04 08:24:24 $
  \brief     Implementation of MRST2001 interface class 

 */
#include "MRST2001.hh"

#include <cmath>
#include <iostream>
#include <iomanip>

extern "C" {
#include "cfortran.h"
#include <unistd.h>
  
//! macro for MRST2001 fortran routine
PROTOCCALLSFFUN11(DOUBLE,MRST2001E,mrst2001e,DOUBLE,DOUBLE,INT,PDOUBLE,PDOUBLE,PDOUBLE,PDOUBLE,PDOUBLE,PDOUBLE,PDOUBLE,PDOUBLE)
#define MRST2001E(x,q,n,upv,dnv,usea,dsea,str,chm,bot,glu)  CCALLSFFUN11(MRST2001E,mrst2001e,DOUBLE,DOUBLE,INT,PDOUBLE,PDOUBLE,PDOUBLE,PDOUBLE,PDOUBLE,PDOUBLE,PDOUBLE,PDOUBLE,x,q,n,upv,dnv,usea,dsea,str,chm,bot,glu)
}

using namespace std;
using namespace QCD;
using namespace PDFLIB;

MRST2001::MRST2001( int set, std::string dir ) : 
  Package( dir ), set_( set ), 
  uv_( 0.0 ), dv_( 0.0 ),
  us_( 0.0 ), ds_( 0.0 ), s_( 0.0 ), c_( 0.0 ), b_( 0.0 ), g_( 0.0 )
{
  q_ = 10.0;
  this->load();
  name_ = "MRST2001";
}

MRST2001::MRST2001( const MRST2001& pdf ) :
  Package( pdf ), 
  set_( pdf.set_ ), 
  uv_( 0.0 ), dv_( 0.0 ),
  us_( 0.0 ), ds_( 0.0 ), s_( 0.0 ), c_( 0.0 ), b_( 0.0 ), g_( 0.0 )
{
}

MRST2001::~MRST2001(){
}

double MRST2001::type(){
  double val=0.0;
  double U = uv_ + 2.0 * us_;
  double D = dv_ + 2.0 * ds_;
  double S = 2.0 * s_;
  switch( type_ ){
  case Flavor::a0  : val = U + D + S; break;
  case Flavor::a3  : val = U - D;     break;
  case Flavor::a8  : val = U + D - 2.0 * S; break;
  }
  return this->weight( x_ ) * val / x_;
}

double MRST2001::parton(){
  
  double val = 0.0; 
  switch( parton_ ){
  case Flavor::U    : val = uv_  + 2.0 * us_; break;
  case Flavor::D    : val = dv_  + 2.0 * ds_; break;
  case Flavor::S    : val =        2.0 * s_;  break;
  case Flavor::u    : val = uv_  + us_ ;  break;
  case Flavor::d    : val = dv_  + ds_ ;  break;
  case Flavor::uv   : val = uv_;          break;
  case Flavor::dv   : val = dv_;          break;
  case Flavor::ubar : val = us_;          break;
  case Flavor::dbar : val = ds_;          break;
  case Flavor::s    : 
  case Flavor::sbar : val = s_;           break;
  case Flavor::c    : 
  case Flavor::cbar : val = c_;           break;
  case Flavor::b    : 
  case Flavor::bbar : val = b_;           break;
  case Flavor::g    : val = g_;           break;
  default:            val = 0.0;           break;
  }
  
  return this->weight( x_ ) * val / x_;
}

void MRST2001::loadGrid(){
  
  MRST2001E( x_, q_, set_, uv_, dv_, us_, ds_, s_, c_, b_, g_ );
  
}
