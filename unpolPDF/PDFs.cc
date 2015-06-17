// $Id: PDFs.cc,v 1.3 2008/05/23 02:08:05 miyachi Exp $
/*!
  \file    PDFs.hh
  \version $Revision: 1.3 $
  \date    $Date: 2008/05/23 02:08:05 $
  \author  $Author: miyachi $
  \brief   Definition of PDF container class
 */
#include "PDFs.hh"
#include <QCD/PdfBase.hh>
#include <iostream>

using namespace std;
using namespace QCD;
using namespace unpolPDF;

PDFs::PDFs() : 
  PdfParametrization()
{
}

PDFs::~PDFs() {
}

PDFs& PDFs::operator<<( ConfigPdf& conf ){
  
  this->name( conf.name() );
  this->scale( conf.scale() );
  this->pdfs( conf.pdfs() );
  
  return *this;
}
