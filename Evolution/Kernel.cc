// $Id: Kernel.cc,v 1.54 2013/04/05 09:02:12 miyachi Exp $
/*!
  \file    Kernel.cc
  \version "$Revision: 1.54 $"
  \date    "$Date: 2013/04/05 09:02:12 $"
  \brief   Implementation of QCD Evolution Kernel in Mellin space
 */
#include "Kernel.hh"

#include <cstdlib>
#include <iomanip>
#include <algorithm>

using namespace std;
using namespace Utility;
using namespace Transform;
using namespace QCD;
using namespace Evolution;

bool   Kernel::initialized = false;

double Kernel::CF   = 4.0 / 3.0;
double Kernel::CF2  = 2.0  * Kernel::CF;
double Kernel::CF4  = 4.0  * Kernel::CF;
double Kernel::CF8  = 8.0  * Kernel::CF;
double Kernel::CF16 = 16.0 * Kernel::CF;
double Kernel::CF32 = 32.0 * Kernel::CF;

double Kernel::CF_2  = Kernel::CF * Kernel::CF;
double Kernel::CF2_2 = Kernel::CF2 * Kernel::CF2;

double Kernel::CA    = 3.0;
double Kernel::CA2   = 2.0 * Kernel::CA;
double Kernel::CA2_2 = Kernel::CA2 * Kernel::CA2;
double Kernel::CA16  = 16.0 * Kernel::CA;
double Kernel::CA32  = 32.0 * Kernel::CA;

double Kernel::CACF  = Kernel::CA * Kernel::CF;
double Kernel::CACF8 = Kernel::CA * Kernel::CF8;

double Kernel::PI4   = 4.0 * M_PI;

Matrix< complex< double > > Kernel::I( 2, 2 );

bool Kernel::initialization() {
  if( ! initialized ) {
    I[ 0 ][ 0 ] = complex< double >( 1.0, 0.0 );
    I[ 0 ][ 1 ] = complex< double >( 0.0, 0.0 );
    I[ 1 ][ 0 ] = complex< double >( 0.0, 0.0 );
    I[ 1 ][ 1 ] = complex< double >( 1.0, 0.0 );
  }
  return initialized = true;
}

Kernel::Kernel( Arguments& args ) : 
  init_( NULL ), 
  P_( NULL ),
  q2_( 1.0 ),
  forwardEvolution_( true ),
  iThresh_( 0 ),
  thresh_( 6, 0.0 ),
  m_( this ),
  qin_(),
  qout_(),
  externalDB_( true ),
  constNf_( false ),
  args_( args )
{
  
  Kernel::initialization();
  
  this->setQ2( ( args.hasOpt( "Q2" ) ? 
		 atof( args.getOpt( "Q2" ).c_str() ) : 1.0  ) );
  
  this->setN( complex< double >( ( args.hasOpt( "N" ) ? 
				   atof( args.getOpt( "N" ).c_str() ) :
				   1.0  ),
				 0.0 ) );
  
  // DataBase control
  if( args.hasOpt( "useDB" ) ){
    if( args.getOpt( "useDB" ) == "false" ) externalDB_ = false;
  }
  
  // Number of flavor control
  if( args.hasOpt( "nf" ) ){
    if( args.getOpt( "nf" ) == "fix" ) constNf_ = true;
  }
  
  if( args.hasOpt( "help" ) || 
      args.hasOpt( "Help" ) || 
      args.hasOpt( "HELP" ) || 
      args.hasOpt( "?" )  ){
    
    args.usage( "Q2",        "value",
		"pQCD scale to be calculated (optional)" );
    
    args.usage( "N",         "value",
		"moment to be calculated     (optional)" );
    
    args.usage( "useDB",      "true",
		"Use external DB to store splitting function (optional)" );
    
    args.usage( "nf", "fix",
		"Handling mode of Number of flavor  (fix|dynamic,optional)" );
  }
  
  // set threshold
  for( int i = 0; i < thresh_.size(); i++ ){
    Flavor::setMassValue();
    thresh_[ i ] = Flavor::mass[ i ] * Flavor::mass[ i ];
  }
  
  this->cdata().resize( 2 + qout_.NS(+1).size() + qout_.NS(-1).size() );
  
}

Kernel::~Kernel() {
}

