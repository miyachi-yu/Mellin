// $Id: Alpha.cc,v 1.15 2008/12/09 01:08:37 imazu Exp $
/*!
  \file    Alpha.cc
  \version $Revision: 1.15 $
  \date    $Date: 2008/12/09 01:08:37 $
  \author  $Author: imazu $
  \brief   Implementation of strong coupling constant class
 */
#include "Alpha.hh"

#include <cstdlib>
#include <cmath>
#include <iostream>

using namespace std;
using namespace Utility;
using namespace QCD;


double Alpha::Differential::operator()( const double& t,
					const double& as ) const {
  static double PI4 = 4.0 * M_PI;
  double a = as / PI4;
  double q = exp( 0.5 * t );
  double r = 0;
  for( int i = Scheme::LO; i <= alpha_->scheme().order(); i++ ) {
    r -= pow( a, 2 + i ) * alpha_->beta( i, q );
  }
  return r * PI4;
}

Flavor Alpha::nf;
double Alpha::C = 4.0 * M_PI;

double Alpha::Beta( const int& i, const double& n ){
  
  if( i > 3 ) return 0.0;
  
  if( i == 3 ) { // from Phys. Lett. B400 (1997) 379
    static double c30 =  29243.0;
    static double c31 = -6946.30;
    static double c32 =  405.089;
    static double c33 =  1093 / 729;
    return c30 + c31 * n + c32 * n * n + c33 * n * n * n;
  }
  
  if( i == 2 ) {
    static double c20 =  2857.0 /  2.0;
    static double c21 = -5033.0 / 18.0;
    static double c22 =  325.0  / 54.0;
    return c20 + c21 * n + c22 * n * n;
  }
  
  if( i == 1 ) {
    static double c11 = - 38.0 / 3.0;
    return 102.0 + c11 * n;
  }
  
  static double c01 = - 2.0 / 3.0;
  return 11.0 + c01 * n;
}

Alpha::Alpha( const Scheme::ORDER& order ) : 
  lambda_( 0.0 ), lambda2_( 0.0 ), 
  scheme_( Scheme::MSbar, order ),
  da_( this ), rg_( &da_ ), dt_( 0.01 ),
  asMz_( 0.1176 ), Mz_( 91.1876 ), // those values are taken from PDG 2006
  mode_( RUNGEKUTTA ),
  factor_( 1.0 )
{
}

Alpha::Alpha( Arguments& args ) : 
  lambda_( 0.0 ), lambda2_( 0.0 ), 
  scheme_( Scheme::MSbar, Scheme::NLO ),
  da_( this ), rg_( &da_ ), dt_( 0.01 ),
  asMz_( 0.1176 ), Mz_( 91.1876 ), //! these values are taken from PDG 2006
  mode_( RUNGEKUTTA ),
  factor_( 1.0 )
{
  
  // set order
  scheme_.order() = 
    ( args.get( "order", "NLO" ) == "LO" ? Scheme::LO : Scheme::NLO );
  
  // set lambda QCD
  lambda_ = args.get( "lambdaQCD", 0.234 );
  
  if( args.hasOpt( "help" ) || 
      args.hasOpt( "Help" ) || 
      args.hasOpt( "HELP" ) || 
      args.hasOpt( "?" )  ){
    
    args.usage( "order",        "LO",
		"pQCD expantion order        (LO|NLO)" );
    
    args.usage( "lambdaQCD", "value",
		"lambda_QCD for alpha_s    (optional)" );
    
  }
  
}

Alpha::Alpha( const double& lambda, const Scheme::ORDER& order ) : 
  lambda_( lambda ), lambda2_( lambda * lambda ), 
  scheme_( Scheme::MSbar, order ),
  da_( this ), rg_( &da_ ), dt_( 0.01 ),
  asMz_( 0.1176 ), Mz_( 91.1876 ), //! these values are taken from PDG 2006
  mode_( LAMBDA ),
  factor_( 1.0 )
{
}

Alpha::~Alpha(){
}

Alpha& Alpha::operator=( const Alpha& as ){
  if( this != &as ){
    scheme_ = as.scheme_;
    mode_   = as.mode_;
    asMz_   = as.asMz_;
    Mz_     = as.Mz_;
    dt_     = as.dt_;
    lambda_ = as.lambda_;
    lambda2_ = as.lambda2_;
  }
  return *this;
}
  
bool Alpha::operator!=( const Alpha& as ){
  return !(  (*this) == as );
}

bool Alpha::operator==( const Alpha& as ){
  if( scheme_ != as.scheme_ ) return false;  // scheme must be same
  if( mode_ != as.mode_ ) return false;      // mode must be same
  switch( mode_ ){
  case RUNGEKUTTA:
    if( asMz_ != as.asMz_ || Mz_ != as.Mz_ || dt_ != as.dt_ ) return false;
    break;
  case LAMBDA:
    if( lambda_ != as.lambda_ ||  lambda2_ != as.lambda2_ ) return false;
    break;
  }
  return true;
}

double Alpha::operator() ( const double& mu2_0 ){
  
  double as = 0.0;
  double mu2 = mu2_0 / factor_;
  
  // calculate alpha_s value with Rungekutta Method
  if( mode_ == RUNGEKUTTA ){
    as = asMz_;
    double tMz = 2.0 * log( Mz_ );
    double t   = log( mu2 );
    double n   = fabs( ( t - tMz ) / dt_ );
    double d   = ( t < tMz ? -1.0 : 1.0 ) * dt_;
    for( int i = 0; i < n; i++ ) as += rg_( as, tMz + i * d, d );
    return as;
  }
  
  // calculate alpha_s value with perturbative expantion
  double Ll       = log( mu2 / lambda2_ );
  double n        = nf( sqrt( mu2 ) );
  double b0       = Beta( 0, n );
  double b0Ll     = Ll * b0;
  double b0Llinv  = 1 / b0Ll;
  as += C * b0Llinv;
  
  if( this->scheme().order()  == Scheme::LO  ) return as;
  
  double b0Llinv2 = b0Llinv * b0Llinv;
  double b1       = Beta( 1, n ) / b0; 
  double lLl      = log( Ll );
  as += -1.0 * C * b0Llinv2 * b1 * lLl;
  
  if( this->scheme().order()  == Scheme::NLO ) return as;
  
  double b0Llinv3 = b0Llinv * b0Llinv2;
  double b12      = b1 * b1;
  double b2       = Beta( 2, n ) / b0;   
  as += C * b0Llinv3 * b12 * ( pow( lLl - 0.5, 2 ) + b2 / b12 - 1.25 );
  
  if( this->scheme().order()  == Scheme::NNLO ) return as;
  
  return as;
}

void Alpha::atMz( const double& v ) {
  asMz_ = v;
  mode_ = RUNGEKUTTA;
}

double Alpha::lambda( const double& v ) { 
  lambda_  = v;
  lambda2_ = v * v;
  mode_    = LAMBDA;
  return lambda_;
}

double Alpha::beta( const int& i, const double& q ){
  if( this->scheme().order() == Scheme::NNLO && i > 2 ) return 0.0;
  if( this->scheme().order() == Scheme::NLO  && i > 1 ) return 0.0;
  if( this->scheme().order() == Scheme::LO   && i > 0 ) return 0.0;
  return Alpha::Beta( i, this->flavor()( q ) );
}

namespace QCD {

  ostream& operator <<( ostream& os, Alpha& alpha ){
    os << "<alpha lambda=\"" << alpha.lambda() << "\">" << endl;
    os << alpha.scheme() << endl;
    os << "</alpha>" << endl;
    return os;
  }
  
}
