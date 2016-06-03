#include <Utility/Arguments.hh>

#include "ConfigStat.hh"
#include "StatParam.hh"
#include "PdfStat.hh"

#include <iomanip>
#include <iostream>

using namespace std;

int main( int argc, char* argv[] ){

  Utility::Arguments& args = Utility::Arguments::ref( argc, argv );

  if( ! args.hasOpt( "Stat" ) ){
    cerr << "Specify Statistical Model PDF parameterization XML file with --Stat option" << endl;
    return 1;
  }

  
  PDFSET::ConfigStat conf( "Stat" );
  PDFSET::StatParam  param;
  param << conf;

  if( ! param.check() ){
    cerr << "Missing information" << endl;
    return 1;
  }

  
  vector< QCD::PdfBase* > pdfs = param.pdfs();

  cout
    << setw(3) << "id"
    << setw(6) << "name"
    << setw(8) << "A"
    << setw(8) << "b"
    << setw(8) << "A~"
    << setw(8) << "b~"
    << setw(8) << "X+"
    << setw(8) << "X-"
    << setw(8) << "x-"
    << setw(16) << "1st"
    << setw(16) << "2nd"
    << endl;
  
  for( int i = 0; i < pdfs.size(); i++ ){
    
    PDFSET::PdfStat* p = dynamic_cast< PDFSET::PdfStat* >( pdfs[ i ] );
    if( p == NULL ) continue;

    //    cout << "Call PdfStat::update() for " << p->name() << endl;
    p->update();
    
    cout << setw(3) << i
	 << setw(6) << p->name()
	 << setw(4) << p->size()
	 << setw(8) << p->A1()
	 << setw(8) << p->b1()
	 << setw(8) << p->A2()
	 << setw(8) << p->b2()
	 << setw(8) << p->Xp()
	 << setw(8) << p->Xm()
	 << setw(8) << p->xb()
	 << setw(16) << (*p )( complex< double >( 1.0, 0.0 ) )
	 << setw(16) << (*p )( complex< double >( 2.0, 0.0 ) )
	 << flush;
    
    cout << endl;
    
  }
  
  return 0;
}
