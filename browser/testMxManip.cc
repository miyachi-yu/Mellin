#include <TMatrixD.h>
#include <TMatrixDEigen.h>
//#include <TMatrixDRow.h>
//#include <TMatrixDColumn.h>
#include <TVectorD.h>

#include <fstream>
#include <iostream>
#include <string>
#include <cmath>
#include <vector>
using namespace std;

void out_mx( const TMatrixD& mx );
void out_vr( const TMatrixDRow& vr );
void out_vc( const TMatrixDColumn& vc );
void out_ve( const TVectorD& vv );

double inner_pro( const TVectorD& v1, const TVectorD& v2 );

int main(){

  cout << "###### Basic Matrix Manipulation ######" << endl;

  int n = 2;
  //  int n = 3;

  //  TMatrixD m( n - 1, n - 1 );
  TMatrixD m;
  m.ResizeTo( n, n );

  for( int i = 0; i < n; i++ )
    for( int j = 0; j < n; j++ ) m[ i ][ j ] = 0.0;

  TMatrixDDiag( m ) += 4;

  m[ 0 ][ 0 ] = 4;
  m[ 1 ][ 1 ] = 4;
  m[ 0 ][ 1 ] = 0;
  m[ 1 ][ 0 ] = 2;

  out_mx( m );

  TMatrixDRow vr( TMatrixDRow( m, 0 ) );
  TMatrixDColumn vc( TMatrixDColumn( m, 0 ) );

  // Note! Reference!!
  vr[ 1 ] = 2;
  out_mx( m );

  out_vr( vr );
  out_vc( vc );

  double det;
  out_mx( m.Invert( &det ) );
  cout << "det : " << det << endl;
  //  out_mx( m.InvertFast() );
  //  m.Invert( &det );
  m.Invert();
  //  cout << "det : " << det << endl;

  TVectorD vvr( vr );
  TVectorD vvc( vc );

  cout << "inner product : " 
       << inner_pro( vvr, vvc ) << endl;

  /*
  out_ve( vvr );
  out_ve( vvc );

  vvr *= m;
  vvc *= m;

  out_ve( vvr );
  out_ve( vvc );
  */
  cout << endl;

  cout << "###### Eigen Problem Manipulation ######" << endl;
  cout << "Basic Matrix " << endl;
  out_mx( m );

  TMatrixDEigen de( m );
  cout << "Eigen Value Matrix : " << endl;
  out_mx( de.GetEigenValues() );
  cout << "Eigen Vector Matrix : " << endl;
  out_mx( de.GetEigenVectors() );
  /*
  cout << "Eigen Vector Matrix Invert : " << endl;
  out_mx( de.GetEigenVectors().Invert() );
  out_mx( de.GetEigenVectors() );
  */
  cout << "Eigen Vector Determinant : " << endl;
  cout << de.GetEigenVectors().Determinant() << endl;

  cout << endl;
  cout << "Cross Check for Reproduction of" << endl;
  cout << "Eigen Value Matrix" << endl;

  //  TMatrixD mm = m * de.GetEigenVectors();
  TMatrixD egmx( de.GetEigenVectors() );
  TMatrixD mm = m * egmx;;

  mm = egmx.Invert() * mm;
  // needed to re-invert again to recover the original
  egmx.Invert();

  out_mx( mm );

  //  out_mx( egmx );
  /*
  cout << " re-showing basic matrix " << endl;
  out_mx( m );
  */
}

void out_mx( const TMatrixD& mx ){

  int n = mx.GetNrows();
  int m = mx.GetNcols();

  TMatrixD tmpmx( mx );

  cout << "matrix : " << flush; 
  cout << "( " << n << " * " << m << " )" << endl;

  for( int i = 0; i < n; i++ ){
    TVectorD v = TMatrixDRow( tmpmx, i );
    for( int j = 0; j < m; j++ )
      cout << v( j ) << " " << flush;
    cout << endl;
  }

}

void out_vr( const TMatrixDRow& vr ){

  int n = vr.GetMatrix()->GetNrows();

  cout << "row vector : " << n << endl;

  for( int j = 0; j < n; j++ ) 
    cout << vr[ j ] << " " << flush;
  cout << endl;

}

void out_vc( const TMatrixDColumn& vc ){

  int n = vc.GetMatrix()->GetNcols();

  cout << "column vector : " << n << endl;

  for( int j = 0; j < n; j++ )
      cout << vc[ j ] << " " << endl;

}

void out_ve( const TVectorD& vv ){

  int n = vv.GetNrows();

  cout << "vector : " << n << endl;

  for( int j = 0; j < n; j++ )
      cout << vv[ j ] << " " << endl;

}

double inner_pro( const TVectorD& v1, const TVectorD& v2 ){
  
  double tmp;
  int n1 = v1.GetNrows();
  int n2 = v2.GetNrows();

  if( n1 != n2 ) throw( 10 );

  for( int i = 0; i < n1; i++ ) 
    tmp += v1[ i ] * v2[ i ];

  return tmp;
}
