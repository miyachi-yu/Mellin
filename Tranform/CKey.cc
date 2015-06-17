// $Id: CKey.cc,v 1.3 2008/05/23 02:08:01 miyachi Exp $
/*!
  \file     CKey.cc
  \version  $Revision: 1.3 $
  \date     $Date: 2008/05/23 02:08:01 $
  \brief    Implementation of extension of complex< double >
 */
#include "CKey.hh"

using namespace std;
using namespace Transform;

CKey::CKey() : 
  complex< double >( 0.0, 0.0 ) 
{
}

CKey::CKey( double r, double i ) :
  complex< double >( r, i )
{
}

CKey::CKey( const complex< double >& c ) :
  complex< double >( c ) 
{
}

CKey::CKey( const CKey& c ) :
  complex< double >( c.real(), c.imag() )
{
}

CKey::~CKey() {
}

CKey& CKey::operator=( const CKey& c ){
  if( this != &c ){ 
    this->real() = c.real(); 
    this->imag() = c.imag();
  }
  return *this;
}

bool CKey::operator<( const CKey& c ) const {
  return ( this->real() != c.real() ? this->real() < c.real() :
	   this->imag() < c.imag() );
}

bool CKey::operator>( const CKey& c ) const {
  return ( this->real() != c.real() ? this->real() > c.real() :
	   this->imag() > c.imag() );
}

bool CKey::operator==( const CKey& c ) const {
    return ( this->real() == c.real() &&  this->imag() == c.imag() );
}
