#include "WC2.hh"
#include "WC.hh"

#include <Evolution/Kernel.hh>
#include <Xsec/CKernel.hh>
#include <cmath>

using namespace std;

#define DEF_COF( T )							\
  class T: public Xsection::Coefficient {				\
  public:								\
  T() {}								\
  virtual ~T() {}							\
  virtual complex< double > eval( const complex< double >& n );		\
  }

namespace unpolDISXsec {
  DEF_COF( C2NULL );
  DEF_COF( C2QQLO );
  DEF_COF( C2QQNLO );
  DEF_COF( C2GGNLO );
};

using namespace Evolution;
using namespace unpolDISXsec;

WC2::WC2( Xsection::XsecComp* xc ) :
  Xsection::Wilsons()
{
  
  qqlo  = new C2QQLO();
  qqnlo = new C2QQNLO();
  ggnlo = new C2GGNLO();
  cnull = new C2NULL();
  
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

WC2::~WC2(){
  delete qqlo;
  delete qqnlo;
  delete ggnlo;
  delete cnull;
}


//for DIS SIA

/*
F2 / x : C2 = C1 + CL  -> F2 / x = 2 * F1 + FL //
*/

complex< double > C2QQLO::eval( const complex< double >& n ){
  return complex< double > ( 1.0, 0.0 );
}

complex< double > C2QQNLO::eval( const complex< double >& n ){
  complex< double > on1 = 1.0 / ( n + 1.0 );

  return (*dynamic_cast< WC* >( xc_->coeff()->wilsons() )->getqqnlo())( n )
    + Kernel::CF * 2.0 * on1 //! FL part
    ;
}

complex< double > C2GGNLO::eval( const complex< double >& n ){
  complex< double > on1 = 1.0 / ( n + 1.0 );
  complex< double > on2 = 1.0 / ( n + 2.0 );

  return (*dynamic_cast< WC* >( xc_->coeff()->wilsons() )->getggnlo())( n )
    // 8.0 * TR = 4.0
    + 4.0 * on1 * on2 //! FL part
    ;
}

complex< double > C2NULL::eval( const complex< double >& n ){
  return complex< double > ( 0.0, 0.0 );
}

