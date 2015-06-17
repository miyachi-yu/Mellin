#ifndef _IntegSPolVBoson_WC_hh_
#define _IntegSPolVBoson_WC_hh_

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

namespace IntegSPolVBoson {
  
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
  DEF_COF( CGQNLO ) // qgnlo != gqnlo 
  DEF_COF( CQGNLO ) // qgnlo unpol C (ref hep-ph/9710508 (Gehrmann))
  // assume evo[ 0 ] polarized PDF & evo[ 1 ] unpolarized PDF 
};


// -------- implementation of several template classes -------- //

using namespace std;
using namespace Evolution;
using namespace IntegSPolVBoson;

//for Polarized Drell-Yan x * z * d sigma
// refer to hep-ph/9702263 (T. Gehrmann) for pol case
// refer to Phys.Rev.D 45, 2349 (P. J. Sutton ) for unpol case
template< class DD >
DD CQQLO::SWC( const DD& x, const DD& z ){
// d Dsig / dt dy
  return - (*delta_[ 0 ])( x ) * (*delta_[ 1 ])( z ); 
// d sig / dt dy
//  return (*delta_[ 0 ])( x ) * (*delta_[ 1 ])( z ); 
}

template< class DD >
DD CQQNLO::SWC( const DD& x, const DD& z ){
  //  /*
  //  cout << x01() << " " << x02() << endl;

  DD A = (*delta_[ 0 ])( x ) * (*delta_[ 1 ])( z ) *
    ( M_PI * M_PI / 3.0 - 8.0 + 2.0 * li2_( x01() ) + 2.0 * li2_( x02() )
      + pow( log( 1.0 - x01() ), 2 ) + pow( log( 1.0 - x02() ), 2 )
      + 2.0 * log( x01() / ( 1.0 - x01() ) ) * log( x02() / ( 1.0 - x02() ) )
      );
  
  DD B = (*delta_[ 0 ])( x ) * (*g_[ 1 ])( z ) *
    ( 1.0 - z - ( 1.0 + z * z ) / ( 1.0 - z ) * log( z ) 
      - ( 1.0 + z ) * log( 1.0 - z ) 
      - ( 1.0 + z ) * ( log( 2.0 * z / ( 1.0 + z ) ) 
			+ log( ( 1.0 - x01() ) / x01() ) ) );
  
  DD C = (*delta_[ 0 ])( x ) * 2.0 *
    ( (*dylogplusf_[ 1 ])( z ) + (*dyplusf_[ 1 ])( z ) * 
      ( log( 2.0 * z / ( 1.0 + z ) ) + log( ( 1.0 - x01() ) / x01() ) ) );
  
  // x <-> z interchange in B, C //
  DD D = (*delta_[ 1 ])( z ) * (*g_[ 0 ])( x ) *
    ( 1.0 - x - ( 1.0 + x * x ) / ( 1.0 - x ) * log( x ) 
      - ( 1.0 + x ) * log( 1.0 - x ) 
      - ( 1.0 + x ) * ( log( 2.0 * x / ( 1.0 + x ) ) 
			+ log( ( 1.0 - x02() ) / x02() ) ) );
  
  DD E = (*delta_[ 1 ])( z ) * 2.0 *
    ( (*dylogplusf_[ 0 ])( x ) + (*dyplusf_[ 0 ])( x ) * 
      ( log( 2.0 * x / ( 1.0 + x ) ) + log( ( 1.0 - x02() ) / x02() ) ) );
  
  DD F = (*dydplusf_[ 0 ])( dGA_, x, z );
  
  DD G = (*dHA_)( x, z ) * (*g_[ 0 ])( x ) * (*g_[ 1 ])( z );
  
  //rescale
  //note we assume rescale( 0 ) = rescale( 1 )! symmetryc under p<->p!
  //  DD A += (*delta_[ 0 ])( x ) * (*delta_[ 1 ])( z ) * 
  //    ( 3.0 + 2.0 * log( ( 1.0 - x01() ) / x01() ) 
  //      + 2.0 * log( ( 1.0 - x02() ) / x02()  ) )
  //    * rescale( 0 ); 
  //  DD B += (*delta_[ 0 ])( x ) *
  //    ( 2.0 * (*dyplusf_[ 1 ])( z ) - ( 1.0 + z ) * (*g_[ 1 ])( z ) )
  //    * rescale( 0 );
  //  DD D += (*delta_[ 1 ])( z ) *
  //    ( 2.0 * (*dyplusf_[ 0 ])( x ) - ( 1.0 + x ) * (*g_[ 0 ])( x ) )
  //    * rescale( 0 );

  // d Dsig / dt dy
  return - Kernel::CF * ( A + B + C + D + E + F + G ); 
  // d sig / dt dy
  //  return Kernel::CF * ( A + B + C + D + E + F + G );  
  //  */
  //  return 0.0;
}

