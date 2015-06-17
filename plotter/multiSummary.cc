#include "MultiFF.hh"
#include <TROOT.h>

using namespace std;
int main( int argc, char* argv[] ){
  Utility::Arguments& args = Utility::Arguments::ref( argc, argv );
  gROOT->SetStyle( args.get( "gstyle", "Plain" ).c_str() );

  try {
    MultiFF mp( argc, argv );
    mp.Run();
  } catch( int e ) {
    args.err() << "catch error (" << e << ")" << endl;
  }
  return 0;
}
