#include "WC.hh"

#include <Evolution/Kernel.hh>
#include <cmath>

using namespace std;

#define DEF_COF( T )							\
  class T: public Xsection::Coefficient {				\
  public:								\
  T(){};								\
  virtual ~T(){};							\
  virtual complex< double > operator()( const complex< double >& n );   \
  virtual complex< double >						\
  operator()( const complex< double >& n, complex< double >& m )	\
    { return this->operator()( n ); }					\
  }
// eval <-> operator()

namespace unpolDISXsec {
  DEF_COF( CNULL );
  DEF_COF( CQQLO );
  DEF_COF( CQQNLO );
  DEF_COF( CGGNLO );
};

using namespace Evolution;
using namespace unpolDISXsec;

WC::WC( Xsection::XsecComp* xc ) :
  Xsection::Wilsons()
{
  
  qqlo  = new CQQLO();
  qqnlo = new CQQNLO();
  ggnlo = new CGGNLO();
  cnull = new CNULL();
  
  qqnlo->set( xc );
  ggnlo->set( xc );
  
// q2 :: xc_->q2(), mu2 :: vevo_[ i ]->Q2()
  
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


//for DIS SIA
complex< double > CQQLO::operator()( const complex< double >& n ){
  return complex< double > ( 1.0, 0.0 );
}

/*
2 * F1
*/
complex< double > CQQNLO::operator()( const complex< double >& n ){

  complex< double > s1 = s_( 1, n );
  complex< double > s2 = s_( 2, n );
  complex< double > on = 1.0 / n;
  complex< double > on1 = 1.0 / ( n + 1.0 );
  complex< double > onn1 = on * on1;
  
  return 
    Kernel::CF * 
    ( - s2
      + s1 * s1 
      + ( 1.5 - onn1 ) * s1
      + on * on
      + 1.5 * on
      - onn1 
      - 4.5 
      + ( 1.5 + onn1 - 2.0 * s1 ) * this->rescale()
      )
    + Kernel::CF * onn1
    ;
}

complex< double > CGGNLO::operator()( const complex< double >& n ){

  complex< double > s1 = s_( 1, n );
  
  complex< double > on = 1.0 / n;
  complex< double > on1 = 1.0 / ( n + 1.0 );
  complex< double > on2 = 1.0 / ( n + 2.0 );
  complex< double > onn1n2 = on * on1 * on2;
  
  // 2 ( q + qb ) * 1/2 ( TR ) -> 1
  return 
    ( 2.0 + n + n * n ) * onn1n2 * ( -1.0 - s1 + this->rescale() )
    + on * on
    ;  
}

complex< double > CNULL::operator()( const complex< double >& n ){
  return complex< double > ( 0.0, 0.0 );
}

