#include "DGLAPMatrix.hh"

#include <cmath>
#include "Kernel.hh"

using namespace std;
using namespace Transform;
using namespace QCD;
using namespace Evolution;

DGLAPMatrix::DGLAPMatrix( Kernel *kernel ) :
  k_( kernel ), scale_( kernel ),
  lambdaP_( 0.0, 0.0 ), lambdaM_( 0.0, 0.0 ), dLambdaPM_( 0.0, 0.0 ),
  S_( 2, Matrix< complex< double > >( 2, 2, complex< double >( 0.0, 0.0 ) ) ),
  NS_( 2, vector< complex< double > >( 2, complex< double >( 0.0, 0.0 ) ) ),
  Pp_( 2, 2 ), Pm_( 2, 2 ), R_( 2, 2 ),
  PS_( 2, 2 ),
  PSp_( 2, 2 ),
  PSm_( 2, 2 ),
  PNSp_( 0.0, 0.0 ), PNSm_( 0.0, 0.0 ),
  vcs_(),
  vcnsp_(),
  vcnsm_()
{
}

DGLAPMatrix::~DGLAPMatrix()
{
}

DGLAPMatrix& DGLAPMatrix::set( double q02, double q2 ){
  
  // set cache objects pointer
  //  if( scale_.q2_[ 1 ] != q2 ){
  if( vcnsp_.find( q2 ) == vcnsp_.end() ){

    // create new entry....
    map< CKey, Matrix< complex< double > > > s;
    map< CKey, complex< double > > nsp;
    map< CKey, complex< double > > nsm;
    
    vcs_[ q2 ]   = s;
    vcnsp_[ q2 ] = nsp;
    vcnsm_[ q2 ] = nsm;
    
  }

  map< CKey, Matrix< complex< double > > >&  cs = vcs_[ q2 ];
  map< CKey, complex< double > >& cnsp = vcnsp_[ q2 ];
  map< CKey, complex< double > >& cnsm = vcnsm_[ q2 ];
  
  CKey n( k_->n() );
  if( cnsp.find( n ) != cnsp.end() ){

    // copy from cache
    PS_   = cs[ n ];
    PNSp_ = cnsp[ n ];
    PNSm_ = cnsm[ n ];
    
  } else {
    
    // set Q2
    this->scale_.set( q02, q2 );
    k_->anomalousDimension()->setQ2( q2 );
    // get numeber of flavor
    double nf = Alpha::nf( sqrt( ( k_->constNf() ? 
				   k_->initialDistribution()->initialScale() :
				   q2 ) ) ); 
    
    k_->anomalousDimension()->nf( nf );
    
    // calculate singlet and non-singlet matrices
    this->setSinglet();
    this->setNonSinglet();
    
    // insert to memory regident cache
    cs[ n ]   = PS_;
    cnsp[ n ] = PNSp_;
    cnsm[ n ] = PNSm_;

  }

  return *this;
}

bool DGLAPMatrix::Scales::set( double q02, double q2 ){

  q2_[ 0 ] = q02;  
  q2_[ 1 ] = q2;
  
  for( int i = 0; i < 2; i++ ){
    q_[ i ]  = sqrt( q2_[ i ] );  
    alpha_[ i ] = k_->alpha()( q2_[ i ] );
  }
  
  for( int i = 0; i < 3; i++ ) {
    beta_[i] = k_->alpha().beta( i, q_[ 1 ] );
    rbeta_[ i ] = beta_[ i ] / beta_[0];
  }
  
  ralpha_ = alpha_[ 1 ] / alpha_[ 0 ];
  dalpha_ = alpha_[ 0 ] - alpha_[ 1 ];
  beta2_ = 2.0 * beta_[0];
  da_4pi_ = 0.25 * dalpha_ / M_PI;
  da_4pi_2b_ = da_4pi_ / beta2_;
  
  return true;
}

