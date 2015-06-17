#ifndef  _Exp_HERMES_HH_
#define  _Exp_HERMES_HH_

#include "DataParser.hh"

namespace Exp {
  
  class HERMES : public DataParser, public DataSet {
  public:
    
    HERMES( const Exp::TYPE&   type   = Exp::MULTI, 
	    const Exp::TARGET& target = Exp::PRO,
	    const Exp::HADRON& hadron = Exp::PIp  ) throw( int ) ;
    
    virtual ~HERMES();
    
  private:
    
    virtual bool parse_line( std::istream& is );
    
    virtual bool comment( const std::string& data );
    
  };
}

#endif //   _Exp_HERMES_HH_
