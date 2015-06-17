// $Id: ConfigMRST.cc,v 1.1 2009/07/28 04:07:31 miyachi Exp $
/*!
  \file     ConfigMRST.cc
  \version  $Revision: 1.1 $
  \author   $Author: miyachi $
  \brief    Implementation of PDF configuration file handling class
 */
#include "ConfigMRST.hh"
#include <QCD/PdfBase.hh>
#include <libxml++/libxml++.h>

using namespace std;
using namespace Utility;
using namespace PDFSET;
using namespace PDFSET;

ConfigMRST::ConfigMRST( const string& name ) throw( int ) : 
  Config( Arguments::ref(), name ), PdfMRSTXml()
{
  this->load( this->rootNode() );
}

ConfigMRST::~ConfigMRST() {
}

vector< QCD::PdfBase* > ConfigMRST::pdfs(){
  xmlpp::Node::NodeList pdfs = this->rootNode()->get_children( "pdf" );
  return dynamic_cast< PdfMRSTXml* >( this )->pdfs( pdfs );
}

