#include "Wilson.hh"
#include <Evolution/Kernel.hh>

#include <Xsec/XsecComp.hh>
#include <Xsec/CKernel.hh>
#include <cmath>

using namespace std;
using namespace Evolution;

/*
  Implementation for Xsec based Structure Function calculation
*/
#define DEF_COF( T )							\
  class T: public Xsection::Coefficient {				\
  public:								\
  T() {}								\
  virtual ~T() {}							\
  virtual complex< double > eval( const complex< double >& z );		\
  virtual complex< double > eval( const complex< double >& n,		\
				  const complex< double >& m )		\
    { return this->eval( n ); }						\
  };

namespace polDIS {
  
  DEF_COF( CQQLO )
  DEF_COF( CQQNLO )
  DEF_COF( CGGNLO )
  DEF_COF( CNULL )
  
}


using namespace polDIS;

Wilson::Wilson( Xsection::XsecComp* xc ) : 
  Xsection::Wilsons(),
  qlo( NULL ), qnlo( NULL ), gnlo( NULL ), cnull( NULL )
{

  qlo   = new CQQLO();
  qnlo  = new CQQNLO();
  gnlo  = new CGGNLO();
  cnull = new CNULL();
  
  qnlo->set( xc );
  gnlo->set( xc );
  
  // for DIS SIA
  (*this)[ 0 ][ 0 ][ 0 ] = qlo; 
  (*this)[ 0 ][ 0 ][ 1 ] = qnlo;
  (*this)[ 0 ][ 1 ][ 0 ] = cnull;     // ( 0.0, 0.0 )
  (*this)[ 0 ][ 1 ][ 1 ] = cnull;     // ( 0.0, 0.0 )
  (*this)[ 0 ][ 2 ][ 0 ] = cnull;     // ( 0.0, 0.0 )
  (*this)[ 0 ][ 2 ][ 1 ] = cnull;     // ( 0.0, 0.0 )
  (*this)[ 1 ][ 0 ][ 0 ] = cnull;     // ( 0.0, 0.0 )
  (*this)[ 1 ][ 0 ][ 1 ] = cnull;     // ( 0.0, 0.0 )
  (*this)[ 1 ][ 1 ][ 0 ] = qlo;
  (*this)[ 1 ][ 1 ][ 1 ] = qnlo;
  (*this)[ 1 ][ 2 ][ 0 ] = cnull;     // ( 0.0, 0.0 )
  (*this)[ 1 ][ 2 ][ 1 ] = cnull;     // ( 0.0, 0.0 )
  (*this)[ 2 ][ 0 ][ 0 ] = cnull;     // ( 0.0, 0.0 )
  (*this)[ 2 ][ 0 ][ 1 ] = cnull;     // ( 0.0, 0.0 )
  (*this)[ 2 ][ 1 ][ 0 ] = cnull;     // ( 0.0, 0.0 )
  (*this)[ 2 ][ 1 ][ 1 ] = cnull;     // ( 0.0, 0.0 )
  (*this)[ 2 ][ 2 ][ 0 ] = cnull;     // ( 0.0, 0.0 )
  (*this)[ 2 ][ 2 ][ 1 ] = gnlo;
}

Wilson::~Wilson(){
  delete qlo;
  delete qnlo;
  delete gnlo;
  delete cnull;
}

complex< double > CQQLO::eval( const complex< double >& n ) {
  return complex< double >( 1.0, 0.0 );
}

complex< double > CNULL::eval( const complex< double >& n ) {
  return complex< double >( 0.0, 0.0 );
}

/*
2 * g1
*/
complex< double > CQQNLO::eval( const complex< double >& n ) {
  
  complex< double > s1 = s_( 1, n );
  complex< double > s2 = s_( 2, n );
  complex< double > on = 1.0 / n;
  complex< double > on1 = 1.0 / ( n + 1.0 );
  
  return Kernel::CF * 
    ( - s2
      + s1 * s1 
      + ( 1.5 - on * on1 ) * s1
      + 1.0 * on * on
      + 1.5 * on
      - 1.0 * on1 * on 
      - 4.5 
      + ( 1.5 + on * on1 - 2.0 * s1 ) * this->rescale()
      );
}

complex< double > CGGNLO::eval( const complex< double >& n ) {
  
  complex< double > s1 = s_( 1, n );
  complex< double > on = 1.0 / n;
  complex< double > on1 = 1.0 / ( n + 1.0 );
  
  // 1/2 (TR) * 1/2 ( 4pi -> 2pi ) * 4 (original factor 2 * 2 (q+qb) ) -> 1
  return 
    ( - ( n - 1.0 ) * on * on1 * ( s1 + 1.0 )
      - 1.0 * on * on * on1
      + 1.0 * on * on1
      + ( 2.0 * on1 - on ) * this->rescale()
      );
}

/*
  Old Implementation 

complex< double > CQLO::operator() ( const complex< double >& n ) {
  return complex< double >( 1.0, 0.0 );
}

complex< double > CGLO::operator() ( const complex< double >& n ) {
  return complex< double >( 0.0, 0.0 );
}

complex< double > CQNLO::operator() ( const complex< double >& n ) {
  
  GeneralHarmonic s;
  
  complex< double > s1 = s( 1, n );
  complex< double > s2 = s( 2, n );
  
  return Kernel::CF * 
    ( - s2
      + s1 * s1 
      + ( 1.5 - 1.0 / n / ( n + 1.0 ) ) * s1
      + 1.0 / n / n
      + 0.5 / n
      + 1.0 / ( n + 1.0 )
      - 4.5 );
}

complex< double > CGNLO::operator() ( const complex< double >& n ) {
  GeneralHarmonic s;
  return 
    //    Alpha::flavor()( p_->kernel()->Q() ) *
    p_->kernel()->dglapPdf().nf() *
    ( - ( n - 1.0 ) / n / ( n + 1.0 ) * ( s( 1, n ) + 1.0 )
      - 1.0 / n / n
      + 2.0 / n / ( n + 1.0 )
      );
}
*/
