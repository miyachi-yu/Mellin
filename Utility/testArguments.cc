#include "Arguments.hh"
#include <sstream>
#include <iomanip>


using namespace std;

int main( int argc, char* argv[] ){
  
  Utility::Arguments& args = *( Utility::Arguments::instance( argc, argv ) );
  
  ARG_LOG << args << endl;
  int level;
  if( args.hasOpt( "level" ) ){
    istringstream istr( args.getOpt( "level" ) );
    istr >> level;
    ARG_LOG << "--level=" << level << " was found." << endl;
  }
  
  ARG_LOG 
    << "-----------------------------------------------------------------\n"
    << "Test of multiple values in one argument. \n"
    << "  --files  :  requires more than one options separated by space. \n"
    << endl;
  
  vector< string > files = args.get( "files", " ", vector< string >( 0 ) );
  for( int i = 0; i < files.size(); i++ ){
    ARG_LOG 
      << "--files: [" << setw(3) << i << "]  "
      << files[ i ] 
      << endl;
  }

  
  // adding or overwriting the argument list

  ARG_LOG << "Test Argument::set method" << endl;
  
  args.set( "input", "config.xml" );
  args.set( "n", 1 );
  args.set( "Q2", 1.0  );
  args.set( "N",  complex< double >( 1.0, 1.0 ) );
  
  ARG_LOG << args << endl;

  // test of error log
  ARG_ERR << "error log" << endl;

  return 0;
}
