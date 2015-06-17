// $Id: testCGamma.cc,v 1.5 2008/07/10 04:01:32 miyachi Exp $
/* 
   \file     testGamma.cc
   \brief    test program for complex gamma function in CERNlib
             (wrapper_gamma.f)
 */


/*
 */
extern "C" {
#include <cfortran.h>

PROTOCCALLSFSUB5(CGAMA,cgama,DOUBLE,DOUBLE,DOUBLE,PDOUBLE,PDOUBLE)
#define CGAMA(A1,A2,A3,A4,A5) CCALLSFSUB5(CGAMA,cgama,DOUBLE,DOUBLE,DOUBLE,PDOUBLE,PDOUBLE,A1,A2,A3,A4,A5)

}

#include <cstdlib>
#include <complex>
#include <ctime>
#include <Tranform/CGamma.hh>
#include <iostream>
#include <iomanip>

using namespace std;
using namespace Transform;

extern complex<double> cgamma( complex<double> z, int OPT );

int main( int argc, char* argv[] ){
  
  if( argc != 3 ) {
    cerr << "USAGE: " << argv[0] << " real imag" << endl;
    return -1;
  }
  
  std::complex< float > cinput( atof( argv[1] ), atof( argv[2] ) );
  
  CGamma cg;
  double gr=0.0, gi=0.0;
  bool loggamma = true;
  int ni = 1000000;
  double dzi = 2000.0 / ni;
  
  
  time_t stopwatch = time( NULL ) ;


  std::cout << "C++: " 
	    << setw(24) << cg.logGamma()( cinput ) 
	    << setw(24) << cg( cinput ) 
	    << std::endl;

  /*  
  std::cout << ctime( &( stopwatch = time( NULL ) ) ) << std::endl;
  for( int i = 0; i < ni; i++ ){
    cg( std::complex< double >( 10.3, -1000.0 + dzi * i ) );
  }
  std::cout << ctime( &( stopwatch = time( NULL ) ) ) << std::endl;
  */
  
  
  CGAMA( cinput.real(), cinput.imag(), ( loggamma ? 0 : 1 ), gr, gi );
  std::cout << "F77: "
	    << setw(10) << gr << ", " 
	    << setw(10) << gi << std::endl;
  /*
    std::cout << ctime( &( stopwatch = time( NULL ) ) ) << std::endl;
  for( int i = 0; i < ni; i++ ){
  CGAMA( 10.3, -1000.0 + dzi * i, 1, gr, gi );
  }
  std::cout << ctime( &( stopwatch = time( NULL ) ) ) << std::endl;
  */
  

  
  
  std::cout << "C  : " 
	    << setw(24) << cgamma( cinput, 1 ) 
	    << setw(24) << cgamma( cinput, 0 ) 
	    << std::endl;
  /*
  std::cout << ctime( &( stopwatch = time( NULL ) ) ) << std::endl;
  for( int i = 0; i < ni; i++ ){
    cgamma( std::complex< double >( 10.3, -1000.0 + dzi * i ), 0 );
  }
  std::cout << ctime( &( stopwatch = time( NULL ) ) ) << std::endl;
  */
  return 0;
};

