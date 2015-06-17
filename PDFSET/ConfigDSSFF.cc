// $Id: ConfigDSSFF.cc,v 1.1 2009/07/28 04:07:30 miyachi Exp $
/*!
  \file     ConfigFF.cc
  \version  $Revision: 1.1 $
  \author   $Author: miyachi $
  \brief    Implementation of PDF configuration file handling class
 */
#include "ConfigDSSFF.hh"

#include <QCD/FFDisFavor.hh>

#include <iostream>
#include <iomanip>
#include <libxml++/libxml++.h>

using namespace std;
using namespace Utility;
using namespace PDFSET;

ConfigDSSFF::ConfigDSSFF( const std::string& name ) throw( int ) : 
  Config( Arguments::ref(), name )
{
  this->load( this->rootNode() );
}

ConfigDSSFF::~ConfigDSSFF() {
}

vector< QCD::PdfBase* > ConfigDSSFF::pdfs(){
  xmlpp::Node::NodeList pdfs = this->rootNode()->get_children( "pdf" );
  
  vector< QCD::PdfBase* > qs = dynamic_cast< DSSFFXml* >( this )->pdfs( pdfs ) ;
  
  // check if the pdf is defined as Dis-Favored FF....
  for( int i = 0; i < qs.size(); i++ ){
    if( qs[ i ]->dfav() != "" ){
      QCD::FFDisFavor *dis = dynamic_cast< QCD::FFDisFavor* >( qs[ i ] );
      if( dis && dis->favored() == NULL )
	for( int j = 0; j < qs.size(); j++ )
	  if( dis->dfav() == qs[ j ]->pid() ) dis->set( qs[ j ] );
    }
  }
  
  return qs;
}

