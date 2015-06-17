#include "XsecCorrCalc.hh"
#include <Xsec/Xsec.hh>
#include <Xsec/XsecComp.hh>
#include <Xsec/xXsec.hh>
#include <Xsec/xzXsec.hh>
#include <fstream>
#include <TGraph.h>
#include <TCanvas.h>
//#include <stdio.h>

using namespace std;
using namespace QCD;
using namespace Evolution;
using namespace Xsection;
using namespace Transform;
using namespace ErrorCalc;

XsecCorrCalc::XsecCorrCalc( Xsection::Xsec* xsec1, 
			    Xsection::Xsec* xsec2 ) :
  ErrCalc( xsec1 ), vxsec_( 0 ), vxxsec_( 0 ), vxzxsec_( 0 )
{
  vxsec_.push_back( xsec1 );
  if( xsec2 && xsec1->xsecComp()->vevo() == xsec2->xsecComp()->vevo() ){
    vxsec_.push_back( xsec2 );
  } else {
    ARG_LOG << "XsecCorrCalc Warning :" << endl;
    ARG_LOG << "xsec1 & xsec2 are inconsistent in their vevos" << endl;
    ARG_LOG << "xsec1 is assinged in second vxsec" << endl;
    vxsec_.push_back( xsec1 );
  }

}

XsecCorrCalc::XsecCorrCalc( Xsection::xXsec* xsec1, 
			    Xsection::xXsec* xsec2 ) :
  ErrCalc( xsec1 ), vxsec_( 0 ), vxxsec_( 0 ), vxzxsec_( 0 )
{
  vxxsec_.push_back( xsec1 );
  if( xsec2 && xsec1->moment()->xsecComp()->vevo() 
      == xsec2->moment()->xsecComp()->vevo() ){
    vxxsec_.push_back( xsec2 );
  } else {
    ARG_LOG << "XsecCorrCalc Warning :" << endl;
    ARG_LOG << "xxsec1 & xxsec2 are inconsistent in their vevos" << endl;
    ARG_LOG << "xxsec1 is assinged in second vxxsec" << endl;
    vxxsec_.push_back( xsec1 );
  }
}

XsecCorrCalc::XsecCorrCalc( Xsection::xzXsec* xsec1, 
			    Xsection::xzXsec* xsec2 ) :
  ErrCalc( xsec1 ), vxsec_( 0 ), vxxsec_( 0 ), vxzxsec_( 0 )
{
  vxzxsec_.push_back( xsec1 );
  if( xsec2 && xsec1->moment()->xsecComp()->vevo() 
      == xsec2->moment()->xsecComp()->vevo() ){
    vxzxsec_.push_back( xsec2 );
  } else {
    ARG_LOG << "XsecCorrCalc Warning :" << endl;
    ARG_LOG << "xzxsec1 & xzxsec2 are inconsistent in their vevos" << endl;
    ARG_LOG << "xzxsec1 is assinged in second vxzxsec" << endl;
    vxzxsec_.push_back( xsec1 );
  }
}

XsecCorrCalc::~XsecCorrCalc(){}

complex< double > XsecCorrCalc::operator()( const complex< double >& n11, 
					    const complex< double >& n22 ){

  complex< double > n1( n11 ), n2( n22 );

  //  if( n2 == complex< double >( 0.0, 0.0 ) ) n2 = n1;
  this->getN() = n1; this->getM() = n2; 

  // ccalc judge in setF_G ( true )
  // dcalc judge false in default
  this->dfunccalc( false );
  
  complex< double > err_( complex< double >( 0.0, 0.0 ) );

  if( vxsec_.size() ){
    err_ = this->calcerror( vxsec_[ 0 ], vxsec_[ 1 ] );
  } else {
    ARG_ERR << "operator( n1, n2 ) requires Xsec vector ( f(n) )"
	    << endl;
    throw( 1 );
  }

  if( err_.imag() != 0.0 ) {
    ARG_ERR << "calculated error has non-zero imaginary part."
	    << endl;
    throw( 2 );
  }

  return err_.real();
}

complex< double > XsecCorrCalc::operator()( const complex< double >& n11, 
					    const complex< double >& m11,
					    const complex< double >& n22,
					    const complex< double >& m22 ){
  
  complex< double > n1( n11 ), n2( n22 );
  complex< double > m1( m11 ), m2( m22 );

  //  if( n2 == complex< double >( 0.0, 0.0 ) ) n2 = n1;
  //  if( m2 == complex< double >( 0.0, 0.0 ) ) m2 = m1;
  this->getN() = n1; this->getNp() = m1;
  this->getM() = n2; this->getMp() = m2;

  // ccalc judge in setF_G ( true )
  // dcalc judge false in default
  this->dfunccalc( true );

  complex< double > err_( complex< double >( 0.0, 0.0 ) );

  if( vxsec_.size() ){
    err_ = this->calcerror( vxsec_[ 0 ], vxsec_[ 1 ] );
  } else {
    ARG_ERR << "operator( n1, m1, n2, m2 ) requires Xsec vector ( f(n,m) )"
	    << endl;
    throw( 1 );
  }

  if( err_.imag() != 0.0 ) {
    ARG_ERR << "calculated error has non-zero imaginary part."
	    << endl;
    throw( 2 );
  }

  return err_.real();
}

