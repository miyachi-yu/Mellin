#include "ALLM97.hh"
#include "Utility/Arguments.hh"
#include <iostream>


int main( int argc, char* argv[] ){

  Utility::Arguments& args = Utility::Arguments::ref( argc, argv );
  
  StrFncParam::ALLM97 allm97;

  double w2 = args.get( "W2", 10.0 );
  double q2 = args.get( "Q2", 10.0 );
  
  std::cout << "ALLM97(" << w2 << ", " << q2 << ") = "
	    << allm97( w2, q2 ) << std::endl;
		       
  

  return 0;
}
