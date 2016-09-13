// $Id: Config.cc,v 1.13 2009/01/26 10:32:17 miyachi Exp $
/*!
  \file     Config.cc
  \version  $Revision: 1.13 $
  \author   $Author: miyachi $
  \brief    Implementation of class for the configuration XML file handling
 */
#include "Config.hh"

#include <vector>
#include <string>
#include <iostream>
#include <cstdlib>
#include <libxml++/libxml++.h>

extern "C" {
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <error.h>
#include <limits.h>
}

using namespace std;
using namespace Utility;

Config::Config( ) : 
  parser_( NULL ), owner_( false ), path_( "" )
{
}

Config::Config( const string& path ) : 
  parser_( NULL ), owner_( false ), path_( path )
{
  this->parse();
}

Config::Config( Arguments& args, const string& option ) throw( int ) : 
  parser_( NULL ), owner_( false )
{
  
  if( args.hasOpt( "help" ) ){
    args.usage( option, "path", "path of configuration xml file" );
  }
  
  if( ! args.hasOpt( option ) ){
    args.err() << "             --" << option << "=[path] is required" << endl;
    throw  1 ;
  } else {
    path_ = args.getOpt( option );
    if( path_ != "" && ! this->parse() ){
      throw  2 ;
    }
  }
}

Config::~Config(){
  if( parser_ && owner_ ) delete parser_;
}

xmlpp::Element* Config::rootNode() {
  if( parser_ == NULL || ! (*parser_) ) return NULL;
  return parser_->get_document()->get_root_node();
}

bool Config::parse( const string& path ){
  path_ = path;
  return this->parse();
}

bool Config::parse(){

  char resolved_path[ PATH_MAX ];
  
  if( parser_ = new xmlpp::DomParser() ){
    owner_ = true;
    parser_->set_validate();
    parser_->set_substitute_entities();
  }
  
  vector< string > config_path( 0 );
  config_path.push_back( path_ );
  if( char* prefix = getenv( "MELLIN_CONFIG_DIR" ) ){
    config_path.push_back( string( prefix ) + "/" + path_ );
  }
  
  for( int i = 0; i < config_path.size(); i++ ){
    
    if( realpath( config_path[ i ].c_str(), resolved_path ) &&
	access( resolved_path, R_OK ) == 0 ){
      
      cout << "Loading: " << config_path[ i ] << endl;
      parser_->parse_file( resolved_path );
      
      return true;
    } else {
      //      cout << "skip:  [" << config_path[ i ].c_str() << "]" << endl;
      //      perror( __FILE__ );
    }
  }
  
  cerr << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__
       << "\tcould not find " 
       << path_ << endl;
  
  return false;
}

/*
  xmlpp::Node::NodeList Config::nodes( const string& name ){
  return ( this->rootNode() ?
  this->rootNode()->get_children( name.c_str() ) :
  xmlpp::Node::NodeList() );
  }
*/

string Config::rootName() {
  xmlpp::Element* root = this->rootNode();
  return ( root == NULL ? "" : root->get_name() );
}

void Config::dump( ostream& os ){
  if( parser_ == NULL || ! (*parser_) ) return;
  parser_->get_document()->write_to_stream( os );
}


namespace Utility {
  ostream& operator<<( ostream& os, Config& conf ){
    conf.dump( os );
    return os;
  }
}
