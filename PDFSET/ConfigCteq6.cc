// $Id: ConfigCteq6.cc,v 1.1 2009/07/29 08:59:48 miyachi Exp $
/*!
  \file     ConfigCteq6.cc
  \version  $Revision: 1.1 $
  \author   $Author: miyachi $
  \brief    Implementation of PDF configuration file handling class
 */
#include "ConfigCteq6.hh"
#include <QCD/PdfBase.hh>
#include <libxml++/libxml++.h>

using namespace std;
using namespace Utility;
using namespace PDFSET;
using namespace PDFSET;

ConfigCteq6::ConfigCteq6( const string& name ) throw( int ) : 
  Config( Arguments::ref(), name ), PdfCteq6Xml()
{
  this->load( this->rootNode() );
}

ConfigCteq6::~ConfigCteq6() {
}

vector< QCD::PdfBase* > ConfigCteq6::pdfs(){
  xmlpp::Node::NodeList pdfs = this->rootNode()->get_children( "pdf" );
  return dynamic_cast< PdfCteq6Xml* >( this )->pdfs( pdfs );
}

