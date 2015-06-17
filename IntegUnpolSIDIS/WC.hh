#ifndef _IntegUnpolSIDIS_WC_hh_
#define _IntegUnpolSIDIS_WC_hh_

#include <IntegXsec/IntegXsecComp.hh>
#include <IntegXsec/Wilsons.hh>
#include <Evolution/Kernel.hh>
#include <cmath>

#include "COFbase.hh"

#define DEF_COF( T ) class T: public COFbase { \
public: \
T( IntegXsec::IntegXsecComp* xc ):COFbase( xc ){}; \
virtual ~T(){}; \
virtual double operator()( const double& x, const double& z ) \
{ return this->SWC( x, z ); } \
virtual std::complex< double > operator() \
( const std::complex< double >& n, const std::complex< double >& m ) \
{ return this->SWC( n, m ); } \
private: \
template< class DD > \
DD SWC( const DD& x, const DD& z ); \
};

namespace IntegUnpolSIDIS {
  
  class WC : public IntegXsec::Wilsons {
  public:
    
    WC( IntegXsec::IntegXsecComp* xc );
    virtual ~WC();
    
  private:
    
    COFbase* qqlo ;
    COFbase* qqnlo;
    COFbase* gqnlo;
    COFbase* qgnlo;
    COFbase* cnull;

  };

  DEF_COF( CNULL )
  DEF_COF( CQQLO )
  DEF_COF( CQQNLO )
  DEF_COF( CGQNLO )
  DEF_COF( CQGNLO )
  
};


// -------- implementation of several template classes -------- //

using namespace std;
using namespace Evolution;
using namespace IntegUnpolSIDIS;

//for SIDIS SIA 2 x * z * G1h
// refer to Phys. Rev. D57, 5811 (1998)
template< class DD >
DD CQQLO::SWC( const DD& x, const DD& z ){
  /*
  cout << "x: " << x << endl;
  cout << "DeltaFunc param:" << endl;
  cout << "y dy" << delta_[ 0 ]->getparam()[ 0 ] << endl;
  cout << "n" << delta_[ 0 ]->getparam()[ 1 ] << endl;
  cout << "ConvIntegPDF param:" << endl;
  cout << "y dy" << delta_[ 0 ]->getgfunc()->getparam()[ 0 ] << endl;
  cout << "n" << delta_[ 0 ]->getgfunc()->getparam()[ 1 ] << endl;
  cout << "Delta Value:" << endl;
  cout << (*delta_[ 0 ])( x ) << endl;
  */
  /*
  cout << "z: " << z << endl;
  cout << "DeltaFunc param:" << endl;
  cout << "y dy" << delta_[ 1 ]->getparam()[ 0 ] << endl;
  cout << "n" << delta_[ 1 ]->getparam()[ 1 ] << endl;
  cout << "ConvIntegPDF param:" << endl;
  cout << "y dy" << delta_[ 1 ]->getgfunc()->getparam()[ 0 ] << endl;
  cout << "n" << delta_[ 1 ]->getgfunc()->getparam()[ 1 ] << endl;
  cout << "Delta Value:" << endl;
  cout << (*delta_[ 1 ])( z ) << endl;
  */
  return (*delta_[ 0 ])( x ) * (*delta_[ 1 ])( z );
}

