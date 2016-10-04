#include "ALLM91.hh"
#include "ALLM97.hh"
#include "DoLara.hh"
#include "Utility/Arguments.hh"
#include <iostream>


int main( int argc, char* argv[] ){

  Utility::Arguments& args = Utility::Arguments::ref( argc, argv );
  
  StrFncParam::ALLM91 allm91;
  StrFncParam::ALLM97 allm97;
  StrFncParam::DoLara dolara;

  double w2 = args.get( "W2", 10.0 );
  double q2 = args.get( "Q2", 10.0 );
  
  std::cout << "ALLM91(" << w2 << ", " << q2 << ") = " << allm91( w2, q2 ) << std::endl;
  std::cout << "ALLM97(" << w2 << ", " << q2 << ") = " << allm97( w2, q2 ) << std::endl;
  std::cout << "DoLara(" << w2 << ", " << q2 << ") = " << dolara( w2, q2 ) << std::endl;
		       
  

  return 0;
}
