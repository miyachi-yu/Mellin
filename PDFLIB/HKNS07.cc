#include "HKNS07.hh"

#include <cstdlib>

extern "C" {
#include <cfortran.h>

PROTOCCALLSFSUB6(HKNSFF,hknsff,DOUBLE,DOUBLE,INT,INT,DOUBLEV,DOUBLEVV)
#define HKNSFF(Q2,X,ISET,ICHARGE,FF,GRAD) CCALLSFSUB6(HKNSFF,hknsff,DOUBLE,DOUBLE,INT,INT,DOUBLEV,DOUBLEVV,Q2,X,ISET,ICHARGE,FF,GRAD)

}

using namespace QCD;
using namespace PDFLIB;

HKNS07::HKNS07( const int& iset , const int& icharge, std::string path ) : 
  Package( path ), set_( iset ), charge_( icharge ), ff_(), grad_()
{
  name_ = "HKNS07";
  this->load();
}

HKNS07::~HKNS07(){
}

double HKNS07::type(){
  double val = 0.0; 
  double U = ff_[6]  + ff_[4];
  double D = ff_[7]  + ff_[3];
  double S = ff_[8]  + ff_[2];
  double C = ff_[9]  + ff_[1];
  double B = ff_[10] + ff_[0];
  switch( type_ ){
  case Flavor::a0 : val = U + D + S;  break;
  case Flavor::a3 : val = U - D;      break;
  case Flavor::a8 : val = U + D - 2.0 * S;  break;
  default:          val = 0.0;       break;
  }
  return this->weight( x_ ) * val;
}  

double HKNS07::parton(){
  double val = 0.0; 
  switch( parton_ ){
  case Flavor::bbar : val = ff_[  0 ] ;  break;
  case Flavor::cbar : val = ff_[  1 ] ;  break;
  case Flavor::sbar : val = ff_[  2 ] ;  break;
  case Flavor::dbar : val = ff_[  3 ] ;  break;
  case Flavor::ubar : val = ff_[  4 ] ;  break;
  case Flavor::g    : val = ff_[  5 ] ;  break;
  case Flavor::u    : val = ff_[  6 ] ;  break;
  case Flavor::d    : val = ff_[  7 ] ;  break;
  case Flavor::s    : val = ff_[  8 ] ;  break;
  case Flavor::c    : val = ff_[  9 ] ;  break;
  case Flavor::b    : val = ff_[ 10 ] ;  break;
  default:            val = 0.0;       break;
  }
  return this->weight( x_ ) * val;
}

void HKNS07::loadGrid() {
  HKNSFF( this->q2(), x_, set_, charge_, ff_, grad_ );
}
