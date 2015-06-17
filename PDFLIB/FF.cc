//
// FF.cc
//  
// Made by Yoshiyuki Miyachi
// Login   <miyachi@yoshiyuki-desktop>
// 
// Started on  Thu Oct  2 18:12:38 2008 Yoshiyuki Miyachi
// Started on  Thu Oct  2 18:12:38 2008 Yoshiyuki Miyachi
//

#include <iostream>
#include "FF.hh"

#include <QCD/Flavor.hh>

extern "C" {
#include <cfortran.h>

PROTOCCALLSFSUB9(DLIB,dlib,DOUBLE,DOUBLE,DOUBLEV,INT,INT,INT,INT,INT,INT)
#define DLIB(z,Q2,dff,ffset,fforder,ihadron,icharge,icp,ipi) CCALLSFSUB9(DLIB,dlib,DOUBLE,DOUBLE,DOUBLEV,INT,INT,INT,INT,INT,INT,z,Q2,dff,ffset,fforder,ihadron,icharge,icp,ipi)

}

using namespace std;
using namespace QCD;
using namespace PDFLIB;

FF::FF( const int & ffset,
	const std::string& path,
	const int& hadron, const int& charge, const int& order ) :
  Package( path ), ffset_( ffset ), fforder_( order ), ihadron_( hadron ),
  icharge_( charge ), icp_( 1 ), ipi_( 1 )
{
  switch( ffset_ ){
  case 1: name_ = "Kretzer"; break;
  case 2: name_ = "KKP";     break;
  case 3: name_ = "BFGW";    break;
  }
  this->load();
}

FF::~FF(){
}

double FF::parton(){
  double val(0.0);
  switch( parton_ ){
  case Flavor::b    : val = dff_[0];  break;
  case Flavor::c    : val = dff_[1];  break;
  case Flavor::s    : val = dff_[2];  break;
  case Flavor::u    : val = dff_[3];  break;
  case Flavor::d    : val = dff_[4];  break;
  case Flavor::g    : val = dff_[5];  break;
  case Flavor::dbar : val = dff_[6];  break;
  case Flavor::ubar : val = dff_[7];  break;
  case Flavor::sbar : val = dff_[8];  break;
  case Flavor::cbar : val = dff_[9];  break;
  case Flavor::bbar : val = dff_[10];  break;
  }
  return this->weight( x_ ) * val ;
}

void FF::loadGrid(){
  DLIB( x_, this->q2(), dff_, 
	ffset_, fforder_, ihadron_, icharge_, icp_, ipi_ );
}
