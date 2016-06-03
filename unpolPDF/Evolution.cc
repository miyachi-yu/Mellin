#include "Evolution.hh"

#include <iostream>
#include <algorithm>

#include <QCD/PdfParametrization.hh>

#include <PDFSET/ConfigMRST.hh>
#include <PDFSET/MRSTParam.hh>
#include <PDFSET/MSTWParam.hh>

#include <PDFSET/ConfigStat.hh>
#include <PDFSET/StatParam.hh>

#include <PDFSET/ConfigCteq6.hh>
#include <PDFSET/Cteq6Param.hh>

#include "P.hh"

using namespace std;
using namespace Utility;
using namespace QCD;
using namespace Evolution;
using namespace unpolPDF;

Evo* Evo::pdf_ = NULL;

Evo* Evo::instance() throw( int ) {

  if( pdf_ ) return pdf_;
  
  try{
    pdf_ = new Evo( Utility::Arguments::ref() );
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

  opts.push_back( "unpolPDF" );
  opts.push_back( "MRST"  );
  opts.push_back( "MSTW"  );
  opts.push_back( "CTEQ"  );
  opts.push_back( "Stat"  );
  
  return opts;
}


Evo::Evo( Arguments& args, const string& option ) throw( int ) : 
  Kernel( args )
{
  vector< string > opts = Evo::options();
  vector< string >::iterator itr;
  itr = ( option == string( "" ) ? 
	  args.hasOpt( opts ) : 
	  find( opts.begin(), opts.end(), option ) );
  if( itr == opts.end() ){
    ARG_ERR << "Requires one of follwing options" << endl;
    for( int i = 0; i < opts.size(); i++ ) 
      ARG_ERR << "\t--" << opts[i] << endl;
    throw( 1 );
  }


  string& opt = *itr;

  try {
    
    PdfParametrization *init = NULL; 
    
    if( opt == "unpolPDF" ){      // ------------------- standard PDF
      ConfigPdfBase config( "unpolPDF" );
      init = new PdfParametrization( config );
      
    } else if( opt == "MRST" ){   // ------------------- MRST
      
      PDFSET::ConfigMRST config( "MRST" );
      init = new PDFSET::MRSTParam( config );
      
      string version = config.name();
      ARG_LOG << version << " was loaded." << endl;
      double atMz    = 0.1175;
      if( version == "MRST2001" ){
	atMz = 0.1190;
      } else if( version == "MRST1998" ){
	atMz = 0.1175;
      } else {
	ARG_LOG << "CAUSION: name is usually MRST1998 or MRST2001." << endl;
	ARG_LOG << "         please checkd name in XML file." << endl;
      }
      
      this->alpha().atMz( args.get( "MRSTMz", atMz ) );
      ARG_LOG << "MRST PDF: alpha_s( Mz ) = " << atMz << endl;
      
      this->alpha().flavor().mass[ 3 ] = args.get( "MRSTMc", 1.4 );
      ARG_LOG << "MRST PDF: charm mass = " 
	      << this->alpha().flavor().mass[ 3 ] << endl;
      
    } else if( opt == "MSTW" ){
      
      PDFSET::ConfigMRST config( opt );
      init = new PDFSET::MSTWParam( config );
      
      double atMz = ( this->alpha().scheme().order() == Scheme::NLO ?
		      0.12018 : 0.13939 );
      
      this->alpha().atMz( args.get( "MSTWMz", atMz ) );
      ARG_LOG << "MSTW PDF: alpha_s( Mz ) = " << atMz << endl;
      
      this->alpha().flavor().mass[ 3 ] = args.get( "MSTWMc", 1.4 );
      ARG_LOG << "MSTW PDF: charm mass = " 
	      << this->alpha().flavor().mass[ 3 ] << endl;
      
    } else if( opt == "CTEQ" ){
      
      if( ! args.hasOpt( "forceLO" ) )
	if( ! this->alpha().scheme().order() == Scheme::NLO )
	  throw( 1 );
      
      double atMz = 0.118;
      
      PDFSET::ConfigCteq6 config( opt );
      init = new PDFSET::Cteq6Param( config );
      
      this->alpha().atMz( args.get( "CTEQMz", atMz ) );
      ARG_LOG << "CTEQ PDF: alpha_s( Mz ) = " << atMz << endl;
      
    } else if( opt == "Stat" ) {
      
      this->alpha().atMz( args.get( "StatMz", 0.119 ) );
      ARG_LOG << "Statistical Model PDF: alpha_s( Mz ) = "
	      << this->alpha().atMz() << endl;

      ARG_LOG << "Statistical Model PDF initiation" << endl;
      
      PDFSET::ConfigStat config( opt );
      init = new PDFSET::StatParam( config );
      this->alpha().scheme().order() = Scheme::NLO; // force to NLO
      
    }

    if( ! init->check() ) throw 2;
    
    if( ! args.hasOpt( "forceLO" ) &&
	! this->alpha().scheme().order() == Scheme::NLO ){
      throw( 1 );
    }
    
    this->initialDistribution( init );
    this->anomalousDimension( new P( this ) );
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

