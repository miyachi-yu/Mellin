// $Id: Flavor.cc,v 1.24 2013/04/05 09:02:13 miyachi Exp $
/*!
  \file    Flavor.cc
  \version $Revision: 1.24 $
  \date    $Date: 2013/04/05 09:02:13 $
  \author  $Author: miyachi $
  \brief   Implementation of quark flavor information class
 */
#include "Flavor.hh"
#include <cmath>
#include <iostream>

using namespace std;
using namespace QCD;

//vector< double > Flavor::mass( 6, 0.0 );
vector< double > Flavor::mass;

void Flavor::setMassValue(){
  if( mass.size() == 0 ) {
    mass.resize( 6, 0.0 );
    mass[ 0 ] = 4.0E-3;
    mass[ 1 ] = 5.5E-3;
    mass[ 2 ] = 0.130;
    mass[ 3 ] = 1.4;
    mass[ 4 ] = 4.5;
    mass[ 5 ] = 175.0;
  }
}

Flavor::Flavor() {
  Flavor::setMassValue();
}

Flavor::~Flavor(){
}

double Flavor::operator() ( const double& q ){
  static int n = mass.size();
  int i;
  for( i = 2; i < n; i++ ) if( q <= mass[ i ] ) break;
  return i;
}

string Flavor::name( const PARTON& q ) {
  switch( q ){
  case tbar: return "t-bar";
  case bbar: return "b-bar";
  case cbar: return "c-bar";
  case sbar: return "s-bar";
  case dbar: return "d-bar";
  case ubar: return "u-bar";
    //  case g: return "gluon";
  case g: return "g";
  case u: return "u";
  case d: return "d";
  case s: return "s";
  case c: return "c";
  case b: return "b";
  case t: return "t";    
  case uv: return "uv";
  case dv: return "dv";
  case sv: return "sv";
  case cv: return "cv";
  case bv: return "bv";
  case tv: return "tv";    
  case uns: return "uns";
  case dns: return "dns";
  case sns: return "sns";
  case cns: return "cns";
  case bns: return "bns";
  case tns: return "tns";    
  case U: return "U";
  case D: return "D";
  case S: return "S";
  case C: return "C";
  case B: return "B";
  case T: return "T";
  }
  return "";
}

string Flavor::name( const TYPE& q ){
  switch( q ){
  case a3m:  return "a3-minus";
  case a8m:  return "a8-minus";
  case a15m: return "a15-minus";
  case a24m: return "a24-minus";
  case a35m: return "a35-minus";
  case a0:   return "a0";
  case a3:   return "a3";
  case a8:   return "a8";
  case a15:  return "a15";
  case a24:  return "a24";
  case a35:  return "a35";
  case DELTA: return "DELTA";
  case SEA:   return "SEA";
  case DURATIO: return "duRatio";
  case DUSUM: return "duSum";
  }
  return "";
}

string Flavor::name( const SFNCTYPE& q ){
  switch( q ){
  case F1p:  return "F_{1}^{p}";
  case F1d:  return "F_{1}^{d}";
  case F1n:  return "F_{1}^{n}";
  case g1p:  return "g_{1}^{p}";
  case g1d:  return "g_{1}^{d}";
  case g1n:  return "g_{1}^{n}";
  }
  return "";
}

string Flavor::name( const HADRON& q, const bool& texflag ){
  if( !texflag ){
    switch( q ){
    case PIp:  return "PIp";
    case PIm:  return "PIm";
    case PI0:  return "PI0";
    case KAp:  return "KAp";
    case KAm:  return "KAm";
    case PRp:  return "PRp";
    case PRm:  return "PRm";
    }
    return "";
  } else {
    switch( q ){
    case PIp:  return "pi+";
    case PIm:  return "pi-";
    case PI0:  return "pi^{0}";
    case KAp:  return "K+";
    case KAm:  return "K-";
    case PRp:  return "P+";
    case PRm:  return "P-";
    }
  }
}