int Kernel::evolveToThreshold( DGLAPpdf& qin ){
  
  complex< double > n = this->n();
  double Q02 = init_->initialScale();

  int ith( 0 );
  
  if( forwardEvolution_ ){
    
    for( ith = iThresh_; 
	 ( ith < thresh_.size() ) && ( q2_ > thresh_[ ith ] ) ;
	 ith++ ) {
      
      if( this->retriveFromCache( n, thresh_[ ith ], qin ) ) {
	
	// set proper number of flavor
	if( ! this->constNf() ) 
	  qin.nf( Alpha::flavor()( sqrt( thresh_[ ith ] ) ) + 1.0  );
	
      } else {
	
	// calculate evolution kernel
	m_.set( ( ith == iThresh_ ? Q02 : thresh_[ ith - 1 ] ), 
		thresh_[ ith ] );
	
	// calculate DEGLAP equation with nf
	qin = m_ * qin;
	
	// renormalize to new nf
	if( ! this->constNf() ) {
	  // before accessing initial distribution, moment of the
	  // initial distribution must be re-set properly
	  init_->setN( n );
	  qin.renormalize( qin.nf() + 1.0 );
	}
	
	// insert to cache
	this->insertToCache( n, thresh_[ ith ], qin );
	
      }
      
    }
    
  } else {
    
    for( ith = iThresh_ - 1; 
	 ( ith > 0 ) && ( q2_ < thresh_[ ith ] ) ;
	 ith-- ) {
      
      if( this->retriveFromCache( n, thresh_[ ith ], qin ) ) {
	
	if( ! this->constNf() ) 
	  qin.nf( Alpha::flavor()( sqrt( thresh_[ ith ] ) ) + 1.0  );
	
      } else {
	
	if( ith == iThresh_ - 1 ){
	  
	  m_.set( Q02, thresh_[ ith ] );
	  
	} else {
	  
	  // renormalize to new nf
	  if( ! this->constNf() ) {
	    // before accessing initial distribution, moment of the
	    // initial distribution must be re-set properly
	    init_->setN( n );
	    qin.renormalize( qin.nf() - 1.0 );
	  }
	  
	  // backword evolution
	  m_.set( thresh_[ ith + 1 ], thresh_[ ith ] );
	  
	}
	
	// calculate DEGLAP equation with nf
	qin = m_ * qin; 
	
	// insert to cache
	this->insertToCache( n, thresh_[ ith ], qin );
	
      }
      
    }
    
  }
  
  return ith;
}

void Kernel::update(){
  
  if( init_ == NULL || P_ == NULL ) return ;
  
  complex< double > n = this->n();
  
  // Do we have already cached?
  if( this->retriveFromCache( n, q2_, qout_ ) ){
    
    bool onThresh = 
      ( find( thresh_.begin(), thresh_.end(), q2_ ) != thresh_.end() );
    
    // set proper Nf
    double nf = ( this->constNf() ? 
		  qin_.nf() :
		  Alpha::flavor()( sqrt( q2_ ) )
		  + ( onThresh ? 1.0 : 0.0 ) );
    
    qout_.nf( nf );
    
    return;
  }
  
  double Q02 = init_->initialScale();
  
  // for initial distribution
  if( ! this->retriveFromCache( n, Q02, qin_ ) ){
    this->loadInitialDistributions();
    this->insertToCache( n, Q02, qin_ );
  }
  
  // if the given scale is same as the initial scale, just return...
  if( q2_ == Q02 ) {
    qout_ = qin_;
    return;
  }
  
  // copy initial distribution
  DGLAPpdf qin = qin_;
  
  // evolve to the threshold levels
  int ith = this->evolveToThreshold( qin );
  
  // calculate DGLAP equation at the last step
  if( forwardEvolution_ ) {
    m_.set( ( ith == iThresh_ ? Q02 : thresh_[ ith - 1 ] ), q2_ );
  } else {
    // calculate DGLAP equation at the last step
    if( ith == iThresh_ - 1 ){
      m_.set( Q02, q2_ );
    } else {
      if( ! this->constNf() ) {
	init_->setN( n );
	qin.renormalize( qin.nf() - 1.0 );
      }
      m_.set( thresh_[ ith ], q2_ );
    }
  }
  
  // insert Memory cache
  this->insertToCache( n, q2_, qout_ = m_ * qin );
  
}

void Kernel::anomalousDimension( AnomalousDimension* ad ){
  P_ = ad; 
  P_->kernel( this );
  P_->useDB( externalDB_ );
  this->cache().clear();
  //  this->update();
}

