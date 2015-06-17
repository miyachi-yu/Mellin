#include "Application.hh"

using namespace std;

int main( int argc, char* argv[] ){

  try {
    Application app( argc, argv );
    app.Run();
  }
  
  catch ( int err ) {
    cerr 
      << __FILE__ << ":" << __LINE__ 
      << "\tcatch error (" << err << ")" << endl;
    return -1;
  }
  
  return 0;
}

