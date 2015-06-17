#include "A1hFit.hh"
#include "A1Data.hh"
#include <Tranform/GridIntegration.hh>
#include <ExpData/DataSet.hh>
#include <Xsec/Xsec.hh>
#include <Xsec/XsecComp.hh>
#include <Xsec/CKernel.hh>
#include <ErrorCalc/ErrCalc.hh>

#include <cmath>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>

using namespace std;
using namespace QCD;
using namespace Utility;
using namespace Exp;
using namespace Fit;

A1hFit::A1hFit( Arguments& args, const bool& judge ) throw( int ) : 
  Fit::Fitting( args, judge ),
  FFevo_( NULL ), 
  type_( 0 ), dsettype_( 0 ),
  z_( 0 ), x_( 0 ), zrange_( 2, 0.0 ), vx_( 1, 0.0 ),
  g1_( 0 ), dg1_( 0 ), f1_( 0 ),
  g1x_( 0 ), dg1x_( 0 ), f1x_( 0 ),
  g1h_( 0 ), dg1h_( 0 ), f1h_( 0 ), 
  g1hx_( 0 ), dg1hx_( 0 ), f1hx_( 0 ),
  g1hxx_( 0 ), dg1hxx_( 0), f1hxx_( 0 ),
  f1cal_( 0 ), f1hcal_( 0 ),
  original_( 0 ),
  useXXsec_( args.hasOpt( "xXsec" ) ),
  ppdfOffset_( 0.0 ), ppdfAngle_( 0.0 ), ppdfLength_( 0.0 ),
  updfOffset_( 0.0 ), updfAngle_( 0.0 ), updfLength_( 0.0 ),
  ffOffset_( 0.0 ),   ffAngle_( 0.0 ),   ffLength_( 0.0 ),
  xpolpdf_( NULL ),  xunpolpdf_( NULL ),
  post_conf_judge_( false )
{

  //config process 
  if( judge ){

    try {
      this->post_config_process();
    }

    catch( int error ){
      ARG_ERR << "catch errror (" << error << ")" << endl;
      throw( error );
    }
  }

}

A1hFit::~A1hFit() {

  if( post_conf_judge_ ){
    this->post_delete_process();
  }
}

void A1hFit::post_delete_process() {

  this->del( dg1hxx_ );
  this->del( dg1hx_ );
  this->del( dg1h_ );
  this->del( dg1x_ );
  this->del( dg1_ );
  this->del( dg1_ );

  this->del( g1hxx_ );
  this->del( g1hx_ );
  this->del( g1h_ );
  this->del( g1x_ );
  this->del( g1_ );
  this->del( g1_ );

  this->del( f1hxx_ );
  this->del( f1hx_ );
  this->del( f1h_ );
  this->del( f1x_ );
  this->del( f1_ );

  delete xpolpdf_;
  delete xunpolpdf_;

}

void A1hFit::post_config_process() throw( int ){

  if( ! this->getconf_judge() ){
    ARG_ERR << "error: config process in the base class " << flush; 
    ARG_ERR << "must be excuted first" << endl;
    throw( 2 );
  }

  if( post_conf_judge_ ){
    ARG_ERR << "error: post configuration process " << flush;
    ARG_ERR << "execution duplicated" << endl;
    throw( 2 );
  }
  
  Arguments& args = Arguments::ref();

#ifdef DEBUG_FITTING
#endif // DEBUG_FITTING
  
  this->load_options();   // load options from arguments
  dynamic_cast< Fitting* >( this )->loadData();       // load experimental data
  
  // check if there is polarized PDF object
  if( ppdf_ == NULL ){
    ARG_ERR << "# " << args.command() << endl;
    ARG_ERR << "#  requires polPDF initial distribuion." << endl;
    ARG_ERR << "#  please specify with one of following options." << endl;
    vector< string > opts = polPDF::Evo::options();
    for( int i = 0; i < opts.size(); i++ ){
      ARG_ERR << "#\t--" << opts[ i ] << "=[xml file]" << endl; 
    }
    throw( 2 );
  } else {
    ppdf_->constNf( true );
  }
  
  // check if there is unpolarized PDF object
  if( updf_ == NULL ){
    ARG_ERR << "# " << args.command() << endl;
    ARG_ERR << "#  requires unpolPDF initial distribuion." << endl;
    ARG_ERR << "#  please specify with one of following options." << endl;
    vector< string > opts = unpolPDF::Evo::options();
    for( int i = 0; i < opts.size(); i++ ){
      ARG_ERR << "#\t--" << opts[ i ] << "=[xml file]" << endl; 
    }
    throw( 3 );
  } else {
    updf_->constNf( false );
  }
  
  // prepare Evos and register PPDF to TMinuit    
  try {
    this->setup_DIS();
    this->setup_SIDIS();
  }
  
  catch( int e ) {
    cerr << __FILE__ << ":" << __LINE__
	 << "\tcatch error (" << e << ")" << endl;
    throw( e );
  }

  // calculate F1 in denominator....
  this->calcF1();

  // resize lest containers
  original_.resize( sig_.size(), 0.0 );
  
  // for positivity conditions
  xpolpdf_ = new Evolution::PDFx( ppdf_, QCD::Flavor::PARTON( -3 ), 
				  4, 6, 1.0E-4, 15, 1.0, 0.55 );
  xunpolpdf_ = new Evolution::PDFx( updf_, QCD::Flavor::PARTON( -3 ),
				  4, 6, 1.0E-4, 15, 1.8, 0.55 );

  post_conf_judge_ = true; 
  // flag for post config process execution 
  // (for duplicate check & delete process)

}

