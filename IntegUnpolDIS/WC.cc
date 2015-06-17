#include "WC.hh"

using namespace IntegUnpolDIS;

WC::WC( IntegXsec::IntegXsecComp* xc ) :
  IntegXsec::Wilsons()
{
  
  qqlo  = new CQQLO( xc );
  qqnlo = new CQQNLO( xc );
  ggnlo = new CGGNLO( xc );
  cnull = new CNULL( xc );
  
  // for DIS SIA
  (*this)[ 0 ][ 0 ][ 0 ] = qqlo; 
  (*this)[ 0 ][ 0 ][ 1 ] = qqnlo;
  (*this)[ 0 ][ 1 ][ 0 ] = cnull;     // ( 0.0, 0.0 )
  (*this)[ 0 ][ 1 ][ 1 ] = cnull;     // ( 0.0, 0.0 )
  (*this)[ 0 ][ 2 ][ 0 ] = cnull;     // ( 0.0, 0.0 )
  (*this)[ 0 ][ 2 ][ 1 ] = cnull;     // ( 0.0, 0.0 )
  (*this)[ 1 ][ 0 ][ 0 ] = cnull;     // ( 0.0, 0.0 )
  (*this)[ 1 ][ 0 ][ 1 ] = cnull;     // ( 0.0, 0.0 )
  (*this)[ 1 ][ 1 ][ 0 ] = qqlo;
  (*this)[ 1 ][ 1 ][ 1 ] = qqnlo;
  (*this)[ 1 ][ 2 ][ 0 ] = cnull;     // ( 0.0, 0.0 )
  (*this)[ 1 ][ 2 ][ 1 ] = cnull;     // ( 0.0, 0.0 )
  (*this)[ 2 ][ 0 ][ 0 ] = cnull;     // ( 0.0, 0.0 )
  (*this)[ 2 ][ 0 ][ 1 ] = cnull;     // ( 0.0, 0.0 )
  (*this)[ 2 ][ 1 ][ 0 ] = cnull;     // ( 0.0, 0.0 )
  (*this)[ 2 ][ 1 ][ 1 ] = cnull;     // ( 0.0, 0.0 )
  (*this)[ 2 ][ 2 ][ 0 ] = cnull;     // ( 0.0, 0.0 )
  (*this)[ 2 ][ 2 ][ 1 ] = ggnlo;
}

WC::~WC(){
  delete qqlo;
  delete qqnlo;
  delete ggnlo;
  delete cnull;
}

