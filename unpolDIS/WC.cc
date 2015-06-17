#include "WC.hh"
#include <cmath>

#include <Evolution/Kernel.hh>

using namespace std;
using namespace Evolution;
using namespace unpolDIS;

#define DEF_COF( T )							\
  class T: public Xsection::Coefficient {				\
  public:								\
  T() {}								\
  virtual ~T() {}							\
  virtual complex< double > operator()( const complex< double >& n );	\
  virtual complex< double >						\
  operator()( const complex< double >& n, complex< double >& m )	\
    { return this->operator()( n ); }					\
  };
// eval <-> operator()

namespace unpolDIS {
  DEF_COF( CNULL )
  DEF_COF( CQQLO )
  DEF_COF( CQQNLO )
  DEF_COF( CGGNLO )
};

WC::WC( Xsection::XsecComp* xc ) :
  Xsection::Wilsons()
{
  
  qqlo  = new CQQLO();
  qqnlo = new CQQNLO();
  ggnlo = new CGGNLO();
  cnull = new CNULL();
  
  qqnlo->set( xc );
  ggnlo->set( xc );
  
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
complex< double > CQQLO::operator()( const complex< double >& z ){
  return complex< double > ( 1.0, 0.0 );
}

complex< double > CNULL::operator()( const complex< double >& z ){
  return complex< double > ( 0.0, 0.0 );
}

/*
F2 / x : C2 = C1 + CL  -> F2 / x = 2 * F1 + FL
// q2 :: xc_->q2(), mu2 :: evo_->Q2()
 */
/*
complex< double > CQQNLO::operator()( const complex< double >& z ){

  complex< double > s1 = s_( 1, z );
  complex< double > s2 = s_( 2, z );
  complex< double > oz = 1.0 / z;
  complex< double > oz1 = 1.0 / ( z + 1.0 );

  return Kernel::CF *
       ( s1 * s1 - s2 + 1.5 * s1 - s1 * oz * oz1 + 1.5 * oz +
	 2.0 * oz1 + oz * oz - 4.5
	    );
}

complex< double > CGGNLO::operator()( const complex< double >& z ) { 

  complex< double > s1 = s_( 1, z );
  complex< double > oz = 1.0 / z;
  complex< double > oz1 = 1.0 / ( z + 1.0 );
  complex< double > oz2 = 1.0 / ( z + 2.0 );

  return // 1/2 (TR) * 1/2 (alpha) * 4 -> 1
       - 1.0 * ( s1 * ( z * z + z + 2.0 ) * oz * oz1 * oz2 + oz - oz * oz 
		 - 6.0 * oz1 + 6.0 * oz2 );
}
*/

/*
2 * F1
 */
///*
complex< double > CQQNLO::operator()( const complex< double >& z ){
  
  complex< double > s1 = s_( 1, z );
  complex< double > s2 = s_( 2, z );
  complex< double > oz = 1.0 / z;
  complex< double > oz1 = 1.0 / ( z + 1.0 );
  
  return 
    Kernel::CF * 
    ( - s2
      + s1 * s1 
      + ( 1.5 - oz * oz1 ) * s1
      + 1.0 * oz * oz
      + 1.5 * oz
      - 1.0 * oz1 * oz 
      - 4.5 
      + ( 1.5 + oz * oz1 - 2.0 * s1 ) * this->rescale()
      )
    + Kernel::CF * oz * oz1
    //    + Kernel::CF * 2.0 * oz1 //! FL part
    ;
  
}

complex< double > CGGNLO::operator()( const complex< double >& z ) { 
  
  complex< double > s1 = s_( 1, z );
  
  complex< double > oz = 1.0 / z;
  complex< double > oz1 = 1.0 / ( z + 1.0 );
  complex< double > oz2 = 1.0 / ( z + 2.0 );

  // 2 ( q + qb ) * 1/2 ( TR ) -> 1
  return 
    - 1.0 * ( 2.0 + z + z * z ) * oz * oz1 * oz2 * ( 1.0 + s1 )
    + oz * oz
    + ( 2.0 + z + z * z ) * oz * oz1 * oz2 * this->rescale()
    //    + 4.0 * oz1 * oz2 //! FL part
    ;  
}
//*/

/*
FL
 */
/*
complex< double > CQQNLO::operator()( const complex< double >& z ){
  complex< double > oz1 = 1.0 / ( z + 1.0 );
     return   Kernel::CF * 2.0 * oz1;
}

complex< double > CGGNLO::operator()( const complex< double >& z ) { 
  complex< double > oz1 = 1.0 / ( z + 1.0 );
  complex< double > oz2 = 1.0 / ( z + 2.0 );
// 2 ( q + qb ) * 1/2 ( TR ) -> 1
//strat 8 -> 4 ( forgot TR factor...)
     return 4.0 * oz1 * oz2 ;  
}
*/

