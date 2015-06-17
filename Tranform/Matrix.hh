// $Id: Matrix.hh,v 1.9 2013/04/05 09:02:13 miyachi Exp $
/*!
  \file   Matrix.hh
  \version $Revision: 1.9 $
  \date    $Date: 2013/04/05 09:02:13 $
  \author  $Author: miyachi $
  \brief   Definition of Matrix template class
 */
#ifndef _Matrix_hh_
#define _Matrix_hh_

#include <vector>
#include <iostream>
#include <iomanip>
#include <cmath>

namespace Transform {

/*!
  \class Matrix< class T >   Matrix.hh   "Tranform/Matrix.hh"
  \brief Matrix template class for type T

  This is a template class to manipulate matrix of type T.
  T should be implemented to be capable for STL vector contents,
  so T::T(), T::T( const T& t ), T& T::operator=( const T& t )
  must be implemented. In addition, operators, such as +=, -=, 
  +, -, * should be also implemented.

  So far, complecated interfases, such as Matrix inversion,
  have not been implemented.
  
 */
template < class T >
class Matrix : public std::vector< std::vector< T > > {
public:

  /*!
    \param[in] row is a number of row 
    \param[in] column is a number of column
    \param[in] t is a default values to be inseted to the matrix
    \brief  a default constructor.

    This constructor constructs matrix of T as  M[ row, colum ] 
    with the default value given as t. 
  */
  Matrix( int row = 1, int column = 1, const T& t = T() );
  Matrix( const Matrix& m );             //!< copy constructor
  virtual ~Matrix();                     //!< destuctor
  
  Matrix& operator=( const Matrix& m ) ; //!< assignment operator
  
  Matrix& operator+=( const Matrix& m ) throw ( int ); //!< increment operator
  Matrix& operator-=( const Matrix& m ) throw ( int ); //!< subtraction 
  
  Matrix& operator*=( const T& t ) throw ( int ); //!< increment operator
  Matrix& operator/=( const T& t ) throw ( int ); //!< increment operator
 
  Matrix operator*( const Matrix& m ) throw( int ); //!< multiplicate
  Matrix operator+( const Matrix& m ) throw( int ); //!< addition
  Matrix operator-( const Matrix& m ) throw( int ); //!< subtraction
  
  Matrix operator*( const T& t );            //!< scale up   by t
  Matrix operator/( const T& t );            //!< scale down by t
  
  Matrix t() const ;

  int& column()   { return col_; }           //!< get number of column
  int& row()      { return row_; }           //!< get number of row
  int& charsize() { return charsize_; }      //!< get character space to dump
  
  int column()   const { return col_; }      //!< get ref of number of column
  int row()      const { return row_; }      //!< get ref of number of row
  int charsize() const { return charsize_; } //!< get ref of number of space
  
