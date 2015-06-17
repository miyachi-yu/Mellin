#include "LookupTableCtl.hh"

#include <cstdlib>
extern "C" {
#include <sys/stat.h>
#include <sys/types.h>
}


using namespace std;

namespace LookupTableCtl {

  ACCESS actl = read_only;

  string cacheDirPath() {
    
    // if MELLIN_CACHE_DIR is set, use the defined directory
    if( char* path = getenv( "MELLIN_CACHE_DIR" ) ){
      mkdir( path, S_IRUSR | S_IWUSR | S_IXUSR );
      return path;
    }
    
    // if not, common cache directory  ~/mellin/cache
    std::string dir(  getenv( "HOME" ) ) ;
    mkdir( ( dir += "/mellin" ).c_str(), S_IRUSR | S_IWUSR | S_IXUSR );
    mkdir( ( dir += "/cache"  ).c_str(), S_IRUSR | S_IWUSR | S_IXUSR );
    
    return dir;
  }
  
};