void A1hFit::setup_SIDIS( const Xsection::Charge::TARGET& n,
			  const QCD::Flavor::HADRON& h ) throw( int ) {
  
  Utility::Arguments& args = Utility::Arguments::ref();
  
  int ntyp = A1Data::nType( n );
  int htyp = A1Data::hType( h );
  
  Evolution::Kernel *ff = NULL;
  switch( h ){
  case QCD::Flavor::PIp : 
  case QCD::Flavor::PIm :   ff = pion_; break;
  case QCD::Flavor::KAp : 
  case QCD::Flavor::KAm :   ff = kaon_; break;
  case QCD::Flavor::REp : 
  case QCD::Flavor::REm :   ff = rest_; break;
  case QCD::Flavor::HAp : 
  case QCD::Flavor::HAm :   ff = hadron_; break;
  }
  
  if( ff == NULL ) return;
  
  ff->constNf( false );
  
  try {
    if( o_ppdf_ && d_ppdf_ ){
      
      ARG_WARN << "g1h(" << setw(2) << ntyp << "," << setw(2) << htyp 
	       << ") will be calcuated in differene mode" 
	       << endl;
      
      g1h_[ ntyp ][ htyp ]  = new polSIDIS::StrFunc( o_ppdf_, ff, n, h );
      dg1h_[ ntyp ][ htyp ] = new polSIDIS::StrFunc( d_ppdf_, ff, n, h );
    } else {
      g1h_[ ntyp ][ htyp ] = new polSIDIS::StrFunc( ppdf_, ff, n, h );
    }
    
    // create F1h and disable cache mechanism
    if( f1h_[ ntyp ][ htyp ] = 
	new unpolSIDIS::StrFunc( updf_, ff, n, h ) )
      f1h_[ ntyp ][ htyp ]->disableCache();

  }
  
  catch( int error ){
    ARG_ERR << "catch error (" << error << ")" << endl;
    throw( error );
  }
  
}

void A1hFit::setup_SIDIS( const Xsection::Charge::TARGET& n ) throw( int ) {
  
  try {
    this->setup_SIDIS( n, QCD::Flavor::PIp );
    this->setup_SIDIS( n, QCD::Flavor::KAp );
    //  this->setup_SIDIS( n, QCD::Flavor::REp );
    this->setup_SIDIS( n, QCD::Flavor::HAp );
    
    this->setup_SIDIS( n, QCD::Flavor::PIm );
    this->setup_SIDIS( n, QCD::Flavor::KAm );
    //  this->setup_SIDIS( n, QCD::Flavor::REm );
    this->setup_SIDIS( n, QCD::Flavor::HAm );
  }
  
  catch( int error ){
    ARG_ERR << "catch error (" << error << ")" << endl;
    throw( error );
  }

}

// -------------------  Semi-Inclusive DIS ---------------------------- //
void A1hFit::setup_SIDIS() throw( int ) {
  
  Utility::Arguments& args = Utility::Arguments::ref();
  
  int nTarget = A1Data::nType( "ALL" );
  int nHadron = A1Data::hType( "ALL" );
  
  f1h_.resize( nTarget, 
	       vector< unpolSIDIS::StrFunc* >( nHadron, (unpolSIDIS::StrFunc*) NULL ) );
  f1hx_.resize( nTarget,
		vector< Xsection::xzXsec* >( nHadron, (Xsection::xzXsec*)NULL ) );
  
  f1hxx_.resize( nTarget, vector< Xsection::xXsec* >( nHadron, (Xsection::xXsec*) NULL ) );
  
  g1h_.resize( nTarget ,
	       vector< polSIDIS::StrFunc* >( nHadron, (polSIDIS::StrFunc*) NULL ) );
  
  g1hx_.resize( nTarget, 
		vector< Xsection::xzXsec* >( nHadron, (Xsection::xzXsec*) NULL ) ); 

  g1hxx_.resize( nTarget, vector< Xsection::xXsec* >( nHadron, (Xsection::xXsec*) NULL ) );
  
  dg1h_.resize( nTarget ,
		vector< polSIDIS::StrFunc* >( nHadron, (polSIDIS::StrFunc*) NULL ) );
  
  dg1hx_.resize( nTarget, 
		 vector< Xsection::xzXsec* >( nHadron, (Xsection::xzXsec*) NULL ) ); 

  dg1hxx_.resize( nTarget, vector< Xsection::xXsec* >( nHadron, (Xsection::xXsec*) NULL ) );
  
  try {
    
    this->setup_SIDIS( Xsection::Charge::p );
    //    this->setup_SIDIS( Xsection::Charge::n );
    this->setup_SIDIS( Xsection::Charge::d );
    
    int Sleg1 = args.get( "Sleg1", 6 );
    int Sleg2 = args.get( "Sleg2", 8 );
    double Sprec = args.get( "Sprec", 5.0E-3 );
    int Sngrid = args.get( "Sngrid", 2 );
    
    vector< double > offset2( 2, ffOffset_ );
    vector< double > angle2(  2, ffAngle_ );
    vector< double > length2( 2, ffLength_ );
    
    vector< double > mom_( 2, 0.0 ); mom_[ 1 ] = 1.0;
    
    offset2[ 0 ] = ppdfOffset_; // 1st element for xx integ
    angle2[ 0 ]  = ppdfAngle_;  // 1st element for xx integ
    length2[ 0 ] = ppdfLength_; // 1st element for xx integ
    
    for( int i = 0; i < g1h_.size(); i++ ){
      for( int j = 0; j < g1h_[ i ].size(); j++ ){
	
	if( g1h_[ i ][ j ] == NULL ) continue;
	
	// DInverseMellin base calculation
	g1hx_[ i ][ j ] = 
	  new Xsection::xzXsec( g1h_[ i ][ j ], Sleg1, Sleg2, Sprec );
	
	g1hx_[ i ][ j ]->setParameter( offset2, angle2 );
	g1hx_[ i ][ j ]->integrand().upper( ppdfLength_ ); // x
	g1hx_[ i ][ j ]->upper( ffLength_ ); // z
	g1hx_[ i ][ j ]->setGngrid( Sngrid );
	g1hx_[ i ][ j ]->setMomParam( mom_ );
	
	// InverseMellin base calculation
	g1hxx_[ i ][ j ] = 
	  new Xsection::xXsec( g1h_[ i ][ j ], Sleg1, Sleg2, Sprec );
	
	g1hxx_[ i ][ j ]->setParameter( offset2, angle2 );
	g1hxx_[ i ][ j ]->upper( length2 ); // z
	g1hxx_[ i ][ j ]->setGngrid( Sngrid );
	g1hxx_[ i ][ j ]->setMomParam( mom_ );
	

	
      }
    }
    
    for( int i = 0; i < dg1h_.size(); i++ ){
      for( int j = 0; j < dg1h_[ i ].size(); j++ ){
	
	if( dg1h_[ i ][ j ] == NULL ) continue;
	
	// DInverseMellin base calculation
	dg1hx_[ i ][ j ] = 
	  new Xsection::xzXsec( dg1h_[ i ][ j ], Sleg1, Sleg2, diffPrec_ );
	
	dg1hx_[ i ][ j ]->setParameter( offset2, angle2 );
	dg1hx_[ i ][ j ]->integrand().upper( ppdfLength_ ); // x
	dg1hx_[ i ][ j ]->upper( ffLength_ ); // z
	dg1hx_[ i ][ j ]->setGngrid( Sngrid );
	dg1hx_[ i ][ j ]->setMomParam( mom_ );
	
	// InverseMellin base calculation
	dg1hxx_[ i ][ j ] = 
	  new Xsection::xXsec( dg1h_[ i ][ j ], Sleg1, Sleg2, diffPrec_ );
	
	dg1hxx_[ i ][ j ]->setParameter( offset2, angle2 );
	dg1hxx_[ i ][ j ]->upper( length2 ); // z
	dg1hxx_[ i ][ j ]->setGngrid( Sngrid );
	dg1hxx_[ i ][ j ]->setMomParam( mom_ );
	
      }
    }
    
    offset2[ 0 ] = updfOffset_; // 1st element for xx integ
    angle2[ 0 ]  = updfAngle_;  // 1st element for xx integ
    length2[ 0 ] = updfLength_; // 1st element for xx integ
    
    for( int i = 0; i < f1h_.size(); i++ ){
      for( int j = 0; j < f1h_[ i ].size(); j++ ){
	
	if( f1h_[ i ][ j ] == NULL ) continue;
	
	f1hx_[ i ][ j ] = 
	  new Xsection::xzXsec( f1h_[ i ][ j ], Sleg1, Sleg2, Sprec );
	
	f1hx_[ i ][ j ]->setParameter( offset2, angle2 );
	f1hx_[ i ][ j ]->integrand().upper( updfLength_ ); // x
	f1hx_[ i ][ j ]->upper( ffLength_ ); // z
	f1hx_[ i ][ j ]->setGngrid( Sngrid );
	f1hx_[ i ][ j ]->setMomParam( mom_ );
	
	f1hxx_[ i ][ j ] = 
	  new Xsection::xXsec( f1h_[ i ][ j ], Sleg1, Sleg2, Sprec );
	
	f1hxx_[ i ][ j ]->setParameter( offset2, angle2 );
	f1hxx_[ i ][ j ]->upper( length2 ); // z
	f1hxx_[ i ][ j ]->setGngrid( Sngrid );
	f1hxx_[ i ][ j ]->setMomParam( mom_ );
	
      }
    }
  }

  catch( int error ){
    ARG_ERR << "catch error (" << error << ")" << endl;
    throw( error );
  }
  
}
  
