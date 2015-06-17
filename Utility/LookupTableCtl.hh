// $Id: LookupTableCtl.hh,v 1.1 2008/12/02 15:03:00 miyachi Exp $
/*!
 */
#ifndef __LookupTableCtl_hh__
#define __LookupTableCtl_hh__

#include <string>

namespace LookupTableCtl {

  enum ACCESS { read_only, read_write, disable, memory };
  
  //! access mode
  extern ACCESS actl;
  
  //! get directory path for data cache files
  extern std::string cacheDirPath(); //!< get cache directory

};

#endif // __LookupTableCtl_hh__
