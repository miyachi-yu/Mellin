#include "Manager.hh"

#include <iostream>
#include <iomanip>
#include <Utility/Arguments.hh>
#include <ExpData/ExpData.hh>
#include <Fragmentation/Evolution.hh>
#include <unpolPDF/Evolution.hh>
#include <polPDF/Evolution.hh>
#include <Xsec/xXsec.hh>
#include <Xsec/xzXsec.hh>
#include <FragXsec_T/FragXsec.hh>
#include <unpolDISXsec/StrFunc.hh>
#include <unpolSIDISXsec/StrFunc.hh>


using namespace std;

Manager* Manager::instance_ = NULL;

Xsection::Charge::TARGET taget[ 3 ] = {
  Xsection::Charge::p,
  Xsection::Charge::n,
  Xsection::Charge::d
};

Manager* Manager::instance(){
  if( Manager::instance_ == NULL ) new Manager();
  return Manager::instance_;
}

Manager::Manager() :
  TObject(), 
  expdata_( NULL ), 
  pionF_( NULL ), kaonF_( NULL ), protonF_( NULL ), restF_( NULL ),
  updf_( NULL ), ppdf_( NULL ),
  fragX_(), fragXX_(),
  sigma_( 3, NULL ), xsigma_( 3, NULL ),
  sigmaHp_( 3,
	   std::map< Evolution::KernelBase*, unpolSIDISXsec::StrFunc* >()  ),
  xsigmaHp_( 3,
	     std::map< Evolution::KernelBase*, Xsection::xzXsec* >()  ),
  sigmaHm_( 3,
	   std::map< Evolution::KernelBase*, unpolSIDISXsec::StrFunc* >()  ),
  xsigmaHm_( 3,
	    std::map< Evolution::KernelBase*, Xsection::xzXsec* >()  )
{
  Manager::instance_ = this;
  
  this->initExpData();
  this->initPDF();
  this->initFF();
  this->initFragXsec();
  this->initDISXsec();
  this->initSIDISXsec();
}


Manager::~Manager()
{
  if( expdata_ )   delete expdata_;

  if( pionF_ )   delete pionF_;
  if( kaonF_ )   delete kaonF_;
  if( protonF_ ) delete protonF_;
  if( restF_ )   delete restF_;
  
  if( updf_ ) delete updf_;
  if( ppdf_ ) delete ppdf_;
}

void Manager::initExpData(){
  Utility::Arguments& args = Utility::Arguments::ref();
  if( args.hasOpt( "expdata" ) ){
    expdata_ = new Exp::ExpData( args.get( "expdata", "" ) );
  }
}

void Manager::listDataSet(){
  if( expdata_ == NULL ) return;
  for( int i = 0; i < expdata_->data().size(); i++ ){
    cout << setw(3) << i
	 << setw(64) << expdata_->data()[ i ].info()
	 << endl;
  }
}

RExpData* Manager::getDataSet( const int& index,
			       const std::string& xkey ){
  if( expdata_ == NULL ){
    cerr << "no expdata is loaded. Specify with --expdata option" << endl;
    return NULL;
  }
  return new RExpData( expdata_->data()[ index ], xkey );
}

RExpData* Manager::getDataSet( const std::string& info,
			       const std::string& xkey ){
  
  for( int i = 0; expdata_->data().size(); i++ ){
    if( expdata_->data()[ i ].info() == info )
      return new RExpData( expdata_->data()[ i ], xkey );
  }
  
  cerr << "could not find " << info << endl;
  return NULL;
}

RExpData* Manager::getDataSet( const std::string& info,
			       const std::string& min,
			       const std::string& max ){
  
  for( int i = 0; expdata_->data().size(); i++ ){
    if( expdata_->data()[ i ].info() == info )
      return new RExpData( expdata_->data()[ i ], min, max );
  }
  
  cerr << "could not find " << info << endl;
  return NULL;
}

void Manager::initFF(){

  this->initFF( QCD::Flavor::PIp );
  this->initFF( QCD::Flavor::KAp );
  this->initFF( QCD::Flavor::PRp );
  this->initFF( QCD::Flavor::REp );

}

