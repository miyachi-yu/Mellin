// $Id: ConfigPdfBase.cc,v 1.3 2008/12/02 10:29:54 miyachi Exp $
/*!
  \file     ConfigPdfBase.cc
  \version  $Revision: 1.3 $
  \author   $Author: miyachi $
  \brief    Implementation of PDF configuration file handling class
 */
#include "ConfigPdfBase.hh"
#include "PdfBase.hh"
#include <libxml++/libxml++.h>

using namespace std;
using namespace Utility;
using namespace QCD;
using namespace QCD;

ConfigPdfBase::ConfigPdfBase() throw( int ) : 
  Config()
{
}

ConfigPdfBase::ConfigPdfBase(  const std::string& name  ) throw( int ) : 
  Config( Arguments::ref(), name )
{
  this->loadRootNode();
}

ConfigPdfBase::~ConfigPdfBase() {
}

vector< PdfBase* > ConfigPdfBase::pdfs(){
  xmlpp::Node::NodeList pdfs = this->rootNode()->get_children( "pdf" );
  return dynamic_cast< PdfSimpleXml* >( this )->pdfs( pdfs );
}
