#include "Arguments.hh"
#include "Config.hh"
#include <sstream>

using namespace std;

int main( int argc, char* argv[] ){
  
  Utility::Arguments *args = 
    Utility::Arguments::instance( argc, argv );

  Utility::Config config( *args, "config" );
  cout << config << endl;
  
  return 0;
}

