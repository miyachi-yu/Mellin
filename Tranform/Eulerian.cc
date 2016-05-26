#include "Eulerian.hh"
#include "Binomial.hh"
#include <cmath>

int Transform::Eulerian( int n, int k ){
  return Eulerian( n, k, k + 1 );
}

int Transform::Eulerian( int n, int k, int j ){
  return j < 0 ?
	     0.0 :
    ( j % 2 == 0 ? 1.0 : -1.0 ) * Binomial( n + 1, j ) * pow( k - j + 1, n )
	     + Eulerian( n, k, j - 1 );
}

