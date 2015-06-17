// $Id: StrFunc.cc,v 1.2 2009/02/13 12:23:41 miyachi Exp $
/*!
  \file     StrFunc.cc
  \version  $Revision: 1.2 $
  \author   $Author: miyachi $
  \date     $Date: 2009/02/13 12:23:41 $
  \brief    class implementation of structure functions in polarized DIS
*/
#include "StrFunc.hh"
#include "XsecComp.hh"

using namespace polDIS;

// ===================================================================== //
//            Implementation of polDIS::StrFunc                //
// ===================================================================== //
StrFunc::StrFunc( Utility::Arguments& args, 
		  const Xsection::Charge::TARGET& n ) 
  throw( int ) : 
  Xsection::Xsec()
{
  
  this->xsecComp( new XsecComp( args, n ) );
  if( this->xsecComp() == NULL ) throw 1 ;
  // for deuteron targe, D-wave correction should be applied.
  // g1d = ( 1 - 1.5 * w_D )( g1p + g1n ), where w_D = 0.05
  if( n == Xsection::Charge::d ) this->globalScale( 0.925 );
  
}

StrFunc::StrFunc( Evolution::Kernel *evo,
		  const Xsection::Charge::TARGET& n ) 
  throw( int ) : 
  Xsection::Xsec()
{
  
  this->xsecComp( new XsecComp( evo, n ) );
  if( this->xsecComp() == NULL ) throw 1 ;
  
  // for deuteron targe, D-wave correction should be applied.
  // g1d = ( 1 - 1.5 * w_D )( g1p + g1n ), where w_D = 0.05
  if( n == Xsection::Charge::d ) this->globalScale( 0.925 );
  
}

StrFunc::~StrFunc(){
  delete this->xsecComp();
}
