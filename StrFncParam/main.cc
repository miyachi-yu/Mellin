#include "ALLM91.hh"
#include "ALLM97.hh"
#include "DoLara.hh"
#include "F1Param.hh"
#include "F2Param.hh"
#include "R1990.hh"
#include "R1998.hh"
#include "F2pSMC.hh"
#include "F2nSMC.hh"
#include "F2dSMC.hh"

#include <iostream>
#include <iomanip>
#include <cmath>

using namespace std;


int main( int argc, char* argv[] ){
  
  StrFncParam::ALLM91 al91;
  StrFncParam::ALLM97 al97;
  StrFncParam::DoLara dl07;
  StrFncParam::R1990  r1990;
  StrFncParam::R1998  r1998;
  
  StrFncParam::F2pSMC  f2psmc;
  StrFncParam::F2nSMC  f2nsmc;
  StrFncParam::F2dSMC  f2dsmc;
  
  StrFncParam::F2Param f2al91( al97, r1998 );
  StrFncParam::F1Param f1al91( al97 );
  
  double q2 = 2.0;
  
  cout 
    << setw(10) << "W^2"
    << setw(5) << "Q^2"
    << setw(12) << "ALLM91"
    << setw(12) << "ALLM97"
    << setw(12) << "DL2007"
    << setw(12) << "x"
    << setw(12) << "F1"
    << setw(12) << "R1990"
    << setw(12) << "R1998"
    << setw(12) << "F2"
    << setw(12) << "F2pSMC"
    << setw(12) << "F2nSMC"
    << setw(12) << "F2dSMC"
    << endl;
  
  for( double lw2 = 1.0; lw2 < 5.0; lw2 += 0.1 ){ 

    double w2 = pow( 10.0, lw2 );
    double x  = al91.x( w2 );
    
    cout 
      << setw(10) << w2
      << setw(5)  << q2
      << setw(12) << al91( w2, q2 ) 
      << setw(12) << al97( w2, q2 ) 
      << setw(12) << dl07( w2, q2 ) 
      << setw(12) << x
      << setw(12) << f1al91( x, q2 )
      << setw(12) << r1990( x, q2 )
      << setw(12) << r1998( x, q2 )
      << setw(12) << f2al91( x, q2 )
      << setw(12) << f2psmc( x, q2 )
      << setw(12) << f2nsmc( x, q2 )
      << setw(12) << f2dsmc( x, q2 )
      << endl;
    
  }
  
  for( double lq2 = 0; lq2 < 2; lq2 += 0.2 ){
    double q2 = pow( 10.0, lq2 );
    double x = 0.015;
    cout << "R(" << x << ", " << setw(10) << q2 << ") = "
	 << setw(12) << r1990( x, q2 )
	 << " +- "
	 << setw(12) << r1990.error( x, q2 )
	 << setw(12) << r1998( x, q2 )
	 << " +- "
	 << setw(12) << r1998.error( x, q2 )
	 << endl;
  }
  


  return 0;
}
