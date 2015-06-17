//
// parprint.cc
//  
// Made by Yoshiyuki Miyachi
// Login   <miyachi@yoshi-desktop>
// 
// Started on  Tue Nov 18 11:23:58 2008 Yoshiyuki Miyachi
// Started on  Tue Nov 18 11:23:58 2008 Yoshiyuki Miyachi
//

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

#include "TableMaker.hh"

#include <Utility/Arguments.hh>
#include <QCD/PdfParametrization.hh>
#include <QCD/PdfErrMatrix.hh>
#include <ExpData/ExpData.hh>

using namespace std;

void Usage() {
  
  Utility::Arguments& args = Utility::Arguments::ref();
  
  ARG_LOG << "# " << args.command() << endl;
  ARG_LOG << "# Usage : " << endl;
  ARG_LOG << "  --pdf=[PDF or FF parametrization XML files]" << endl;
  ARG_LOG << "     (multiple files can be given with ,)" << endl;
  ARG_LOG << "  --expdata=[ExpData XML file]" << endl;
  ARG_LOG << "        ( --pdf or --expdata is required )" << endl;
  ARG_LOG << " (optional arguments)" << endl;
  ARG_LOG << "  --style=(text|tex|latex|html)    Table format" << endl;
  ARG_LOG << "  --precision=4                    Number fo Digit" << endl;
  ARG_LOG << "  --pdffile=pdfdata                filename for pdf" << endl;
  ARG_LOG << "  --datafile=expdata               filename for exp data" << endl;
  ARG_LOG << "  --emfile=emdata                  filename for em" << endl;
  ARG_LOG << "  --without-error                  without error" << endl;
  ARG_LOG << "  --nodisplay                      no display" << endl;
  
}

int main( int argc, char* argv[] ){
  
  Utility::Arguments& args = Utility::Arguments::ref( argc, argv );
  
  if( args.hasOpt( "help" ) ){
    
    Usage();
    
    ARG_LOG 
      << "# --------------------------------------------------- #\n"
      << args
      << "# --------------------------------------------------- #\n"
      << endl;
    return 0;
  }
  
  if( ! args.hasOpt( "pdf" ) && ! args.hasOpt( "expdata" ) ){
    Usage();
    return 1;
  }
  
  // ----------------------------------------------------- //
  //                 QCD::PdfParametrization               //
  //                 Exp::ExpData                          //
  // ----------------------------------------------------- //
  vector< QCD::PdfParametrization* > pdf( 0 );
  Exp::ExpData* edata = NULL;
  
  vector< string > path( 0 );
  path = args.get( "pdf", ",", path );
  
  try {
    
    QCD::PdfParametrization *q = NULL;
    for( int i = 0; i < path.size(); i++ ){
      QCD::ConfigPdfBase config;
      config.parse( path[ i ] );
      config.loadRootNode();
      if( q = new QCD::PdfParametrization( config ) ) pdf.push_back( q );
    }
    
    if( args.hasOpt( "expdata" ) ) edata = new Exp::ExpData( args );
  }
  
  catch( int error ){
    ARG_ERR << "catch error (" << error << ")" << endl;
    return 1;
  }
  
  //  if( edata ) edata->summary( ARG_LOG );

  if( pdf.size() == 0 && edata == NULL ){
    ARG_ERR << "Fail to create PDF nor ExpData" << endl;
    return 2;
  }
  
  // ----------------------------------------------------- //
  //                  QCD::PdfErrMatrix                    //
  // ----------------------------------------------------- //
  vector< QCD::PdfErrMatrix > vem;
  for( int i = 0; i < pdf.size(); i++ ){
    if( pdf[ i ]->emNumber() < 0 ) continue;
    vector< QCD::PdfErrMatrix >::iterator itr = 
      find( vem.begin(), vem.end(), QCD::PdfErrMatrix( pdf[ i ]->emNumber() ) );
    if( itr == vem.end() ){
      vem.push_back( QCD::PdfErrMatrix( pdf[ i ] ) );
    } else {
      itr->load( pdf[ i ] );
    }
  }
  
  if( edata && edata->emNumber() >= 0 ){
    vector< QCD::PdfErrMatrix >::iterator itr = 
      find( vem.begin(), vem.end(), QCD::PdfErrMatrix( edata->emNumber() ) );
    if( itr == vem.end() ){
      vem.push_back( QCD::PdfErrMatrix( edata ) );
    } else {
      itr->load( edata );
    }
  }
  
  // ----------------------------------------------------- //
  //              TableMaker configuration                 //
  // ----------------------------------------------------- //
  TableMaker tmaker;
  
  //     Table style
  string style = args.get( "style", "text" );
  if( style == "tex" || style == "latex" ) 
    tmaker.style( TableMaker::TEX );
  
  //     Precision
  int precision = args.get( "precision", 4 );
  tmaker.precision( precision );
  
  //     Table with error
  tmaker.withError( !args.hasOpt( "without-error" ) );
  
  // ----------------------------------------------------- //
  //                     Print Table                       //
  // ----------------------------------------------------- //
  //  PDF
  for( int i = 0; i < pdf.size(); i++ ) {
    ostringstream ostr;
    ostr << args.get( "pdffile", "pdfdata" ) ;
    if( pdf.size() > 1 ) ostr << "_" << i ;
    ostr << tmaker.suffix();
    cout << endl;
    cout << "  creating " << setw(24) << ostr.str() 
	 << " for " << setw(32) << pdf[ i ]->path()
	 << endl;
    
    ofstream ofs( ostr.str().c_str() );
    tmaker.print( ofs, pdf[ i ] );
    
    if( style == "text" ) {
      cout << endl;
      if( ! args.hasOpt( "nodisplay" ) ) tmaker.print( cout, pdf[ i ] );
    }
  }
  
  // ExpData
  if( edata ){
    ostringstream ostr;
    ostr << args.get( "datafile", "expdata" ) ;
    ostr << tmaker.suffix();
    cout << endl;
    cout << "  creating " << setw(24) << ostr.str() 
	 << " for " << setw(32) << edata->config().path()
	 << endl;
    
    ofstream ofs( ostr.str().c_str() );
    tmaker.print( ofs, edata );
    
    if( style == "text" ) {
      cout << endl;
     if( ! args.hasOpt( "nodisplay" ) ) tmaker.print( cout, edata );
    }
  }
  
  //  error matrix
  for( int i = 0; i < vem.size(); i++ ){
    tmaker.withError( false );
    if( vem[ i ].ID() < 0 ) continue;
    ostringstream ostr;
    ostr << args.get( "emfile", "emdata" ) ;
    if( vem.size() > 1 ) ostr << "_" << i ;
    ostr << tmaker.suffix();
    cout << "  creating " << ostr.str() << endl;
    ofstream ofs( ostr.str().c_str() );
    tmaker.print( ofs, &( vem[ i ] ) );
  }
  
  // ----------------------------------------------------- //
  //                     Delete objects                    //
  // ----------------------------------------------------- //
  for( int i = 0; i < pdf.size(); i++ ) if( pdf[ i ] ) delete pdf[ i ];
  if( edata ) delete edata;
  
  return 0;
};
