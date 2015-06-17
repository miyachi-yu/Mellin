#include "Charge.hh"
#include <cmath>

#include <Xsec/CKernel.hh>
#include <iostream>
#include <Utility/Arguments.hh>

using namespace std;
using namespace QCD;
using namespace Utility;
using namespace FragXsec_T;

/*
  These are implementation of static member variables and functions
*/

// 0 = electron, 1 = up-type, 2 = down-type // 
const double Charge::EVC[3] = {
  -1.0, 2.0 / 3.0, -1.0 / 3.0 
};

const double Charge::WAC[3] = {
  -1.0 / 2.0, 1.0 / 2.0, -1.0 / 2.0 
};

double Charge::WVC[3];
double Charge::ELEM[3];
double Charge::INTF[3];
double Charge::WEAK[ 3 ];

const double Charge::SWVA = 0.23120;
const double Charge::WVAN = asin( sqrt( Charge::SWVA ) );
const double Charge::sinWVAN = sin( Charge::WVAN );
const double Charge::cosWVAN = cos( Charge::WVAN );

const double Charge::MZ   = 91.1876;
const double Charge::ZW   = 2.4952;

const double Charge::MZ2  = Charge::MZ  * Charge::MZ;
const double Charge::ZW2  = Charge::ZW  * Charge::ZW;
const double Charge::MZZW = Charge::MZ2 * Charge::ZW2;

bool Charge::init = false;

bool Charge::initialize(){
  
  for( int i = 0; i < 3; i++ ){
    
    Charge::WVC[ i ]  = 
      Charge::WAC[ i ] - 2.0 * Charge::EVC[ i ] * Charge::SWVA; 
    
    Charge::ELEM[ i ] = 
      pow( Charge::EVC[ i ] * Charge::EVC[ 0 ], 2 );
    
    Charge::INTF[ i ] = 
      2.0 * Charge::EVC[ i ] * Charge::EVC[ 0 ] * Charge::WVC[ i ] * Charge::WVC[ 0 ];

    Charge::WEAK[ i ] = 
      ( pow( Charge::WVC[ i ], 2 ) + pow( Charge::WAC[ i ], 2 ) ) 
      * ( pow( Charge::WVC[ 0 ], 2 ) + pow( Charge::WAC[ 0 ], 2 ) );
    
  }
  return Charge::init = true;
}

/*
  non-static members implementation
*/
Charge::Charge( Xsection::CKernel* coeff ) :
  Xsection::Charge( coeff )
{
  if( ! Charge::init ) Charge:: initialize();
}

Charge::~Charge(){
}

double Charge::elem( const int& EUD ){
  return Charge::ELEM[ EUD ];
}

double Charge::intf( const int& EUD ){
  return Charge::INTF[ EUD ] * this->rho_1();
}

double Charge::weak( const int& EUD ){
  return Charge::WEAK[ EUD ] * rho_2();
}

double Charge::rho_1(){
  
  static const double A = 
    1.0 / pow ( 2.0 * Charge::sinWVAN * Charge::cosWVAN, 2 );
  
  double q2 = coeff_->q2();
  
  return A * q2 * ( q2 - Charge::MZ2 ) / ( pow( Charge::MZ2 - q2, 2 ) + Charge::MZZW );
}

double Charge::rho_2(){
  
  static const double A = 
    1.0 / pow( 4.0 * pow( Charge::sinWVAN * Charge::cosWVAN, 2 ), 2 ); 
  
  double q2 = coeff_->q2();
  
  return A * q2 * q2 / ( pow( MZ2 - q2, 2 ) + Charge::MZZW );
}

double Charge::effCh( const int& EUD ){
  return this->elem( EUD ) + this->intf( EUD ) + this->weak( EUD );
}

double Charge::bare(  const Flavor::PARTON& q ){
//  double q2  = coeff_->q2();
//  double alp = coeff_->alpha()( q2 );
  switch( q ){
  case Flavor::tbar: 
  case Flavor::cbar: 
  case Flavor::ubar: 
  case Flavor::u:    
  case Flavor::c:    
  case Flavor::t:    return this->effCh( 1 );
  case Flavor::bbar: 
  case Flavor::sbar: 
  case Flavor::dbar: 
  case Flavor::d:    
  case Flavor::s:    
  case Flavor::b:    return this->effCh( 2 );
  case Flavor::g:    return this->sum(); 
    
  }
  return this->effCh( 0 );
}

double Charge::operator()( const Flavor::PARTON& q ){
  //  if( Arguments::instance()->hasOpt( "KretzerFF" ) )
  return this->bare( q ) / this->sum() / coeff_->alpha_pi_plus_1();  
  //  return this->bare( q ) / this->sum();
}