// -------------------  Inclusive DIS ---------------------------- //
void A1hFit::setup_DIS() throw( int ) {
  
  Utility::Arguments& args = Utility::Arguments::ref();

  int nTarget = A1Data::nType( "ALL" );

  f1_.resize( nTarget, NULL );
  f1x_.resize( nTarget, NULL );

  g1_.resize( nTarget, NULL );
  g1x_.resize( nTarget, NULL );
  
  dg1_.resize( nTarget, NULL );
  dg1x_.resize( nTarget, NULL );
  
  int leg1    = args.get( "leg1", 6 );
  int leg2    = args.get( "leg2", 8 );
  double prec = args.get( "prec", 5.0E-3 );
  int ngrid   = args.get( "ngrid", 2 );
  
  try {
    
    if( f1_[ 0 ] = 
	new unpolDIS::StrFunc( updf_, Xsection::Charge::p ) )
      f1_[ 0 ]->disableCache();
    
    if( f1_[ 1 ] = 
	new unpolDIS::StrFunc( updf_, Xsection::Charge::n ) )
      f1_[ 1 ]->disableCache();
    
    if( f1_[ 2 ] = 
	new unpolDIS::StrFunc( updf_, Xsection::Charge::d ) )
      f1_[ 2 ]->disableCache();
    
    if( o_ppdf_ && d_ppdf_ ){
      
      ARG_WARN << "g1 will be calculated in differene mode..." << endl; 
      
      g1_[ 0 ] = new polDIS::StrFunc( o_ppdf_, Xsection::Charge::p ); 
      g1_[ 1 ] = new polDIS::StrFunc( o_ppdf_, Xsection::Charge::n ); 
      g1_[ 2 ] = new polDIS::StrFunc( o_ppdf_, Xsection::Charge::d ); 
      
      dg1_[ 0 ] = new polDIS::StrFunc( d_ppdf_, Xsection::Charge::p ); 
      dg1_[ 1 ] = new polDIS::StrFunc( d_ppdf_, Xsection::Charge::n ); 
      dg1_[ 2 ] = new polDIS::StrFunc( d_ppdf_, Xsection::Charge::d ); 
      
    } else {
      
      g1_[ 0 ] = new polDIS::StrFunc( ppdf_, Xsection::Charge::p ); 
      g1_[ 1 ] = new polDIS::StrFunc( ppdf_, Xsection::Charge::n ); 
      g1_[ 2 ] = new polDIS::StrFunc( ppdf_, Xsection::Charge::d ); 
      
    }
    
    for( int i = 0; i < g1_.size(); i++ ){
      g1x_[ i ] = new Xsection::xXsec( g1_[ i ], leg1, leg2, prec );
      g1x_[ i ]->setParameter( ppdfOffset_, ppdfAngle_ );
      g1x_[ i ]->upper( ppdfLength_ );
      g1x_[ i ]->setGngrid( ngrid );
    }
    
    for( int i = 0; i < dg1_.size(); i++ ){
      if( dg1_[ i ] == NULL ) continue;
      ARG_WARN << "xg1 will be calculated in differene mode..." << endl; 
      dg1x_[ i ] = new Xsection::xXsec( dg1_[ i ], leg1, leg2, diffPrec_ );
      dg1x_[ i ]->setParameter( ppdfOffset_, ppdfAngle_ );
      dg1x_[ i ]->upper( ppdfLength_ );
      dg1x_[ i ]->setGngrid( ngrid );
    }
    
    for( int i = 0; i < f1_.size(); i++ ){
      f1x_[ i ] = new Xsection::xXsec( f1_[ i ], leg1, leg2, prec );      
      f1x_[ i ]->setParameter( updfOffset_, updfAngle_ );
      f1x_[ i ]->upper( updfLength_ );
      f1x_[ i ]->setGngrid( ngrid );
    }
    
  }
  
  catch( int error ){
    ARG_ERR << "catch error (" << error << ")" << endl;
    throw( error );
  }
  
}

