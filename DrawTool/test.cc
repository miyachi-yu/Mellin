#include <iostream>
#include <iomanip>
#include <string>

#include <TRint.h>


#include <Utility/Arguments.hh>

using namespace std;

class MyApp : public TRint {
public:
  
  MyApp( int& argc, char* argv[] ) throw( int ) : 
    TRint( "MyApp", &argc, argv )
  {
  }
  
  virtual ~MyApp() {
  }
  
private:

};



int main( int argc, char* argv[] ){
  
  Utility::Arguments& args = Utility::Arguments::ref( argc, argv );

  try {
    MyApp rint( argc, argv );
    rint.Run();
  } catch ( int e ){
  }

  return 0;
}
