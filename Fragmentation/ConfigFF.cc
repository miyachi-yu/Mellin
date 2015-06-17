// $Id: ConfigFF.cc,v 1.4 2008/05/23 02:08:00 miyachi Exp $
/*!
  \file     ConfigFF.cc
  \version  $Revision: 1.4 $
  \author   $Author: miyachi $
  \brief    Implementation of PDF configuration file handling class
 */
#include "ConfigFF.hh"

using namespace std;
using namespace Utility;
using namespace QCD;
using namespace Fragmentation;

ConfigFF::ConfigFF( Arguments& args ) throw( int ) : 
  Config( args, "FF" )
{
  this->load( this->rootNode() );
}

ConfigFF::~ConfigFF() {
}

vector< PdfBase* > ConfigFF::pdfs(){
  xmlpp::Node::NodeList pdfs = this->rootNode()->get_children( "pdf" );
  return dynamic_cast< FFSimpleXml* >( this )->pdfs( pdfs ) ;
}

