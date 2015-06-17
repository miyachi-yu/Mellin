// $Id: Arguments.cc,v 1.30 2010/04/30 08:10:05 miyachi Exp $
/*!
  \file     Arguments.cc
  \version  "$Revision: 1.30 $"
  \author   "$Author: miyachi $"
  \brief    Implementation of command line arguments paser
 */

#include "Arguments.hh"
#include <iomanip>
#include <fstream>
#include <cstdlib>
#include <sstream>
#include <complex>

#include "LookupTableCtl.hh"

using namespace std;
using namespace Utility;

string Arguments::NO_OPTION = "__NO_OPTION__";
string Arguments::NO_VALUE  = "__NO_VALUE__";


Arguments* Arguments::instance_ = NULL;

Arguments& Arguments::ref( int argc, char* argv[] ){
  if( ! Arguments::instance_ ) Arguments::instance( argc, argv );
  return *( Arguments::instance_ );
}

Arguments& Arguments::ref() {
  return *( Arguments::instance_ );
}

Arguments* Arguments::instance() {
  return Arguments::instance_;
}

void Arguments::callAtExit(){
  if( Arguments::instance_ ) delete Arguments::instance_;
}

Arguments* Arguments::instance( int argc, char* argv[] ) {
  if( Arguments::instance_ ==  NULL ){
    if( Arguments::instance_ = new Arguments( argc, argv ) )
      atexit( Arguments::callAtExit );
  }
  return Arguments::instance_;
}

Arguments::Arguments( int argc, char* argv[] ) : 
  opts_(), args_( 0 ), out_( &cout ),
  verbose_( LOG ), null_( "/dev/null" ),
  vlabel_(), argc_( argc ), argv_( argv )
{
  
  vlabel_[ DETAIL ]  = "[detail] : ";
  vlabel_[ LOG ]     = "[log]    : ";
  vlabel_[ WARNING ] = "[warning]: ";
  vlabel_[ ERROR ]   = "[error]  : ";
  vlabel_[ FAITAL ]  = "[faital] : ";
  
  command_ = argv[ 0 ];
  
  for( int i = 1; i < argc; i++ ){
    string data( argv[ i ] );
    if( data.substr( 0, 2 ) == "--" ) {
      // long option
      this->option( data.substr( 2, data.size() - 2 ) );
      continue;
    }
    
    if( data.substr( 0, 1 ) == "-" ){
      // short option
      if( i == argc - 1 || argv[ i + 1 ][ 0 ] == '-' ) 
	this->option( data.substr( 1, data.size() - 1 ), "" ); 
      else 
	this->option( data.substr( 1, data.size() ), argv[ i+=1 ] );
      
      continue;
    }
    
    this->option( "", data );
    
  }
  
  // prepare log and error output
  if( this->hasOpt( "log" ) ){
    string ofilename = this->get( "log", "mellin.log" );
    ofstream *f;
    if( ( f = new ofstream( ofilename.c_str() ) ) ){
      out_ = f;
      this->log( LOG ) << " create log file:  " << ofilename << endl;
    }
  }
  
  // verbose level
  verbose_ = (V_LEVEL) this->get( "verbose", 1 );
  this->log( LOG ) << " set verbose level:  " << vlabel_[ verbose_ ] << endl;
  
  // Lookup Table control
  if( this->hasOpt( "writeDB" ) ) {
    LookupTableCtl::actl = LookupTableCtl::read_write;
    this->log( LOG ) << " open DB in read/write mode" << endl;
  } else if( this->hasOpt( "disableDB" ) ){
    LookupTableCtl::actl = LookupTableCtl::disable;
    this->log( LOG ) << " disable DB access" << endl;
  } else if( this->hasOpt( "readDB" ) ){
    LookupTableCtl::actl = LookupTableCtl::read_only;
    this->log( LOG ) << " open DB in read-only mode" << endl;
  } else {
    LookupTableCtl::actl = LookupTableCtl::memory;
    this->log( LOG ) << " use memory based DB" << endl;
  }
  
  // Print usage
  if( this->hasOpt( "help" ) ){
    this->usage( "help", "",    "Show help" );
    this->usage( "log", "path", "Path of log file     (optional)" );
    this->usage( "verbose", "", "set verborse level 0 - 4 " );
    this->usage( "writeDB", "", "DB in read/write mode (default: memory)" );
    this->usage( "readDB", "", "DB in read only mode (default: memory)" );
    this->usage( "disableDB", "", "disable DB access (default: memory)" );
  }
  
}

