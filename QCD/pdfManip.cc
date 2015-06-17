//
// pdfManip.cc
//  
// Made by Yoshiyuki Miyachi
// Login   <miyachi@yoshi-desktop>
// 
// Started on  Wed Feb  4 18:12:17 2009 Yoshiyuki Miyachi
// Started on  Wed Feb  4 18:12:17 2009 Yoshiyuki Miyachi
//

#include <iostream>
#include <iomanip>
#include <list>
#include <cstdio>

#include "PdfParametrization.hh"
#include "ConfigPdfBase.hh"
#include "PdfBase.hh"
#include "PdfSimple.hh"

#include <Utility/Arguments.hh>

using namespace std;

void Usage(){

  Utility::Arguments& args = Utility::Arguments::ref();
  
  args.usage( "config", "", "parameterization xml file" );
  args.usage( "pdf",    "", "name of pdt to be modified" );
  args.usage( "index",  "", "index of parameter to be modified" );
  args.usage( "output", "input file", "output xml file name.");
  
  args.usage( "pid",   "", "ID of the specifed pdf" );
  args.usage( "src",   "", "source ID of the specifed pdf" );
  args.usage( "dfav",  "", "target favored fragmentation function ID" );
  args.usage( "fav",   "", "target disfavored ff (Kretzer gFF)" );
  
  args.usage( "name",   "", "name  of the specified Parameter" );
  args.usage( "value",  "", "value of the specified Parameter" );
  args.usage( "error",  "", "error of the specified Parameter" );
  args.usage( "state",  "", "state of the specified Parameter" );
  args.usage( "lower",  "", "lower limit of the specified Parameter" );
  args.usage( "upper",  "", "upper limit of the specified Parameter" );
  
}

void dump( QCD::Parameter& p ){
  
  ARG_LOG 
    << setw( 6 )  << p.name()
    << setw( 12 ) << p.value()
    << setw( 12 ) << p.error()
    << setw( 8 )  << p.state()
    << setw( 8 )  << p.lower()
    << setw( 8 )  << p.upper()
    << flush;
  
}

void dumpBaseInfo( QCD::PdfBase* pdf ){
  
  ARG_LOG 
    << setw( 16 ) << string( "name=\"" + pdf->name() + "\"" )
    << setw( 16 ) << string( "pid=\""  + pdf->pid() + "\"" )
    << setw( 16 ) << string( "src=\""  + pdf->src() + "\"" )
    << setw( 16 ) << string( "fav=\""  + pdf->fav() + "\"" )
    << setw( 16 ) << string( "dfav=\"" + pdf->dfav() + "\"" )
    << endl;
}

void dump( QCD::PdfBase* pdf ){
  
  if( pdf == NULL ) return;
  
  for( int j = 0; j < 80; j++ ) ARG_LOG << "-";
  ARG_LOG << endl;

  dumpBaseInfo( pdf );
  
  for( int j = 0; j < 80; j++ ) ARG_LOG << "-";
  ARG_LOG << endl;
  
  ARG_LOG
    << setw( 6 ) << "No."
    << setw( 6 )  << "name"
    << setw( 12 ) << "value"
    << setw( 12 ) << "error"
    << setw( 8 )  << "state"
    << setw( 8 )  << "lower"
    << setw( 8 )  << "upper"
    << endl;
  
  for( QCD::PdfBase::iterator itr = pdf->begin();
       itr != pdf->end();
       itr++ ){
    
    ARG_LOG << setw(6) << itr->first << flush;
    
    dump( itr->second );
    
    ARG_LOG << endl;
  }
}

void dump( const QCD::PdfParametrization* param ){
  
  vector< QCD::PdfBase* > pdfs = param->pdfs();
  for( int i = 0; i < pdfs.size(); i++ ){
    dump( pdfs[ i ] );
    ARG_LOG << endl;
  }
}