void Kernel::initialDistribution( PdfParametrization *init ){

  init_ = init;
  //thresh_[ 0 ] = init_->initialScale();
  
  int nf =
    static_cast< int >( Alpha::flavor()( sqrt( init_->initialScale() ) ) );

  qin_.nf( nf );        // set initial number of flavor 
  iThresh_ = nf;        // set the first threshold in evolution
  
  this->cache().clear();
  //  this->update();
}

void Kernel::setN( const complex< double >& n ) {
  
  if( this->cache().size() != 0 && this->n() == n ) return;
  
  this->updateN( n );
  this->update();

}

void Kernel::setQ2( const double& q2 ){
  
  if( q2_ == q2 ) return; // do nothing.....
  q2_ = q2;
  
  if( ! init_ ) {
    ARG_WARN << "Initial distribution has not been defined." << endl;
    return;
  }
  
  if( ( forwardEvolution_ = ( q2_ >= init_->initialScale() ) ) == false ){
    //   ARG_WARN << "Backward evolution has not been implemented." << endl;
    //    ARG_WARN << "Set target Q2 to be the initial scale: " 
    //	     << setw(6) << init_->initialScale() << " GeV^2" << endl;
    //    q2_ = init_->initialScale();
    //    forwardEvolution_ = true;

    ARG_WARN 
      << "The given Q2 = "           << setw(6) << q2_ 
      << " is smaller than Q_0^2 = " << setw(6) << init_->initialScale()
      << endl;
    ARG_WARN
      << "Evolution will be calculated to backward!!" << endl;

  }
  
  this->update();
}

complex< double > Kernel::q( const Flavor::PARTON& q ){

  complex< double > a0av = this->q( Flavor::a0 ) / qout_.nf();
  
  vector< complex< double > >& nsp = qout_.NS(+1);
  vector< complex< double > >& nsm = qout_.NS(-1);
  
  switch( q ) {
    
  case Flavor::g : return qout_.S()[1][0];
  case Flavor::u : return 0.5*( nsp[0] + a0av + nsm[0] );
  case Flavor::d : return 0.5*( nsp[1] + a0av + nsm[1] );
  case Flavor::s : return 0.5*( nsp[2] + a0av + nsm[2] );
  case Flavor::c : return ( qout_.nf() > 3 ? 0.5*( nsp[3] + a0av + nsm[3] ) : 0.0 );
  case Flavor::b : return ( qout_.nf() > 4 ? 0.5*( nsp[4] + a0av + nsm[4] ) : 0.0 );
  case Flavor::t : return ( qout_.nf() > 5 ? 0.5*( nsp[5] + a0av + nsm[5] ) : 0.0 );
    
  case Flavor::ubar : return 0.5*( nsp[0] + a0av - nsm[0] );
  case Flavor::dbar : return 0.5*( nsp[1] + a0av - nsm[1] );
  case Flavor::sbar : return 0.5*( nsp[2] + a0av - nsm[2] );
  case Flavor::cbar : return ( qout_.nf() > 3 ? 0.5*( nsp[3] + a0av - nsm[3] ) : 0.0 );
  case Flavor::bbar : return ( qout_.nf() > 4 ? 0.5*( nsp[4] + a0av - nsm[4] ) : 0.0 );
  case Flavor::tbar : return ( qout_.nf() > 5 ? 0.5*( nsp[5] + a0av - nsm[5] ) : 0.0 );
    
  case Flavor::uv : return nsm[0];
  case Flavor::dv : return nsm[1];
  case Flavor::sv : return nsm[2];
  case Flavor::cv : return nsm[3];
  case Flavor::bv : return nsm[4];
  case Flavor::tv : return nsm[5];
    
  case Flavor::uns : return nsp[0];
  case Flavor::dns : return nsp[1];
  case Flavor::sns : return nsp[2];
  case Flavor::cns : return nsp[3];
  case Flavor::bns : return nsp[4];
  case Flavor::tns : return nsp[5];
    
  case Flavor::U : return this->q( Flavor::u ) + this->q( Flavor::ubar );
  case Flavor::D : return this->q( Flavor::d ) + this->q( Flavor::dbar );
  case Flavor::S : return this->q( Flavor::s ) + this->q( Flavor::sbar );
  case Flavor::C : return this->q( Flavor::c ) + this->q( Flavor::cbar );
  case Flavor::B : return this->q( Flavor::b ) + this->q( Flavor::bbar );
  case Flavor::T : return this->q( Flavor::t ) + this->q( Flavor::tbar );
       
  case Flavor::ubarubar : return this->q( Flavor::ubar ) + this->q( Flavor::ubar );
    
  }
  
  return complex< double >( 0.0 );
}

