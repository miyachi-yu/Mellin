// $Id: Scheme.cc,v 1.5 2008/11/11 04:06:22 miyachi Exp $
/*!
  \file    Scheme.cc
  \version $Revision: 1.5 $
  \date    $Date: 2008/11/11 04:06:22 $
  \author  $Author: miyachi $
  \brief   Implementation of QCD renormalization scheme class
 */
#include "Scheme.hh"

using namespace std;
using namespace QCD;

Scheme::Scheme( TYPE type, ORDER order ) : 
  type_(type), order_( order )
{
}

Scheme::~Scheme() {
}

Scheme& Scheme::operator=( const Scheme& sc ){
  if( this != &sc ){
    type_ = sc.type_;
    order_ = sc.order_;
  }
  return *this;
}

bool Scheme::operator!=( const Scheme& sc ){
  return !( (*this) == sc );
}

bool Scheme::operator==( const Scheme& sc ){
  return ( type_ == sc.type_ && order_ == sc.order_ );
}

namespace QCD {
  ostream& operator<<( ostream& os, Scheme& scheme ){
    os << "<scheme type=\"" 
       << ( scheme.type() == Scheme::MSbar ? "MSbar" : "" )
       << "\" order=\""
       << ( scheme.order() == Scheme::LO ? "LO" :
	    ( scheme.order() == Scheme::NLO ? "NLO" : "NNLO" ) )
     << "\"/>" 
       << flush;
    return os;
  }
}
