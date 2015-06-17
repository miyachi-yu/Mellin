// $ID:$
/*!
  \brief class implementation of hadron crossection in z space
// XFragXsec.cc
//  
// Made by Yoshiyuki Miyachi
// Login   <miyachi@yoshiyuki-desktop>
// 
// Started on  Wed Sep 24 17:30:42 2008 Yoshiyuki Miyachi
// Started on  Wed Sep 24 17:30:42 2008 Yoshiyuki Miyachi
//
*/

#include <iostream>
#include <string>

#include "XFragXsec.hh"
#include "FragXsec.hh"

#include <Utility/Arguments.hh>
#include <Fragmentation/Evolution.hh>
#include <Xsec/XsecComp.hh>

using namespace std;
using namespace FragXsec_T;

XFragXsec::XFragXsec( const QCD::Flavor::HADRON& h ) throw( int ) :
  Xsection::xXsec( NULL, 
		   Utility::Arguments::ref().get( "leg1", 6 ),
		   Utility::Arguments::ref().get( "leg2", 8 ),
		   Utility::Arguments::ref().get( "precision", 5.0E-3 ),
		   Utility::Arguments::ref().get( "length", 8.0 ),
		   Utility::Arguments::ref().get( "offset", 3.0 ),
		   Utility::Arguments::ref().get( "angle",  0.55 ) )
{
  this->moment( new FragXsec( Utility::Arguments::ref(), h ) );
}

XFragXsec::XFragXsec( Fragmentation::Evo *evo,
		      const QCD::Flavor::HADRON& h ) throw( int ) :
  Xsection::xXsec( NULL, 
		   Utility::Arguments::ref().get( "leg1", 6 ),
		   Utility::Arguments::ref().get( "leg2", 8 ),
		   Utility::Arguments::ref().get( "precision", 5.0E-3 ),
		   Utility::Arguments::ref().get( "length", 8.0 ),
		   Utility::Arguments::ref().get( "offset", 3.0 ),
		   Utility::Arguments::ref().get( "angle",  0.55 ) )
{
  this->moment( new FragXsec( evo, h ) );
}

XFragXsec::~XFragXsec(){
  if( this->moment() ) delete this->moment();
}

double XFragXsec::eval( Exp::DataPoint& dp ){
  
  if( dp.info().substr( 0, 4 ) != "e+e-" ) return 0.0;
  
  this->moment()->setQ2( dp.kine( "Q^2" ) );

  vector< double > z( 2 );
  z[ 0 ] = dp.kine( "zmin" );
  z[ 1 ] = dp.kine( "zmax" );

  return ( dp.info().size() == 7 ? 2.0 : 4.0 ) * (*this)( z );
  
}