template< class DD >
DD CGQNLO::SWC( const DD& x, const DD& z ) { 
  //  /*  // d Dsig / dt dy
  DD A = (*delta_[ 1 ])( z ) * (*g_[ 0 ])( x ) *
    ( ( 2.0 * x - 1.0 ) * ( log( 2.0 * ( 1.0 - x ) / ( 1.0 + x ) ) 
			    + log( ( 1.0 - x02() ) / x02() ) )
      + 2.0 * ( 1.0 - x ) ); 
  
  DD B = (*dyplusf_[ 1 ])( dGC_, x, z ) * (*g_[ 0 ])( x );  
  
  DD C = (*dHC_)( x, z ) * (*g_[ 0 ])( x ) * (*g_[ 1 ])( z );
  
  //rescale
  //
  //    A += (*delta_[ 1 ])( z ) * ( 2.0 * x - 1.0 ) * (*g_[ 0 ])( x )
  //    * rescale( 0 );
  //    
  
  // TR = 1 / 2 
  return - 0.5 * ( A + B + C );
  // */
  
  /*  // d sig / dt dy
  DD A = (*delta_[ 1 ])( z ) * (*g_[ 0 ])( x ) *
    ( ( x * x + ( 1.0 - x ) * ( 1.0 - x ) ) * 
      ( log( 2.0 * ( 1.0 - x ) / ( 1.0 + x ) ) 
	+ log( ( 1.0 - x02() ) / x02() ) )
      + 1.0 );
  
  DD B = (*dyplusf_[ 1 ])( dGC_, x, z ) * (*g_[ 0 ])( x );  
  
  DD C = (*dHC_)( x, z ) * (*g_[ 0 ])( x ) * (*g_[ 1 ])( z );
  
  //rescale
  //
  //    A += (*delta_[ 1 ])( z ) * ( x * x + ( 1.0 - x ) * ( 1.0 - x ) ) 
  //    * (*g_[ 0 ])( x ) * rescale( 0 );
  //
  
  // TR = 1 / 2 
  return 0.5 * ( A + B + C );
  */
}

template< class DD >
DD CQGNLO::SWC( const DD& x, const DD& z ) { 
  /*  // d Dsig / dt dy
  DD A = (*delta_[ 0 ])( x ) * (*g_[ 1 ])( z ) *
    ( ( 2.0 * z - 1.0 ) * ( log( 2.0 * ( 1.0 - z ) / ( 1.0 + z ) ) 
			    + log( ( 1.0 - x01() ) / x01() ) )
      + 2.0 * ( 1.0 - z ) ); 
  
  DD B = (*dyplusf_[ 0 ])( dGC_, z, x ) * (*g_[ 1 ])( z );  
  
  DD C = (*dHC_)( z, x ) * (*g_[ 1 ])( z ) * (*g_[ 0 ])( x );
  
  //rescale
  //
  //    A += (*delta_[ 0 ])( x ) * ( 2.0 * z - 1.0 ) * (*g_[ 1 ])( z )
  //    * rescale( 1 );
  //    
  
  // TR = 1 / 2 
  return - 0.5 * ( A + B + C );
  */
  
  //  /*  // d sig / dt dy
  DD A = (*delta_[ 0 ])( x ) * (*g_[ 1 ])( z ) *
    ( ( z * z + ( 1.0 - z ) * ( 1.0 - z ) ) * 
      ( log( 2.0 * ( 1.0 - z ) / ( 1.0 + z ) ) 
	+ log( ( 1.0 - x01() ) / x01() ) )
      + 1.0 );
  
  DD B = (*dyplusf_[ 0 ])( dUGC_, z, x ) * (*g_[ 1 ])( z );  
  
  DD C = (*dHC_)( z, x ) * (*g_[ 1 ])( z ) * (*g_[ 0 ])( x );
  
  //rescale
  //
  //    A += (*delta_[ 0 ])( x ) * ( z * z + ( 1.0 - z ) * ( 1.0 - z ) ) 
  //    * (*g_[ 1 ])( z ) * rescale( 1 );
  //
  
  // TR = 1 / 2 
  return 0.5 * ( A + B + C );
  //  */
}

template< class DD >
DD CNULL::SWC( const DD& x, const DD& z ){
  return 0.0;
}

/*
  cout << x01() << " " << x02() << endl;
  cout << x << " " << z << endl;
  cout << (*delta_[ 0 ]->getgfunc())( x ) << " "
       << (*delta_[ 1 ]->getgfunc())( z ) << endl; 

  cout << (*plusf_[ 0 ]->getgfunc())( x ) << " "
       << (*plusf_[ 1 ]->getgfunc())( z ) << endl; 

  cout << (*logplusf_[ 0 ]->getgfunc())( x ) << " "
       << (*logplusf_[ 1 ]->getgfunc())( z ) << endl; 

  cout << (*dyplusf_[ 0 ]->getgfunc())( x ) << " "
       << (*dyplusf_[ 1 ]->getgfunc())( z ) << endl; 

  cout << (*dylogplusf_[ 0 ]->getgfunc())( x ) << " "
       << (*dylogplusf_[ 1 ]->getgfunc())( z ) << endl; 

  cout << (*dydplusf_[ 0 ]->getgfunc()[ 0 ])( x ) << " "
       << (*dydplusf_[ 0 ]->getgfunc()[ 1 ])( z ) << endl; 
*/

/*
  cout << x01() << " " << x02() << endl;
  cout << x << " " << z << endl;
  cout << (delta_[ 0 ]->getparam()[ 0 ]) << " "
       << (delta_[ 1 ]->getparam()[ 0 ]) << endl; 

  cout << (plusf_[ 0 ]->getparam()[ 0 ]) << " "
       << (plusf_[ 1 ]->getparam()[ 0 ]) << endl; 
  cout << (logplusf_[ 0 ]->getparam()[ 0 ]) << " "
       << (logplusf_[ 1 ]->getparam()[ 0 ]) << endl; 

  cout << (dyplusf_[ 0 ]->getparam()[ 0 ]) << " "
       << (dyplusf_[ 1 ]->getparam()[ 0 ]) << endl; 

  cout << (dylogplusf_[ 0 ]->getparam()[ 0 ]) << " "
       << (dylogplusf_[ 1 ]->getparam()[ 0 ]) << endl; 

  cout << (dydplusf_[ 0 ]->getparam()[ 0 ][ 0 ]) << " "
       << (dydplusf_[ 0 ]->getparam()[ 1 ][ 0 ]) << endl; 
*/

#endif //  _IntegSPolVBoson_WC_hh_
