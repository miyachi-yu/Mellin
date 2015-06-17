#include "Matrix.hh"
#include <iostream>
#include <complex>
#include <cstdlib>

using namespace std;
using namespace Transform;

int main( int argc, char* argv[] ){
  
  Matrix< int > m( 2, 2, 3 );
  Matrix< int > n( 3, 3, 2 );
  Matrix< int > l( 2, 3, 1 );
  Matrix< int > copy( m );

  m.charsize() = 4;
  cout << m << endl;
  cout << copy << endl;
  cout << l << endl;
  
  try {
    Matrix< int >  nm = n * m;
    cout << nm << endl;
  }
  
  catch ( int i ) {
    cout << "Error" << endl;
  }

  try{
    cout << m << "*" << endl;
    cout << l << "=" << endl;
    cout << m * l << endl;
  }
  
  catch( int i ){
    cout << "It should not happen." << endl;
  }
  
  try {
    cout << ( m - m ) << endl;
  }
  
  catch ( int i ) {
    cout << "It should not happen." << endl;
  }

  cout << "Assign m to n " << endl;
  cout << ( n = m ) << endl;

  cout << "complex version" << endl;
  Matrix< complex< double > > cm( 2, 2, complex< double >( 1.0, 2.0 ) );
  cout << cm << endl;

  
  cout << "-----------------------------------" << endl;
  
  Matrix< complex< double > > P( 2, 2 );
  P[0][0] = complex< double >( 1.0, 0.0 );
  P[0][1] = complex< double >( 0.0, 0.0 );
  P[1][0] = complex< double >( 0.0, 0.0 );
  P[1][1] = complex< double >( 1.0, 0.0 );
  
  Matrix< complex< double > > q( 2, 1 );
  q[ 0 ][ 0 ] = complex< double >( 1.0, 0.0 );
  q[ 1 ][ 0 ] = complex< double >( 1.0, 0.0 );
  
  cout 
    << "P * q = \n"
    << P
    << "*\n"
    << q
    << "=\n"
    << P * q 
    << endl;
  
  cout << "-----------------------------------" << endl;
  Matrix< double > a( 2, 2 );
  Matrix< double > b( 2, 2 );
  
  a[0][0] = 1.0;
  a[0][1] = 2.0;
  a[1][0] = 3.0;
  a[1][1] = 4.0;

  b[0][0] = 5.0;
  b[0][1] = 6.0;
  b[1][0] = 7.0;
  b[1][1] = 8.0;

  cout << " | 1  2 |   | 5  6 |    | 19  22 |" << endl;
  cout << " |      | * |      | =  |        |" << endl;
  cout << " | 3  4 |   | 7  8 |    | 43  50 |" << endl;
  
  cout << a << endl;
  cout << b << endl;
  cout << a * b << endl;
  
  Matrix< double > c( 2, 2 );
  c[0][0] = 1.0;
  c[0][1] = 4.0;
  c[1][0] = 2.0;
  c[1][1] = 3.0;

  cout << " | 1  2 |   | 5  6 |   | 1  4 |    |  61  142 |" << endl;
  cout << " |      | * |      | * |      | =  |          |" << endl;
  cout << " | 3  4 |   | 7  8 |   | 2  3 |    | 143  322 |" << endl;
  cout << ( a * b ) * c << endl;
  cout << a * ( b * c ) << endl;
  
  try {
    cout << "****************************************************" << endl;
    cout << "******    Determinant                        *******" << endl;
    cout << "****************************************************" << endl;
    
    int dim = 4;
    
    Matrix< double > large( dim, dim );
    for( int i = 0; i < large.column(); i++ )
      for( int j = 0; j< large.row(); j++ )
	large[ i ][ j ] = 10.0 * rand() / (RAND_MAX + 1.0);
    
    cout << "-- basic matrix " << endl;
    cout << large << endl;
    
    cout << "-- cofactor matrix : M~" << endl;
    Matrix< double > cofMat = large.cofactorMatrix();
    cout << cofMat << endl;
    cout << "-- determinant " << large.det() << endl;
    
    cout << "-- M * M~.t() = det * I " << endl;
    cout << cofMat.t() * large << endl;
    
    if( large.det() != 0 ){
      cout << "-- inverse matrix" << endl;
      Matrix< double > invMat = cofMat.t() / large.det();
      //Matrix< double > invMat = cofMat / large.det();
      cout << invMat << endl;
      
      cout << "-- M * M^{-1} " << endl;
      cout << large * invMat << endl;
    }
    
  }
  
  catch ( int err ) {
    cout << "catch ( " << err << " )" << endl;
  }




  try {
    cout << "****************************************************" << endl;
    cout << "******    Determinant  complex version       *******" << endl;
    cout << "****************************************************" << endl;
    
    int dim = 4;
    
    Matrix< complex< double > > large( dim, dim );
    for( int i = 0; i < large.column(); i++ )
      for( int j = 0; j< large.row(); j++ )
	large[ i ][ j ] = 
	  complex< double >( 10.0 * rand() / ( RAND_MAX + 1.0 ),
			     10.0 * rand() / ( RAND_MAX + 1.0 ) );
    
    cout << "-- basic matrix " << endl;
    cout << large << endl;
    
    cout << "-- cofactor matrix : M~" << endl;
    Matrix< complex< double > > cofMat = large.cofactorMatrix();
    cout << cofMat << endl;
    cout << "-- determinant " << large.det() << endl;
    
    cout << "-- M * M~.t() = det * I " << endl;
    cout << cofMat.t() * large << endl;
    
    if( large.det() != 0.0 ){
      cout << "-- inverse matrix" << endl;
      Matrix< complex< double > > invMat = large.inverse();
      //Matrix< double > invMat = cofMat / large.det();
      cout << invMat << endl;
      
      cout << "-- M * M^{-1} " << endl;
      cout << large * invMat << endl;
    }
    
  }
  
  catch ( int err ) {
    cout << "catch ( " << err << " )" << endl;
  }

  return 0;
}