Arguments::Arguments( const Arguments& args ) :
  opts_( args.opts_ ), args_( args.args_ ), 
  out_( args.out_ ),
  verbose_( args.verbose_ ),
  command_( args.command_ ),
  vlabel_( args.vlabel_ )
{
}

Arguments::~Arguments(){
  if( out_ != &cout ) delete out_;
}

void Arguments::usage( const string& name, const string& defval,
		       const string& remark ) {

  string opt( "--" + name );
  if( defval != "" )
    opt += string( "=" + ( this->hasOpt( name ) ? this->getOpt( name ) : 
			   string( "[" + defval + "]" ) ) );
  this->log( LOG )
    << setiosflags( ios::left ) 
    << setw( 25 ) << opt 
    << remark
    << endl;

}

void Arguments::usage( const string& name, const int& defval,
		       const string& remark ) {
  ostringstream ostr;
  ostr << defval;
  this->usage( name, ostr.str(), remark );
}

void Arguments::usage( const string& name, const double& defval,
		       const string& remark ) {
  ostringstream ostr;
  ostr << defval;
  this->usage( name, ostr.str(), remark );
}

bool Arguments::hasOpt( const string& name ) {
  map< string, string >::iterator itr = opts_.find( name );
  return ( itr != opts_.end() ? true : false );
}

vector< string >::iterator Arguments::hasOpt( vector< string >& name ) {
  return this->hasOpt( name.begin(), name.end() );
}

vector< string >::iterator
Arguments::hasOpt( const vector< string >::iterator& itr1,
		   const vector< string >::iterator& itr2 ){
  for( vector< string >::iterator itr = itr1;
       itr != itr2;
       itr++ ){
    if( this->hasOpt( *itr ) ) return itr;
  }
  return itr2;
}

string Arguments::getOpt( const string& name )  {
  return ( this->hasOpt( name ) ? opts_[ name ] : Arguments::NO_OPTION );
}

void Arguments::option( const string& name, const string& value ){
  if( name == "" ) { 
    if( value != "" ) args_.push_back( value );
    return;
  }
  
  if( this->hasOpt( name ) ){
    cerr << "Option " << name << " was already given " 
	 << ( opts_[ name ] != "" ? 
	      string("as (" + opts_[ name ] + ")" ) : "" )<< endl;
    cerr << "       new value " << value << " is ignored" << endl;
    return ;
  }
  opts_[ name ] = value;
}

void Arguments::option( const string& data ){
  int pos = data.find( '=' );
  if( pos == string::npos ) this->option( data, Arguments::NO_VALUE );
  else this->option( data.substr( 0, pos ),
		     data.substr( pos + 1, data.size() - pos - 1 ) );
}

ostream& Arguments::log( const V_LEVEL& level ) { 
  if( level < verbose_ ) return null_;
  if( level > LOG ) *out_ << vlabel_[ level ] ;
  return *out_;
}

ostream& Arguments::err() { 
  return this->log( ERROR );
}

/*
bool Arguments::get( string option, const bool& def ){
  return ( this->hasOpt( option ) ? 
	 ( this->getOpt( option ) == "true" ? true : false ) : def );
}
*/

int Arguments::get( const string& option, const int& def ){
  string val = this->getOpt( option );
  return ( val ==  Arguments::NO_VALUE || val == Arguments::NO_OPTION ? 
	   def : atoi( val.c_str() ) );
}

