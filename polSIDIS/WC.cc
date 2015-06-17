#include "WC.hh"
#include <cmath>
#include <Evolution/Kernel.hh>

using namespace std;

#define DEF_COF( T )							\
  class T: public Xsection::Coefficient {				\
  public:								\
  T() {}								\
  virtual ~T() {}							\
  virtual complex< double > eval( const complex< double >& n )		\
    { return this->eval( n, complex< double >( 0.0 ) ); }		\
  virtual complex< double >						\
  eval( const complex< double >& n, const complex< double >& m );	\
  }

namespace polSIDIS {
  DEF_COF( CNULL );
  DEF_COF( CQQLO );
  DEF_COF( CQQNLO );
  DEF_COF( CGQNLO );
  DEF_COF( CQGNLO );
};

using namespace Evolution;
using namespace polSIDIS;

WC::WC( Xsection::XsecComp* xc ) :
  Xsection::Wilsons()
{
  
  qqlo  = new CQQLO();
  qqnlo = new CQQNLO();//q(pdf)->q(ff)
  gqnlo = new CGQNLO();//q(pdf)->g(ff)
  qgnlo = new CQGNLO();//g(pdf)->q(ff)
  cnull = new CNULL();
  
  qqnlo->set( xc );
  gqnlo->set( xc );
  qgnlo->set( xc );
  // q2 :: xc_->q2(), mu2 :: vevo_[ i ]->Q2()
  
  // for SIDIS
  (*this)[ 0 ][ 0 ][ 0 ] = qqlo; 
  (*this)[ 0 ][ 0 ][ 1 ] = qqnlo;
  (*this)[ 0 ][ 1 ][ 0 ] = cnull;     // ( 0.0, 0.0 )
  (*this)[ 0 ][ 1 ][ 1 ] = cnull;     // ( 0.0, 0.0 )
  (*this)[ 0 ][ 2 ][ 0 ] = cnull;     // ( 0.0, 0.0 )
  (*this)[ 0 ][ 2 ][ 1 ] = gqnlo;
  (*this)[ 1 ][ 0 ][ 0 ] = cnull;     // ( 0.0, 0.0 )
  (*this)[ 1 ][ 0 ][ 1 ] = cnull;     // ( 0.0, 0.0 )
  (*this)[ 1 ][ 1 ][ 0 ] = qqlo;
  (*this)[ 1 ][ 1 ][ 1 ] = qqnlo;
  (*this)[ 1 ][ 2 ][ 0 ] = cnull;     // ( 0.0, 0.0 )
  (*this)[ 1 ][ 2 ][ 1 ] = gqnlo;
  (*this)[ 2 ][ 0 ][ 0 ] = cnull;     // ( 0.0, 0.0 )
  (*this)[ 2 ][ 0 ][ 1 ] = qgnlo;
  (*this)[ 2 ][ 1 ][ 0 ] = cnull;     // ( 0.0, 0.0 )
  (*this)[ 2 ][ 1 ][ 1 ] = qgnlo;
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


//for SIDIS
complex< double > CQQLO::eval
  ( const complex< double >& n, const complex< double >& m ){
  return complex< double > ( 1.0, 0.0 );
}

/*
2 * G1h
*/
complex< double > CQQNLO::eval
  ( const complex< double >& n, const complex< double >& m ){
  
  complex< double > s1n = s_( 1, n );
  complex< double > s2n = s_( 2, n );
  complex< double > on = 1.0 / n;
  complex< double > on1 = 1.0 / ( n + 1.0 );
  
  complex< double > s1m = s_( 1, m );
  complex< double > s2m = s_( 2, m );
  complex< double > om = 1.0 / m;
  complex< double > om1 = 1.0 / ( m + 1.0 );
  
  // q2 :: xc_->q2(), mu2 :: vevo_[ i ]->Q2()  
  return Kernel::CF *
    ( - 8.0 - om * om + 2.0 * om1 * om1 + on * on 
      + ( pow( 1.0 + m + n, 2 ) - 1.0 ) * om * om1 * on * on1
      + 3.0 * s2m - s2n 
      + ( s1m + s1n ) * ( s1m + s1n - om * om1 - on * on1 )
      + ( on * on1 + 1.5 - 2.0 * s1n ) * this->rescale( 0 )
      + ( om * om1 + 1.5 - 2.0 * s1m ) * this->rescale( 1 )
      );
}

complex< double > CGQNLO::eval
  ( const complex< double >& n, const complex< double >& m ){
  
  complex< double > s1n = s_( 1, n );
  complex< double > on = 1.0 / n;
  complex< double > on1 = 1.0 / ( n + 1.0 );

  complex< double > s1m = s_( 1, m );
  complex< double > om = 1.0 / m;
  complex< double > om2 = om * om;
  complex< double > om1 = 1.0 / ( m + 1.0 );
  complex< double > om12 = om1 * om1;
  complex< double > omm1 = 1.0 / ( m - 1.0 );
  complex< double > omm12 = omm1 * omm1;

  complex< double > m2 = m * m;
  complex< double > m3 = m2 * m;
  complex< double > m4 = m3 * m;
  complex< double > m5 = m4 * m;

  return Kernel::CF * 
       ( ( 2.0 - 2.0 * m - 9.0 * m2 + m3 - m4 + m5 ) * om2 * om12 * omm12
	 + 2.0 * m * on * om1 * omm1
	 - ( 2.0 - m + m2 ) * om * om1 * omm1 * on1
	 - ( 2.0 + m + m2 ) * om * om1 * omm1 * ( s1m + s1n )
	 - 2.0 * om1 * on * on1 
	 + ( 2.0 + m + m2 ) * om * om1 * omm1 * this->rescale( 1 )
	 );  
}

complex< double > CQGNLO::eval
     ( const complex< double >& n, const complex< double >& m ){

  complex< double > s1n = s_( 1, n );
  complex< double > on = 1.0 / n;
  complex< double > on1 = 1.0 / ( n + 1.0 );

  complex< double > s1m = s_( 1, m );
  complex< double > om = 1.0 / m;
  complex< double > omm1 = 1.0 / ( m - 1.0 );

// TR = 1/2
  return 0.5 * ( n - 1.0 ) * on * on1 *
    ( omm1 - om + on - s1m - s1n 
      + this->rescale( 0 )
      );
}

complex< double > CNULL::eval
  ( const complex< double >& n, const complex< double >& m ){
  return complex< double > ( 0.0, 0.0 );
}