bool DGLAPMatrix::setLambdas(){
  
  AnomalousDimension *p = k_->anomalousDimension();
  complex< double > n   = k_->n();
  
  double ob2 = 1.0 / scale_.beta2_;
  
  for( int i = 0; i < 2; i++ ){
    
    S_[ i ][ 0 ][ 0 ] = 
      ob2 * p->r( AnomalousDimension::Pqq, (Scheme::ORDER) i, n );
    
    S_[ i ][ 0 ][ 1 ] = 
      ob2 * p->r( AnomalousDimension::Pqg, (Scheme::ORDER) i, n );

    S_[ i ][ 1 ][ 0 ] = 
      ob2 * p->r( AnomalousDimension::Pgq, (Scheme::ORDER) i, n );

    S_[ i ][ 1 ][ 1 ] = 
      ob2 * p->r( AnomalousDimension::Pgg, (Scheme::ORDER) i, n );
    
  }
  
  lambdaP_ = lambdaM_ = S_[0][0][0] + S_[0][1][1];
  dLambdaPM_ = sqrt( pow( S_[0][1][1] - S_[0][0][0], 2 ) + 
		     4.0 * S_[ 0 ][ 0 ][ 1 ] * S_[ 0 ][ 1 ][ 0 ]);
  
  lambdaP_ += dLambdaPM_; 
  lambdaM_ -= dLambdaPM_;
  lambdaP_ *= 0.5;
  lambdaM_ *= 0.5;
  
  return true;
}

bool DGLAPMatrix::setSinglet(){
  
  this->setLambdas();
  
  R_  =  S_[1] - S_[0] * complex< double >( scale_.rbeta_[1], 0.0 );
  Pp_ =  ( S_[0] - Kernel::I * lambdaM_ ) / dLambdaPM_; 
  Pm_ =  ( S_[0] - Kernel::I * lambdaP_ ) / ( - dLambdaPM_ );
  
  complex< double > C2 = pow( scale_.ralpha_, dLambdaPM_ );
  
  CmRm_ = complex< double >( scale_.da_4pi_, 0.0 );
  CpRp_ = complex< double >( scale_.da_4pi_, 0.0 );
  
  CmRp_ = 
    ( scale_.alpha_[0] - scale_.alpha_[1] * C2 ) / 
    ( Kernel::PI4 * ( 1.0 + dLambdaPM_ ) );
  
  CpRm_ = ( dLambdaPM_ == complex< double >( 1.0, 0.0 ) ? 0.0 : 
	    ( scale_.alpha_[0] - scale_.alpha_[1] / C2 ) / 
	    ( Kernel::PI4 * ( 1.0 - dLambdaPM_ ) ) );
  
  PSm_ = Pm_ - ( Pm_ * R_ * Pm_ ) * CmRm_ - ( Pm_ * R_ * Pp_ ) * CmRp_;
  PSm_ *= pow( scale_.ralpha_, lambdaM_ );
  
  PSp_ = Pp_ - ( Pp_ * R_ * Pp_ ) * CpRp_ - ( Pp_ * R_ * Pm_ ) * CpRm_;
  PSp_ *= pow( scale_.ralpha_, lambdaP_ );
  
  PS_ = PSp_ + PSm_;

  return true;
}

bool DGLAPMatrix::setNonSinglet(){
  
  AnomalousDimension *p = k_->anomalousDimension();
  complex< double > n = k_->n();
  
  NS_[ 0 ][ 0 ] = p->r( AnomalousDimension::Pnsp, Scheme::LO,  n );
  NS_[ 0 ][ 1 ] = p->r( AnomalousDimension::Pnsm, Scheme::LO,  n );
  
  NS_[ 1 ][ 0 ] = p->r( AnomalousDimension::Pnsp, Scheme::NLO, n );
  NS_[ 1 ][ 1 ] = p->r( AnomalousDimension::Pnsm, Scheme::NLO, n );
  
  for( int i = 0; i < NS_.size(); i++ )
    for( int j = 0; j < NS_[ i ].size(); j++ )
      NS_[ i ][ j ] /= scale_.beta2_;
  
  PNSp_ = 
    ( 1.0 - scale_.da_4pi_ * ( NS_[1][0] - scale_.rbeta_[1] * NS_[0][0] ) ) *
    pow( scale_.ralpha_, NS_[0][0] );
  
  PNSm_ = 
    ( 1.0 - scale_.da_4pi_ * ( NS_[1][1] - scale_.rbeta_[1] * NS_[0][1] ) ) *
    pow( scale_.ralpha_, NS_[0][1] );
  
  return true;
}

