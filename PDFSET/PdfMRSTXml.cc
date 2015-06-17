// $Id: PdfMRSTXml.cc,v 1.2 2009/07/28 08:05:31 miyachi Exp $
/*!
  \file     PdfMRSTXml.cc
  \version  $Revision: 1.2 $
  \author   $Author: miyachi $
  \brief    Implementation of PDF configuration file handling class
*/
#include "PdfMRSTXml.hh"
#include "PdfMRST.hh"

using namespace PDFSET;

PdfMRSTXml::PdfMRSTXml() throw( int ) :
  QCD::PdfSimpleXml()
{}

PdfMRSTXml::~PdfMRSTXml() {
}

QCD::PdfBase* PdfMRSTXml::pdf( xmlpp::Element* elm ){
  PdfMRST *pdf = new PdfMRST();
  this->set( pdf, elm );
  return pdf;
}
