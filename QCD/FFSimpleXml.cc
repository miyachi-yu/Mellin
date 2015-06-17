// $Id: FFSimpleXml.cc,v 1.7 2010/02/15 18:56:07 imazu Exp $
/*!
  \file     FFSimpleXml.cc
  \version  $Revision: 1.7 $
  \author   $Author: imazu $
  \brief    Implementation of PDF configuration file handling class
*/
#include "FFSimpleXml.hh"
#include "FFSimple.hh"
#include "FFDisFavor.hh"
#include "PdfBase.hh"

#include <libxml++/libxml++.h>

using namespace std;
using namespace QCD;

FFSimpleXml::FFSimpleXml() throw( int ) :
  PdfSimpleXml()
{}

FFSimpleXml::~FFSimpleXml() {
}

PdfBase* FFSimpleXml::pdf( xmlpp::Element* elm ){
  
  PdfBase *pdf = NULL;
  
  // if it has dfav attribute, create FFDisFavore object instead.
  // if it has fav attribute too, create FFSimple(FavDFav) object instead.
  xmlpp::Attribute* fav  = elm->get_attribute("fav");
  xmlpp::Attribute* dfav = elm->get_attribute("dfav");

  if( dfav && fav ) {
    pdf = new FFSimple();
    pdf->fav( fav->get_value() );
    pdf->dfav( dfav->get_value() );
  } else if( dfav ) {
    pdf = new FFDisFavor();
    pdf->set( dfav->get_value() );
  } else {
    pdf = new FFSimple();
  }
  
  this->set( pdf, elm );
  
  return pdf;
}
