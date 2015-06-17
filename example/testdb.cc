#include <Utility/LookupTable.hh>
#include <Tranform/CKey.hh>
#include <iostream>

#include <cstdlib>
#include <ctime>

using namespace std;
using namespace Transform;

extern "C" {
#include <sys/time.h>
}

double get_random(){
  return ( 1.0 * random() ) / RAND_MAX ;
}

int main( int argc, char* argv[] ){
  
  CKey key;
  complex< double > data;
  
  srandom( gettimeofday( NULL, NULL ) );
  
  try {
    Utility::LookupTable< CKey, complex< double > > ltable( key, data, "test.db" );
    for( int i = 0; i < 100; i++ ){
      key  = complex< double >( get_random(), get_random() );
      data = complex< double >( get_random(), get_random() );
      cout << "Put:  " << key << "\t" << ltable.put() << endl;
    }
  }
  
  catch ( std::exception& e ){
    cout << "Exception:\t" << e.what() << endl; 
  }

  return 0;
}