  T det() const ;                //!< get determinant 
  Matrix minorMatrix( int r, int c ) const throw( int ); //!< get a minor matrix
  T cofactor( int r, int c ) const throw( int ); //!< get a cofactor
  Matrix cofactorMatrix() const throw( int ); //!< get a cofactor matrix
  Matrix inverse() const throw( int );
  

private:
  int col_;  //!< number of column 
  int row_;  //!< number of row
  int charsize_; //!< number of space size one one element for printing
  
};

//! dump elements of matrix to output stream
template < class T >
std::ostream& operator<<( std::ostream& os, const Matrix< T >& m );

//!< multiplications
template< class T >
Matrix< T > operator*( const T& t, const Matrix< T >& m );

// -------------------------------------------------------------------- //
// ------------ implementation reqired for temlate class -------------- //
// -------------------------------------------------------------------- //

template < class T >
Matrix< T >::Matrix( int row, int column, const T& t ) : 
  std::vector< std::vector < T > >( row, std::vector< T >( column, t ) ),
  col_( column ), row_( row ),
  charsize_( 16 )
{
}

template < class T >
Matrix< T >::Matrix( const Matrix& m ) :
  std::vector< std::vector < T > >( m ),
  col_( m.col_ ), row_( m.row_ ), charsize_( m.charsize_ )
{
}

template < class T >
Matrix< T >::~Matrix(){
}

template < class T >
Matrix< T >& Matrix< T >::operator=( const Matrix& m ){
  if( this != &m ){
    this->resize( m.row_ );
    for( int i = 0; i < m.row_; i++ ){
      (*this)[ i ].resize( m.col_ );
      for( int j = 0; j < m.col_; j++ ) (*this)[ i ][ j ] = m[ i ][ j ];
    }
    col_ = m.col_;
    row_ = m.row_;
    charsize_ = m.charsize_;
  }
  return *this;
}

template < class T >
Matrix< T >& Matrix< T >::operator+=( const Matrix& m ) throw( int ) {
  
  if( col_ != m.col_ ) throw 1;
  if( row_ != m.row_ ) throw 2;
  
  for( int i = 0; i < row_; i++ ) for( int j = 0; j < col_; j++ )
    (*this)[ i ][ j ] += m[ i ][ j ];
  
  return *this;
}

template < class T >
Matrix< T >& Matrix< T >::operator-=( const Matrix& m ) throw( int ) {
  
  if( col_ != m.col_ ) throw 1;
  if( row_ != m.row_ ) throw 2;
  
  for( int i = 0; i < row_; i++ ) for( int j = 0; j < col_; j++ )
    (*this)[ i ][ j ] -= m[ i ][ j ];
  
  return *this;
}

template < class T >
Matrix< T >& Matrix< T >::operator*=( const T& t ) throw ( int ){
  for( int i = 0; i < row_; i++ ) 
    for( int j = 0; j < col_; j++ )
      (*this)[ i ][ j ] *= t;
  return *this;
}

template < class T >
Matrix< T >& Matrix< T >::operator/=( const T& t ) throw ( int ){
  return (*this) *= ( 1.0 / t );
}

template < class T >
Matrix< T > Matrix< T >::operator*( const Matrix& m ) throw( int ) {
  
  if( this->column() != m.row() ) throw 1 ;
  
  Matrix< T > v( this->row(), m.column() );
  
  for( int i = 0; i < v.row(); i++ )
    for( int j = 0; j < v.column(); j++ )
      for( int k = 0; k < this->column(); k++ )
	if( k == 0 ) v[ i ][ j ] = (*this)[ i ][ k ] * m[ k ][ j ];
	else v[ i ][ j ] += (*this)[ i ][ k ] * m[ k ][ j ];
  
  return v;
}

template < class T >
Matrix< T > Matrix< T >::operator+( const Matrix& m ) throw( int ) {
  
  if( this->column() != m.column() ) throw 1 ;
  if( this->row()    != m.row()    ) throw 2 ;
  
  Matrix< T > v( *this );
  for( int i = 0; i < v.row(); i++ )
    for( int j = 0; j < v.column(); j++ )
      v[ i ][ j ] += m[ i ][ j ];
  
  return v;
}

template < class T >
Matrix< T > Matrix< T >::operator-( const Matrix& m ) throw( int ) {
  
  if( this->column() != m.column() ) throw 1 ;
  if( this->row()    != m.row()    ) throw 2 ;
  
  Matrix< T > v( *this );
  for( int i = 0; i < v.row(); i++ )
    for( int j = 0; j < v.column(); j++ )
      v[ i ][ j ] -= m[ i ][ j ];
  
  return v;
}

template < class T >
Matrix< T > Matrix< T >::operator*( const T& t ){
  Matrix< T > v( *this );
  for( int i = 0; i < v.row(); i++ ) for( int j = 0; j < v.column(); j++ )
    v[ i ][ j ] *= t;
  return v;
}

template < class T >
Matrix< T > Matrix< T >::operator/( const T& t ){
  Matrix< T > v( *this );
  for( int i = 0; i < v.row(); i++ ) for( int j = 0; j < v.column(); j++ )
    v[ i ][ j ] /= t;
  return v;
}

template < class T >
T Matrix< T >::det() const {
  
  if( this->column() != this->row() ) return T();
  if( this->column() == 1 ) return (*this)[0][0];
  if( this->column() == 2 ) 
    return (*this)[0][0] * (*this)[1][1] - (*this)[1][0] * (*this)[0][1] ;
  
  T determinant;
  int ir = 0;
  for( int ic = 0; ic < this->column(); ic++ ){
    T val = (*this)[ir][ic] * this->cofactor( ir, ic );
    determinant = ( ic == 0 ? val : determinant + val );
  }
  return determinant;
}

template < class T >
Matrix< T > Matrix< T >::minorMatrix( int row, int col ) const throw( int ){
  if( this->column() == 1 || this->row() == 1 )         throw 11 ;
  if( ! ( col < this->column() && row < this->row() ) ) throw 12 ;
  Matrix< T > m( this->row() - 1, this->column() - 1 );
  for( int ir = 0; ir < m.row(); ir++ )
    for( int ic = 0; ic < m.column(); ic++ )
      m[ ir ][ ic ] = 
	(*this)[( ir < row ? ir : ir + 1 )][ ( ic < col ? ic : ic + 1 ) ];
  return m;
}

template < class T >
T Matrix< T >::cofactor( int row, int col ) const throw( int ){
  if( this->column() != this->row() )                   throw 20 ;
  if( ! ( col < this->column() && row < this->row() ) ) throw 22 ;
  Matrix< T > m = this->minorMatrix( row, col );
  return m.det() * pow( -1.0, row + col );
}

template < class T >
Matrix< T > Matrix< T >::cofactorMatrix() const throw( int ){
  if( this->column() != this->row() )  throw 30 ;
  if( this->column() == 1 )            throw 31 ;
  Matrix< T > m( this->row(), this->column());
  
  for( int ir = 0; ir < m.row(); ir++ ) 
    for( int ic = 0; ic < m.column(); ic++ )
      m[ ir ][ ic ] = this->cofactor( ir, ic );
  
  return m;
}

template < class T >
Matrix< T > Matrix< T >::t() const {
  Matrix< T > m( this->column(), this->row() );
  for( int ic = 0; ic < m.column(); ic++ )
    for( int ir = 0; ir < m.row(); ir++ )
      m[ ir ][ ic ] = (*this)[ ic ][ ir ];
  return m;
}

template < class T >
Matrix< T >  Matrix< T >::inverse() const throw( int ){
  if( this->column() != this->row() )  throw 40 ;
  if( this->det() == T( 0.0 ) ) throw 41;
  return this->cofactorMatrix().t() / this->det();
}

// global functions 
  
  template < class T >
  std::ostream& operator<<( std::ostream& os, const Matrix< T >& m ){
    for( int i = 0; i < m.row(); i++ ){
      os << "| ";
      for( int j = 0; j < m.column(); j++ ){
	os << std::setw( m.charsize() ) << m[ i ][ j ] << std::flush;
      }
      os << " |" << std::endl;
    }
    return os;
  }
  
  template< class T >
  Matrix< T > operator*( const T& t, const Matrix< T >& m ){
    return m * t;
  }
  
};

#endif // _Matrix_hh_
