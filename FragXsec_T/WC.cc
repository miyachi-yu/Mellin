#include "WC.hh"

#include <cmath>
#include <vector>
#include <Evolution/Kernel.hh>

using namespace std;
using namespace Evolution;
using namespace FragXsec_T;
using namespace Transform;

#define DEF_COF( T )							\
  class T: public Xsection::Coefficient {				\
  public:								\
  T() {}								\
  virtual ~T() {}							\
  virtual complex< double > operator() ( const complex< double >& z );	\
  };

namespace FragXsec_T {
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
  Transverse component
 */
complex< double > CQQNLO::operator()( const complex< double >& z ){

  complex< double > s1  = s_( 1, z );
  complex< double > s12 = s1 * s1;
  complex< double > s2  = s_( 2, z );
  complex< double > z1  = z + 1.0;
  
  return 
    Kernel::CF *
    (
     5.0 * s2 
     + s12 
     + s1 * ( 1.5 - 1.0 / z / z1 ) 
     - 2.0 / z / z 
     + 3.0 / z1 / z1 
     - 1.5 / z1 
     - 4.5
     + ( 1.0 / z / z1 - 2.0 * s1 + 1.5 ) * this->rescale( 0 )
     + 1.0 / z //temporarily add CL part
     )
    ;
}

complex< double > CGGNLO::operator()( const complex< double >& z ) { 
  
  complex< double > z2   = z * z;
  complex< double > zp1  = z + 1.0;
  complex< double > zm1  = z - 1.0;
  complex< double > s1   = s_( 1, z );
  
  return 
    Kernel::CF * 
    (
     2.0 * ( - s1 * ( z2 + z + 2.0 ) / zm1 / z / zp1 
	     - 4.0 / pow( zm1, 2 ) 
	     + 4.0 / z2 
	     - 3.0 / pow( zp1, 2 )
	     )
     + 2.0 * ( z2 + z + 2.0 ) / z / ( z2 - 1.0 ) * this->rescale( 0 )
     + 4.0 / ( z - 1.0 ) / z //temporarily add CL part
     )
    ;
}


/*
  Longitudinal component
*/
/* CL
   complex< double > WC::CQQNLO::operator()( const complex< double >& z, const complex< double >& y )
{
return Kernel::CF * 1.0 / z ;
}

complex< double > WC::CGGNLO::operator()( const complex< double >& z, const complex< double >& y )
{
     return Kernel::CF * 4.0 / ( z - 1.0 ) / z ;
}
*/