void Manager::initFF( const QCD::Flavor::HADRON& h ){
  Utility::Arguments& args = Utility::Arguments::ref();
  vector< string > opts = Fragmentation::Evo::options( h );
  if( args.hasOpt( opts ) == opts.end() ) return;
  try{
    
    switch( h ){

    case QCD::Flavor::PIp :
    case QCD::Flavor::PIm :
      cout << "Generate pion fragmentation function" << endl;
      pionF_ = Fragmentation::Evo::instance( h );
      return;
      
    case QCD::Flavor::KAp :
    case QCD::Flavor::KAm :
      cout << "Generate kaon fragmentation function" << endl;
      kaonF_ = Fragmentation::Evo::instance( h );
      return;
      
    case QCD::Flavor::PRp :
    case QCD::Flavor::PRm :
      cout << "Generate proton fragmentation function" << endl;
      protonF_ = Fragmentation::Evo::instance( h );
      return;
      
    case QCD::Flavor::REp :
    case QCD::Flavor::REm :
      cout << "Generate rest fragmentation function" << endl;
      restF_ = Fragmentation::Evo::instance( h );
      return;
    }

  } catch( int e ){
    cerr << "Fail to initialize FF" << endl;
  }
  
  return ;
}

void Manager::initPDF(){
  Utility::Arguments& args = Utility::Arguments::ref();
  vector< string > opts;
  
  try {
    opts = unpolPDF::Evo::options();
    if( args.hasOpt( opts ) != opts.end() ) updf_ = unpolPDF::Evo::instance();

    opts = polPDF::Evo::options();
    if( args.hasOpt( opts ) != opts.end() ) ppdf_ = polPDF::Evo::instance();

  } catch ( int e ){
    cerr << "Failed to initiate PDF" << endl;
  }
  return;
}
	    
void Manager::initFragXsec(){

  Utility::Arguments& args = Utility::Arguments::ref();
  
  // -----------   Mellin inversion of FragXsec   ------------- //
  double offset = args.get( "offset", 3.0  );
  double length = args.get( "length", 8.0  );
  double angle  = args.get( "angle",  0.55 );
  
  int leg1    = args.get( "leg1", 6 );
  int leg2    = args.get( "leg2", 8 );
  double prec = args.get( "prec", 5.0E-3 );
  int ngrid   = args.get( "ngrid", 2 );
  
  if( pionF_ ) {
    fragX_[ pionF_ ] = 
      new FragXsec_T::FragXsec( pionF_, QCD::Flavor::PIp );
  }
  
  if( kaonF_ ) fragX_[ kaonF_ ] = 
		 new FragXsec_T::FragXsec( kaonF_, QCD::Flavor::KAp );

  if( protonF_ ) fragX_[ protonF_ ] = 
		 new FragXsec_T::FragXsec( protonF_, QCD::Flavor::PRp );

  if( restF_ ) fragX_[ restF_ ] = 
		 new FragXsec_T::FragXsec( restF_, QCD::Flavor::REp );
  
  map< Evolution::KernelBase*, FragXsec_T::FragXsec* >::iterator itr;
  for( itr = fragX_.begin(); itr != fragX_.end(); itr++ ){
    Xsection::xXsec *xxsec = 
      ( fragXX_[ itr->first ] = 
	new Xsection::xXsec( itr->second, leg1, leg2, prec ) );
    xxsec->setGngrid( ngrid );      
    xxsec->setParameter( offset, angle );
    xxsec->upper( length );   
  }
}

void Manager::initDISXsec(){

  if( updf_ == NULL ) return;
  
  // -------------------------------------------------------------
  //                      for denominator
  // -------------------------------------------------------------
  Utility::Arguments& args = Utility::Arguments::ref();

  double UPDFoffsetN = args.get( "UPDFoffsetN", 4.5  );  
  double UPDFangleN  = args.get( "UPDFangleN",  0.60 );  
  double UPDFlengthN = args.get( "UPDFlengthN", 8.0 );

  int Mleg1 = args.get( "Mleg1", 6 );
  int Mleg2 = args.get( "Mleg2", 8 );
  double Mprec = args.get( "Mprec", 5.0E-3 );
  int Mngrid   = args.get( "Mngrid", 2 );
  
  for( int i = 0; i < sigma_.size(); i++ ){
    sigma_[ i ] =  new unpolDISXsec::StrFunc( updf_, taget[ i ] );
    
    Xsection::xXsec* xx = 
      ( xsigma_[ i ] =
	new Xsection::xXsec( sigma_[ i ], Mleg1, Mleg2, Mprec ) );
    
    xx->setGngrid( Mngrid );
    xx->setParameter( UPDFoffsetN, UPDFangleN );
    xx->upper( UPDFlengthN );
  }
  
}