template< class DD >
DD CQQNLO::SWC( const DD& x, const DD& z ){
  DD A = - 8.0 *  (*delta_[ 0 ])( x ) * (*delta_[ 1 ])( z );
  DD B = (*delta_[ 0 ])( x ) * 
    ( 2.0 * (*logplusf_[ 1 ])( z ) + 
      ( - ( 1.0 + z ) * log( 1.0 - z ) + 
	( 1.0 + z * z ) / ( 1.0 - z ) * log( z ) + 1.0 - z ) * (*g_[ 1 ])( z )
      );
  DD C = (*delta_[ 1 ])( z ) * 
    ( 2.0 * (*logplusf_[ 0 ])( x ) + 
      ( - ( 1.0 + x ) * log( 1.0 - x ) - 
	( 1.0 + x * x ) / ( 1.0 - x ) * log( x ) + 1.0 - x ) * (*g_[ 0 ])( x )
      );
  DD D = 2.0 * (*plusf_[ 0 ])( x ) * (*plusf_[ 1 ])( z );
  DD E = - 1.0 * (*plusf_[ 0 ])( x ) * ( 1.0 + z ) * (*g_[ 1 ])( z );
  DD F = - 1.0 * (*plusf_[ 1 ])( z ) * ( 1.0 + x ) * (*g_[ 0 ])( x );
  DD G = 2.0 * ( 1.0 + x * z );
    //    - 2.0 * ( 1.0 - x ) * ( 1.0 - z ); // for dCqq 
  G *= (*g_[ 0 ])( x ) * (*g_[ 1 ])( z );
  
  // rescale
  /*
    B += ( 2.0 * (*plusf_[ 1 ])( z ) - ( 1.0 + z ) * (*g_[ 1 ])( z ) 
    + 3.0 / 2.0 * (*delta_[ 1 ])( z ) ) 
    * (*delta_[ 0 ])( x ) * rescale( 1 );
    C += ( 2.0 * (*plusf_[ 0 ])( x ) - ( 1.0 + x ) * (*g_[ 0 ])( x ) 
    + 3.0 / 2.0 * (*delta_[ 0 ])( x ) ) 
    * (*delta_[ 1 ])( z ) * rescale( 0 );
    */
  return Kernel::CF * ( A + B + C + D + E + F + G ); 
}

template< class DD >
DD CGQNLO::SWC( const DD& x, const DD& z ) { 
  DD A = ( (*delta_[ 0 ])( x ) * log ( z * ( 1.0 - z ) )
	       + (*plusf_[ 0 ])( x ) )
    * ( 1.0 + ( 1.0 - z ) * ( 1.0 - z ) ) / z * (*g_[ 1 ])( z );
  DD B = z * (*delta_[ 0 ])( x ) * (*g_[ 1 ])( z );
  DD C = 2.0 * ( 1.0 + x - x * z ) - ( 1.0 + x ) / z;
    //    - 2.0 * z * ( 1.0 - x ); // for dCgq
  C *= (*g_[ 0 ])( x ) * (*g_[ 1 ])( z );

  //rescale
  /*
    A += (*delta_[ 0 ])( x ) 
    * ( 1.0 + ( 1.0 - z ) * ( 1.0 - z ) ) / z * (*g_[ 1 ])( z ) 
    * rescale( 1 );
    */

    return Kernel::CF * ( A + B + C );
}

template< class DD >
DD CQGNLO::SWC( const DD& x, const DD& z ) { 
  DD A = (*delta_[ 1 ])( z ) * 
    ( ( x * x + ( 1.0 - x ) * ( 1.0 - x ) ) * log( ( 1.0 - x ) / x )
      + 2.0 * x * ( 1.0 - x ) ) * (*g_[ 0 ])( x );
  DD B = ( x * x + ( 1.0 - x ) * ( 1.0 - x ) ) * (*g_[ 0 ])( x )
    * ( (*plusf_[ 1 ])( z ) + ( 1.0 / z - 2.0 ) * (*g_[ 1 ])( z ) );

  // rescale
  /*
    A += (*delta_[ 1 ])( z ) * 
    ( x * x + ( 1.0 - x ) * ( 1.0 - x ) ) * (*g_[ 0 ])( x )
    * rescale( 0 );
    */  
  
  // TR = 1 / 2, 
  return 0.5 * ( A + B );
}

template< class DD >
DD CNULL::SWC( const DD& x, const DD& z ){
  return 0.0;
}

#endif //  _IntegUnpolSIDIS_WC_hh_