bool Flavor::isParton( const string& name ){
  if( name == "gluon" ||
      name == "g" ||
      name == "u" ||
      name == "d" ||
      name == "s" ||
      name == "c" ||
      name == "b" ||
      name == "t" ||
      name == "U" ||
      name == "D" ||
      name == "S" ||
      name == "C" ||
      name == "B" ||
      name == "T" ||
      name == "u-bar" ||
      name == "d-bar" ||
      name == "s-bar" ||
      name == "c-bar" ||
      name == "b-bar" ||
      name == "t-bar" ||
      name == "ubar" ||
      name == "dbar" ||
      name == "sbar" ||
      name == "cbar" ||
      name == "bbar" ||
      name == "tbar" ||
      name == "uv" ||
      name == "dv" ||
      name == "sv" ||
      name == "cv" ||
      name == "bv" ||
      name == "tv" ||
      name == "uns" ||
      name == "dns" ||
      name == "sns" ||
      name == "cns" ||
      name == "bns" ||
      name == "tns" 
      ) return true;
  return false;
}

int Flavor::id( const string& name ) {
  
  if( Flavor::isParton( name ) ){
    
    if( name == "gluon" ) return g;
    if( name == "g" ) return g;
    if( name == "u" ) return u;
    if( name == "d" ) return d;
    if( name == "s" ) return s;
    if( name == "c" ) return c;
    if( name == "b" ) return b;
    if( name == "t" ) return t;
    if( name == "u-bar" || name == "ubar" ) return ubar;
    if( name == "d-bar" || name == "dbar" ) return dbar;
    if( name == "s-bar" || name == "sbar" ) return sbar;
    if( name == "c-bar" || name == "cbar" ) return cbar;
    if( name == "b-bar" || name == "bbar" ) return bbar;
    if( name == "t-bar" || name == "tbar" ) return tbar;
    if( name == "uv" ) return uv;
    if( name == "dv" ) return dv;
    if( name == "sv" ) return sv;
    if( name == "cv" ) return cv;
    if( name == "bv" ) return bv;
    if( name == "tv" ) return tv;
    if( name == "uns" ) return uns;
    if( name == "dns" ) return dns;
    if( name == "sns" ) return sns;
    if( name == "cns" ) return cns;
    if( name == "bns" ) return bns;
    if( name == "tns" ) return tns;
    if( name == "U" ) return U;
    if( name == "D" ) return D;
    if( name == "S" ) return S;
    if( name == "C" ) return C;
    if( name == "B" ) return B;
    if( name == "T" ) return T;
    if( name == "ubarubar" ) return ubarubar;
    
  } else {
    
    if( name == "a0" ) return a0;
    if( name == "a3" ) return a3;
    if( name == "a8" ) return a8;
    if( name == "a15" ) return a15;
    if( name == "a24" ) return a24;
    if( name == "a35" ) return a35;
    
    if( name == "a3-minus" ) return a3m;
    if( name == "a8-minus" ) return a8m;
    if( name == "a15-minus" ) return a15m;
    if( name == "a24-minus" ) return a24m;
    if( name == "a35-minus" ) return a35m;
    
    if( name == "DELTA" )   return DELTA;
    if( name == "SEA" )     return SEA;

    if( name == "duSum" )   return DUSUM;
    if( name == "duRatio" ) return DURATIO;

  }
  return 0;
}

int Flavor::color( const Flavor::PARTON& p ){
  switch( p ){

  case bbar: return 32;
  case cbar: return 39;
  case sbar: return 44;
  case dbar: return 38;
  case ubar: return 50;
    
  case g: return 8;
    
  case u: return 2;
  case d: return 4;
  case s: return 6;
  case c: return 9;
  case b: return 30;

  case uv: return 152;
  case dv: return 154;
  case sv: return 156;
  case cv: return 159;
  case bv: return 178;

  case uns: return 46;
  case dns: return 36;
  case sns: return 49;
  case cns: return 32;
  case bns: return 43;
    
  case U: return 196;
  case D: return 186;
  case S: return 199;
  case C: return 182;
  case B: return 193;

  case T:
  case tv:
  case tbar:
  case tns:
  case t: return 1;

  }
  return 1;
}

int Flavor::style( const Flavor::PARTON& p ){
  switch( p ){

  case bbar: return Flavor::LDASHDDOT;
  case cbar: return Flavor::LDASHDOT;
  case sbar: return Flavor::LDASH;
  case dbar: return Flavor::DASH;
  case ubar: return Flavor::SOLID;
    
  case g: return Flavor::DOTTED;
    
  case u: return Flavor::SOLID;
  case d: return Flavor::DASH;
  case s: return Flavor::LDASH;
  case c: return Flavor::LDASHDOT;
  case b: return Flavor::LDASHDDOT;

  }
  return Flavor::SOLID;
}
