// $Id: G1hFit.cc,v 1.5 2008/11/26 10:11:10 miyachi Exp $
/*!
  \file   G1hFit.cc
*/
#include "G1hFit.hh"

using namespace std;
using namespace Utility;
using namespace QCD;
using namespace Fit;

G1hFit::G1hFit() throw( int ) :
  G1Fit(),
  ff_( 2, ( Fragmentation::Evo* ) NULL ),
  g1h_( 2, vector< polSIDIS::StrFunc* >( 4, ( polSIDIS::StrFunc* ) NULL ) ),
  xg1h_( 2, vector< Xsection::xzXsec* >( 4, ( Xsection::xzXsec* ) NULL ) )
{
  
  Arguments& args = Arguments::ref();
  
  // fragmentation functions.....
  if( ! args.hasOpt( "FF" ) || ! args.hasOpt( "FFK" ) ){
    throw( 1 );
  }

  try {
    
    ff_[ 0 ] = new Fragmentation::Evo( args, Flavor::PIp );
    ff_[ 1 ] = new Fragmentation::Evo( args, Flavor::KAp );
    
    // structure functions
    // g1p^h
    g1h_[ 0 ][ 0 ] = new polSIDIS::StrFunc( evolution_, ff_[ 0 ], Xsection::Charge::p, Flavor::PIp );
    g1h_[ 0 ][ 1 ] = new polSIDIS::StrFunc( evolution_, ff_[ 0 ], Xsection::Charge::p, Flavor::PIm );
    g1h_[ 0 ][ 2 ] = new polSIDIS::StrFunc( evolution_, ff_[ 1 ], Xsection::Charge::p, Flavor::KAp );
    g1h_[ 0 ][ 3 ] = new polSIDIS::StrFunc( evolution_, ff_[ 1 ], Xsection::Charge::p, Flavor::KAm );
    
    // g1d^h
    g1h_[ 0 ][ 0 ] = new polSIDIS::StrFunc( evolution_, ff_[ 0 ], Xsection::Charge::d, Flavor::PIp );
    g1h_[ 0 ][ 1 ] = new polSIDIS::StrFunc( evolution_, ff_[ 0 ], Xsection::Charge::d, Flavor::PIm );
    g1h_[ 0 ][ 2 ] = new polSIDIS::StrFunc( evolution_, ff_[ 1 ], Xsection::Charge::d, Flavor::KAp );
    g1h_[ 0 ][ 3 ] = new polSIDIS::StrFunc( evolution_, ff_[ 1 ], Xsection::Charge::d, Flavor::KAm );
    
    // Double Mellin Inversion
    for( int i = 0; i < g1h_.size(); i++ )
      for( int j = 0; j < g1h_[ i ].size(); j++ ){
	xg1h_[ i ][ j ] = 
	  new Xsection::xzXsec( g1h_[ i ][ j ],
				4, 6, 1.0E-3, 10.0, 2.0, 0.5 );
      }
  } 
  
  catch( int e ){
    throw( e );
  }
  
}


G1hFit::~G1hFit(){
  
  // delete all created objects in the constructor
  if( ff_[ 0 ] ) delete ff_[ 0 ];
  if( ff_[ 1 ] ) delete ff_[ 1 ];

  for( int i = 0; i < g1h_.size(); i++ )
    for( int j = 0; j < g1h_[ i ].size(); j++ )
      if( g1h_[ i ][ j ] ) delete g1h_[ i ][ j ];
  
}

bool G1hFit::calc( const int& i ){
  return true;
}

void G1hFit::data_summary( std::ostream& os ){
}

void G1hFit::postUpdate(){
}

void G1hFit::loadData(){
}

bool G1Fit::postFit(){
  return true;
};

bool G1Fit::postErrorMatrix(){
  return true;
}
