#include "AAC.hh"

#include <cstdlib>

extern "C" {
#include "cfortran.h"

PROTOCCALLSFSUB5(AAC08PDF,aac08pdf,DOUBLE,DOUBLE,INT,DOUBLEV,DOUBLEVV)
#define AAC08PDF(Q2,X,ISET,XPPDF,GRAD) CCALLSFSUB5(AAC08PDF,aac08pdf,DOUBLE,DOUBLE,INT,DOUBLEV,DOUBLEVV,Q2,X,ISET,XPPDF,GRAD)

}

using namespace QCD;
using namespace PDFLIB;

AAC::AAC( const int& iset , std::string path ) : 
  Package( path ), set_( iset )
{
  name_ = "AAC";
  this->load();
}

AAC::~AAC(){
}

double AAC::type(){
  double val = 0.0; 
  double U = xppdf_[4] + xppdf_[2];
  double D = xppdf_[5] + xppdf_[1];
  double S = xppdf_[6] + xppdf_[0];
  switch( type_ ){
  case Flavor::a0 : val = U + D + S;  break;
  case Flavor::a3 : val = U - D;      break;
  case Flavor::a8 : val = U + D - 2.0 * S;  break;
  default:          val = 0.0;       break;
  }
  return this->weight( x_ ) * val / x_;
}  

double AAC::parton(){
  double val = 0.0; 
  
  switch( parton_ ){
    
  case Flavor::sbar : val =  xppdf_[0] ;  break;
  case Flavor::dbar : val =  xppdf_[1] ;  break;
  case Flavor::ubar : val =  xppdf_[2] ;  break;
  case Flavor::g    : val =  xppdf_[3] ;  break;
  case Flavor::u    : val =  xppdf_[4] ;  break;
  case Flavor::d    : val =  xppdf_[5] ;  break;
  case Flavor::s    : val =  xppdf_[6] ;  break;

  default:            val = 0.0;       break;
  }
  
  return this->weight( x_ ) * val / x_;
}

void AAC::loadGrid() {
  
  AAC08PDF( this->q2(), x_, set_, xppdf_, grad_ );

  /*
  double *buff = (double*) malloc( sizeof( double[7] ) );
  double **buf2 = (double**) malloc( sizeof( double[7][11] ) );


  AAC08PDF( this->q2(), x_, set_, &(buff[4]), &( buf2[4] ) );
  */
}
