#include "ExpData.hh"
#include "DataSet.hh"

using namespace std;
using namespace Utility;
using namespace Exp;

int main( int argc, char* argv[] ){
  
  Arguments& args = Arguments::ref( argc, argv );
  
  try{ 
    
    ExpData expd( args.get( "expdata", "test.xml" ) );
    
    cout << expd << endl;

    expd.summary( cout );
    
  }

  catch( int err ) {
  }

  return 0;
}
