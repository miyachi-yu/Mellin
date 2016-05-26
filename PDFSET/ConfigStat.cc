// $Id: ConfigStat.cc,v 1.1 2009/07/28 04:07:30 miyachi Exp $
/*!
  \file     ConfigStat.cc
  \version  $Revision: 1.1 $
  \author   $Author: miyachi $
  \brief    Implementation of PDF configuration file handling class
 */
#include "ConfigStat.hh"

#include <QCD/PdfBase.hh>
#include <libxml++/libxml++.h>

using namespace std;
using namespace Utility;
using namespace PDFSET;
using namespace PDFSET;

ConfigStat::ConfigStat( const string& name ) throw( int ) : 
  Config( Arguments::ref(), name ), PdfStatXml()
{
  this->load( this->rootNode() );
}

ConfigStat::~ConfigStat() {
}

vector< QCD::PdfBase* > ConfigStat::pdfs(){
  xmlpp::Node::NodeList pdfs = this->rootNode()->get_children( "pdf" );
  return dynamic_cast< PdfStatXml* >( this )->pdfs( pdfs );
}

