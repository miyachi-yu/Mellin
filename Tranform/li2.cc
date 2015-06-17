#include "Utility/Timer.hh"

#include "Li2.hh"
#include "Li2Int.hh"
#include "Li2Int2.hh"
#include "GridIntegration.hh"
#include "CPolyGamma.hh"

#include <cmath>
#include <iostream>
#include <iomanip>

using namespace std;
using namespace Transform;

double f( const double& x ){
  return 0.0030 
    + 1.0990 * x
    - 1.5463 * pow( x, 2 )
    + 3.2860 * pow( x, 3 )
    - 3.7887 * pow( x, 4 )
    + 1.7646 * pow( x, 5 );
};

complex< double > cf( const complex< double >& n ){
  return 0.0030 / n 
    + 1.0990 / ( n + 1.0 ) 
    - 1.5463 / ( n + 2.0 )
    + 3.2860 / ( n + 3.0 ) 
    - 3.7887 / ( n + 4.0 ) 
    + 1.7646 / ( n + 5.0 );
};
#include <cstdlib>
#include <ctime>

extern "C" {
#include <sys/time.h>
}


double get_random(){
  return ( 1.0 * random() ) / RAND_MAX ;
}

int main( int argc, char* argv[] ){
  
  Li2     li2;
  Li2Int  li2int;
  Li2Int2 li2int2;
  CPolyGamma pg_;

  /*
  Timer timer;
  timer.start();
  for( double x = 0.0; x < 1.0; x += 0.0001 ){
    li2( x );
    li2int( complex< double >( x, 0.0 ) );
    li2int2( complex< double >( x, 0.0 ) );
  }
  
  cout << "Time spend: " << timer.check( Timer::Usec ) << " usec" << endl;

  srandom( gettimeofday( NULL, NULL ) );

  for( int i = 0; i < 10000; i++ ){
    //    double x = get_random();
    //    cout << setw(10) << x << setw(10) << li2( x ) << endl;
    li2( get_random() );
  }
  cout << "Time spend: " << timer.check( Timer::Usec ) << " usec" << endl;
  */

  /*
  cout << li2int( complex< double >( -0.0865262, 1.80905 ) ) 
       << "\t"
       << cf( complex< double >( -0.0865262, 1.80905 ) ) 
       << endl;
  */
  
  for( double x = 0.0; x < 1.0; x += 0.1 ){
    cout << setw(6) << x  << flush;
    cout << setw(12) << li2( x ) << flush;
    cout << setw(12) << f(x) * ( 1 + x ) << flush;
    cout << setw(12) << li2( x ) - f(x) * ( 1 + x ) << flush;
    cout << endl;
  }

  for( double x = 0.0; x < 3.0; x += 0.2 ){
    cout << setw(12) << x << flush;
    cout << setw(18) << li2int( complex< double >( x, 0.0 ) ) << flush;
    cout << setw(18) << li2int2( complex< double >( x, 0.0 ) ) << flush;
    cout << endl;
  }
  
  
  double p2 = M_PI * M_PI;
  cout << "***************************************************" << endl;
  cout << "*         test of Li2(s)" << endl;
  cout << "***************************************************" << endl;
  cout << "Li2(0) = 0                        " << setw(18) << li2( 0 ) << endl;
  cout << "Li2(-1) - pi^2/12 = 0             " 
       << setw(18) << li2( -1 ) - p2 / 12.0 << endl;
  cout << "Li(1/2) - pi^2/12 + (ln2)^2/2 = 0 "
       << setw(18) << li2( 0.5 ) - p2 / 12.0 + 0.5 * log(2) * log(2) 
       << endl;
  cout << "Li2(1) - pi^2/6 = 0               "
       << setw(18) << li2( 1.0 ) - p2 / 6.0 << endl;


  cout << "* ---------------------------------------------- " << endl;
  cout << "li2int( 1.0 )  =  zeta(2) * ln(2) - 5/8 zeta(3)" 
       << setw(12) << p2 / 6.0 * log( 2 ) - 5.0 / 8.0 * 1.202057
       << setw(18) << li2int( 1.0 ) << endl; 


  return 0;
};
