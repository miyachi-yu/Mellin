#include <Utility/Arguments.hh>

#include "PdfBase.hh"
#include "PdfParametrization.hh"

using namespace std;
using namespace Utility;
using namespace QCD;


int main( int argc, char* argv[] ){
  
  Arguments *args = Arguments::instance( argc, argv );
  
  /*
  ConfigPdf config( args );
  cout << "*****************************************************************"
       << endl;
  cout << config << endl; 
  cout << "*****************************************************************"
       << endl;
  
  PdfParametrization param;
  param <<  config;
  
  cout <<  param << endl;
  
  vector< PdfBase* > pdfs = param.pdfs();
  */

  return 0;
}
