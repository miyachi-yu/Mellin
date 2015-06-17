#include "scaleFFset.hh"

#include <TROOT.h>
#include <TApplication.h>

int main( int argc, char* argv[] ){
  
  Utility::Arguments& args = *( Utility::Arguments::instance( argc, argv ) );
  TApplication TA( "browser", &argc, argv ); //needed if you use root
  
  try {
    
    gROOT->SetStyle( args.get( "gstyle", "Plain" ).c_str() );
    gROOT->SetBatch( args.hasOpt( "nodisplay" ) ); 
    // or simply set '--b' for the batch mode 
    // (become active through TApplication)
    
    scaleFFset sets;

    string xmlfile;
    double scale;

    if( args.hasOpt( "rchi2" ) ){
      vector< string > vfile( 0 );
      vfile = args.get( "rchi2", ",", vfile );
      if( vfile.size() == 1 ){
	sets.plotchi2( vfile[ 0 ] );
	return 0;
      } else {
	sets.plotchi2( vfile );
	return 0;
      }
    }

    while( cin >> xmlfile && xmlfile != "" && cin >> scale ){
      sets.insertxml( xmlfile, scale );
    }

    if( args.hasOpt( "rflavor" ) ){
      sets.plot( args.get( "rN", 2 ), args.get( "rQ2", 1.0 ), 
		 args.get( "rflavor", 0 ) );
      return 0;
    }

    if( args.hasOpt( "sumtable" ) ){
      sets.sum_table( args.get( "rN", 2 ) );
      return 0;
    }

  }
  
  catch ( int error ) {
    cerr << __FILE__ << ":" << __LINE__ << "\tcatch error ("
         << error << ") !!" << endl;
  }
  
  return 0;
}
