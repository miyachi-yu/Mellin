// $Id: ConfigKretzerFF.cc,v 1.1 2009/07/28 04:07:31 miyachi Exp $
/*!
  \file     ConfigFF.cc
  \version  $Revision: 1.1 $
  \author   $Author: miyachi $
  \brief    Implementation of PDF configuration file handling class
*/
#include "ConfigKretzerFF.hh"
#include "KretzerFF.hh"

#include <QCD/FFDisFavor.hh>

#include <iostream>
#include <iomanip>
#include <libxml++/libxml++.h>

using namespace std;
using namespace Utility;
using namespace PDFSET;

ConfigKretzerFF::ConfigKretzerFF( const std::string& name ) throw( int ) : 
  Config( Arguments::ref(), name )
{
  this->load( this->rootNode() );
}

ConfigKretzerFF::~ConfigKretzerFF() {
}

vector< QCD::PdfBase* > ConfigKretzerFF::pdfs(){
  xmlpp::Node::NodeList pdfs = this->rootNode()->get_children( "pdf" );
  vector< QCD::PdfBase* > qs = 
    dynamic_cast< KretzerFFXml* >( this )->pdfs( pdfs ) ;
  
  for( int i = 0; i < qs.size(); i++ ){
    if( qs[ i ]->dfav() != "" ){
      
      // configure FFDisFav
      QCD::FFDisFavor *dis = dynamic_cast< QCD::FFDisFavor* >( qs[ i ] );
      if( dis && dis->favored() == NULL )
	for( int j = 0; j < qs.size(); j++ )
	  if( dis->dfav() == qs[ j ]->pid() ) dis->set( qs[ j ] );
      
      // configure KretzerFF
      KretzerFF *gFF = dynamic_cast< KretzerFF* >( qs[ i ] );
      if( gFF ){
	for( int j = 0; j < qs.size(); j++ ){
	  if( gFF->dfav() == qs[ j ]->pid() ) gFF->favored( qs[ j ] );
	  if( gFF->fav()  == qs[ j ]->pid() ) gFF->disfavored( qs[ j ] );
	}
      }
      
    }
  }
  
  for( int i = 0; i < qs.size(); i++ ){
    if( qs[ i ]->size() != 0 ) qs[ i ]->update();
  }
  
  return qs;
}

