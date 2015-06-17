// $Id: KretzerFFXml.cc,v 1.2 2010/01/20 05:38:02 imazu Exp $
/*!
  \file     KretzerFFXml.cc
  \version  $Revision: 1.2 $
  \author   $Author: imazu $
  \brief    Implementation of PDF configuration file handling class
*/
#include "KretzerFFXml.hh"
#include "KretzerFF.hh"

#include <QCD/FFSimple.hh>
#include <QCD/FFDisFavor.hh>
#include <QCD/PdfBase.hh>

#include <string>
#include <libxml++/libxml++.h>

using namespace std;
using namespace PDFSET;

KretzerFFXml::KretzerFFXml() throw( int ) :
  QCD::PdfSimpleXml()
{}

KretzerFFXml::~KretzerFFXml() {
}

QCD::PdfBase* KretzerFFXml::pdf( xmlpp::Element* elm ){
  
  QCD::PdfBase *pdf = NULL;
  
  // if it has dfav attribute, create FFDisFavore object instead.
  // if it has fav attribute too, create KretzerFF object instead.
  xmlpp::Attribute* fav  = elm->get_attribute("fav");
  xmlpp::Attribute* dfav = elm->get_attribute("dfav");

  if( dfav && fav ) {
    pdf = new KretzerFF();
    dynamic_cast< KretzerFF* >( pdf )->fav( fav->get_value() );
    pdf->set( dfav->get_value() );
  } else if( dfav ) {
    pdf = new QCD::FFDisFavor();
    pdf->set( dfav->get_value() );
  } else {
    pdf = new QCD::FFSimple();
  }
  
  this->set( pdf, elm );
  
  return pdf;
}
