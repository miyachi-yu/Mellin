#include "HERMES.hh"

#include <iostream>

using namespace std;
using namespace Exp;

HERMES::HERMES( const Exp::TYPE& type,
		const Exp::TARGET& target,
		const Exp::HADRON& hadron  ) throw( int ) : 
  DataParser(), DataSet() {
  
  string datafile;
  
  switch( type ){
    
  case MULTI:
    break;
    
  case A1:
    break;
    
  default:
    throw 1;
    
  }
  
  this->parse( datafile );
  
}

HERMES::~HERMES(){
  
}

bool HERMES::parse_line( std::istream& is ){

  
  return true;
}

bool HERMES::comment( const std::string& data ){
  

  return true;
}    