void A1hFit::load_options(){

  Utility::Arguments& args = Utility::Arguments::ref();
  
  // ---------------------------------------------------------------------- //
  //                           load options                                 //
  // ---------------------------------------------------------------------- //
  
  // A1h defalut z range
  this->zrange( args.get( "zminPolPDF", 0.2 ), args.get( "zmaxPolPDF", 0.80 ) );
  
  // check lower Q2 limit
  q2limit_ = args.get( "q2limitPolPDF", 1.0 );
  // To distinguish them from those for FF analysis, "PolPDF" added
  // (required for MultiFrag_A1hFit)

  updfOffset_ = args.get( "UPDFoffset", 2.0  );  
  updfAngle_  = args.get( "UPDFangle",  0.55 );  
  updfLength_ = args.get( "UPDFlength", 8.0 );
  
  ppdfOffset_ = args.get( "PPDFoffset", 2.0  );  
  ppdfAngle_  = args.get( "PPDFangle",  0.55 );  
  ppdfLength_ = args.get( "PPDFlength", 8.0 );
  
  ffOffset_   = args.get( "FFoffset", 2.0  );  
  ffAngle_    = args.get( "FFangle",  0.55 );  
  ffLength_   = args.get( "FFlength", 8.0 );

}

void A1hFit::postLoop() {
#ifdef DEBUG_FITTING

  ARG_DETAIL << " uPDF" << setw(6) << updf_->dglapMatrix().cache_size() << flush;

  if( difference_ ){

    if( d_pion_ && d_pion_->dglapMatrix().cache_size() > 0 )
      ARG_DETAIL << " pi" << setw(6) << d_pion_->dglapMatrix().cache_size() << flush;
    
    if( d_kaon_ && d_kaon_->dglapMatrix().cache_size() > 0 )
      ARG_DETAIL << " ka" << setw(6) << d_kaon_->dglapMatrix().cache_size() << flush;
    
    if( d_rest_ && d_rest_->dglapMatrix().cache_size() > 0 )
      ARG_DETAIL << " re" << setw(6) << d_rest_->dglapMatrix().cache_size() << flush;
    
    if( d_proton_ && d_proton_->dglapMatrix().cache_size() > 0 )
      ARG_DETAIL << " pr" << setw(6) << d_proton_->dglapMatrix().cache_size() << flush;
    
    ARG_DETAIL << " g1h" << flush;
    for( int i = 0; i < dg1h_.size(); i++ ){
      for( int j = 0; j < dg1h_[ i ].size(); j++ ){
	if( dg1h_[ i ][ j ] && dg1h_[ i ][ j ]->cache2().size() ) 
	  ARG_DETAIL << setw(6) << dg1h_[ i ][ j ]->cache2().size() << flush;
      }
    }
    
  } else {  
    
    if( pion_ && pion_->dglapMatrix().cache_size() > 0 )
      ARG_DETAIL << " pi" << setw(6) << pion_->dglapMatrix().cache_size() << flush;
    
    if( kaon_ && kaon_->dglapMatrix().cache_size() > 0 )
      ARG_DETAIL << " ka" << setw(6) << kaon_->dglapMatrix().cache_size() << flush;
    
    if( rest_ && rest_->dglapMatrix().cache_size() > 0 )
      ARG_DETAIL << " re" << setw(6) << rest_->dglapMatrix().cache_size() << flush;
    
    if( proton_ && proton_->dglapMatrix().cache_size() > 0 )
      ARG_DETAIL << " pr" << setw(6) << proton_->dglapMatrix().cache_size() << flush;
    
    ARG_DETAIL << " g1h" << flush;
    for( int i = 0; i < g1h_.size(); i++ ){
      for( int j = 0; j < g1h_[ i ].size(); j++ ){
	if( g1h_[ i ][ j ] && g1h_[ i ][ j ]->cache2().size() ) 
	  ARG_DETAIL << setw(6) << g1h_[ i ][ j ]->cache2().size() << flush;
      }
    }

  }
  
#endif // DEBUG_FITTING
}

