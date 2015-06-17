#include "Evolution.hh"

#include <iostream>
#include <vector>
#include <string>

#include <QCD/PdfParametrization.hh>
#include <PDFSET/DSSVParam.hh>
#include <PDFSET/BBParam.hh>

#include "P.hh"

using namespace std;
using namespace Utility;
using namespace QCD;
using namespace polPDF;

Evo* Evo::pdf_ = NULL;

Evo* Evo::instance() throw( int ) {
  
  if( pdf_ ) return pdf_;
  
  Utility::Arguments& args = Utility::Arguments::ref();
  vector< string > opts = Evo::options();
  vector< string >::iterator itr = args.hasOpt( opts );
  
  if( itr == opts.end() ) {
    ARG_ERR << "check option for polPDF::Evo object" << endl;
    throw( 1 );
  }
  
  try{
    pdf_ = new Evo( args, *itr );
  }
  
  catch( int error ){
    ARG_ERR << "catch error (" << error << ")" << endl;
    throw( error );
  }
  
  if( pdf_ == NULL ){
    ARG_ERR << "error on creating polPDF::Evo" << endl;
    throw( 2 );
  }
  
  atexit( Evo::atExit ); // register at exit function
  return pdf_;
}

void Evo::atExit() {
  if( pdf_ ) delete pdf_;
}

vector< string > Evo::options(){
  vector< string > opts( 0 );
  opts.push_back( "polPDF" );
  opts.push_back( string( "BB" ) );
  opts.push_back( string( "DSSV" ) );
  return opts;
}

Evo::Evo( Arguments& args, const string& option ) throw( int ) : 
  Kernel( args )
{ 
  string opt( option );
  vector< string > opts = Evo::options();
  if( option == "" ){
    vector< string >::iterator itr;
    if( ( itr = args.hasOpt( opts ) ) != opts.end() ) opt = *itr;
  }
  
  if( opt == "" ) {
    ARG_ERR << "Requires one of follwing options" << endl;
    for( int i = 0; i < opts.size(); i++ ) ARG_ERR << "\t--" << opts[i] << endl;
    throw( 1 );
  }
  
  try {
    
    PdfParametrization *init = NULL;
    
    if( args.hasOpt( opt ) ) {
      
      if( opt == "polPDF" ){
	ConfigPdfBase config( opt );
	init = new PdfParametrization( config );
      } else if( opt == "DSSV" ){
	PDFSET::ConfigDSSV config( opt );
	init = new PDFSET::DSSVParam( config );
	this->alpha().atMz( args.get( "alphaDSSVMz", 
				      ( this->alpha().scheme().order() 
					== Scheme::NLO ? 0.1195 : 0.125 ) ) );
	// ref. PRL 101,072001 DSSV08 and hep-ph/0211080 MRST2002
      } else if( opt == "BB" ) {
	ConfigPdfBase config( opt );
	init = new PDFSET::BBParam( config );
	this->alpha().atMz( args.get( "alphaBBMz", 
				      ( this->alpha().scheme().order() 
					== Scheme::NLO ? 0.114 : 0.125 ) ) );
      }
      
      if( ! init->check() ) throw 2;
    }
    
    if( init == NULL ) {
      ARG_ERR << "Fail to create Initial Parameterization object" << endl;
      throw 3;
    }
    
    this->initialDistribution( init );
    this->anomalousDimension( new P( this ) );
    this->constNf( true );
  }
  
  catch( int error ) {
    cerr << __FILE__ << ":" << __LINE__ << "\tcatch error ("
	 << error << ") !!" << endl;
    throw error;
  }
  
}

Evo::~Evo() {
  delete this->initialDistribution();
  delete this->anomalousDimension();  
}

