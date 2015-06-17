#include "Evolution.hh"
#include "P.hh"

#include <iostream>

#include <QCD/PdfParametrization.hh>
#include <QCD/ConfigFFbase.hh>
#include <QCD/HadronParam.hh>

#include <PDFSET/ConfigKretzerFF.hh>
#include <PDFSET/KretzerParam.hh>
#include <PDFSET/DSSFFParam.hh>
#include <PDFSET/ConfigDSSFF.hh>

using namespace std;
using namespace Utility;
using namespace QCD;
using namespace Fragmentation;

// static object 
Evo* Evo::pion_   = NULL;
Evo* Evo::kaon_   = NULL;
Evo* Evo::proton_ = NULL;
Evo* Evo::rest_   = NULL;

Evo* Evo::init_ff( const QCD::Flavor::HADRON& h ) throw( int ) {
  
  Utility::Arguments& args = Utility::Arguments::ref();
  
  vector< string > opts = Evo::options( h );
  
  vector< string >::iterator itr = args.hasOpt( opts );
  if( itr == opts.end() ) throw( 1 );
  
  Evo *evo = NULL; 
  try {
    evo = new Evo( args, h, *itr );
    evo->constNf( false );
  }
  
  catch( int error ){
    ARG_ERR << "catch error (" << error << ")" << endl;
    throw( error );
  }
  
  if( evo == NULL ){
    ARG_ERR << "Fail to create FF for hadron " << h << endl;
    throw( 1 );
  }
  
  return evo;
}


Evo* Evo::instance( const QCD::Flavor::HADRON& h ) throw( int ) {
  
  Utility::Arguments& args = Utility::Arguments::ref();
  
  // register atexit function call if needed.
  if( pion_ == NULL && kaon_ == NULL && proton_ == NULL && rest_ == NULL ){
    atexit( Evo::atExit );
  }
  
  try {
    
    switch( h ){
      
    case Flavor::PIp: 
    case Flavor::PIm: 
      if( pion_ == NULL ) pion_ = Evo::init_ff( h );
      return pion_;
    
    case Flavor::KAp: 
    case Flavor::KAm: 
      if( kaon_ == NULL ) kaon_ = Evo::init_ff( h );
      return kaon_;
      
    case Flavor::PRp: 
    case Flavor::PRm: 
      if( proton_ == NULL ) proton_ = Evo::init_ff( h );
      return proton_;
      
    case Flavor::REp: 
    case Flavor::REm: 
      if( rest_ == NULL ) rest_ = Evo::init_ff( h );
      return rest_;
      
    }
    
  }
  
  catch( int error ){
    ARG_ERR << "catch error (" << error << ")" << endl;
    throw( error );
  }
  
  ARG_ERR << "Hadron type: " << h << "has not been implemented." << endl;
  throw( 1 );
  return NULL;
}

void Evo::atExit(){
  if( pion_ )   delete pion_;
  if( kaon_ )   delete kaon_;
  if( proton_ ) delete proton_;
  if( rest_ )   delete rest_;
}

string Evo::base( const Flavor::HADRON& hadron ){
  
  string val( "FF" );
  
  switch( hadron ){
    
  case Flavor::KAp :
  case Flavor::KAm :
    val = val + "K";
    break;
    
  case Flavor::REp :
  case Flavor::REm :
    val = val + "R";
    break;
    
  case Flavor::PRp :
  case Flavor::PRm :
    val = val + "P";
    break;
    
  case Flavor::HAp :
  case Flavor::HAm :
    val = val + "H";
    break;

  default:
    break;
  }

  return val;
}

