#include "Charge.hh"
#include <cmath>

using namespace std;
using namespace QCD;
using namespace IntegUnpolVBoson;

/*
  These are implementation of static member variables and functions
*/

// absolute CKM Matrix ( u c t -> d s b )
const double Charge::CKMMX[ 3 ][ 3 ] = { 
  { 0.974, 0.226, 4.31 * pow( 10.0, -3 ) }, // u -> d, s, b
  { 0.230, 0.957, 41.6 * pow( 10.0, -3 ) }, // c -> d, s, b
  { 7.4 * pow( 10.0, -3 ),  35.6 * pow( 10.0, -3 ), 0.78 } // t -> d, s, b
};

// 0 = electron, 1 = up-type, 2 = down-type // 
const double Charge::EVC[ 3 ] = {
  -1.0, 2.0 / 3.0, -1.0 / 3.0 
};

const double Charge::WAC[ 3 ] = {
  -1.0 / 2.0, 1.0 / 2.0, -1.0 / 2.0 
};

double Charge::WVC[ 3 ];
const double Charge::SWVA = 0.23120;

void Charge::initialize(){ 
  for( int i = 0; i < 3; i++ )    
    Charge::WVC[ i ]  = 
      Charge::WAC[ i ] - 2.0 * Charge::EVC[ i ] * Charge::SWVA; 
}

Charge::Charge( IntegXsec::CKernel* coeff, 
		const IntegXsec::Charge::TARGET& n ) :
  IntegXsec::Charge( coeff, n )
{
  this->initialize();
}

Charge::~Charge(){
}

double Charge::bare( const Flavor::PARTON& q ){
  // ref hep-ph/9710508 (Gehrmann) & kretzer

  switch( q ){
  case Flavor::tbar: 
  case Flavor::cbar: 
  case Flavor::c:    
  case Flavor::t:
  case Flavor::ubar: 
  case Flavor::u:     return pow( WVC[ 1 ], 2 ) + pow( WAC[ 1 ], 2 );
  case Flavor::bbar: 
  case Flavor::sbar: 
  case Flavor::s:    
  case Flavor::b:
  case Flavor::dbar: 
  case Flavor::d:    return pow( WVC[ 2 ], 2 ) + pow( WAC[ 2 ], 2 );
  case Flavor::g:    return 0.0;     
  }
  return 0.0;
}

double Charge::bare( const Flavor::PARTON& q1, const Flavor::PARTON& q2 ){
  // CKM matrix ( u c t -> d s b ) implementated from PDG 
  // proper combination of q1 & q2 should be chosen in upper level 
  // ( xcalc: CoeffFunc or ncalc: x(z)Xsec )
  switch( q1 ){

    // q1 up kind ( u c t -> d s b )
  case Flavor::tbar: 
  case Flavor::t:
    switch( q2 ){
    case Flavor::dbar:
    case Flavor::d: return pow( CKMMX[ 2 ][ 0 ], 2 );
    case Flavor::sbar: 
    case Flavor::s: return pow( CKMMX[ 2 ][ 1 ], 2 );
    case Flavor::bbar:
    case Flavor::b: return pow( CKMMX[ 2 ][ 2 ], 2 );
    }
  case Flavor::cbar:
  case Flavor::c:    
    switch( q2 ){
    case Flavor::dbar:
    case Flavor::d: return pow( CKMMX[ 1 ][ 0 ], 2 );
    case Flavor::sbar: 
    case Flavor::s: return pow( CKMMX[ 1 ][ 1 ], 2 );
    case Flavor::bbar:
    case Flavor::b: return pow( CKMMX[ 1 ][ 2 ], 2 );
    }
  case Flavor::ubar: 
  case Flavor::u:
    switch( q2 ){
    case Flavor::dbar:
    case Flavor::d: return pow( CKMMX[ 0 ][ 0 ], 2 );
    case Flavor::sbar: 
    case Flavor::s: return pow( CKMMX[ 0 ][ 1 ], 2 );
    case Flavor::bbar:
    case Flavor::b: return pow( CKMMX[ 0 ][ 2 ], 2 );
    }

    // q1 down kind ( u c t -> d s b )
  case Flavor::bbar: 
  case Flavor::b:
    switch( q2 ){
    case Flavor::ubar:
    case Flavor::u: return pow( CKMMX[ 0 ][ 2 ], 2 );
    case Flavor::cbar: 
    case Flavor::c: return pow( CKMMX[ 1 ][ 2 ], 2 );
    case Flavor::tbar:
    case Flavor::t: return pow( CKMMX[ 2 ][ 2 ], 2 );
    }
  case Flavor::sbar:
  case Flavor::s:    
    switch( q2 ){
    case Flavor::ubar:
    case Flavor::u: return pow( CKMMX[ 0 ][ 1 ], 2 );
    case Flavor::cbar: 
    case Flavor::c: return pow( CKMMX[ 1 ][ 1 ], 2 );
    case Flavor::tbar:
    case Flavor::t: return pow( CKMMX[ 2 ][ 1 ], 2 );
    }
  case Flavor::dbar: 
  case Flavor::d:
    switch( q2 ){
    case Flavor::ubar:
    case Flavor::u: return pow( CKMMX[ 0 ][ 0 ], 2 );
    case Flavor::cbar: 
    case Flavor::c: return pow( CKMMX[ 1 ][ 0 ], 2 );
    case Flavor::tbar:
    case Flavor::t: return pow( CKMMX[ 2 ][ 0 ], 2 );
    }

  case Flavor::g:    return 0.0; 
  }
  return 0.0;
}