double Arguments::get( const string& option, const double& def ){
  string val = this->getOpt( option );
  return ( val ==  Arguments::NO_VALUE || val == Arguments::NO_OPTION ? 
	   def : atof( val.c_str() ) );
}

string Arguments::get( const string& option, const string& def ){
  string val = this->getOpt( option );
  return ( val ==  Arguments::NO_VALUE || val ==  Arguments::NO_OPTION ? 
	   def : val );
}

complex< double > Arguments::get( const string& option,
				  const complex< double >& def ){
  if( ! this->hasOpt( option ) ) return def;
  istringstream ist( this->getOpt( option ) );
  complex< double > v;
  ist >> v;
  return v;
}

vector< string > Arguments::get( const string& option,
				 const string& sep,
				 const vector< string >& def ){
  if( ! this->hasOpt( option ) ) return def;
  string opt = this->getOpt( option );
  
  vector< string > val( 0 );
  size_t pos = 0;
  size_t pos2 = 0;
  do {
    pos2 = opt.find( sep, pos );
    int size = ( pos2 != string::npos ? pos2 : opt.size() ) - pos ; 
    val.push_back( opt.substr( pos, size ) );
    pos = pos2 + 1;
  } while( pos2 != string::npos );
  
  return val;  
}

vector< int > Arguments::get( const string& option,
				 const string& sep,
				 const vector< int >& def ){
  if( ! this->hasOpt( option ) ) return def;
  vector< string > opts = this->get( option, sep, vector< string >( 0 ) );
  vector< int > val( 0 );
  for( int i = 0; i < opts.size(); i++ )
    val.push_back( atoi( opts[ i ].c_str() ) );
  
  return val;  
}

vector< double > Arguments::get( const string& option,
				 const string& sep,
				 const vector< double >& def ){
  if( ! this->hasOpt( option ) ) return def;
  vector< string > opts = this->get( option, sep, vector< string >( 0 ) );
  vector< double > val( 0 );
  for( int i = 0; i < opts.size(); i++ )
    val.push_back( atof( opts[ i ].c_str() ) );
  
  return val;  
}

vector< complex< double > > Arguments::get( const string& option,
					    const string& sep,
					    const vector< complex< double > >& def ){
  if( ! this->hasOpt( option ) ) return def;
  vector< string > opts = this->get( option, sep, vector< string >( 0 ) );
  vector< complex< double > > val( 0 );
  for( int i = 0; i < opts.size(); i++ ){
    istringstream ist( opts[ i ] );
    complex< double > v;
    ist >> v;
    val.push_back( v );
  }
  return val;  
}

void Arguments::set( const string& option, const string& val ){
  opts_[ option ] = val;
}

void Arguments::set( const string& option, const int& val ){
  ostringstream ostr;
  ostr << val;
  this->set( option, ostr.str() );
}

void Arguments::set( const string& option, const double& val ){
  ostringstream ostr;
  ostr << val;
  this->set( option, ostr.str() );
}

void Arguments::set( const string& option, const complex< double >& val ){
  ostringstream ostr;
  ostr << val;
  this->set( option, ostr.str() );
}

namespace Utility {
  ostream& operator<<( ostream& os, const Arguments& args ){
    
    os << "# --------------------------------------------------------" << endl;
    os << "# " << args.command() << endl;
    os << "# " << endl;
    
    map< string, string > opts = args.options();
    
    for( map< string, string >::iterator itr = opts.begin();
	 itr != opts.end();
	 itr++ ){
      os << "#  --" << itr->first << flush;
      if( itr->second != Arguments::NO_VALUE ) os << "=" << itr->second << flush;
      os << endl;
    }
    
    vector< string > argv = args.arguments();
    for( int i = 0; i < argv.size(); i++ ){
      os << "#    " << argv[ i ] << endl;
    }
    
    os << "# " << endl;
    return os;
  }
}
