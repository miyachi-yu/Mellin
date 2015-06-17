#include "Browser.hh"

int main( int argc, char* argv[] ){
  
  Browser *browser = Browser::instance( argc, argv );
  browser->Run();
  
  return 0;
}