complex< double > Kernel::q( const Flavor::TYPE&  q ){

  switch( q ){
  case Flavor::a0 : return qout_.S()[0][0];
    
  case Flavor::a3 : 
    return this->q( Flavor::uns ) - this->q( Flavor::dns );
    
  case Flavor::a8 :
    return this->q( Flavor::uns ) + this->q( Flavor::dns ) 
      -2.0 * this->q( Flavor::sns );
    
  case Flavor::a3m : 
    return this->q( Flavor::uv ) - this->q( Flavor::dv );
    
  case Flavor::a8m :
    return this->q( Flavor::uv ) + this->q( Flavor::dv ) 
      -2.0 * this->q( Flavor::sv );
    
  case Flavor::NSp :
    return
    this->q( Flavor::a3 )  + this->q( Flavor::a8 ) 
      + this->q( Flavor::a15 ) + this->q( Flavor::a24 ) 
      + this->q( Flavor::a35 ) ;
    
  case Flavor::NSm :
    return
      this->q( Flavor::a3m )  + this->q( Flavor::a8m ) 
      + this->q( Flavor::a15m ) + this->q( Flavor::a24m ) 
      + this->q( Flavor::a35m ) ;
    
  case Flavor::NSv :
    return
      this->q( Flavor::uv )  + this->q( Flavor::dv ) 
      + this->q( Flavor::sv ) + this->q( Flavor::cv ) 
      + this->q( Flavor::bv ) + this->q( Flavor::tv );
    
  case Flavor::SEA:
    return
      2.0 * ( this->q( Flavor::ubar ) + this->q( Flavor::dbar ) ) +
      this->q( Flavor::S );
    
  case Flavor::DELTA:
    return this->q( Flavor::dbar ) - this->q( Flavor::ubar );
    
  case Flavor::DUSUM:
    return this->q( Flavor::dbar ) + this->q( Flavor::ubar );
    
  case Flavor::DURATIO:
    return this->q( Flavor::dbar ) / this->q( Flavor::ubar );
    
  }
  return complex< double >( 0.0 );
}

double& Kernel::initialScale(){
  return init_->initialScale();
}

void Kernel::insertToCache( const complex< double >& n, 
			    const double& q2, 
			    DGLAPpdf& pdf ){
  this->insertToCache( n, q2, pdf.S(), pdf.NS(+1), pdf.NS(-1) );
}

void Kernel::insertToCache( const complex< double >& n, 
			    const double& q2,
			    Matrix< complex< double > >& qS,
			    vector< complex< double > >& qNSp,
			    vector< complex< double > >& qNSm  ){
  
  if( this->cache().cached( n, q2 ) ) {
    cerr 
      << __FILE__ << ":" << __LINE__ 
      << "(" << n << ", " << q2 << ") has been cached already."
      << endl;
    return;
  }
  
  this->cdata()[ 0 ] = qS[0][0];
  this->cdata()[ 1 ] = qS[1][0];
  
  for( int i = 0; i < qNSp.size(); i++ ){ 
    this->cdata()[ 2 + i ] = qNSp[i];
  }
  
  for( int i = 0; i < qNSm.size(); i++ ){ 
    this->cdata()[ 2 + qNSp.size() + i ] = qNSm[i];
  }
  
  this->cache().cache( n, q2, this->cdata() );
}

bool Kernel::retriveFromCache( const complex< double >& n, 
			       const double& q2,
			       DGLAPpdf& pdf ){
  return this->retriveFromCache( n, q2, pdf.S(), pdf.NS(+1), pdf.NS(-1) );
}

bool Kernel::retriveFromCache( const complex< double >& n, 
			       const double& q2,
			       Matrix< complex< double > >& qS,
			       vector< complex< double > >& qNSp,
			       vector< complex< double > >& qNSm  ){
  
  if( ! this->cache().cached( n, q2 ) ) return false;
  
  qS[0][0] = this->cache().cache( n, q2 )[ 0 ];
  qS[1][0] = this->cache().cache( n, q2 )[ 1 ];;
  
  for( int i = 0; i < qNSp.size(); i++ )
    qNSp[i] =  this->cache().cache( n, q2 )[ 2 + i ];
  
  for( int i = 0; i < qNSp.size(); i++ )
    qNSm[i] =  this->cache().cache( n, q2 )[ 2 + qNSp.size() + i ];
  
  return true;
}

