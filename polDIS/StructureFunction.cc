// $Id: StructureFunction.cc,v 1.21 2008/08/28 11:21:48 imazu Exp $
/*!
  \file     StructureFunction.cc
  \version  $Revision: 1.21 $
  \author   $Author: imazu $
  \date     $Date: 2008/08/28 11:21:48 $
  \brief    class implementation of structure functions in polarized DIS
*/
#include "StructureFunction.hh"
#include "XsecComp.hh"

using namespace polDIS;

// ===================================================================== //
//            Implementation of polDIS::StructureFunction                //
// ===================================================================== //
StructureFunction::StructureFunction( Utility::Arguments& args, 
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

StructureFunction::StructureFunction( polPDF::Evo *evo,
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

StructureFunction::~StructureFunction(){
  delete this->xsecComp();
}