int main( int argc, char* argv[] ){

  Utility::Arguments& args = Utility::Arguments::ref( argc, argv );
  
  if( args.hasOpt( "help" ) ){
    Usage();
    return 0;
  }

  if( ! args.hasOpt( "config" ) ){
    Usage();
    
    ARG_ERR << "This requires --config option with Parameterization xml file."
	    << endl;
    
    return 0;
  }


  ARG_LOG << args << endl;
  
  string path = args.getOpt( "config" );
  
  QCD::PdfParametrization *param = NULL;
  
  try {
    QCD::ConfigPdfBase       config;
    config.parse( path );
    config.loadRootNode();
    param = new QCD::PdfParametrization( config );
  }
  
  catch( int error ){
    ARG_ERR << "catch error (" << error << ")" << endl;
    return 1;
  }
  
  if( param == NULL ){
    ARG_ERR << "Fail to create PdfParametrization object" << endl;
    return 2;
  }
  
  
  // ----------------------------------------------------------------- //
  map< string, list< int > > pdf;
  
  vector< string > pdfstr = args.get( "pdf", ",", vector< string >( 0 ) );
  for( int i = 0; i < pdfstr.size(); i++ ){
    string& str = pdfstr[ i ];
    string pdfname;
    int pos = str.find( ":" );
    pdfname = str.substr( 0, pos );
    if( pdf.find( pdfname ) == pdf.end() ){
      pdf[ pdfname ] = list< int >( 0 );
    }
    if( pos == string::npos ) continue;
    pdf[ pdfname ].push_back( atoi( str.substr( pos + 1 ).c_str() ) );
    pdf[ pdfname ].sort();
  }
  
  if( pdf.size() == 0 ) dump( param );
  
  for( map< string, list< int > >::iterator itr = pdf.begin();
       itr != pdf.end(); itr++ ){ 
    
    string name = itr->first;
    QCD::PdfBase *base = param->find( name );
    if( base == NULL ) {
      if( args.hasOpt( "add" ) ){
	ARG_LOG << "creating new PDF " << name << endl;
	QCD::PdfBase *newBase = new QCD::PdfSimple();
	param->pdfs().push_back( newBase );
	base = param->pdfs().back();
	base->type() = ( QCD::Flavor::isParton( name ) ? 
			 QCD::PdfBase::parton : QCD::PdfBase::combo );
	base->id()   =   QCD::Flavor::id( name );
      } else {
	ARG_WARN << "No such PDF found:" << name << endl;
	continue;
      }
    }
    
    if( args.hasOpt( "delete" ) && itr->second.size() == 0 ) {
      param->remove( name );
      continue;
    }
    
    base->pid()  = args.get( "pid", base->pid() );
    base->src()  = args.get( "src", base->src() );
    base->fav()  = args.get( "fav", base->fav() );
    base->dfav() = args.get( "dfav", base->dfav() );
    
    dump( base );
    
    if( itr->second.size() > 0 ) ARG_LOG << endl;
    
    for( list< int >::iterator jtr = itr->second.begin();
	 jtr != itr->second.end();
	 jtr++ ){
      
      int& index = *jtr;
      QCD::Parameter *p = param->find( name, index );
      if( p == NULL ){
	if( args.hasOpt( "add" ) ){
	  ARG_LOG << "creating new parameter " << index << endl;
	  p = &( (*base)[ index ] = QCD::Parameter() );
	  p->index() = index;
	} else {
	  ARG_WARN 
	    << "No such PDF found: " << name << ":" << index
	    << endl;
	  continue;
	}
      }
      
      p->name()  = args.get( "name",  p->name() );
      p->value() = args.get( "value", p->value() );
      p->error() = args.get( "error", p->error() );
      p->state( args.get( "state", p->state() ) );
      p->upper() = args.get( "upper", p->upper() );
      p->lower() = args.get( "lower", p->lower() );
      
      if( args.hasOpt( "delete" ) ){
	ARG_LOG << "deleating " << index << endl;
	base->erase( index );
      } else {
	ARG_LOG << "->" << setw(4) << p->index() ;
	dump( *p );
	ARG_LOG << endl;
      };
      
    }
    
  }
  
  if( args.hasOpt( "output" ) ){
    
    string ofilename = args.get( "output", path );
    
    rename( ofilename.c_str(), string( ofilename + ".back" ).c_str() );
    
    ofstream ofs( ofilename.c_str() );
    if( ! ofs ){
      ARG_ERR << "Fail to open " << args.get( "output", "pdf.xml" ) << endl;
    } else {
      ofs << *param << endl;
    }
  }
  
  delete param;
  return 0;
}