void A1hFit::calcF1(){

#ifdef DEBUG_FITTING
  ARG_LOG << endl;
  ARG_LOG << "# -------------------------------------------------- " << endl;
  ARG_LOG << "#     unpol structure function preparation           " << endl;
  ARG_LOG << "# -------------------------------------------------- " << endl;
#endif //DEBUG_FITTING
  
  if( useFData_ ){
    
    vector< double > xv( 1 );  

    for( int i = 0; i < fData_.size(); i++ ){
      
      A1Data *ad = dynamic_cast< A1Data* >( fData_[ i ] );
      if( ad == NULL ) continue;
      
      double& q2 = ad->q2_;
      int& it   = ad->it_;
      int& ih   = ad->ih_;
      double& x = ad->x_;
      
#ifdef DEBUG_FITTING
      ostringstream ostr;
      ostr << "F1" << A1Data::targetName( it );
      if( ih >=0 ) ostr << "^" << A1Data::hadronName( ih );
      ostr << "(" << setw(6) << x << "," << setw(6) << q2 << ") = ";
      ARG_DETAIL << setw(30) << ostr.str() <<flush;
#endif // DEBUG_FITTING
      
      if( ad->a1h_ == false ){                                 // A1 Part //
	f1_[ it ]->setQ2( q2 );
	ad->f1_ = (*f1x_[ it ])( x );
	
#ifdef DEBUG_FITTING
	ARG_DETAIL << setw(12) << ad->f1_ << endl;
#endif //      

	// in order to save memory for fitting
	f1_[ it ]->clearCache();
	
	
      } else {                                     // A1h Part //
	
	if( f1h_[ it ][ ih ] == NULL ){
#ifdef DEBUG_FITTING
	  ARG_DETAIL << "Not calculated" << endl;
#endif //      
	  continue;
	}
	
	f1h_[ it ][ ih ]->setQ2( q2 );
	if( useXXsec_ ){
	  vector< double > length = f1hxx_[ it ][ ih ]->length();
	  if( x < 0.01 )      { 
	    length[0] = 12.0; f1hxx_[ it ][ ih ]->upper( length ); 
	  } else if( x < 0.1  ) { 
	    length[0] =  8.0; f1hxx_[ it ][ ih ]->upper( length ); 
	  } else { 
	    length[0] =  5.0; f1hxx_[ it ][ ih ]->upper( length ); }
	}
	
	xv[ 0 ] = x;
	ad->f1_ = ( useXXsec_ ? 
		    (*f1hxx_[ it ][ ih ])( xv, zrange_ ) :
		    (*f1hx_[ it ][ ih ])( xv, zrange_ ) );
	
#ifdef DEBUG_FITTING
	ARG_DETAIL << setw(12) << ad->f1_ << endl;
#endif //      
	
	// in order to save memory for fitting
	f1h_[ it ][ ih ]->clearCache();
	
      }
      
    }    
    
    
  } else {

    f1cal_.resize( sig_.size(), 0.0 );     // resize container size
    f1hcal_.resize( sig_.size(), 0.0 ); 

    vector< double > xv( 1 );  
    for( int i = 0; i < sig_.size(); i++ ){
      
      double& q2 = q2_[ i ];
      int& it = type_[ i ];
      int& ih = dsettype_[ i ];
      double& x = x_[ i ];
      
#ifdef DEBUG_FITTING
      ostringstream ostr;
      ostr << "F1" << A1Data::targetName( it );
      if( ih >=0 ) ostr << "^" << A1Data::hadronName( ih );
      ostr << "(" << setw(6) << x << "," << setw(6) << q2 << ") = ";
      ARG_DETAIL << setw(30) << ostr.str() <<flush;
#endif // DEBUG_FITTING
      
      if( ih < 0 ){                                 // A1 Part //
	f1_[ it ]->setQ2( q2 );
	f1cal_[ i ] = (*f1x_[ it ])( x );
	
#ifdef DEBUG_FITTING
	ARG_DETAIL << setw(12) << f1cal_[ i ] << endl;
#endif //      

	// in order to save memory for fitting
	f1_[ it ]->clearCache();
	
	
      } else {                                     // A1h Part //
	
	if( f1h_[ it ][ ih ] == NULL ){
#ifdef DEBUG_FITTING
	  ARG_DETAIL << setw(12) << f1hcal_[ i ] << endl;
#endif //      
	  continue;
	}
	
	f1h_[ it ][ ih ]->setQ2( q2 );
	if( useXXsec_ ){
	  vector< double > length = f1hxx_[ it ][ ih ]->length();
	  if( x < 0.01 )      { 
	    length[0] = 12.0; f1hxx_[ it ][ ih ]->upper( length ); 
	  } else if( x < 0.1  ) { 
	    length[0] =  8.0; f1hxx_[ it ][ ih ]->upper( length ); 
	  } else { 
	    length[0] =  5.0; f1hxx_[ it ][ ih ]->upper( length ); }
	}
	
	xv[ 0 ] = x;
	f1hcal_[ i ] = ( useXXsec_ ? 
			 (*f1hxx_[ it ][ ih ])( xv, zrange_ ) :
			 (*f1hx_[ it ][ ih ])( xv, zrange_ ) );
	
#ifdef DEBUG_FITTING
	ARG_DETAIL << setw(12) << f1hcal_[ i ] << endl;
#endif //
	
	// in order to save memory for fitting
	f1h_[ it ][ ih ]->clearCache();
	
      }
    }
  }
  
}

bool A1hFit::calc( FitData* fd ) {

  A1Data *ad = dynamic_cast< A1Data* >( fd );
  if( ad == NULL ) return false;

  int&   it = ad->it_;
  int&   ih = ad->ih_;
  double& x = ad->x_;
  
  if( ad->a1h_ == false ){                           // A1 Part //
    
    if( g1_[ it ] == NULL ) return false;
    
    g1_[ it ]->setQ2( ad->q2_ );
    this->g1x_path( x, g1x_[ it ] );
    
    if( ! difference_ ){
      ad->fit_ = (*g1x_[ it ])( x );
    } else {

      if( ad->original_ == 0.0 ) ad->original_ = (*g1x_[ it ])( x );
      
      if( dg1_[ it ] ){
	dg1_[ it ]->setQ2( ad->q2_ );
	this->g1x_path( x, dg1x_[ it ] );
	ad->fit_ = ad->original_ + (*dg1x_[ it ])( x );
      }
    }

  } else {                                          // A1h Part //
    
    if( g1h_[ it ][ ih ] == NULL ) return false;

    g1h_[ it ][ ih ]->setQ2( ad->q2_ );
    if( useXXsec_ ) this->g1hx_path( x, g1hxx_[ it ][ ih ] );
    vx_[ 0 ] = x;
    
    if( ! difference_ ){

      ad->fit_ = ( useXXsec_ ? 
		   (*g1hxx_[ it ][ ih ])( vx_, zrange_ ) :
		   (*g1hx_[ it ][ ih ])( vx_, zrange_ ) ) ;

    } else {
      
      if( ad->original_ == 0.0 )
	ad->original_ = ( useXXsec_ ? 
			  (*g1hxx_[ it ][ ih ])( vx_, zrange_ ) :
			  (*g1hx_[ it ][ ih ])( vx_, zrange_ ) ) ;
      
      if( dg1h_[ it ][ ih ] ){
	dg1h_[ it ][ ih ]->setQ2( ad->q2_ );
	if( useXXsec_ ) this->g1hx_path( x, dg1hxx_[ it ][ ih ] );
	ad->fit_ = ad->original_ +
	  ( useXXsec_ ? 
	    (*dg1hxx_[ it ][ ih ])( vx_, zrange_ ) :
	    (*dg1hx_[ it ][ ih ])( vx_, zrange_ ) ) ;
      }
    }
  }
  
  ad->fit_ /= ad->f1_;
  return true;
}