void Manager::initSIDISXsec(){
  if( updf_ == NULL ) return;
  for( int i = 0; i < sigma_.size(); i++ ){
    
    if( pionF_ ){
      
      sigmaHp_[ i ][ pionF_ ] = 
	new unpolSIDISXsec::StrFunc( updf_, pionF_, taget[ i ],
				     QCD::Flavor::PIp );

      sigmaHm_[ i ][ pionF_ ] = 
	new unpolSIDISXsec::StrFunc( updf_, pionF_, taget[ i ],
				     QCD::Flavor::PIm );
    }
    
    if( kaonF_ ){
      sigmaHp_[ i ][ kaonF_ ] = 
	new unpolSIDISXsec::StrFunc( updf_, kaonF_, taget[ i ],
				     QCD::Flavor::KAp );
      sigmaHm_[ i ][ kaonF_ ] = 
	new unpolSIDISXsec::StrFunc( updf_, kaonF_, taget[ i ],
				     QCD::Flavor::KAm );
    }

    if( protonF_ ){
      sigmaHp_[ i ][ protonF_ ] = 
	new unpolSIDISXsec::StrFunc( updf_, protonF_, taget[ i ],
				     QCD::Flavor::PRp );
      sigmaHm_[ i ][ protonF_ ] = 
	new unpolSIDISXsec::StrFunc( updf_, protonF_, taget[ i ],
				     QCD::Flavor::PRm );
    }

    if( restF_ ){
      sigmaHp_[ i ][ restF_ ] = 
	new unpolSIDISXsec::StrFunc( updf_, restF_, taget[ i ],
				     QCD::Flavor::REp );
      sigmaHm_[ i ][ restF_ ] = 
	new unpolSIDISXsec::StrFunc( updf_, restF_, taget[ i ],
				     QCD::Flavor::REm );
    }
  }

  Utility::Arguments& args = Utility::Arguments::ref();
  
  int Mleg1 = args.get( "Mleg1", 6 );
  int Mleg2 = args.get( "Mleg2", 8 );

  double Mprec = args.get( "Mprec", 5.0E-3 );
  int Mngrid   = args.get( "Mngrid", 2 );
  
  // -------------------------------------------------------------
  //                      for numerator
  // -------------------------------------------------------------
  vector< double > offset2( 2, args.get( "offset", 3.0 )  );
  vector< double > angle2(  2, args.get( "angle",  0.55 ) );
  vector< double > length2( 2, args.get( "length", 8.0 )  );
  
  offset2[ 0 ] = args.get( "UPDFoffset", 4.5 );  
  angle2[ 0 ]  = args.get( "UPDFangle",  0.6 );  
  length2[ 0 ] = args.get( "UPDFlength", 8.0 );
  
  map< Evolution::KernelBase*, unpolSIDISXsec::StrFunc*>::iterator itr;
  for( int i = 0; i < xsigmaHp_.size(); i++ ){
    
    for( itr = sigmaHp_[ i ].begin(); itr != sigmaHp_[ i ].end(); itr++ ){
      
      Xsection::xzXsec *xz =
	( xsigmaHp_[ i ][ itr->first ] = 
	  new Xsection::xzXsec( itr->second, Mleg1, Mleg2, Mprec ) );
      
      xz->setGngrid( Mngrid );
      xz->setParameter( offset2, angle2 );
      xz->integrand().upper( length2[ 0 ] ); // x
      xz->upper( length2[ 1 ] ); // z

    }

    for( itr = sigmaHm_[ i ].begin(); itr != sigmaHm_[ i ].end(); itr++ ){
      
      Xsection::xzXsec *xz =
	( xsigmaHm_[ i ][ itr->first ] = 
	  new Xsection::xzXsec( itr->second, Mleg1, Mleg2, Mprec ) );
      
      xz->setGngrid( Mngrid );
      xz->setParameter( offset2, angle2 );
      xz->integrand().upper( length2[ 0 ] ); // x
      xz->upper( length2[ 1 ] ); // z

    }
  }
  
}

TGraphErrors* Manager::getGraph( const int& n, const double& min, const double& max,
				 const bool& fLog ){

  cout << "Create graph for " 
       << n << "-bin from " << min << " to " << max 
       << " in " << ( fLog ? "LOG" : "LINEAR" )
       << endl;

  TGraphErrors *g = new TGraphErrors( n );
  double d = ( fLog ? log10( max ) - log10( min ) : max - min ) / n;
  for( int i = 0; i < n; i++ ){
    g->SetPoint( i, 
		 ( fLog ? 
		   pow( 10, log10( min ) + i * d ) :
		   min + i * d ),
		 0.0 );
  }
  return g;
}
  


