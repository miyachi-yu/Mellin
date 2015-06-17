// $Id: PdfDSSVXml.cc,v 1.1 2009/07/28 04:07:32 miyachi Exp $
/*!
  \file     PdfDSSVXml.cc
  \version  $Revision: 1.1 $
  \author   $Author: miyachi $
  \brief    Implementation of PDF configuration file handling class
*/
#include "PdfDSSVXml.hh"
#include "PdfDSSV.hh"

using namespace PDFSET;

PdfDSSVXml::PdfDSSVXml() throw( int ) :
  QCD::PdfSimpleXml()
{}

PdfDSSVXml::~PdfDSSVXml() {
}

QCD::PdfBase* PdfDSSVXml::pdf( xmlpp::Element* elm ){
  PdfDSSV *pdf = new PdfDSSV();
  this->set( pdf, elm );
  return pdf;
}
