// $Id: CTEQ6.cc,v 1.4 2008/10/07 14:48:47 miyachi Exp $
/*!
  \file      CTEQ6.cc
  \version   $Revison:$
  \author    $Author: miyachi $
  \date      $Date: 2008/10/07 14:48:47 $
  \brief     Implementation of CTEQ6 interface class 

 */
#include "CTEQ6.hh"

#include <cmath>
#include <iostream>

extern "C" {
#include <cfortran.h>
#include <unistd.h>

//! macro for CTEQ6 fortran routine
PROTOCCALLSFSUB1(SETCTQ6,setctq6,INT)
#define SETCTQ6(A1)  CCALLSFSUB1(SETCTQ6,setctq6,INT,A1)

//! macro for CTEQ6 fortran routine
PROTOCCALLSFFUN3(DOUBLE,CTQ6PDF,ctq6pdf,INT,DOUBLE,DOUBLE)
#define CTQ6PDF(ID,X,Q)  CCALLSFFUN3(CTQ6PDF,ctq6pdf,INT,DOUBLE,DOUBLE,ID,X,Q)
}

using namespace QCD;
using namespace PDFLIB;

CTEQ6::CTEQ6( int set, std::string dir ) : 
  Package( dir ), set_( set )
{
  this->load();
  name_ = "CTEQ6";
}

CTEQ6::CTEQ6( const CTEQ6& pdf ) :
  Package( pdf ), 
  set_( pdf.set_ )
{
}

CTEQ6::~CTEQ6(){
}

CTEQ6& CTEQ6::operator=( const CTEQ6& pdf ){
  if( this != &pdf ){
    path_   = pdf.path_; 
    q_      = pdf.q_;
    parton_ = pdf.parton_;
    type_   = pdf.type_;
    sfunc_  = pdf.sfunc_;
    mode_   = pdf.mode_;
    n_      = pdf.n_;
    set_    = pdf.set_;
  }
  return *this;
}

double CTEQ6::operator ()( const double& x ) {
  if( mode_ != "PARTON" ) return 0;
  return this->weight( x ) * 
    ( x == 1.0 ? 0.0 : CTQ6PDF( (int) parton_, x, this->q() ) );
}

double CTEQ6::operator ()( const double& x, const double& q2 ) {
  this->setQ2( q2 );
  if( mode_ != "PARTON" ) return 0;
  return this->weight( x ) * 
    ( x == 1.0 ? 0.0 : CTQ6PDF( (int) parton_, x, this->q() ) );
}

void CTEQ6::loadGrid(){
  SETCTQ6( set_ );
}

CTEQ6 CTEQ6::u(){ this->setPID( Flavor::u ); return *this; }
CTEQ6 CTEQ6::d(){ this->setPID( Flavor::d ); return *this; }
CTEQ6 CTEQ6::s(){ this->setPID( Flavor::s ); return *this; }
CTEQ6 CTEQ6::c(){ this->setPID( Flavor::c ); return *this; }
CTEQ6 CTEQ6::b(){ this->setPID( Flavor::b ); return *this; }

CTEQ6 CTEQ6::ubar(){ this->setPID( Flavor::ubar ); return *this; }
CTEQ6 CTEQ6::dbar(){ this->setPID( Flavor::dbar ); return *this; }
CTEQ6 CTEQ6::sbar(){ this->setPID( Flavor::sbar ); return *this; }
CTEQ6 CTEQ6::cbar(){ this->setPID( Flavor::cbar ); return *this; }
CTEQ6 CTEQ6::bbar(){ this->setPID( Flavor::bbar ); return *this; }

CTEQ6 CTEQ6::g(){ this->setPID( Flavor::g );  return *this; }


