#include "XSset.hh"

#include <TROOT.h>
#include <TApplication.h>

int main( int argc, char* argv[] ){
  
  Utility::Arguments& args = *( Utility::Arguments::instance( argc, argv ) );
  TApplication TA( "browser", &argc, argv ); //needed if you use root
  
  try {
    
    gROOT->SetStyle( "Plain" );
    
    XSset sets;

    string datafile;
    string titlename;

    while( cin >> datafile && datafile != "0" && cin >> titlename ){
      sets.insertdata( datafile, titlename );
    }

    double QQ2 = args.get( "Q2", 5.0 );
    bool cdraw = ( args.get( "cdraw", "false" ) == "false" ? false : true );  

    if( args.hasOpt( "sum" ) ){
      vector< double > vq2( 0 );
      //      vq2.push_back( 1.0 );
      //      vq2.push_back( 2.0 );
      vq2.push_back( 4.0 );
      vq2.push_back( 20.0 );
      vq2.push_back( 200.0 );

      sets.sumplot( vq2, cdraw );
      return 0;
    }
    sets.plot( QQ2, cdraw );

  }
  
  catch ( int error ) {
    cerr << __FILE__ << ":" << __LINE__ << "\tcatch error ("
         << error << ") !!" << endl;
  }
  
  return 0;
}
