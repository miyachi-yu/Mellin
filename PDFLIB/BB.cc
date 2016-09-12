//
// BB.cc
//  
// Made by Yoshiyuki Miyachi
// Login   <miyachi@yoshiyuki-desktop>
// 
// Started on  Thu Oct  2 18:12:38 2008 Yoshiyuki Miyachi
// Started on  Thu Oct  2 18:12:38 2008 Yoshiyuki Miyachi
//

#include <iostream>
#include "BB.hh"

#include <QCD/Flavor.hh>

extern "C" {
#include "cfortran.h"

PROTOCCALLSFSUB15(PPDF,ppdf,INT,DOUBLE,DOUBLE,PDOUBLE,PDOUBLE,PDOUBLE,PDOUBLE,PDOUBLE,PDOUBLE,PDOUBLE,PDOUBLE,PDOUBLE,PDOUBLE,PDOUBLE,PDOUBLE)
#define PPDF(ISET,X,Q2,UV,DUV,DV,DDV,GL,DGL,QB,DQB,G1P,DG1P,G1N,DG1N) CCALLSFSUB15(PPDF,ppdf,INT,DOUBLE,DOUBLE,PDOUBLE,PDOUBLE,PDOUBLE,PDOUBLE,PDOUBLE,PDOUBLE,PDOUBLE,PDOUBLE,PDOUBLE,PDOUBLE,PDOUBLE,PDOUBLE,ISET,X,Q2,UV,DUV,DV,DDV,GL,DGL,QB,DQB,G1P,DG1P,G1N,DG1N)

}


using namespace std;
using namespace QCD;
using namespace PDFLIB;

BB::BB( const int& set, const std::string& path ) :
  Package( path ), iset_( set )
{
  name_ = "BB";
  this->load();
}

BB::~BB(){
}

double BB::type(){
  double val=0.0;
  double U = uv_ + 2.0 * qb_;
  double D = dv_ + 2.0 * qb_;
  double S = 2.0 * qb_;
  switch( type_ ){
  case Flavor::a0  : val = U + D + S; break;
  case Flavor::a3  : val = U - D;     break;
  case Flavor::a8  : val = U + D - 2.0 * S; break;
  }
  return this->weight( x_ ) * val / x_;
}

double BB::parton(){
  double val(0.0);
  switch( parton_ ){
  case Flavor::u    : val =  ( uv_ + qb_ );  break;
  case Flavor::d    : val =  ( dv_ + qb_ );  break;
  case Flavor::ubar : 
  case Flavor::dbar : 
  case Flavor::s    : 
  case Flavor::sbar : val =  qb_;  break;
  case Flavor::g    : val =  gl_;  break;
  case Flavor::uv   : val =  uv_;  break;
  case Flavor::dv   : val =  dv_;  break;
  default:            val = 0.0;       break;
  }
  return this->weight( x_ ) * val / x_;
}

double BB::func(){
  double val = 0.0;
  switch( sfunc_ ){
  case Flavor::g1p : val = g1p_; break;
  case Flavor::g1n : val = g1n_; break;
  case Flavor::g1d : val = 0.925 * ( g1p_ + g1n_ ); break;
  default:           val = 0.0;       break;
  }  
  return this->weight( x_ ) * val / x_;
}

void BB::loadGrid(){
  PPDF( iset_, x_, this->q2(), uv_, duv_, dv_, ddv_, 
	gl_, dgl_, qb_, dqb_, g1p_, dg1p_, g1n_, dg1n_ );

}
