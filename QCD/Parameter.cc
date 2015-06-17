#include "Parameter.hh"

#include <iomanip>
#include <sstream>

using namespace std;
using namespace QCD;

Parameter::Parameter() :
  emNumber_( -1 ),
  emid_( -1 ), index_( 0 ), 
  srcidsrc_( "" ), srcid_( 0 ),
  name_( "" ), 
  value_( 0.0 ), error_( 0.0 ), 
  upper_( 0.0 ), lower_( 0.0 ), 
  state_( "" ), 
  status_( FREE )
{
}

Parameter::Parameter( const Parameter& param ):
  emNumber_( param.emNumber_ ),
  emid_( param.emid_ ), index_( param.index_ ), 
  srcidsrc_( param.srcidsrc_ ), srcid_( param.srcid_ ),
  name_( param.name_ ), 
  value_( param.value_ ), error_( param.error_ ), 
  upper_( param.upper_ ), lower_( param.lower_ ), 
  state_( param.state_ ), 
  status_( param.status_ )
{
}

Parameter::~Parameter(){
}

Parameter& Parameter::operator=( const Parameter& param ){
  if( this != &param ){
    emNumber_ = param.emNumber_;
    emid_  = param.emid_ ;
    index_ = param.index_ ;
    srcidsrc_  = param.srcidsrc_ ;
    srcid_  = param.srcid_ ;
    name_  = param.name_ ;
    name_  = param.name_ ;
    value_ = param.value_ ;
    error_ = param.error_ ;
    upper_ = param.upper_ ;
    lower_ = param.lower_ ;
    state_ = param.state_ ;
    status_ = param.status_;
  }
  return *this;
}

namespace QCD {
  ostream& operator<<( ostream& os, Parameter& param ){
    
    os << "<parameter" << flush;
    if( ! ( param.emid_ < 0 ) ) {
      os << " emid=\""  << flush ;
      if( param.emNumber_ > 0 ) os << param.emNumber_ << "-" << flush;
      os << param.emid_    << "\"" << flush;
    }
    os << " index=\"" << param.index_ << "\"" << flush;
    os << " name=\""  << param.name_  << "\"" << flush;

    if( param.checkSrcid() ){
      os << " srcid=\""  << param.srcid_  << "\"" << flush;
    }

    os << " value=\"" << param.value_ << "\"" << flush;
    os << " error=\"" << param.error_ << "\"" << flush;
    os << " state=\"" << param.state_ << "\"" << flush;
    os << " upper=\"" << param.upper_ << "\"" << flush;
    os << " lower=\"" << param.lower_ << "\"" << flush;
    os << ">" << flush;
    
    if(  param.ems_.size() > 0 ) os << endl;
    for( int i = 0; i < param.ems_.size(); i++ ){
      os << "<ems emid=\"" << i 
	 << "\" value =\"" << param.ems_[ i ] << "\"/>" 
	 << endl;
    }
    
    os << "</parameter>" << flush;
    
    return os;
  }
}

void Parameter::status( const STATUS& status ) {
  
  switch( status_ = status ){
  case FREE:  state_ = "free";  break;
  case FIXED: state_ = "fixed"; break;
  case ADD:   state_ = "add";   break;
  case MULTI: state_ = "multi"; break;
  }

}

void Parameter::state( const std::string& state ) {
  
  state_ = state;
  if(      state_ == "fixed" ) { status_ = FIXED; }
  else if( state_ == "add" )   { status_ = ADD; }
  else if( state_ == "multi" ) { status_ = MULTI; }
  else {
    state_ = "free"; 
    status_ = FREE;
  }
  
}

int& Parameter::setSrcid( const std::string& val ){
  istringstream istr( val );
  if( istr >> srcid_ ){
    srcidsrc_ = val;
    // if this status is free, then chenge to fixed state
    // add and multi will be kept.
    if( state_ == "free" && status_ == FREE ) this->state( "fixed" );
  }
  return srcid_;
}
