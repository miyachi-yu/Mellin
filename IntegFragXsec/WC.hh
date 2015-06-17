#ifndef _IntegFragXsec_WC_hh_
#define _IntegFragXsec_WC_hh_

#include <IntegXsec/IntegXsecComp.hh>
#include <IntegXsec/Wilsons.hh>
#include <Evolution/Kernel.hh>
#include <cmath>

#include "COFbase.hh"

#define DEF_COF( T ) class T: public COFbase { \
public: \
T( IntegXsec::IntegXsecComp* xc ):COFbase( xc ){}; \
virtual ~T(){}; \
virtual double operator()( const double& x ) \
{ return this->SWC( x ); } \
virtual std::complex< double > operator()( const std::complex< double >& n ) \
{ return this->SWC( n ); } \
private: \
template< class DD > \
DD SWC( const DD& x ); \
};

namespace IntegFragXsec {
  
  class WC : public IntegXsec::Wilsons {
  public:
    
    WC( IntegXsec::IntegXsecComp* xc );
    virtual ~WC();
    
  private:
    
    COFbase* qqlo ;
    COFbase* qqnlo;
    COFbase* ggnlo;
    COFbase* cnull;

  };

  DEF_COF( CNULL )
  DEF_COF( CQQLO )
  DEF_COF( CQQNLO )
  DEF_COF( CGGNLO )
  
};


// -------- implementation of several template classes -------- //

using namespace std;
using namespace Evolution;
using namespace IntegFragXsec;

//for DIS SIA 2 x * F1
template< class DD >
DD CQQLO::SWC( const DD& x ){
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
  return (*delta_[ 0 ])( x ); // / x ; // "/ x" remnant of integration
}

template< class DD >
DD CQQNLO::SWC( const DD& x ){
  DD A = (*delta_[ 0 ])( x ) * ( 2.0 / 3.0 * M_PI * M_PI - 9.0 / 2.0 ); 
  DD B = - 3.0 / 2.0 * (*plusf_[ 0 ])( x );
  DD C = 2.0 * (*logplusf_[ 0 ])( x ); 
  DD D =  - ( 1.0 + x ) * log( 1.0 - x ) 
    + 2.0 * ( 1.0 + x * x ) / ( 1.0 - x ) * log( x )
    + 3.0 / 2.0 * ( 1.0 - x ) 
    + 1.0; // FL part 
  D *= (*g_[ 0 ])( x );

  // rescale
  //  A += 3.0 / 2.0 * (*delta_[ 0 ])( x ) * rescale( 0 );
  //  B += 2.0 * (*plusf_[ 0 ])( x ) * rescale( 0 );
  //  D += - ( 1.0 + x ) * (*g_[ 0 ])( x ) * rescale( 0 );

  return Kernel::CF * ( A + B + C + D );
}

template< class DD >
DD CGGNLO::SWC( const DD& x ) { 
  DD A = ( 1.0 + ( 1.0 - x ) * ( 1.0 - x ) ) / x * log( x * x * ( 1.0 - x ) )
    - 2.0 * ( 1.0 - x ) / x
    + 2.0 * ( 1.0 - x ) / x; // FL part 

  // rescale
  //  + ( 1.0 + ( 1.0 - x ) * ( 1.0 - x ) ) / x * rescale( 0 );
  
  // TR = 1 / 2 -> CF !! // 2.0 for 2 * F1
  return Kernel::CF * 2.0 * A * (*g_[ 0 ])( x );
}

template< class DD >
DD CNULL::SWC( const DD& x ){
  return 0.0;
}

#endif //  _IntegFragXsec_WC_hh_
