// $Id: ConfigDSSV.cc,v 1.1 2009/07/28 04:07:30 miyachi Exp $
/*!
  \file     ConfigDSSV.cc
  \version  $Revision: 1.1 $
  \author   $Author: miyachi $
  \brief    Implementation of PDF configuration file handling class
 */
#include "ConfigDSSV.hh"

#include <QCD/PdfBase.hh>
#include <libxml++/libxml++.h>

using namespace std;
using namespace Utility;
using namespace PDFSET;
using namespace PDFSET;

ConfigDSSV::ConfigDSSV( const string& name ) throw( int ) : 
  Config( Arguments::ref(), name ), PdfDSSVXml()
{
  this->load( this->rootNode() );
}

ConfigDSSV::~ConfigDSSV() {
}

vector< QCD::PdfBase* > ConfigDSSV::pdfs(){
  xmlpp::Node::NodeList pdfs = this->rootNode()->get_children( "pdf" );
  return dynamic_cast< PdfDSSVXml* >( this )->pdfs( pdfs );
}

