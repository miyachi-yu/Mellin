// $Id: DSSFFXml.cc,v 1.1 2009/07/28 04:07:31 miyachi Exp $
/*!
  \file     DSSFFXml.cc
  \version  $Revision: 1.1 $
  \author   $Author: miyachi $
  \brief    Implementation of PDF configuration file handling class
*/
#include "DSSFFXml.hh"
#include "DSSFF.hh"

#include <QCD/FFDisFavor.hh>
#include <QCD/PdfBase.hh>

#include <libxml++/libxml++.h>

using namespace PDFSET;

DSSFFXml::DSSFFXml() throw( int ) :
  QCD::PdfSimpleXml()
{}

DSSFFXml::~DSSFFXml() {
}

QCD::PdfBase* DSSFFXml::pdf( xmlpp::Element* elm ){
  
  QCD::PdfBase *pdf = NULL;
  
  // if it has dfav attribute, create FFDisFavore object instead.
  xmlpp::Attribute* dfav = elm->get_attribute("dfav");
  if( dfav ) {
    pdf = new QCD::FFDisFavor();
    pdf->set( dfav->get_value() );
  } else {
    pdf = new DSSFF();
  }
  
  this->set( pdf, elm );
  
  return pdf;
}
