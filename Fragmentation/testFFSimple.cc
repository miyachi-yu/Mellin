#include <Utility/Arguments.hh>

#include <QCD/PdfParametrization.hh>
#include <QCD/ConfigFFbase.hh>
#include <QCD/PdfBase.hh>

#include <PDFSET/KretzerParam.hh>
#include <PDFSET/ConfigKretzerFF.hh>

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>

using namespace std;
using namespace Utility;
using namespace QCD;

int main( int argc, char* argv[] ){
  
  Arguments& args = Arguments::ref( argc, argv );

  PdfParametrization *ffs = NULL;
  if( args.hasOpt( "FF" ) ) {
    ConfigFFbase config( "FF" );
    ffs = new PdfParametrization( config );
  } else if( args.hasOpt( "KretzerFF" ) ){
    PDFSET::ConfigKretzerFF config( "KretzerFF" );
    ffs = new PDFSET::KretzerParam( config ) ;
  }

  if( ffs == NULL ){
    return 1;
  }
  
  //  complex< double > N( 3.0, 0.6 );
  //  complex< double > N( 0.592754, -0.0317442 );
  complex< double > N( 10.1, -3.13764 );
  vector< PdfBase* > pdfs = ffs->pdfs();
  for( int i = 0; i < pdfs.size(); i++ ){
    cout << setw(2) << i 
	 << setw(18) << ( * pdfs[i] )( N )
	 << endl;
  }
  
  string ofile = args.get( "output", "output.xml" );
  ofstream ofs( ofile.c_str() );

  cout << "---------------------------------------------------------------" << endl;
  cout << "--  FF will be wrote as well as in " << ofile << endl;
  cout << *ffs << endl;
  ofs << *ffs << endl;
  cout << "---------------------------------------------------------------" << endl;

  vector< vector< double > > em = ffs->errorMatrix();
  for( int i = 0; i < em.size(); i++ ){
    if( i == 0 ){
      cout << setw(3) << "#" << flush;
      for( int j = 0; j < em.size(); j++ ) cout << setw(13) << j << flush;
      cout << endl;
    }
    cout << setw(3) << i << flush;
    for( int j = 0; j < em.size(); j++ ) 
      cout << setw(13) << em[ i ][ j ] << flush;
    cout << endl;
  }

  if( ffs ) delete ffs;

  return 0;
}
