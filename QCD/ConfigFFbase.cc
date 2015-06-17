// $Id: ConfigFFbase.cc,v 1.6 2010/02/15 18:56:06 imazu Exp $
/*!
  \file     ConfigFF.cc
  \version  $Revision: 1.6 $
  \author   $Author: imazu $
  \brief    Implementation of PDF configuration file handling class
 */
#include "ConfigFFbase.hh"
#include "FFDisFavor.hh"
#include "FFSimple.hh"

#include <iostream>
#include <iomanip>
#include <libxml++/libxml++.h>

using namespace std;
using namespace Utility;
using namespace QCD;

ConfigFFbase::ConfigFFbase( const std::string& name ) throw( int ) : 
  Config( Arguments::ref(), name )
{
  this->load( this->rootNode() );
}

ConfigFFbase::~ConfigFFbase() {
}

vector< PdfBase* > ConfigFFbase::pdfs(){

  xmlpp::Node::NodeList pdfs = this->rootNode()->get_children( "pdf" );

  vector< PdfBase* > qs = dynamic_cast< FFSimpleXml* >( this )->pdfs( pdfs ) ;

  // check if the pdf is defined as Dis-Favored FF....
  for( int i = 0; i < qs.size(); i++ ){
    if( qs[ i ]->dfav() != "" ){

      // configure FFDisFav
      FFDisFavor *dis = dynamic_cast< FFDisFavor* >( qs[ i ] );
      if( dis && dis->favored() == NULL )
	for( int j = 0; j < qs.size(); j++ )
	  if( dis->dfav() == qs[ j ]->pid() ) dis->set( qs[ j ] );

      // configure KretzerFF
      FFSimple *gFF = dynamic_cast< FFSimple* >( qs[ i ] );
      if( gFF && qs[ i ]->fav() != "" ){
	if( !gFF->favored() || !gFF->disfavored() )
	  for( int j = 0; j < qs.size(); j++ ){
	    if( gFF->dfav() == qs[ j ]->pid() && !gFF->disfavored() ) 
	      gFF->disfavored( qs[ j ] );
	    if( gFF->fav()  == qs[ j ]->pid() && !gFF->favored() ) 
	      gFF->favored( qs[ j ] );
	}
      }

    }
  }

  for( int i = 0; i < qs.size(); i++ ){
    if( qs[ i ]->size() != 0 ) qs[ i ]->update();
  }

  return qs;
}

