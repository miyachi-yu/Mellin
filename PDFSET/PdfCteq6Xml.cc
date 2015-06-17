// $Id: PdfCteq6Xml.cc,v 1.2 2009/07/29 08:59:48 miyachi Exp $
/*!
  \file     PdfCteq6Xml.cc
  \version  $Revision: 1.2 $
  \author   $Author: miyachi $
  \brief    Implementation of PDF configuration file handling class
*/
#include "PdfCteq6Xml.hh"
#include "PdfCteq6.hh"

#include <QCD/Flavor.hh>

using namespace std;
using namespace PDFSET;

PdfCteq6Xml::PdfCteq6Xml() throw( int ) :
  QCD::PdfSimpleXml() {
}

PdfCteq6Xml::~PdfCteq6Xml() {
}

QCD::PdfBase* PdfCteq6Xml::pdf() {
  return new PdfCteq6();
}

