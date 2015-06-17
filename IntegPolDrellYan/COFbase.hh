#ifndef _IntegPolDrellYan_COFbase_hh_
#define _IntegPolDrellYan_COFbase_hh_

#include <IntegXsec/ConvIntegrand.hh>
#include <IntegXsec/IntegXsecComp.hh>
#include <Tranform/Li2.hh>
#include <Tranform/RealCompFunction.hh>

#include "DYDPlusFunc.hh"
#include "DYPlusFunc.hh"
#include "DYLogPlusFunc.hh"

#define DEF_FUNC( T ) class T: public Transform::RealCompFunction { \
public: \
T(){}; \
virtual ~T(){}; \
\
virtual double operator() ( const double& x ){ return 0.0; }; \
virtual double operator() ( const double& x, const double& z ) \
{ return this->SWC( x, z ); } \
\
virtual std::complex< double > operator() ( const std::complex< double >& n ) \
{ return std::complex< double >( 0.0, 0.0 ); } \
virtual std::complex< double > operator() \
( const std::complex< double >& n, const std::complex< double >& m ) \
{ return this->SWC( n, m ); } \
\
private: \
template< class DD > \
DD SWC( const DD& x, const DD& z ); \
};

namespace IntegPolDrellYan {
  
  class COFbase : public IntegXsec::ConvIntegrand {
  public:
    COFbase( IntegXsec::IntegXsecComp* xc ) throw( int );
                 //!< a default constructor
    virtual ~COFbase();        //!< a destructor

    virtual double operator()( const double& x )
    { return 0.0; }
    virtual double operator()( const double& x, const double& z )
    { return 0.0; }
    
    virtual std::complex< double > operator()
      ( const std::complex< double >& n )
    { return std::complex< double >( 0.0, 0.0 ); }
    virtual std::complex< double > operator()
      ( const std::complex< double >& n, const std::complex< double >& m )
    { return std::complex< double >( 0.0, 0.0 ); }
    
    virtual double rescale( const int& i = 0 ) throw( int );

    double x01(){ return vy_[ 0 ][ 0 ].real(); }
    double x02(){ return vy_[ 1 ][ 0 ].real(); }

  protected:
    static Transform::Li2 li2_;
    const std::vector< std::vector< std::complex< double > > >& vy_;

    std::vector< DYPlusFunc* > dyplusf_;
    std::vector< DYDPlusFunc* > dydplusf_;
    std::vector< DYLogPlusFunc* > dylogplusf_;

    Transform::RealCompFunction* dGA_;
    Transform::RealCompFunction* dHA_;
    Transform::RealCompFunction* dGC_;
    Transform::RealCompFunction* dHC_;

  };

  DEF_FUNC( DGA );
  DEF_FUNC( DHA );
  DEF_FUNC( DGC );
  DEF_FUNC( DHC );
};


// -------- implementation of several template classes -------- //

using namespace std;
using namespace Transform;
using namespace IntegPolDrellYan;

template< class DD >
DD DGA::SWC( const DD& x, const DD& z ){
  return 2.0 * ( 1.0 + x * z ) * ( 1.0 + x * x * z * z ) 
    / ( 1.0 + x ) / ( 1.0 + z );
}

template< class DD >
DD DHA::SWC( const DD& x, const DD& z ){
  return - 4.0 * x * z * ( 1.0 + x * z ) / ( x + z ) / ( x + z );
}

template< class DD >
DD DGC::SWC( const DD& x, const DD& z ){
  // d Dsig / dt dy
  //  /*
  return 2.0 * z * ( pow( x * z , 2 ) - pow( x * z - 1.0 , 2 ) ) 
    * ( x * z + 1.0 ) / ( x + z ) / ( 1.0 + z );
  //    */
  // d sig / dt dy
  /*
    return 2.0 * z * ( pow( x * z , 2 ) + pow( x * z - 1.0 , 2 ) ) 
    * ( x * z + 1.0 ) / ( x + z ) / ( 1.0 + z );
    */
}

template< class DD >
DD DHC::SWC( const DD& x, const DD& z ){
  return 2.0 * x * z * ( x * z + 1.0 ) * ( x + x * z * ( z + 2.0 * x ) )
    / pow( x + z ,3 );
}

#endif //  _IntegPolDrellYan_COFbase_hh_
