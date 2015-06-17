#include "A1hFit.hh"

#include <iostream>

using namespace std;
using namespace Utility;

int main( int argc, char* argv[] ){
  
  Arguments& args = *( Arguments::instance( argc, argv ) );
  
  try {

    args.set( "scaledirect", "" );
    
    vector< double > scale_( 2, 0.0 );
    vector< string > sname_( 2, "" );
    scale_[ 0 ] = args.get( "sfact1", 0.5 );
    sname_[ 0 ] = "scale1";
    scale_[ 1 ] = args.get( "sfact2", 2.0 );
    sname_[ 1 ] = "scale2";
    
    for( int i = 0; i < 2; i++ ){
      
      args.set( sname_[ i ], "" );
      if( ! args.hasOpt( "no_rescaleQ2" ) ) 
	args.set( "rescaleQ2", scale_[ i ] );
      if( ! args.hasOpt( "no_rescaleFF" ) ) 
	args.set( "rescaleFF", scale_[ i ] );
      if( ! args.hasOpt( "no_rescalePPDF" ) ) 
	args.set( "rescalePPDF", scale_[ i ] );
      if( ! args.hasOpt( "no_rescaleUPDF" ) ) 
	args.set( "rescaleUPDF", scale_[ i ] );
      
      Fit::A1hFit fit( args );
      fit.start();
      
    }
    
  }
  
  catch( int e ){
    cerr << __FILE__ << ":" << __LINE__ 
	 << "\tcatch error (" << e << ")" << endl;
  }
  
  return 0;
}
