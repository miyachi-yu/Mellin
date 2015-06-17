#include "ExpData.hh"
#include "DataSet.hh"

using namespace std;
using namespace Utility;
using namespace Exp;

int main( int argc, char* argv[] ){
  
  Arguments *args = Arguments::instance( argc, argv );
  
  try{ 
    
    ExpData expd;
    
    cout << expd << endl;
    
  }

  catch( int err ) {
  }

  return 0;
}
