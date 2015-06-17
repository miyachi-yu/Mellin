//
// PDF_SW.cc
//  
// Made by Yoshiyuki Miyachi
// Login   <miyachi@yoshi-desktop>
// 
// Started on  Thu Nov 20 15:44:14 2008 Yoshiyuki Miyachi
// Started on  Thu Nov 20 15:44:14 2008 Yoshiyuki Miyachi
//

#include <iostream>
#include "PDF_SW.hh"

using namespace std;
using namespace Xsection;

int PDF_SW::su2swap( const int& i ){
  if( i * i == 1 ) return i * 2;
  if( i * i == 4 ) return i / 2;
  return i;
}

complex< double > PDF_P::operator()
  ( const int& j, const complex< double >& m ){
  return ( *( vpdf_[ j + 6 ] ) )( m );
}

complex< double > PDF_N::operator()
  ( const int& j, const complex< double >& m ){
  return ( *( vpdf_[ su2swap( j ) + 6 ] ) )( m );
}

complex< double > PDF_D::operator()
  ( const int& j, const complex< double >& m ){
  return 0.5 * ( ( *( vpdf_[ j + 6 ] ) )( m ) + 
		 ( *( vpdf_[ su2swap( j ) + 6 ] ) )( m ) );
}
