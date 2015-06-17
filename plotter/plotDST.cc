#include "FFset.hh"

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
    
    FFset sets;

    string datafile;
    string titlename;

    while( cin >> datafile && datafile != "0" && cin >> titlename ){
      sets.insertdata( datafile, titlename );
    }

    double QQ2 = args.get( "Q2", 5.0 );
    bool cdraw = ( args.get( "cdraw", "false" ) == "false" ? false : true );  

    if( args.hasOpt( "flavor" ) ){
      sets.plot( QQ2, atoi( args.getOpt( "flavor" ).c_str() ), cdraw );
      return 0;
    }

    if( args.hasOpt( "sumplot" ) )
      { sets.sumplot( QQ2 ); return 0; }

    if( args.hasOpt( "ratio" ) && args.hasOpt( "ratiobase" ) ){ 
      vector< int > rflav = args.get( "ratio", ",", vector< int >( 0 ) );
      int bflav = args.get( "ratiobase", 100 );
      if( rflav.size() && bflav != 100 )
	sets.ratio( QQ2, rflav, bflav );
      return 0; 
    }
    
    for( int iFlav = -5 ; iFlav <= 5; iFlav++ )
      sets.plot( QQ2, iFlav, cdraw );
    
  }
  
  catch ( int error ) {
    cerr << __FILE__ << ":" << __LINE__ << "\tcatch error ("
         << error << ") !!" << endl;
  }
  
  return 0;
}