double XsecCorrCalc::operator()( const double& x11, const double& x22 ){

  double x1( x11 ), x2( x22 );

  //  if( !x2 ) x2 = x1;
  this->getX() = x1; this->getZ() = x2;

  // ccalc judge in setF_G ( false )
  // dcalc judge false in default
  this->dfunccalc( false );

  double err_( 0.0 );

  if( vxxsec_.size() ){
    err_ = this->calcerror( vxxsec_[ 0 ], vxxsec_[ 1 ] );
  } else {
    ARG_ERR << "operator( x1, x2 ) requires xXsec vector ( f(x) )" 
	    << endl;
    throw( 1 );
  }

  return err_;
}

double XsecCorrCalc::operator()( const double& x11, const double& z11,
				 const double& x22, const double& z22 ){

  double x1( x11 ), x2( x22 );
  double z1( z11 ), z2( z22 );

  //  if( !x2 ) x2 = x1;
  //  if( !z2 ) z2 = z1;
  this->getX() = x1; this->getXp() = z1; 
  this->getZ() = x2; this->getZp() = z2;

  // ccalc judge in setF_G ( false )
  // dcalc judge false in default
  this->dfunccalc( true );

  double err_( 0.0 );

  if( vxxsec_.size() ){
    err_ = this->calcerror( vxxsec_[ 0 ], vxxsec_[ 1 ] );
  } else {
    // note xzXsec does not permit f( x, z ) operation
    // only f( vx, vz )
    ARG_ERR << "operator( x1, z1, x2, z2 ) requires xXsec vector ( f(x,z) )" 
	    << endl;
    throw( 1 );
  }

  return err_;
}

double XsecCorrCalc::operator()( const vector< double >& vx11, 
				 const vector< double >& vx22 ){

  vector< double > vx1( vx11 ), vx2( vx22 );

  //  if( !vx2.size() ) vx2 = vx1;
  if( !vx1.size() || !vx2.size() ){
    ARG_ERR << "vx1 must be non-zero vector"
	    << endl;
    throw( 1 );
  }

  this->getvX() = vx1; this->getvZ() = vx2;

  // ccalc judge in setF_G ( false )
  // dcalc judge false in default
  // vectvar judge false in default
  this->dfunccalc( false );
  this->vectcalc( true );

  double err_( 0.0 );

  if( vxxsec_.size() ){
    err_ = this->calcerror( vxxsec_[ 0 ], vxxsec_[ 1 ] );
  } else {
    ARG_ERR << "operator( vx1, vx2 ) requires xXsec vector ( f(vx) )" 
	    << endl;
    throw( 1 );
  }

  return err_;
}

double XsecCorrCalc::operator()( const vector< double >& vx11,
				 const vector< double >& vz11,
				 const vector< double >& vx22,
				 const vector< double >& vz22 ){

  vector< double > vx1( vx11 ), vx2( vx22 );
  vector< double > vz1( vz11 ), vz2( vz22 );

  //  if( !vx2.size() ) vx2 = vx1;
  //  if( !vz2.size() ) vz2 = vz1;
  if( !vx1.size() || !vz1.size() || !vx2.size() || !vz2.size() ){
    ARG_ERR << "vx1 & vz1 & vx2 & vz2 must be non-zero vectors"
	    << endl;
    throw( 1 );
  }

  this->getvX() = vx1; this->getvXp() = vz1; 
  this->getvZ() = vx2; this->getvZp() = vz2;

  // ccalc judge in setF_G ( false )
  // dcalc judge false in default
  // vectvar judge false in default
  this->dfunccalc( true );
  this->vectcalc( true );

  double err_( 0.0 );

  if( vxxsec_.size() ){
    err_ = this->calcerror( vxxsec_[ 0 ], vxxsec_[ 1 ] );
  } else if ( vxzxsec_.size() ){
    err_ = this->calcerror( vxzxsec_[ 0 ], vxzxsec_[ 1 ] );
  } else {
    ARG_ERR << "operator( vx1, vz1, vx2, vz2 ) " 
	    << "requires xXsec or xzXsec vector (f(vx,vz)) " 
	    << endl;
    throw( 1 );
  }

  return err_;
}

void XsecCorrCalc::idswap(){
  if( vxsec_.size() ){
    Xsec* xsec_ = vxsec_[ 0 ];
    vxsec_[ 0 ] = vxsec_[ 1 ]; vxsec_[ 1 ] = xsec_;
  }
  if( vxxsec_.size() ){
    xXsec* xxsec_ = vxxsec_[ 0 ];
    vxxsec_[ 0 ] = vxxsec_[ 1 ]; vxxsec_[ 1 ] = xxsec_;
  }
  if( vxzxsec_.size() ){
    xzXsec* xzxsec_ = vxzxsec_[ 0 ];
    vxzxsec_[ 0 ] = vxzxsec_[ 1 ]; vxzxsec_[ 1 ] = xzxsec_;
  }
}
