#include <iostream>
#include <iomanip>
#include <cmath>


#include "Alpha.hh"
#include "Flavor.hh"

using namespace std;
using namespace QCD;

int main( int argc, char* argv[] ){

  Alpha  lo( 0.234, Scheme::LO );
  Alpha  nlo( 0.234, Scheme::NLO );
  Alpha  nnlo( 0.234, Scheme::NNLO );

  Flavor nf;

  double Mz = 91.1876;
  double Mz2 = Mz * Mz;
  
  cout << "--------------------------------------------------------" << endl;
  cout << setw(10) << "Lambda" 
       << setw(10) << "LO" 
       << setw(10) << "NLO" 
       << setw(10) << "NNLO" 
       << endl;
  
  for( double lambda = 0.001; lambda < 0.236; lambda += 0.001 ){
    lo.lambda( lambda );
    nlo.lambda( lambda );
    nnlo.lambda( lambda );
    cout << setw(10) << lo.lambda()
	 << setw(10) << lo( Mz2 )
	 << setw(10) << nlo( Mz2 )
	 << setw(10) << nnlo( Mz2 )
	 << endl;
  }
  
  cout << "--------------------------------------------------------" << endl;
  cout << "Alpha_s configuration dump: " << endl;
  Alpha  alpha;
  cout << alpha << endl;
  cout << endl;
  
  cout 
    << setw(14) << "Q^2" 
    << setw(14) << "Q" 
    << setw(14) << "N_f(Q)"
    << setw(14) << "alpha_s(Q)"
    << endl;
  
  for( double lq2 = -1.0; lq2 <= 5.0; lq2 += 0.2 ){
    double q2 = pow( 10.0, lq2 );
    double q  = sqrt( q2 );
    cout 
      << setw(14) << q2 
      << setw(14) << q 
      << setw(14) << nf( q )
      << setw(14) << alpha( q2 )
      << endl;
  }
  cout << "--------------------------------------------------------" << endl;
  cout << "----   RUNGE-KUTTA Method                    -----------" << endl;
  cout << "--------------------------------------------------------" << endl;
  
  alpha.mode( Alpha::RUNGEKUTTA );
  for( double lq2 = -1.0; lq2 <= 5.0; lq2 += 0.2 ){
    double q2 = pow( 10.0, lq2 );
    double q  = sqrt( q2 );
    cout 
      << setw(14) << q2 
      << setw(14) << q 
      << setw(14) << nf( q )
      << setw(14) << alpha( q2 )
      << endl;
  }
  cout << "--------------------------------------------------------" << endl;
  


  return 0;
}