bool A1hFit::calc( const int& i ) {
  
  int& it = type_[ i ];
  int& ih = dsettype_[ i ];
  double& x = x_[ i ];
  
  if( ih < 0 ){                                      // A1 Part //
    
    if( g1_[ it ] == NULL ) return false;
    
    g1_[ it ]->setQ2( q2_[ i ] );
    this->g1x_path( x, g1x_[ it ] );
    if( ! difference_ ){
      fit_[ i ] = (*g1x_[ it ])( x ) ;
    } else {
      if( original_[ i ] == 0.0 ) original_[ i ] = (*g1x_[ it ])( x ) / f1cal_[ i ] ;
      if( dg1_[ it ] ){
	dg1_[ it ]->setQ2( q2_[ i ] );
	this->g1x_path( x, dg1x_[ it ] );
	fit_[ i ] = original_[ i ] + (*dg1x_[ it ])( x );
      }
    }
    
    fit_[ i ] /=  f1cal_[ i ];
    
  } else {                                           // A1h Part //
    
    if( g1h_[ it ][ ih ] == NULL ) return false;
    
    g1h_[ it ][ ih ]->setQ2( q2_[ i ] );
    if( useXXsec_ ) this->g1hx_path( x, g1hxx_[ it ][ ih ] );
    vx_[ 0 ] = x;
    
    if( ! difference_ ){
      
      fit_[ i ] = ( useXXsec_ ? 
		    (*g1hxx_[ it ][ ih ])( vx_, zrange_ ) :
		    (*g1hx_[ it ][ ih ])( vx_, zrange_ ) ) ;
      
    } else {
      
      if( original_[ i ] == 0.0 )
	original_[ i ] = ( useXXsec_ ? 
			   (*g1hxx_[ it ][ ih ])( vx_, zrange_ ) :
			   (*g1hx_[ it ][ ih ])( vx_, zrange_ ) ) ;
      
      if( dg1h_[ it ][ ih ] ){
	dg1h_[ it ][ ih ]->setQ2( q2_[ i ] );
	if( useXXsec_ ) this->g1hx_path( x, dg1hxx_[ it ][ ih ] );
	fit_[ i ] = original_[ i ] +
	  ( useXXsec_ ? 
	    (*dg1hxx_[ it ][ ih ])( vx_, zrange_ ) :
	    (*dg1hx_[ it ][ ih ])( vx_, zrange_ ) ) ;
      }
      
    }
    
    fit_[ i ] /=  f1hcal_[ i ];

  }    
  return true;
}

void A1hFit::loadData( Exp::DataSet& ds ){
  
  // check if it is A1 or A1h
  const string& dInfo = ds[ 0 ].info(); // data type
  bool dis   = A1Data::isDIS( dInfo );
  bool sidis = A1Data::isSIDIS( dInfo );
  
  // if neigher A1 or A1h, ignore
  if( dis == false && sidis == false ) {
    ARG_DETAIL << setw(35) <<  ds.info() << " is not A1(h) data. ("
	       << dInfo << ")" << endl;
    return;
  }
  int it = A1Data::nType( dis, dInfo );
  int ih = A1Data::hType( dis, dInfo );
  
#ifdef DEBUG_FITTING
  ARG_LOG << "Data Info:\t" << dInfo << endl;
  ARG_LOG << "Data Type:\t" << ( dis ? "DIS" : "SIDIS" ) << flush;
  ARG_LOG << "\ttarget ID = " << it << flush;
  ARG_LOG << ", hadron ID = " << ih << flush;
  ARG_LOG << "  NORM:"  << setw(12) << ds.norm()
	  << "  NCOEF:" << setw(12) << ds.ncoef()
	  << "  NCERR:" << setw(12) << ds.ncerr()
	  << "  ( " << ds.ncstat() << ")" << flush;
  ARG_LOG << endl;
#endif // DEBUG_FITTING
  
  for( int id = 0; id < ds.size(); id++ ){
    
    DataPoint& d = const_cast< DataPoint& >( ds[ id ] );
    double q2 = d.kine( "Q^2" );
    
    if( q2 < q2limit_ ) continue;
    
    if( useFData_ ){
      fData_.push_back( new A1Data( ds, ds[ id ] ) );
    } else {
      
      x_.push_back( d.kine( "x" ) );
      z_.push_back( dis ? 0.0 : d.kine( "z" ) );
      q2_.push_back( q2 );
      sig_.push_back( d.data() );
      dsig_.push_back( d.error() );
      expInfo_.push_back( ds.info() );
      type_.push_back( it );
      dsettype_.push_back( ih );
      dset_.push_back( const_cast< DataSet* >( &ds ) ); // add pointer to DataSet
      c2weight_.push_back( ds.expdata() ? ds.expdata()->lambda() : 1.0 );
      point_.push_back( &d );
      
#ifdef DEBUG_FITTING
      ARG_DETAIL
	<< setw(6)  << x_.back()
	<< setw(6)  << z_.back()
	<< setw(6)  << q2_.back()
	<< setw(12) << sig_.back()
	<< setw(12) << dsig_.back()
	<< setw(3)  << type_.back()
	<< setw(3)  << dsettype_.back()
	<< setw(20) << expInfo_.back()
	<< endl;
#endif // DEBUG_FITTING
    }
  }
}