bool Kernel::constNf( const bool& chNfstate ) { 
  if( constNf_ == chNfstate ) return constNf_;
  constNf_= chNfstate; 
  
  // if constNf_ state is changed, one has to change also 
  // cache file for Anomalous dimension
  if( this->anomalousDimension() ){
    this->anomalousDimension()->closeDB();
    this->anomalousDimension()->openDB();
  }

  return constNf_;
}

namespace Evolution {

  ostream& operator<<( ostream& os, Kernel& k ){
    
    PdfParametrization *pdf = k.initialDistribution();
    AnomalousDimension *ad  = k.anomalousDimension();
    DGLAPMatrix&        m   = k.dglapMatrix();
    
    os
      << "N       = " << setw(12) << k.n() << endl 
      << "Q^2     = " << setw(12) << k.Q2() << "\n"
      << "alpha_s = " << setw(12) << k.alpha()( k.Q2() ) << '\n'
      << endl;
  
    if( ad ){
      os << *ad << '\n' << endl;
      os << m << endl;
    }
    
    if( pdf && ad ){
      
      os 
	<< "*******************************************************************\n"
	<< " Output PDF\n"
	<< k.qout_ 
	<< endl;
      
      os 
	<< "*******************************************************************\n"
	<< "Flavor:" << setw(16) << "Q_0^2" << setw(16) << "(Q_^2)" << endl
	<< "u:     " 
	<< setw(16) << pdf->q( Flavor::u )    << setw(16) << k.q( Flavor::u ) 
	<< endl
	<< "u-bar: " 
	<< setw(16) << pdf->q( Flavor::ubar ) << setw(16) << k.q( Flavor::ubar )
	<< endl
	<< "d:     " 
	<< setw(16) << pdf->q( Flavor::d )    << setw(16) << k.q( Flavor::d ) 
	<< endl
	<< "d-bar: " 
	<< setw(16) << pdf->q( Flavor::dbar ) << setw(16) << k.q( Flavor::dbar )
	<< endl
	<< "s:     " 
	<< setw(16) << pdf->q( Flavor::s )    << setw(16) << k.q( Flavor::s ) 
	<< endl
	<< "s-bar: " 
	<< setw(16) << pdf->q( Flavor::sbar ) << setw(16) << k.q( Flavor::sbar )
	<< endl
	<< "c:     " 
	<< setw(16) << pdf->q( Flavor::c )    << setw(16) << k.q( Flavor::c ) 
	<< endl
	<< "c-bar: " 
	<< setw(16) << pdf->q( Flavor::cbar ) << setw(16) << k.q( Flavor::cbar )
	<< endl
	<< "b:     " 
	<< setw(16) << pdf->q( Flavor::b )    << setw(16) << k.q( Flavor::b ) 
	<< endl
	<< "b-bar: " 
	<< setw(16) << pdf->q( Flavor::bbar ) << setw(16) << k.q( Flavor::bbar )
	<< endl
	<< "t:     " 
	<< setw(16) << pdf->q( Flavor::t )    << setw(16) << k.q( Flavor::t ) 
	<< endl
	<< "t-bar: " 
	<< setw(16) << pdf->q( Flavor::tbar ) << setw(16) << k.q( Flavor::tbar )
	<< endl
	<< "g:     " 
	<< setw(16) << pdf->q( Flavor::g )    << setw(16) << k.q( Flavor::g ) 
	<< endl
	<< "a0:    " 
	<< setw(16) << pdf->q( Flavor::a0 )   << setw(16) << k.q( Flavor::a0 )
	<< endl
	<< "a3:    " 
	<< setw(16) << pdf->q( Flavor::a3 )   << setw(16) << k.q( Flavor::a3 ) 
	<< endl
	<< "a8:    " 
	<< setw(16) << pdf->q( Flavor::a8 )   << setw(16) << k.q( Flavor::a8 ) 
	<< endl
	<< "a3-:   " 
	<< setw(16) << pdf->q( Flavor::a3m )  << setw(16) << k.q( Flavor::a3m )
	<< endl
	<< "a8-:   " 
	<< setw(16) << pdf->q( Flavor::a8m )  << setw(16) << k.q( Flavor::a8m )
	<< endl;
    }
    
    return os;
  }
}

void Kernel::loadInitialDistributions(){
  init_->setN( this->n() );
  qin_.load( init_ );
}