vector< string > Evo::options( const Flavor::HADRON& hadron ){
  
  /*
    if( hadron == Flavor::HAp || hadron == Flavor::HAm ){
    
    vector< string > pis = Evo::options( Flavor::PIp );
    vector< string > kas = Evo::options( Flavor::KAp );
    vector< string > pro = Evo::options( Flavor::PRp );
    vector< string > res = Evo::options( Flavor::REp );
    
    pis.insert( pis.end(), kas.begin(), kas.end() );
    pis.insert( pis.end(), pro.begin(), pro.end() );
    pis.insert( pis.end(), res.begin(), res.end() );
    
    return pis;
  }
  */
  
  string ff = Evo::base( hadron );
  
  vector< string > opts( 0 );
  opts.push_back( ff );
  opts.push_back( string( "Kretzer" + ff ) );
  opts.push_back( string( "DSS" + ff ) );
  
  return opts;
}


Evo::Evo( Arguments& args, 
	  const Flavor::HADRON& hadron, const string& option ) throw( int ) : 
  Kernel( args ),
  hadron_( hadron )
{
  string opt( option );
  
  vector< string > opts = Evo::options( hadron );
  if( option == "" ){
    vector< string >::iterator itr = args.hasOpt( opts ) ;
    if( itr != opts.end() ) opt = *itr;
  }
  
  if( ! args.hasOpt( opt ) ){
    args.err()
      << "given option (" << opt 
      << ")does not found in the arguments"
      << endl;
    throw 1;
  }
  
  if( args.getOpt( opt ) == "" ){
    args.err()
      << "--" << opt << " must have option value for an input XML file."
      << endl;
    throw 1;
  }
  
  try {
    PdfParametrization *init = Evo::create_pdf( hadron, opt );
    this->initialization( init );
  }
  catch( int error ) {
    ARG_ERR << "\tcatch error (" << error << ") !!" << endl;
    throw error;
  }
  
}

Evo::Evo( Utility::Arguments& args, 
	  QCD::PdfParametrization* init,
	  const QCD::Flavor::HADRON& hadron ) throw( int ) :
  Kernel( args ),
  hadron_( hadron )
{
  try {
    this->initialization( init );
  }
  catch( int error ) {
    ARG_ERR << "\tcatch error (" << error << ") !!" << endl;
    throw error;
  }
}

Evo::~Evo() {
  delete this->initialDistribution();
  delete this->anomalousDimension();  
}

PdfParametrization* Evo::create_pdf( const QCD::Flavor::HADRON& hadron,
				     const std::string& opt ) 
  throw( int ) {
  
  Utility::Arguments& args = Utility::Arguments::ref();
  
  PdfParametrization *init = NULL ;
  
  string ff = Evo::base( hadron );
  
  if( opt == ff ){
    
    ConfigFFbase config( opt );
    init = new PdfParametrization( config );

  } else if( opt == "Kretzer" + ff ){
    
    PDFSET::ConfigKretzerFF config( opt );
    init = new PDFSET::KretzerParam( config );
    
  } else if( opt == "DSS" + ff ){
    
    PDFSET::ConfigDSSFF config( opt );
    init = new PDFSET::DSSFFParam( config );
    
  } else {
    
    ARG_ERR
      << "option (--" << opt 
      << ") should be one of the following options:"
      << "\t--" << ff << "\n"
      << "\t--Kretzer" << ff << "\n"
      << "\t--DSS" << ff << endl;
    
    throw 2;
  }
  
  if( init == NULL ){
    ARG_ERR << "Fail to construct initial distribution" << endl;
    throw 3;
  }
  
  return init;
}

void Evo::initialization( PdfParametrization *init ) throw( int ){
  
  if( ! init->check() ) throw 4;
  
  Utility::Arguments& args = Utility::Arguments::ref();
  
  if( dynamic_cast< PDFSET::KretzerParam* >( init ) ){
    this->alpha().atMz( args.get( "alphaKreMz", 
				  ( this->alpha().scheme().order() 
				    == Scheme::NLO ? 0.114 : 0.125 ) ) );
  }
  
  this->initialDistribution( init );
  this->anomalousDimension( new P( this ) );
  
  // default behaviour of Fragmentation evolution is 
  // dynamic Nf generation.....
  this->constNf( false );
  
}