DGLAPpdf DGLAPMatrix::operator*( DGLAPpdf& pdf ){
  
  // In order to keep information of pdf parameterization,
  // New pdf is copied from the given pdf
  // number of flavor also copied from initial distribution...
  // should it be get from anomalous dimension???
  DGLAPpdf opdf = pdf; 
  
  opdf.S() = this->singlet() * pdf.S();
  for( int i = 0; i < pdf.NS( +1 ).size(); i++ ){
    opdf.NS( +1 )[i]  = this->nonSinglet( +1 ) * pdf.NS( +1 )[ i ];
    opdf.NS( -1 )[i]  = this->nonSinglet( -1 ) * pdf.NS( -1 )[ i ];
  }
  
  return opdf;
}

int DGLAPMatrix::cache_size() {
  
  int n = 0;
  for( map< double, map< CKey, Matrix< complex< double > > > >::iterator itr = vcs_.begin();
       itr != vcs_.end();
       itr++ ){
    n += itr->second.size();
  }
  
  return ( vcs_.size() > 0 ? n / vcs_.size(): 0 );
  
  //  return cacheS_.size();
}

ostream& Evolution::operator<<( ostream& os, DGLAPMatrix& m ){

  os 
    << "********************************************************\n"
    << "*  DGLAP Equation (Matrix Elements)                    *\n"
    << "********************************************************\n"
    << flush;

  os << m.scale() << endl;

  os << "------ Single components: " << endl;
  
  os << "Anomalous dimension ( LO, NLO ) : normalized by beta2" << endl;
  os << m.r()[0] << endl;
  os << m.r()[1] << endl;
  
  os 
    << "Lambda : " 
    << "+ " << setw(16) << m.lambda( +1 ) << ", " 
    << "- " << setw(16) << m.lambda( -1 ) << ", "
    << "diff " << setw(16) << m.lambda( 0 ) 
    << endl; 

  os << "R = r(1) - " 
     << setw(10) << m.scale().rbeta_[1] << "* r(0)" 
     << endl;
  os << m.R() << endl;
  
  os << "P+ = (r(0) - lM )/(lP - lM):" << endl;
  os << m.P( +1 ) << endl; 
  os << "P- = (r(0) - lP )/(lM - lP):" << endl;
  os << m.P( -1 ) << endl;

  /*
  os << "   ***** check (P+)^2 - P+ = 0" << endl;
  os << m.P( +1 ) * m.P( +1 ) - m.P( +1 ) << endl;
  os << "   ***** check (P-)^2 - P- = 0" << endl;
  os << m.P( -1 ) * m.P( -1 ) - m.P( -1 ) << endl;
  
  os << "   ***** check (P+)*(P-) = 0" << endl;
  os << m.P( +1 ) * m.P( -1 ) << endl;
  
  os << "   ***** check (P+) + (P-) - I = 0" << endl;
  os << m.P( +1 ) + m.P( -1 ) - Kernel::I << endl;
  
  os << "   ***** check r(0) - (l+)*(P+) + (l-)*(P-) = 0" << endl;
  os << m.P( +1 ) * m.lambda( +1 ) + m.P( -1 ) * m.lambda( -1 ) - m.r()[0]
     << endl;
  */

  os << "(P+)*R*(P+)" << endl;
  os << "C: " << m.CR( +1, +1 ) << endl;
  os << m.P( +1 ) * m.R() * m.P( +1 ) << endl;
  
  os << "(P+)*R*(P-)" << endl;
  os << "C: " << m.CR( +1, -1 ) << endl;
  os << m.P( +1 ) * m.R() * m.P( -1 ) << endl;
  
  os << "(P-)*R*(P-)" << endl;
  os << "C: " << m.CR( -1, -1 ) << endl;
  os << m.P( -1 ) * m.R() * m.P( -1 ) << endl;
  
  os << "(P-)*R*(P+)" << endl;
  os << "C: " << m.CR( -1, +1 ) << endl;
  os << m.P( -1 ) * m.R() * m.P( +1 ) << endl;
  
  os << "ralpha^{lamnda-} : " 
     << pow( m.scale().ralpha_, m.lambda( -1 ) ) << endl;
  os << "ralpha^{lamnda+} : " 
     << pow( m.scale().ralpha_, m.lambda( +1 ) ) << endl;
  os << "ralpha^{dlamnda} : " 
     << pow( m.scale().ralpha_, m.lambda( 0 ) ) << endl;
  
  os << "( a(Q02)-a(q2)*(r_alpha^{+dlambda}) ) / 4PI: "
       << ( m.scale().alpha_[0] - 
	    m.scale().alpha_[1] * pow( m.scale().ralpha_, m.lambda(0) ) ) /
    4.0 / M_PI 
     << endl;
  
  os << "( a(Q02)-a(q2)*(r_alpha^{-dlambda}) ) / 4PI: "
     << ( m.scale().alpha_[0] - 
	  m.scale().alpha_[1] * pow( m.scale().ralpha_, - m.lambda(0) ) ) /
    4.0 / M_PI 
    //     << "\t"
    //     << ( m.scale().alpha_[0] - 
    //	  m.scale().alpha_[1] / pow( m.scale().ralpha_, m.lambda(0) ) ) /
    //    4.0 / M_PI 
     << endl;
  
  os << "Singlet Matrix:" << endl;
  os << m.PS() << endl;
  os << "   - component" << endl;
  os << m.PS( -1 ) << endl;
  os << "   + component" << endl;
  os << m.PS( +1 ) << endl;
  
  os
    << "*********************************************\n"
    << "* Non-singlet calculation:                  *\n"
    << "*********************************************\n"
    << endl;
  
  os << "Anomalous dimension : normalized by " << endl;
  os << "  " << setw(16) << "LO" << setw(16) << "NLO" << endl;
  os << "+:" 
     << setw(16) << m.rns()[0][0]
     << setw(16) << m.rns()[1][0]
     << endl;
  os << "-:" 
     << setw(16) << m.rns()[0][1]
     << setw(16) << m.rns()[1][1]
     << endl;
  
  os << "Singlet Matrix (factor):" << endl;
  os << "+: " << setw(16) << m.PNS( +1 ) << endl;
  os << "-: " << setw(16) << m.PNS( -1 ) << endl;
  
  return os;
}

