#include "GRSV2000.hh"

extern "C" {
#include "cfortran.h"
  
PROTOCCALLSFSUB11(PARPOL,parpol,INT,DOUBLE,DOUBLE,PDOUBLE,PDOUBLE,PDOUBLE,PDOUBLE,PDOUBLE,PDOUBLE,PDOUBLE,PDOUBLE)
#define PARPOL(ISET,X,Q2,U,D,UB,DB,ST,GL,G1P,G1N)  CCALLSFSUB11(PARPOL,parpol,INT,DOUBLE,DOUBLE,PDOUBLE,PDOUBLE,PDOUBLE,PDOUBLE,PDOUBLE,PDOUBLE,PDOUBLE,PDOUBLE,ISET,X,Q2,U,D,UB,DB,ST,GL,G1P,G1N)

}

int PDFLIB::GRSV2000::set_ = 0;

using namespace QCD;
using namespace PDFLIB;

GRSV2000::GRSV2000( const TYPE& set, std::string path ) : 
  Package( path )
{
  if( GRSV2000::set_ != (int) set ){
    GRSV2000::set_ = (int) set;
  }
  name_ = "GRSV2000";
  this->load();
}

GRSV2000::~GRSV2000(){
}

double GRSV2000::type(){
  if( x_ < 1.E-4 || x_ > 1.0 ) return 0.0;
  double val=0.0;
  double U = u_ + ub_;
  double D = d_ + db_;
  double S = 2.0 * st_;
  switch( type_ ){
  case Flavor::a0  : val = U + D + S; break;
  case Flavor::a3  : val = U - D;     break;
  case Flavor::a8  : val = U + D - 2.0 * S; break;
  }
  return this->weight( x_ ) * val / x_;
}

double GRSV2000::parton(){
  
  if( x_ < 1.E-4 || x_ > 1.0 ) return 0.0;
  double val = 0.0; 
  switch( parton_ ){
  case Flavor::u    : val =  u_ ;  break;
  case Flavor::d    : val =  d_ ;  break;
  case Flavor::ubar : val =  ub_;  break;
  case Flavor::dbar : val =  db_;  break;
  case Flavor::s    : 
  case Flavor::sbar : val =  st_;  break;
  case Flavor::g    : val =  gl_;  break;
  default:            val = 0.0;       break;
  }
  
  return this->weight( x_ ) * val / x_;

}

double GRSV2000::func(){    
  if( x_ < 1.E-4 || x_ > 1.0 ) return 0.0;
  double val = 0.0;
  switch( sfunc_ ){
  case Flavor::g1p : val = g1p_; break;
  case Flavor::g1n : val = g1n_; break;
  case Flavor::g1d : val = 0.925 * ( g1p_ + g1n_ ); break;
  default:           val = 0.0;       break;
  }
  return this->weight( x_ ) * val;
  
}

void GRSV2000::loadGrid() {
  PARPOL( GRSV2000::set_, x_, this->q2(), 
	  u_, d_, ub_, db_, st_, gl_, g1p_, g1n_ );
}
