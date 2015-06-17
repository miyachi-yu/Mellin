// $Id: MSTW2008.cc,v 1.2 2009/07/28 08:05:31 miyachi Exp $
/*!
  \file      MSTW2008.cc
  \version   $Revison:$
  \author    $Author: miyachi $
  \date      $Date: 2009/07/28 08:05:31 $
  \brief     Implementation of MSTW2008 interface class 

 */
#include "MSTW2008.hh"
#include "mstwpdf.hh"

#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>

using namespace std;
using namespace QCD;
using namespace PDFLIB;

MSTW2008::MSTW2008( const string& prefix, const int& cl,
		    const int& eid, const string& dir ) : 
  Package( dir ), prefix_( prefix ), mstwpdf_( NULL ), cl_( cl ), eid_( eid )
{
  q_ = 10.0;
  this->load();
  name_ = "MSTW2008";
}

MSTW2008::MSTW2008( const MSTW2008& pdf ) :
  Package( pdf ), 
  mstwpdf_( NULL )
{
  
}

MSTW2008::~MSTW2008(){
  if( mstwpdf_ ) delete mstwpdf_;
}

double MSTW2008::type(){
  double val=0.0;
  double U = mstwpdf_->cont.upv + 2.0 * mstwpdf_->cont.usea;
  double D = mstwpdf_->cont.dnv + 2.0 * mstwpdf_->cont.dsea;
  double S = mstwpdf_->cont.str + mstwpdf_->cont.sbar;
  switch( type_ ){
  case Flavor::a0  : val = U + D + S; break;
  case Flavor::a3  : val = U - D;     break;
  case Flavor::a8  : val = U + D - 2.0 * S; break;
  case Flavor::SEA : 
    val = 2.0 * ( mstwpdf_->cont.usea + mstwpdf_->cont.dsea ) +
      mstwpdf_->cont.str + mstwpdf_->cont.sbar;
    break;
  case Flavor::DELTA :
    val = mstwpdf_->cont.dsea - mstwpdf_->cont.usea;
    break;
  }
  return this->weight( x_ ) * val / x_;
}

double MSTW2008::parton(){
  
  double val = 0.0; 
  switch( parton_ ){
  case Flavor::U    : val = mstwpdf_->cont.upv + 2.0 * mstwpdf_->cont.usea; break;
  case Flavor::D    : val = mstwpdf_->cont.dnv + 2.0 * mstwpdf_->cont.dsea; break;
  case Flavor::S    : val = mstwpdf_->cont.str + mstwpdf_->cont.sbar; break;
  case Flavor::u    : val = mstwpdf_->cont.upv + mstwpdf_->cont.usea; break;
  case Flavor::d    : val = mstwpdf_->cont.dnv + mstwpdf_->cont.dsea;  break;
  case Flavor::uv   : val = mstwpdf_->cont.upv;  break;
  case Flavor::dv   : val = mstwpdf_->cont.dnv;  break;
  case Flavor::ubar : val = mstwpdf_->cont.usea; break;
  case Flavor::dbar : val = mstwpdf_->cont.dsea; break;
  case Flavor::s    : val = mstwpdf_->cont.str;  break;
  case Flavor::sbar : val = mstwpdf_->cont.sbar; break;
  case Flavor::c    : val = mstwpdf_->cont.chm;  break;
  case Flavor::cbar : val = mstwpdf_->cont.cbar; break;
  case Flavor::b    : val = mstwpdf_->cont.bot;  break;
  case Flavor::bbar : val = mstwpdf_->cont.bbar; break;
  case Flavor::g    : val = mstwpdf_->cont.glu;  break;
  default:            val = 0.0;            break;
  }
  
  return this->weight( x_ ) * val / x_;
}

void MSTW2008::loadGrid(){
  if( mstwpdf_ == NULL ){
    
    ostringstream ostr;
    ostr << prefix_;
    if( ( cl_ ==  68 || cl_ == 90 ) && eid_ > 0 && eid_ < 41 ){
      ostr << "." << cl_ << "." ;
      ostr << setw(2) << setfill('0') << eid_;
    } else {
      ostr << ".00";
    }
    ostr << ".dat";
    mstwpdf_ = new c_mstwpdf( ostr.str() );
  }
  if( mstwpdf_ ) mstwpdf_->update( x_, q_ );
}

