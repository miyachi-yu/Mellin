// $Id: ConfigPdf.cc,v 1.6 2008/05/29 04:09:12 miyachi Exp $
/*!
  \file     ConfigPdf.cc
  \version  $Revision: 1.6 $
  \author   $Author: miyachi $
  \brief    Implementation of PDF configuration file handling class
 */
#include "ConfigPdf.hh"

using namespace polPDF;

ConfigPdf::ConfigPdf() throw( int ) : 
  ConfigPdfBase( "polPDF" )
{
}

ConfigPdf::~ConfigPdf() {
}