TGraphErrors* Manager::getFragXsec_Z( const int& h,
				      const int& n, 
				      const double& zmin, const double& zmax, 
				      const bool& fLog, 
				      const double& q2,
				      const double& scale ){
  
  return this->getFragXsec_Z( h, 
			      this->getGraph( n, zmin, zmax, fLog ), 
			      q2, scale );
}

TGraphErrors* Manager::getFragXsec_Z( const int& h,
				      TGraphErrors *g,
				      const double& q2,
				      const double& scale ){
  
  
  Xsection::xXsec* xxsec = NULL;
  
  switch( h ){
    
  case QCD::Flavor::PIp :
  case QCD::Flavor::PIm :
    if( pionF_ ) 
      xxsec = fragXX_[ dynamic_cast< Evolution::KernelBase* >( pionF_ ) ];
    break;
    
  case QCD::Flavor::KAp :
  case QCD::Flavor::KAm :
    if( kaonF_ ) xxsec = fragXX_[ kaonF_ ];
    break;
    
  case QCD::Flavor::PRp :
  case QCD::Flavor::PRm :
    if( protonF_ ) xxsec = fragXX_[ protonF_ ];
    break;
    
  case QCD::Flavor::REp :
  case QCD::Flavor::REm :
    if( restF_ ) xxsec = fragXX_[ restF_ ];
    break;
  }
  
  if( xxsec == NULL ){
    cerr << "xXsec object is not ready" << endl;
    return g;
  }
  
  cout << "start FragXsec_T calculation" << endl;
  cout << "|" << flush;
  for( int i = 0; i < g->GetN(); i++ ) cout << "-" << flush;
  cout << "|" << endl;

  cout << "|" << flush;
  xxsec->moment()->setQ2( q2 );
  double *zv = g->GetX();
  for( int i = 0; i < g->GetN(); i++ ){
    double& z = zv[ i ];
    cout << "*" << flush;
    g->SetPoint( i, z, scale * (*xxsec)( z ) );
  }
  cout << "| DONE!" << endl;
  
  return g;
}

TGraphErrors* Manager::getMulti_x( const int& t, // (0: p, 1: n, 2: d)
				   const int& h,
				   const int& n, const double& min, const double& max,
				   const bool& fLog,
				   const double& s,
				   const double& q2,
				   const double& z,
				   const double& scale ) {
  return this->getMulti_x( t, h, 
			   this->getGraph( n, min, max, fLog ),
			   s, q2, z, scale );
}

TGraphErrors* Manager::getMulti_x( const int& t, // (0: p, 1: n, 2: d)
				   const int& h,
				   TGraphErrors *g,
				   const double& s,
				   const double& q2,
				   const double& z,
				   const double& scale ){
  
  if( t < 0 || t >= sigma_.size() ){
    cerr << "t should be 0, 1, or 2. (" << t << ")" << endl;
    return NULL;
  }
  
  Xsection::xXsec *sig = xsigma_[ t ];;
  Xsection::xzXsec *sigh = NULL;
  
  switch( h ){
    
  case QCD::Flavor::PIp :
    if( pionF_ ) sigh = xsigmaHp_[ t ][ pionF_ ];
    break;

  case QCD::Flavor::PIm :
    if( pionF_ ) sigh = xsigmaHm_[ t ][ pionF_ ];
    break;
    
  case QCD::Flavor::KAp :
    if( kaonF_ ) sigh = xsigmaHp_[ t ][ kaonF_ ];
    break;

  case QCD::Flavor::KAm :
    if( kaonF_ ) sigh = xsigmaHm_[ t ][ kaonF_ ];
    break;
    
  case QCD::Flavor::PRp :
    if( protonF_ ) sigh = xsigmaHp_[ t ][ protonF_ ];
    break;
    
  case QCD::Flavor::PRm :
    if( protonF_ ) sigh = xsigmaHm_[ t ][ protonF_ ];
    break;
    
  case QCD::Flavor::REp :
    if( restF_ ) sigh = xsigmaHp_[ t ][ restF_ ];
    break;

  case QCD::Flavor::REm :
    if( restF_ ) sigh = xsigmaHm_[ t ][ restF_ ];
    break;

  }
  
  if( sig == NULL || sigh == NULL ){
    cerr << "xsection objects were not found." << endl;
    return g;
  }

  
  cout << "start SIDIS multiplicity calculation" << endl;
  cout << "|" << flush;
  for( int i = 0; i < g->GetN(); i++ ) cout << "-" << flush;
  cout << "|" << endl;
  
  cout << "|" << flush;
  
  dynamic_cast< unpolDISXsec::StrFunc* >( sig->moment() )->setS( s );
  dynamic_cast< unpolSIDISXsec::StrFunc* >( sigh->moment() )->setS( s );
  sig->moment()->setQ2( q2 );
  sigh->moment()->setQ2( q2 );
  vector< double >zvec( 1, z );

  double *xv = g->GetX();
  for( int i = 0; i < g->GetN(); i++ ){
    const double& x = xv[ i ];
    vector< double > xvec( 1, x );

    cout << "*" << flush;
    g->SetPoint( i, x, scale * (*sigh)( xvec, zvec ) / (*sig)( x ) );
  }
  cout << "| DONE!" << endl;
  
  return g;
}

