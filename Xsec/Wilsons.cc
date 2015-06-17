// $Id: Wilsons.cc,v 1.3 2008/05/23 02:08:03 miyachi Exp $
/*!
  \file     Wilsons.cc
  \version  $Revision: 1.3 $
  \date     $Date: 2008/05/23 02:08:03 $
  \author   $Author: miyachi $
  \brief    class implementation of Wilson coefficient function collection
 */
#include "Wilsons.hh"

using namespace std;
using namespace Xsection;

Wilsons::Wilsons() :
  vector< vector< vector< Coefficient* > > >
  ( 3, vector< vector< Coefficient* > >
    ( 3, vector< Coefficient* >( 2, ( Coefficient* ) NULL ) ) )
{
}

Wilsons::~Wilsons(){
}

