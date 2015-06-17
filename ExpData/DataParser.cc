#include "DataParser.hh"

#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;
using namespace Exp;

#ifdef DATA_DIR_PATH
string DataParser::data_dir = DATA_DIR_PATH;
#else
string DataParser::data_dir = "../Data/";
#endif

DataParser::DataParser( ) throw( int ) :
  filename_( "" )
{
  
}

DataParser::~DataParser(){
}

bool DataParser::parse( const string& filename ){
  
  string path = data_dir + filename_;
  ifstream ifs( path.c_str() );
  if( ! ifs ){
    cerr << "Fail to open data file " << path << endl; 
    return false;
  } else {
    cout << "Loading " << path << endl;
  }
  
  string buffer;
  while( getline( ifs, buffer ) ){
    if( buffer.size() == 0 ) continue;
    if( buffer[ 0 ] == '*' ||
	buffer[ 0 ] == '#' ){
      this->comment( buffer );
    } else {
      istringstream istr( buffer.c_str() );
      this->parse_line( istr );
    }
  }  
  
  return true;
}

/*

bool DataParser::parse( std::istream& is ){
  
  istringstream& istr = dynamic_cast< istringstream& >( is );
  cout << istr.str() << endl;

  return true;
}

bool DataParser::comment( const std::string& data ){
  cout << data << endl;
  return true;
}
*/
