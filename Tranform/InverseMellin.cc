#include "InverseMellin.hh"
#include "LaguerreQuadrature.hh"

#include <iostream>
#include <iomanip>
#include <cmath>

using namespace std;
using namespace Transform;


// --------------------  Integrand function ------------------------------ //

InverseMellin::Integrand::Integrand( InverseMellinIntegrand* f, IntegPath *p1, IntegPath *p2 ) :
  f_( f ), p1_( p1 ), p2_( p2 ) {
} 

void InverseMellin::Integrand::setIMIntegrand( InverseMellinIntegrand* iminteg )
{ f_ = iminteg; }      

double InverseMellin::Integrand::operator()( const double& t ){
  return ( p1_->differential( t ) * (*f_)( (*p1_)( t ) ) ).imag();
}

double InverseMellin::Integrand::operator()( const double& t1, 
					     const double& t2 ){

  complex< double > n = (*p1_)( t1 );
  complex< double > dn = p1_->differential( t1 );
  
  complex< double > m = ( p2_ ? (*p2_)( t2 ) : (*p1_)( t2 ) );
  complex< double > dm =( p2_ ? 
			  p2_->differential( t2 ) :
			  p1_->differential( t2 ) );
  
  return 
    ( dn * 
      ( dm * (*f_)( n, m ) -  conj( dm ) * (*f_)( n, conj( m ) ) ) ).real();
}

double InverseMellin::SteepInteg::operator()( const double& u ){
  
  double squ = c2 * sqrt( u );
  
  return
    exp( u ) * ( complex< double >( 1.0, - c3 * squ ) *
		 (*F_)( path_( squ ) ) ).real();
}

void 
InverseMellin::SteepInteg::setIMIntegrand( InverseMellinIntegrand* iminteg ){
  F_ = iminteg; 
}

// --------------------  Mellin Inversion  ------------------------- //
//	a default constructor. 
InverseMellin::InverseMellin( ComplexFunction *g, Integration *inter ) :
  Transformer(),
  type_( STRAIGHT ), steepest_(), straight1_(), straight2_(), 
  vupper_( 2, 10.0 ), vlower_( 2, 0.0 ),
  func_( g ), integ_( NULL, NULL, NULL ), steepInt_( NULL, steepest_ ),
  c_( 0.0 ), phi_( 0.0 ), activefunc_( NULL ),
  upperHalf_( 0.0 ), lowerHalf_( 0.0 )
{
  this->setIMIntegrand( &func_ );
  
  LaguerreQuadrature* lag = dynamic_cast< LaguerreQuadrature* >( inter );
  
  this->integration( inter );
  
  if( lag == NULL ){
    this->integPath( STRAIGHT );
    this->upper( 10. );
    this->lower( 0.0 );
  } else {
    this->integPath( STEEPEST );
  }
}

//a constructor with integration method 
InverseMellin::InverseMellin( Integration *inter ) :
  Transformer(),
  type_( STRAIGHT ), steepest_(), straight1_(), straight2_(),
  vupper_( 2, 10.0 ), vlower_( 2, 0.0 ),
  func_( NULL ), integ_( NULL, NULL, NULL ),
  steepInt_( NULL, steepest_ ),
  c_( 0.0 ), phi_( 0.0 ), activefunc_( NULL ),
  upperHalf_( 0.0 ), lowerHalf_( 0.0 )
{
  this->setIMIntegrand( &func_ );
  
  this->integration( inter );
  LaguerreQuadrature* lag = dynamic_cast< LaguerreQuadrature* >( inter );
  
  if( lag == NULL ){
    this->integPath( STRAIGHT );
    this->upper( 10. );
    this->lower( 0.0 );
  } else {
    this->integPath( STEEPEST );
  }
  
}

void InverseMellin::setParameter( const double& c, const double& phi ){
  
  c_ = c;
  phi_ = phi;
  
  straight1_.set( c, phi );
  straight2_.set( c, phi );
  
}	

void InverseMellin::setParameter( const vector< double >& c,
				  const vector< double >& phi ){
  
  if( c.size() == 0 || phi.size() == 0 ) {
    cerr 
      << __FILE__ << ":" << __LINE__ 
      << "  size must be grater than 1."
      << endl;
    return;
  }
  
  c_ = c[0];
  phi_ = phi[0];
  
  straight1_.set( c_, phi_ );
  
  // ignore elements above 2nd.....
  straight2_.set( c[ c.size() > 1 ? 1 : 0 ] ,
		  phi[ c.size() > 1 ? 1 : 0 ] );
  
}