void A1hFit::postUpdate() {

  if( this->kernels().size() == 0 ) return;
  
  //  denominators do not change in A1 A1h fit
  if( difference_ ) {
    
    for( int i = 0; i < dg1_.size(); i++ ) dg1_[ i ]->clearCache();
    for( int i = 0; i < dg1h_.size(); i++ ) 
      for( int j = 0; j < dg1h_[ i ].size(); j++ ) 
	if( g1h_[ i ][ j ] ) dg1h_[ i ][ j ]->clearCache();
    
  } else {
    
    for( int i = 0; i < g1_.size(); i++ ) g1_[ i ]->clearCache();
    for( int i = 0; i < g1h_.size(); i++ ) 
      for( int j = 0; j < g1h_[ i ].size(); j++ ) 
	if( g1h_[ i ][ j ] ) g1h_[ i ][ j ]->clearCache();
    
  }
}

bool A1hFit::postFit( Exp::DataSet& ds ){

  if( ds.size() == 0 ) return true;
  
  // check if it is A1 or A1h
  string& dInfo = ds[ 0 ].info(); // data type
  bool dis   = A1Data::isDIS( dInfo );
  bool sidis = A1Data::isSIDIS( dInfo );
  
  // if neigher A1 or A1h, ignore
  if( dis == false && sidis == false ) return false;
  int it = A1Data::nType( dis, dInfo );
  int ih = A1Data::hType( dis, dInfo );
  
  if( sidis && ( g1h_[ it ][ ih ] == NULL || f1h_[ it ][ ih ] == NULL ) )
    return false;
  
  for( int id = 0; id < ds.size(); id++ ){
    DataPoint& d = ds[ id ];
    double x  = d.kine( "x" );
    double q2 = d.kine( "Q^2" );
    
    if( q2 < q2limit_ ) continue;
    
    if( dis ){ // for A1
      g1_[ it ]->setQ2( q2 );
      f1_[ it ]->setQ2( q2 );
      
      double tmpvalue = (*g1x_[ it ])( x ) / (*f1x_[ it ])( x );
      
      if( Arguments::ref().hasOpt( "scale2" ) ){
	d.scale2() = tmpvalue; 
      } else if( Arguments::ref().hasOpt( "scale1" ) ){
	d.scale1() = tmpvalue; 
      } else {
	d.model() = tmpvalue; 
      }
      
    } else { // for A1h
      double z  = d.kine( "z" );
      vector< double > xx( 1, x ), zz( zrange_ );
      
      g1h_[ it ][ ih ]->setQ2( q2 );
      f1h_[ it ][ ih ]->setQ2( q2 );
      
      double tmpvalue =
	( useXXsec_ ?
	  (*g1hxx_[ it ][ ih ])( xx, zz ) / (*f1hxx_[ it ][ ih ])( xx, zz ) :
	  (*g1hx_[ it ][ ih ])( xx, zz ) / (*f1hx_[ it ][ ih ])( xx, zz ) );
      
      if( Arguments::ref().hasOpt( "scale2" ) ){
	d.scale2() = tmpvalue; 
      } else if( Arguments::ref().hasOpt( "scale1" ) ){
	d.scale1() = tmpvalue; 
      } else {
	  d.model() = tmpvalue; 
      }
      
    }
    
    //      if( q2 > q2limit_ ) d.fitted() = true;
    
  }

  return true;
};

bool A1hFit::postErrorMatrix( Exp::DataSet& ds ){
  
  if( ds.size() == 0 ) return true;
  
  // check if it is A1 or A1h
  string& dInfo = ds[ 0 ].info(); // data type
  bool dis   = A1Data::isDIS( dInfo );
  bool sidis = A1Data::isSIDIS( dInfo );
  
  // if neigher A1 or A1h, ignore... 
  if( dis == false && sidis == false ) return false;
  int it = A1Data::nType( dis, dInfo );
  int ih = A1Data::hType( dis, dInfo );
  
  if( sidis && ( g1h_[ it ][ ih ] == NULL || f1h_[ it ][ ih ] == NULL ) )
    return false;
  
  for( int id = 0; id < ds.size(); id++ ){
    DataPoint& d = ds[ id ];
    double x  = d.kine( "x" );
    double q2 = d.kine( "Q^2" );
    
    if( q2 < q2limit_ ) continue;
    
    if( dis ){ // for A1
      ErrorCalc::ErrCalc eCalc( g1x_[ it ] );
      // for normalization
      setNormMp( &eCalc, &ds );
      
      g1_[ it ]->setQ2( q2 );
      f1_[ it ]->setQ2( q2 );
      
      d.modelError() = sqrt( eCalc( x ) ) / (*f1x_[ it ])( x );
    } else { // for A1h
      
      g1h_[ it ][ ih ]->setQ2( q2 );
      f1h_[ it ][ ih ]->setQ2( q2 );
      
      ErrorCalc::ErrCalc eCalc( g1hx_[ it ][ ih ] );
      // for normalization
      setNormMp( &eCalc, &ds );
      double z  = d.kine( "z" );
      vector< double > xx( 1, x ), zz( zrange_ );
      d.modelError() = sqrt( eCalc( xx, zz ) ) /
	(*f1hx_[ it ][ ih ])( xx, zz );
    }
  }

  return true;
}

void A1hFit::zrange( const double& zmin, const double& zmax ){
  if( zmin < zmax ) {
    zrange_[ 0 ] = zmax;
    zrange_[ 1 ] = zmin;
  } else {
    zrange_[ 0 ] = zmin;
    zrange_[ 1 ] = zmax;
  }
}


