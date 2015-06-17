#include "GeneralHarmonic.hh"
#include "GeneralHarmonicPrime.hh"
#include "GeneralHarmonicTilde.hh"
#include <iostream>
#include <iomanip>

using namespace std;
using namespace Transform;

int main( int argc, char* argv[] ){
  
  GeneralHarmonic cs;
  GeneralHarmonicPrime csp;
  GeneralHarmonicTilde cst;


  cout << "*************************************************************" 
       << endl;
  cout 
    << "*************************************************************\n" 
    << "* S                                                         *\n"
    << "*************************************************************\n" 
    << endl;
  
  cout 
    << setw(18) << "n"
    << setw(18) << "S_1(n)"
    << setw(18) << "S_2(n)"
    << setw(18) << "S_3(n)"
    << setw(18) << "S_{1,2}(n)"
    << setw(18) << "S_{2,1}(n)"
    << endl;
    
  for( int i = 0; i < 100; i++ ){
    complex< double > cn( 0.1 * i , 0.0 );
    cout << setw(18) << cn << flush;
    for( int j = 1; j <= 3; j++ ) cout << setw(18) << cs( j, cn ) << flush;
    cout << setw(18) << cs( 1, 2, cn ) << flush;
    cout << setw(18) << cs( 2, 1, cn ) << flush;

    cout << endl;
  }
  
  cout 
    << "*************************************************************\n" 
    << "* S' : eta = +1                                             *\n"
    << "*************************************************************\n" 
    << endl;
  
  cout 
    << setw(18) << "n"
    << setw(18) << "S'_1(n)"
    << setw(18) << "S'_2(n)"
    << setw(18) << "S'_3(n)"
    << endl;
    
  for( int i = 0; i < 100; i++ ){
    complex< double > cn( 0.1 * i , 0.0 );
    cout << setw(18) << cn << flush;
    for( int j = 1; j <= 3; j++ ) 
      cout << setw(18) << csp( j, cn, +1 ) << flush;
    cout << endl;
  }
  
  cout 
    << "*************************************************************\n" 
    << "* S' : eta = -1                                             *\n"
    << "*************************************************************\n" 
    << endl;
  
  cout 
    << setw(18) << "n"
    << setw(18) << "S'_1(n)"
    << setw(18) << "S'_2(n)"
    << setw(18) << "S'_3(n)"
    << endl;
    
  for( int i = 0; i < 100; i++ ){
    complex< double > cn( 0.1 * i , 0.0 );
    cout << setw(18) << cn << flush;
    for( int j = 1; j <= 3; j++ ) 
      cout << setw(18) << csp( j, cn, -1 ) << flush;
    cout << endl;
  }
  
  cout 
    << "*************************************************************\n" 
    << "* S~ : eta = +1                                             *\n"
    << "*************************************************************\n" 
    << endl;
  
  cout 
    << setw(18) << "n"
    << setw(18) << "S~_1(n)"
    << setw(18) << "S~_2(n)"
    << setw(18) << "S~_3(n)"
    << setw(18) << "S~(n)"
    << setw(18) << "S~_{1,2}(n)"
    << endl;
    
  for( int i = 0; i < 100; i++ ){
    complex< double > cn( 0.1 * i , 0.0 );
    cout << setw(18) << cn << flush;
    for( int j = 1; j <= 3; j++ ) cout << setw(18) << cst( j, cn, +1 ) << flush;
    cout << setw(18) << cst( cn, +1 ) << flush;
    cout << setw(18) << cst( 1, 2, cn, +1 ) << flush;
    cout << endl;
  }
  
  cout 
    << "*************************************************************\n" 
    << "* S~ : eta = -1                                             *\n"
    << "*************************************************************\n" 
    << endl;
  
  cout 
    << setw(18) << "n"
    << setw(18) << "S~_1(n)"
    << setw(18) << "S~_2(n)"
    << setw(18) << "S~_3(n)"
    << setw(18) << "S~(n)"
    << setw(18) << "S~_{1,2}(n)"
    << endl;
    
  for( int i = 0; i < 100; i++ ){
    complex< double > cn( 0.1 * i , 0.0 );
    cout << setw(18) << cn << flush;
    for( int j = 1; j <= 3; j++ ) cout << setw(18) << cst( j, cn, -1 ) << flush;
    cout << setw(18) << cst( cn, -1 ) << flush;
    cout << setw(18) << cst( 1, 2, cn, -1 ) << flush;
    cout << endl;
  }
  
  return 0;
}
