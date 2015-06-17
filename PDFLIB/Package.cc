#include "Package.hh"

extern "C" {
#include <limits.h>
#include <unistd.h>
}

#include <cstdlib>
#include <iostream>
#include <cmath>

using namespace std;
using namespace QCD;
using namespace PDFLIB;

Package::Package( const std::string& path ) : 
  RealFunction(),
  path_( "" ), q_( 1.0 ), x_( 0.5 ),
  parton_( Flavor::g ), 
  type_( Flavor::a0 ), 
  sfunc_( Flavor::g1p ),
  mode_( "PARTON" ),
  n_( 0.0 )
{

  vector< string > search( 0 );
  search.push_back( path );
  if( char* prefix = getenv( "MELLIN_PDFTAB_DIR" ) ){
    search.push_back( string( prefix ) + "/" + path );
  }

  char resolved_path[ PATH_MAX ];
  for( int i = 0; i < search.size(); i++ ){
    if( realpath( search[ i ].c_str(), resolved_path ) &&
	access( resolved_path, R_OK ) == 0 ){
      path_ = string( resolved_path );
      break;
    }
  }
  
  if( path_ == "" ){
    cerr << path << " not found." << endl;
    cerr << "Please check if the given table is properly installed." << endl;
  }
  
}

Package::Package( const Package& pdf ) :
  RealFunction(),
  path_( pdf.path_ ),
  q_( pdf.q_ ),
  x_( pdf.x_ ),
  parton_( pdf.parton_ ), 
  type_( pdf.type_ ), 
  sfunc_( pdf.sfunc_ ),
  mode_( pdf.mode_ ),
  n_( pdf.n_ )
{
}

Package::~Package() {
}

void Package::setQ2( const double& q2 ) {
  q_ = ( q2 > 0 ? sqrt( q2 ) : 1.0 );
}

void Package::load() {
  
  if( path_ == "" ){
    cerr << "No table directory specified." << endl; 
    return;
  }
  
  int length=2048;
  char buffer[ length ];
  
  if( ! getcwd( buffer, length ) ){
    std::cerr << "Fail to get current working directory name" << std::endl;
  }
  
  if( chdir( path_.c_str() ) ){
    std::cerr << "Fail to chdir to " << path_ << std::endl;
  } else {
    this->loadGrid();
    chdir( buffer );    // back to the current working directory
  }
  
}

double  Package::weight( const double& x ) const {
  return pow( x, n_ );
}

double Package::operator()( const double& x ) {
  x_ = x;
  this->load();
  if( mode_ == "PARTON" ) return this->parton();
  if( mode_ == "TYPE"   ) return this->type();
  return this->func();
}