string A1hFit::ofilename( const int& index ){
  
  ostringstream  ostr;
  
  Evolution::Kernel *kernel = this->kernels()[ index ];
  Fragmentation::Evo *ff = dynamic_cast< Fragmentation::Evo* >( kernel );
  polPDF::Evo *ppdf      = dynamic_cast< polPDF::Evo* >( kernel );
  unpolPDF::Evo *updf    = dynamic_cast< unpolPDF::Evo* >( kernel );
  
  ostr << "A1h_";
  
  if( ppdf ) ostr << "pPDF_" ;
  if( updf ) ostr << "uPDF_" ;
  if( ff ){
    if( ff->hadron() == QCD::Flavor::PIp ){
      ostr << "FF_";
    } else if( ff->hadron() == QCD::Flavor::KAp ){
      ostr << "FFK_";
    } else if( ff->hadron() == QCD::Flavor::REp ){
      ostr << "FFR_";
    } else {
      ostr << "FFX_";
    }
  }
  
  ostr << ( kernel->alpha().scheme().order() == Scheme::NLO ? "NLO_" : "LO_" ) ;
  ostr << zrange_[0] << "-" << zrange_[1];
  if( Arguments::ref().hasOpt( "rescaleFF" ) ) 
    ostr << "_FFr" << Arguments::ref().get( "rescaleFF", 1.0 );
  if( Arguments::ref().hasOpt( "rescaleUPDF" ) ) 
    ostr << "_UPDFr" << Arguments::ref().get( "rescaleUPDF", 1.0 );
  if( Arguments::ref().hasOpt( "rescalePPDF" ) ) 
    ostr << "_PPDFr" << Arguments::ref().get( "rescalePPDF", 1.0 );
  if( this->emflag() ) ostr << "_werr";
  ostr << ".xml";
  
  return ostr.str();
}


void A1hFit::data_summary( std::ostream& os ) {
  
  os 
    << "*******************  Fitting condition *********************\n"
    << "  A1h z max:     " << setw(5) <<  zrange_[ 0 ] << endl
    << "  A1h z min:     " << setw(5) <<  zrange_[ 1 ] << endl
    << endl;
  
  os << "********************  Data listing  ***********************" << endl;
  if( useFData_ ){
    
    os << A1Data::header() << endl;
    for( int i = 0; i < fData_.size(); i++ ){
      os << dynamic_cast< A1Data& >( *( fData_[ i ] ) ) << endl;
    }

  } else {

    os << setw( 16 ) << "x"
       << setw( 10 ) << "Q2"
       << setw( 10 ) << "z"
       << setw( 12 ) << "sigma"
       << setw( 12 ) << "dsigma"
       << setw( 12 ) << "fit"
       << setw( 3 )  << "id"
       << setw( 7 )  << "dsetid"
       << setw( 12 ) << "chi2"
       << setw( 22 ) << "DataSetInfo"
      
       << endl;
    
    for( int i = 0; i < sig_.size(); i++ ){
      os 
	<< setw( 16 ) << x_[ i ]
	<< setw( 10 ) << q2_[ i ]
	<< setw( 16 ) << z_[ i ]
	<< setw( 12 ) << sig_[ i ]
	<< setw( 12 ) << dsig_[ i ]
	<< setw( 12 ) << fit_[ i ]
	<< setw( 3 )  << type_[ i ]
	<< setw( 7 )  << dsettype_[ i ]
	<< setw( 12 ) << c2_[ i ]
	<< setw( 22 ) << expInfo_[ i ]
	<< endl;
    }
  }
}


void A1hFit::g1x_path( const double& x, Xsection::xXsec* xsec ){
  if( x < 0.01 )      xsec->upper( 15.0 );
  else if( x < 0.1  ) xsec->upper(  8.0 );
  else                xsec->upper(  6.5 );
}

void A1hFit::g1hx_path( const double& x, Xsection::xXsec* xsec ){
  vector< double > length = xsec->length();
  if( x < 0.01 ){         length[0] = 12.0; xsec->upper( length );
  } else if( x < 0.1  ) { length[0] =  8.0; xsec->upper( length );
  } else {                length[0] =  5.0; xsec->upper( length );
  }
}

bool A1hFit::condition(){
  // not called in case with --NoCondition option

  // canceled out JUST TEMPORARILY!! 
  /*
  // from MultiFragFit.cc
  if( hadron_ ){
    hadron_->setN( 2.0 );
    hadron_->setQ2( hadron_->initialScale() );
    for( int i = -6; i < 7; i++ )
      if( abs( hadron_->q( Flavor::PARTON( i ) ) ) >= 0.5 ) 
        return false;
  }
  */

  for( int i = -6; i < 7; i++ ){

    char chr[2];
    sprintf( chr, "%d", i );
    string tmpstr = "NoCond" + string( chr );
    if( Arguments::ref().hasOpt( tmpstr ) ) continue;

    ppdf_->setQ2( ppdf_->initialScale() );
    updf_->setQ2( ppdf_->initialScale() );
    
    xpolpdf_->pdf().getPID() = QCD::Flavor::PARTON( i );
    xunpolpdf_->pdf().getPID() = QCD::Flavor::PARTON( i );
    
    //      for( double x_ = 0.3; x_ < 0.99; x_ += 0.1 ){
    for( double x_ = 0.4; x_ < 0.79; x_ += 0.1 ){
      double xpol = abs( (*xpolpdf_)( x_ ) );
      double xunpol = abs( (*xunpolpdf_)( x_ ) );
      
      //	if( xpol < 0.001 ) continue;
      if( xpol < 0.01 ) continue;
      //	if( ( xpol - xunpol ) / xpol > 0.05 ){
      if( ( xpol - xunpol ) / xpol > 0.10 ){
	//	  /*
	cout << i << " : " << x_ << " : " 
	     << xpol << " < " << xunpol << endl; 
	//	  */
	return false;
      }
    }
    
  }
  
  return true;
  
}