//	a destructor 
InverseMellin::~InverseMellin(){ }

double InverseMellin::integrate_(){
  
  static double pinv = 1.0 / M_PI;
  
  // a straight contour
  if( type_ == STRAIGHT ) {

    return
      pinv * this->integration()->integral( this->lower(), this->upper(),
					    integ_ ) ;
  }
  
  return 
    0.5 * steepInt_.c2 * pinv *
    this->integration()->integral( this->lower(), this->upper(), steepInt_  );
  
}

//calculate a value of M-1(f)(z) where M-1 means InerseMellin transform 
double InverseMellin::operator() ( const double& x ) {
  activefunc_->x( x );
  return this->integrate_();
}

double InverseMellin::operator() ( const double& x, const double& z ) {
  activefunc_->x( x );
  activefunc_->z( z );
  static double c = - 0.5 / M_PI / M_PI;
  return c * this->integration()->integral( vlower_[0], vupper_[0], 
					    vlower_[1], vupper_[1],
					    integ_ );
}

double InverseMellin::operator()( const vector< double >& x,
				  const vector< double >& z ){
  if( x.size() == 0 || x.size() > 2 ) return 0.0;
  if( z.size() == 0 || z.size() > 2 ) (*this)( x );
  activefunc_->x( x );
  activefunc_->z( z );
  static double c = - 0.5 / M_PI / M_PI;
  return c * this->integration()->integral( vlower_[0], vupper_[0], 
					    vlower_[1], vupper_[1],
					    integ_ );
}

double InverseMellin::operator()( const std::vector< double >& x ) {
  if( x.size() == 0 ) return 0.0;
  if( x.size() == 1 || x[ 0 ] == x[ 1 ] ) return (*this)( x[0] ); 
  activefunc_->x( x );
  return this->integrate_();
}

//	set Complex function for Mellin transformation 
RealFunction& InverseMellin::transform( ComplexFunction *g )
{
  activefunc_->set( g );
  return *this;
}

void InverseMellin::integPath( const INTEGPATH& type ){ 
  type_ = type; 
  if( type_ == STEEPEST ){
    integ_.path( &steepest_ );
  } else {
    integ_.path( &straight1_,  &straight2_ );
  }
}

void InverseMellin::upper( const std::vector< double >& l ){
  
  if( l.size() == 0 ) {
    cerr << __FILE__ << ":" << __LINE__
	 << "  upper should have more than 1 elements"
	 << endl;
    return;
  }
  
  upper_ = l[ 0 ];
  vupper_[ 0 ] = l[ 0 ];
  vupper_[ 1 ] = ( l.size() > 1 ? l[ 1 ] : l [ 0 ] );
  
}

double InverseMellin::upper(){
  return upper_;
}

void InverseMellin::upper( const double& l ){
  upper_ = l;
  vupper_[ 0 ] = l;
  vupper_[ 1 ] = l;
}

void InverseMellin::lower( const std::vector< double >& l ){
  
  if( l.size() == 0 ) {
    cerr << __FILE__ << ":" << __LINE__
	 << "  lower should have more than 1 elements"
	 << endl;
    return;
  }
  
  lower_ = l[ 0 ];
  vlower_[ 0 ] = l[ 0 ];
  vlower_[ 1 ] = ( l.size() > 1 ? l[ 1 ] : l [ 0 ] );
  
}

double InverseMellin::lower(){
  return lower_;
}

void InverseMellin::lower( const double& l ){
  lower_ = l;
  vlower_[ 0 ] = l;
  vlower_[ 1 ] = l;
}


void InverseMellin::steepestDescent( const double& c0, const double& c1, 
				      const double& c2 ){
  steepest_.set( c0, c1 );
  steepInt_.c2 = c2;
  steepInt_.c3 = 2.0 * c1;
  
}

void InverseMellin::setMomParam( const vector< double >& mom ){
  activefunc_->setMomParam( mom );
}

void InverseMellin::setIMIntegrand( InverseMellinIntegrand* iminteg ){
  activefunc_ = iminteg;
  integ_.setIMIntegrand( activefunc_ );
  steepInt_.setIMIntegrand( activefunc_ );
}

vector< double > InverseMellin::angle() {
  vector< double > val( 2 );
  val[ 0 ] = straight1_.angle();
  val[ 1 ] = straight2_.angle();
  return val;
}

vector< double > InverseMellin::offset(){
  vector< double > val( 2 );
  val[ 0 ] = straight1_.offset();
  val[ 1 ] = straight2_.offset();
  return val;
}