TGraphErrors* Manager::getMulti_Q2( const int& t, // (0: p, 1: n, 2: d)
				    const int& h,
				    const int& n, const double& min, const double& max,
				    const bool& fLog,
				    const double& s,
				    const double& xmin, const double& xmax,
				    const double& zmin, const double& zmax,
				    const double& scale ) {
  return this->getMulti_Q2( t, h, 
			    this->getGraph( n, min, max, fLog ),
			    s, xmin, xmax, zmin, zmax, scale );
}

TGraphErrors* Manager::getMulti_Q2( const int& t, // (0: p, 1: n, 2: d)
				    const int& h,
				    TGraphErrors *g,
				    const double& s,
				    const double& xmin, const double& xmax,
				    const double& zmin, const double& zmax,
				    const double& scale ){
  
  if( t < 0 || t >= sigma_.size() ){
    cerr << "t should be 0, 1, or 2. (" << t << ")" << endl;
    return NULL;
  }
  
  Xsection::xXsec *sig = xsigma_[ t ];;
  Xsection::xzXsec *sigh = NULL;
  
  switch( h ){
    
  case QCD::Flavor::PIp :
    if( pionF_ ) sigh = xsigmaHp_[ t ][ pionF_ ];
    break;

  case QCD::Flavor::PIm :
    if( pionF_ ) sigh = xsigmaHm_[ t ][ pionF_ ];
    break;
    
  case QCD::Flavor::KAp :
    if( kaonF_ ) sigh = xsigmaHp_[ t ][ kaonF_ ];
    break;

  case QCD::Flavor::KAm :
    if( kaonF_ ) sigh = xsigmaHm_[ t ][ kaonF_ ];
    break;
    
  case QCD::Flavor::PRp :
    if( protonF_ ) sigh = xsigmaHp_[ t ][ protonF_ ];
    break;
    
  case QCD::Flavor::PRm :
    if( protonF_ ) sigh = xsigmaHm_[ t ][ protonF_ ];
    break;
    
  case QCD::Flavor::REp :
    if( restF_ ) sigh = xsigmaHp_[ t ][ restF_ ];
    break;

  case QCD::Flavor::REm :
    if( restF_ ) sigh = xsigmaHm_[ t ][ restF_ ];
    break;

  }
  
  if( sig == NULL || sigh == NULL ){
    cerr << "xsection objects were not found." << endl;
    return g;
  }

  
  cout << "start SIDIS multiplicity calculation" << endl;
  cout << "|" << flush;
  for( int i = 0; i < g->GetN(); i++ ) cout << "-" << flush;
  cout << "|" << endl;
  
  cout << "|" << flush;

  vector< double > x( 2 );
  vector< double > z( 2 );
  
  x[ 0 ] = xmin; x[ 1 ] = xmax;
  z[ 0 ] = zmin; z[ 1 ] = zmax;
  
  double *q2v = g->GetX();
  dynamic_cast< unpolDISXsec::StrFunc* >( sig->moment() )->setS( s );
  dynamic_cast< unpolSIDISXsec::StrFunc* >( sigh->moment() )->setS( s );
  for( int i = 0; i < g->GetN(); i++ ){
    double& q2 = q2v[ i ];
    sig->moment()->setQ2( q2 );
    sigh->moment()->setQ2( q2 );
    cout << "*" << flush;
    g->SetPoint( i, q2, scale * (*sigh)( x, z ) / (*sig)( x ) );
  }
  cout << "| DONE!" << endl;
  
  return g;
}

ClassImp( Manager )
