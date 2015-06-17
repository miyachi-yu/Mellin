// $Id: ConfigFFK.cc,v 1.3 2008/05/23 02:08:00 miyachi Exp $
/*!
  \file     ConfigFFK.cc
  \version  $Revision: 1.3 $
  \author   $Author: miyachi $
  \brief    Implementation of PDF configuration file handling class
 */
#include "ConfigFFK.hh"

using namespace std;
using namespace Utility;
using namespace QCD;
using namespace Fragmentation;

ConfigFFK::ConfigFFK( Arguments& args ) throw( int ) : 
  Config( args, "FFK" )
{
  this->load( this->rootNode() );
}

ConfigFFK::~ConfigFFK() {
}

vector< PdfBase* > ConfigFFK::pdfs(){
  
  xmlpp::Node::NodeList pdfs = this->rootNode()->get_children( "pdf" );
  
  return dynamic_cast< FFSimpleXml* >( this )->pdfs( pdfs ) ;
  
}

