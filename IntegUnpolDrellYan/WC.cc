#include "WC.hh"

using namespace IntegUnpolDrellYan;

WC::WC( IntegXsec::IntegXsecComp* xc ) :
  IntegXsec::Wilsons()
{
  
  qqlo  = new CQQLO( xc );
  qqnlo = new CQQNLO( xc );
  gqnlo = new CGQNLO( xc );
  qgnlo = new CQGNLO( xc );
  cnull = new CNULL( xc );
  
  // for DrellYan
  (*this)[ 0 ][ 0 ][ 0 ] = cnull;     // ( 0.0, 0.0 )
  (*this)[ 0 ][ 0 ][ 1 ] = cnull;     // ( 0.0, 0.0 )
  (*this)[ 0 ][ 1 ][ 0 ] = qqlo; 
  (*this)[ 0 ][ 1 ][ 1 ] = qqnlo;
  (*this)[ 0 ][ 2 ][ 0 ] = cnull;     // ( 0.0, 0.0 )
  (*this)[ 0 ][ 2 ][ 1 ] = qgnlo;
  (*this)[ 1 ][ 0 ][ 0 ] = qqlo;
  (*this)[ 1 ][ 0 ][ 1 ] = qqnlo;
  (*this)[ 1 ][ 1 ][ 0 ] = cnull;     // ( 0.0, 0.0 )
  (*this)[ 1 ][ 1 ][ 1 ] = cnull;     // ( 0.0, 0.0 )
  (*this)[ 1 ][ 2 ][ 0 ] = cnull;     // ( 0.0, 0.0 )
  (*this)[ 1 ][ 2 ][ 1 ] = qgnlo;
  (*this)[ 2 ][ 0 ][ 0 ] = cnull;     // ( 0.0, 0.0 )
  (*this)[ 2 ][ 0 ][ 1 ] = gqnlo;
  (*this)[ 2 ][ 1 ][ 0 ] = cnull;     // ( 0.0, 0.0 )
  (*this)[ 2 ][ 1 ][ 1 ] = gqnlo;
  (*this)[ 2 ][ 2 ][ 0 ] = cnull;     // ( 0.0, 0.0 )
  (*this)[ 2 ][ 2 ][ 1 ] = cnull;     // ( 0.0, 0.0 )
}

WC::~WC(){
  delete qqlo;
  delete qqnlo;
  delete gqnlo;
  delete qgnlo;
  delete cnull;
}
