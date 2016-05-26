// $Id: PdfStatXml.cc,v 1.1 2009/07/28 04:07:32 miyachi Exp $
/*!
  \file     PdfStatXml.cc
  \version  $Revision: 1.1 $
  \author   $Author: miyachi $
  \brief    Implementation of PDF configuration file handling class
*/
#include "PdfStatXml.hh"
#include "PdfStat.hh"

using namespace PDFSET;

PdfStatXml::PdfStatXml() throw( int ) :
  QCD::PdfSimpleXml()
{}

PdfStatXml::~PdfStatXml() {
}

QCD::PdfBase* PdfStatXml::pdf( xmlpp::Element* elm ){
  PdfStat *pdf = new PdfStat();
  this->set( pdf, elm );
  return pdf;
}
