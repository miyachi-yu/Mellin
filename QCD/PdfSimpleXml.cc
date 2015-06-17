// $Id: PdfSimpleXml.cc,v 1.19 2010/03/03 03:04:38 miyachi Exp $
/*!
  \file     PdfSimpleXml.cc
  \version  $Revision: 1.19 $
  \author   $Author: miyachi $
  \brief    Implementation of PDF configuration file handling class
*/
#include "PdfSimpleXml.hh"

#include "Flavor.hh"
#include "PdfBase.hh"
#include "PdfSimple.hh"

#include <cstdlib>
#include <iomanip>
#include <libxml++/libxml++.h>

using namespace std;
using namespace QCD;

PdfSimpleXml::PdfSimpleXml() throw( int ) {
}

bool PdfSimpleXml::load( xmlpp::Element *pdfset ){
  xmlpp::Element::AttributeList alist = pdfset->get_attributes();
  for( xmlpp::Element::AttributeList::iterator itr = alist.begin();
       itr != alist.end(); itr++ ){
    if( (*itr)->get_name() == "scale" )    scale_ = atof( (*itr)->get_value().data() );
    else if( (*itr)->get_name() == "name" ) name_ = (*itr)->get_value();
  }
}

PdfSimpleXml::~PdfSimpleXml() {
}

vector< PdfBase* > PdfSimpleXml::pdfs( list< xmlpp::Node* >& pdfs ){
  
  vector< PdfBase* > pdflist( 0 );
  for( xmlpp::Node::NodeList::iterator itr = pdfs.begin();
       itr != pdfs.end(); itr++ ){
    
    xmlpp::Element* pdfelm = dynamic_cast< xmlpp::Element* >( *itr );
    PdfBase *ptrpdf = NULL;
    if( pdfelm && ( ptrpdf = this->pdf( pdfelm ) ) )
      pdflist.push_back( ptrpdf );
  }
  
  return pdflist;
}

void PdfSimpleXml::set( PdfBase *pdf, xmlpp::Element* elm ){
  
  PdfBase& p = *pdf;
  
  // set name 
  xmlpp::Attribute* name = elm->get_attribute("name");

  if( name ) {
    p.type() = ( Flavor::isParton( name->get_value() ) ? 
		 PdfBase::parton : PdfBase::combo );
    p.id()   =   Flavor::id( name->get_value() );
  }
  
  xmlpp::Attribute* pid = elm->get_attribute("pid");
  if( pid ) p.pid() = pid->get_value();
  
  xmlpp::Attribute* src = elm->get_attribute("src");
  if( src ) p.src() = src->get_value();
  
  xmlpp::Node::NodeList pars = elm->get_children( "parameter" );
  //  p.resize( pars.size() );
  
  for( xmlpp::Node::NodeList::iterator itr = pars.begin();
       itr != pars.end(); 
       itr++ ){
    
    xmlpp::Element* parElm = dynamic_cast< xmlpp::Element* >( *itr );
    if( parElm ){
      
      int ind = atoi( parElm->get_attribute( "index" )->get_value().data() );
      if( p.find( ind ) != p.end() ){
	cerr << __FILE__ << ":" << __LINE__ 
	     << "Index (" << ind << ") is defined more than once. " 
	     << endl;
	break;
      }
      
      Parameter& param = p[ ind ];
      param.index() = ind;
      param.value() = atof( parElm->get_attribute("value")->get_value().data() );
      param.error() = atof( parElm->get_attribute("error")->get_value().data() );

      param.name()  = 
	( parElm->get_attribute( "name" ) ? 
	  parElm->get_attribute( "name" )->get_value() : "" );
      
      param.state( parElm->get_attribute("state")->get_value().data() );

      // srcid should be configure after the state
      if( parElm->get_attribute( "srcid" ) ){ 
	param.setSrcid( parElm->get_attribute( "srcid" )->get_value() );
      }
      
      param.upper() = 
	( parElm->get_attribute( "upper" ) ? 
	  atof( parElm->get_attribute( "upper" )->get_value().data() ) : PdfBase::upperlimit );
      
      param.lower() =
	( parElm->get_attribute( "lower" ) ? 
	  atof( parElm->get_attribute( "lower" )->get_value().data() ) : PdfBase::lowerlimit );
      
      // error matrix ID
      if( parElm->get_attribute( "emid" ) ) {
	
	string emid = parElm->get_attribute( "emid" )->get_value();
	int pos = emid.find( "-" );
	if( pos == string::npos ){
	  param.emNumber() = 0;
	  param.emid() = atoi( emid.c_str() );
	} else {
	  param.emNumber() = atoi( emid.substr( 0, pos - 1 ).c_str() );
	  param.emid()     = atoi( emid.substr( pos + 1 ).c_str() );
	}
	
      } else {
	param.emNumber() = -1;
	param.emid() = -1;
      }
      
      // get error matrix elements
      xmlpp::Node::NodeList emslist = parElm->get_children( "ems" );
      param.ems().resize( emslist.size() );
      
      for( xmlpp::Node::NodeList::iterator jtr = emslist.begin();
	   jtr != emslist.end(); jtr++ ){
	xmlpp::Element* emsElm = dynamic_cast< xmlpp::Element* >( *jtr );
	if( emsElm ){
	  int emid = 
	    atoi( emsElm->get_attribute( "emid" )->get_value().data()  );
	  param.ems()[ emid ] = 
	    atof( emsElm->get_attribute( "value" )->get_value().data()  );
	}
      }
    }
  }
  
  // do not forget call update process to reflect the new parameter setting...
  if( pars.size() > 0 ) p.update();

}

PdfBase* PdfSimpleXml::pdf( xmlpp::Element* elm ){
  PdfBase *pdf = new PdfSimple();
  this->set( pdf, elm );
  return pdf;
}