void DGLAPMatrix::cache_dump( ostream& os ) {

  os << "Cached data size: " << this->cache_size() << endl;
  
  os << "*** siglet sector **" << endl;
  map< double, map< CKey, Matrix< complex< double > > > >::iterator itr;
  for( itr = vcs_.begin(); itr != vcs_.end(); itr++ ){
    
    cout << "Q^2 = " << itr->first << endl;
    int c = 1;
    for( map< CKey, Matrix< complex< double > > >::iterator jtr = 
	   itr->second.begin(); jtr != itr->second.end(); jtr++ ){
      cout << setw(5) << c++ 
	   << setw(18) << jtr->first.real()
	   << setw(18) << jtr->first.imag()
	   << endl;
    }
  }
}

ostream& Evolution::operator<<( ostream& os, DGLAPMatrix::Scales& s ){
  os << "------ Hard scales:"
     << endl;
  
  os << setw(12) << "Q" 
     << setw(12) << "Q^2" 
     << setw(12) << "alpha(Q^2)" << endl;
  for( int i = 0; i < 2; i++ ){
    os 
      << setw(2)  << i
      << setw(10) << s.q_[i] 
      << setw(12) << s.q2_[i]
      << setw(12) << s.alpha_[i] 
      << endl;
  }
  
  os << "alpha ratio:  " << setw(10) << s.ralpha_ << endl;
  os << "alpha diff.:  " << setw(10) << s.dalpha_ << endl;

  
  os << "beta:  " << flush;
  for( int i = 0; i < 3; i++ ) os << setw(8) << s.beta_[i] << flush;
  os << endl;
  
  os << "rbeta: " << flush;
  for( int i = 0; i < 3; i++ ) os << setw(8) << s.rbeta_[i] << flush;
  os << endl;
  
  os << "2.0 * beta_0: " << setw(8) << s.beta2_ << endl;
  os << "( alpha(Q_0^2) - alpha(Q^2) ) / ( 4pi ): " 
     << setw(10) << s.da_4pi_ << endl;
  os << "( alpha(Q_0^2) - alpha(Q^2) ) / ( 4pi ) / ( 2.0 * beta_0 ): " 
     << setw(10) << s.da_4pi_2b_ << endl;
  
  return os;
}

