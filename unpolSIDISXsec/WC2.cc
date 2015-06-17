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
  virtual complex< double > operator()( const complex< double >& n )	\
    { return this->operator()( n, complex< double >( 0.0, 0.0 ) ); }	\
  virtual complex< double > operator()( const complex< double >& n,     \
					const complex< double >& m );	\
  }
// eval <-> operator()

namespace unpolSIDISXsec {
  DEF_COF( C2NULL );
  DEF_COF( C2QQLO );
  DEF_COF( C2QQNLO );
  DEF_COF( C2GQNLO );
  DEF_COF( C2QGNLO );
};

using namespace Evolution;
using namespace unpolSIDISXsec;

WC2::WC2( Xsection::XsecComp* xc ) :
  Xsection::Wilsons()
{
  
  qqlo  = new C2QQLO();
  qqnlo = new C2QQNLO();//q(pdf)->q(ff)
  gqnlo = new C2GQNLO();//q(pdf)->g(ff)
  qgnlo = new C2QGNLO();//g(pdf)->q(ff)
  cnull = new C2NULL();
  
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

WC2::~WC2(){
  delete qqlo;
  delete qqnlo;
  delete gqnlo;
  delete qgnlo;
  delete cnull;
}


//for SIDIS

/*
F2_h / x : C2 = C1 + CL  -> F2_h / x = 2 * F1_h + FL_h //
*/

complex< double > C2QQLO::operator() 
     ( const complex< double >& n, const complex< double >& m ){
  return complex< double > ( 1.0, 0.0 );
}

complex< double > C2QQNLO::operator()
     ( const complex< double >& n, const complex< double >& m ){
  complex< double > om1 = 1.0 / ( m + 1.0 );
  complex< double > on1 = 1.0 / ( n + 1.0 );

  return (*dynamic_cast< WC* >( xc_->coeff()->wilsons() )->getqqnlo())( n, m )
    + Kernel::CF * 4.0 * om1 * on1 //! FL part
    ;
}

complex< double > C2GQNLO::operator()
  ( const complex< double >& n, const complex< double >& m ){
  complex< double > om = 1.0 /  m;
  complex< double > om1 = 1.0 / ( m + 1.0 );
  complex< double > on1 = 1.0 / ( n + 1.0 );

  return (*dynamic_cast< WC* >( xc_->coeff()->wilsons() )->getgqnlo())( n, m )
    + Kernel::CF * 4.0 * om * om1 * on1 //! FL part
    ;
}

complex< double > C2QGNLO::operator()
  ( const complex< double >& n, const complex< double >& m ){
  complex< double > on1 = 1.0 / ( n + 1.0 );
  complex< double > on2 = 1.0 / ( n + 2.0 );

  return (*dynamic_cast< WC* >( xc_->coeff()->wilsons() )->getqgnlo())( n, m )
    + 2.0 * on1 * on2 //! FL part
    ;
}

complex< double > C2NULL::operator()
     ( const complex< double >& n, const complex< double >& m ){
  return complex< double > ( 0.0, 0.0 );
}

