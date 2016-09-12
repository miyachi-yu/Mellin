//
// LSS.cc
//  
// Made by Yoshiyuki Miyachi
// Login   <miyachi@yoshiyuki-desktop>
// 
// Started on  Thu Oct  2 18:12:38 2008 Yoshiyuki Miyachi
// Started on  Thu Oct  2 18:12:38 2008 Yoshiyuki Miyachi
//

#include <iostream>
#include <iomanip>

#include "LSS.hh"

#include <QCD/Flavor.hh>

extern "C" {
#include "cfortran.h"

PROTOCCALLSFSUB16(LSS2006,lss2006,INT,DOUBLE,DOUBLE,PDOUBLE,PDOUBLE,PDOUBLE,PDOUBLE,PDOUBLE,PDOUBLE,PDOUBLE,PDOUBLE,PDOUBLE,PDOUBLE,PDOUBLE,PDOUBLE,PDOUBLE)
#define LSS2006(ISET,X,Q2,UUB,DDB,SSB,GL,UV,DV,UB,DB,ST,g1pLT,g1p,g1nLT,g1n)  CCALLSFSUB16(LSS2006,lss2006,INT,DOUBLE,DOUBLE,PDOUBLE,PDOUBLE,PDOUBLE,PDOUBLE,PDOUBLE,PDOUBLE,PDOUBLE,PDOUBLE,PDOUBLE,PDOUBLE,PDOUBLE,PDOUBLE,PDOUBLE,ISET,X,Q2,UUB,DDB,SSB,GL,UV,DV,UB,DB,ST,g1pLT,g1p,g1nLT,g1n)

PROTOCCALLSFSUB16(LSS2005,lss2005,INT,DOUBLE,DOUBLE,PDOUBLE,PDOUBLE,PDOUBLE,PDOUBLE,PDOUBLE,PDOUBLE,PDOUBLE,PDOUBLE,PDOUBLE,PDOUBLE,PDOUBLE,PDOUBLE,PDOUBLE)
#define LSS2005(ISET,X,Q2,UUB,DDB,SSB,GL,UV,DV,UB,DB,ST,g1pLT,g1p,g1nLT,g1n)  CCALLSFSUB16(LSS2005,lss2005,INT,DOUBLE,DOUBLE,PDOUBLE,PDOUBLE,PDOUBLE,PDOUBLE,PDOUBLE,PDOUBLE,PDOUBLE,PDOUBLE,PDOUBLE,PDOUBLE,PDOUBLE,PDOUBLE,PDOUBLE,ISET,X,Q2,UUB,DDB,SSB,GL,UV,DV,UB,DB,ST,g1pLT,g1p,g1nLT,g1n)

PROTOCCALLSFSUB12(LSS2001,lss2001,INT,DOUBLE,DOUBLE,PDOUBLE,PDOUBLE,PDOUBLE,PDOUBLE,PDOUBLE,PDOUBLE,PDOUBLE,PDOUBLE,PDOUBLE)
#define LSS2001(ISET,X,Q2,UUB,DDB,SSB,GL,UV,DV,UB,DB,ST)  CCALLSFSUB12(LSS2001,lss2001,INT,DOUBLE,DOUBLE,PDOUBLE,PDOUBLE,PDOUBLE,PDOUBLE,PDOUBLE,PDOUBLE,PDOUBLE,PDOUBLE,PDOUBLE,ISET,X,Q2,UUB,DDB,SSB,GL,UV,DV,UB,DB,ST)


}


using namespace std;
using namespace QCD;
using namespace PDFLIB;

int PDFLIB::LSS::iset_ = 0;

LSS::LSS( const int& set,
	  const std::string& path,
	  const VERSION& version ) :
  Package( path ), version_( version )
{
  if( LSS::iset_ != set ){
    LSS::iset_ = set;
  } else {
  }
  this->load();

  switch( version_ ){
  case v2006 : name_ = "LSS2006"; break;
  case v2005 : name_ = "LSS2005"; break;
  case v2001 : name_ = "LSS2001"; break;
  }
}

LSS::~LSS(){
}

double LSS::type(){
  double val = 0.0;
  switch( type_ ){
  case Flavor::a0  : val = uub_ + ddb_ + ssb_; break;
  case Flavor::a3  : val = uub_ - ddb_;        break;
  case Flavor::a8  : val = uub_ + ddb_ -2.0 * ssb_; break;
  }
  return this->weight( x_ ) * val / x_ ;
}

double LSS::parton(){
  
  double val(0.0);
  switch( parton_ ){
  case Flavor::u    : val =  ( uv_ + ub_ );  break;
  case Flavor::d    : val =  ( dv_ + db_ );  break;
  case Flavor::ubar : val =  ub_;  break;
  case Flavor::dbar : val =  db_;  break;
  case Flavor::s    : 
  case Flavor::sbar : val =  st_;  break;
  case Flavor::g    : val =  gl_;  break;
  case Flavor::uv   : val =  uv_;  break;
  case Flavor::dv   : val =  dv_;  break;
  case Flavor::U    : val = uub_;  break;
  case Flavor::D    : val = ddb_;  break;
  case Flavor::S    : val = ssb_;  break;
  default:            val = 0.0;       break;
  }
  return this->weight( x_ ) * val / x_ ;
}

double LSS::func(){

  if( version_ == v2001 ) return 0.0;
  
  double val = 0.0;
  switch( sfunc_ ){
  case Flavor::g1p : val = g1p_; break;
  case Flavor::g1n : val = g1n_; break;
  case Flavor::g1d : val = 0.925 * ( g1p_ + g1n_ ); break;
  default:           val = 0.0;       break;
  }
  
  return this->weight( x_ ) * val / x_ ;
}
  
void LSS::loadGrid(){

  //INTINI.iini = 0;

  switch( version_ ){
    
  case v2006 :
    LSS2006( LSS::iset_, x_, this->q2(), 
	     uub_, ddb_, ssb_, gl_, uv_, dv_, ub_, db_, st_, 
	     g1plt_, g1p_, g1nlt_, g1n_ );
    break;

  case v2005 :
    LSS2005( LSS::iset_, x_, this->q2(), 
	     uub_, ddb_, ssb_, gl_, uv_, dv_, ub_, db_, st_, 
	     g1plt_, g1p_, g1nlt_, g1n_ );
    break;
    
  case v2001 :
    LSS2001( LSS::iset_, x_, this->q2(), 
	     uub_, ddb_, ssb_, gl_, uv_, dv_, ub_, db_, st_ );

    break;
    
  }
}
