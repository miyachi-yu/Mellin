//
// DataParser.hh
// 
// Made by Yoshiyuki Miyachi
// Login   <miyachi@yoshi-desktop>
// 
// Started on  Mon Jan 19 17:59:34 2009 Yoshiyuki Miyachi
// Last update Mon Jan 19 17:59:34 2009 Yoshiyuki Miyachi
//

#ifndef _Exp_DATAPARSER_HH_
#define _Exp_DATAPARSER_HH_

#include "DataSet.hh"
#include <iostream>

namespace Exp {
  
  class DataParser {
  public:
    
    DataParser() throw( int ) ;
    virtual ~DataParser();
    
    virtual bool parse( const std::string& filename );
    
  private:
    
    virtual bool parse_line( std::istream& is ) = 0 ;
    
    virtual bool comment( const std::string& data ) = 0 ;
    
    static std::string data_dir;
    std::string        filename_;
    
  };
  
}

#endif // _Exp_DATAPARSER_HH_
