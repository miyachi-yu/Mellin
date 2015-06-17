#include "MultiFragFit.hh"
#include "FragData.hh"
#include "MultiData.hh"

#include <Tranform/GridIntegration.hh>
#include <Tranform/LegQuadrature.hh>
#include <ExpData/DataSet.hh>
#include <Xsec/Xsec.hh>
#include <Xsec/XsecComp.hh>
#include <Xsec/CKernel.hh>
#include <ErrorCalc/ErrCalc.hh>
#include <ErrorCalc/XsecCorrCalc.hh>

#include <cmath>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;
using namespace QCD;
using namespace Utility;
using namespace Exp;
using namespace Fit;

MultiFragFit::MultiFragFit( Arguments& args, const bool& judge ) throw( int ) : 
  Fit::Fitting( args, judge ),
  xsec_( 0 ), f_( 0 ),
  xmnsec_( 0 ), fmn_( 0 ), xfmn_( 0 ), 
  xmnsec0_( 0 ), fmn0_( 0 ), xfmn0_( 0 ), 
  xmnsec1_( 0 ), fmn1_( 0 ), xfmn1_( 0 ), 
  xmnsec2_( 0 ), fmn2_( 0 ), xfmn2_( 0 ), 
  xmdsec_( 0 ), fmd_( 0 ),
  vz_( 0 ), z_( 0 ), dz_( 0 ), type_( 0 ), 
  zrange_( 2, 0.0 ), vzran_( ),
  vx_( 0 ), 
  dsettype_( 0 ), frag_( 0 ),
  xave_( 0 ), zave_( 0 ), s_( 0 ),
  denom_( 0 ),
  useXXsec_( args.hasOpt( "xXsec" ) ),
  useAverage_( false ),
  fragData_( 0 ), multData_( 0 ), xXsecLength_( 3 ),
  useXsecSub_( args.hasOpt( "xsecSub" ) ),
  xx_( 0 ), zz_( 0 ),
  fragc2w_( 0 ), multc2w_( 0 ),
  post_conf_judge_( false )
{

  useFData_ = true; // force to use MultiData object
  
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

MultiFragFit::~MultiFragFit() {

  if( post_conf_judge_ ){
    this->post_delete_process();
  }
}

void MultiFragFit::post_delete_process() {  

  this->del( fmn_ );
  this->del( fmn0_ );
  this->del( fmn1_ );
  this->del( fmn2_ );
  
  this->del( xfmn_ );
  this->del( xfmn0_ );
  this->del( xfmn1_ );
  this->del( xfmn2_ );
  
  this->del( fmd_ );
  this->del( f_ );
  
  this->del( xmnsec_ );
  this->del( xmnsec0_ );
  this->del( xmnsec1_ );
  this->del( xmnsec2_ );
  
  this->del( xmdsec_ );
  this->del( xsec_ );
  
}

void MultiFragFit::post_config_process() throw( int ){

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

  // ------------------------------------------------------------
  //   check if calculation should be done at averaged kinematics or not
  // ------------------------------------------------------------
  if( useAverage_ = Arguments::ref().hasOpt( "Ave" ) ){
    ARG_LOG << "--Ave option is give."
	    << " The calculation will be done at averaged kinematics."
	    << endl;
  }

  // check lower Q2 limit
  q2limit_ = Arguments::ref().get( "q2limit", 1.0 );
  
  // ------------------------------------------------------------
  //   Xsec(SIA) defalut z range
  // ------------------------------------------------------------
  this->zrange( args.get( "zmin", 0.05 ), args.get( "zmax", 0.80 ) );
  
  ARG_LOG << "zragne to be fitted (SIA): " 
	  << setw(6) << zrange_[ 0 ] << " - " << setw(6) << zrange_[ 1 ] 
	  << endl; 
  
  this->config_vzrange();

  this->config_c2weight();

  // ------------------------------------------------------------
  //   Xsec(SIDIS) defalut z range
  // ------------------------------------------------------------
  ARG_LOG << "zragne to be fitted (Multi): " 
	  << setw(6) << Arguments::ref().get( "MultiZlimitL", 0.0 ) 
	  << " - " 
	  << setw(6) << Arguments::ref().get( "MultiZlimit", 0.6 )
	  << endl; 
  
  // ------------------------------------------------------------
  //   load experimental data
  // ------------------------------------------------------------
  fragData_.resize( FragData::nType( "ALL" ), 
		    vector< bool >( FragData::hType( "ALL" ), false ) );
  
  multData_.resize( MultiData::nType( "ALL" ), 
		    vector< bool >( MultiData::hType( "ALL" ), false ) );
  
  dynamic_cast< Fitting* >( this )->loadData();  
  
  // --------  check if there is unpolarized PDF object -------- //
#ifdef DEBUG_FITTING
  ARG_LOG << "# -----------------------------------------------" << endl;
  ARG_LOG << "#     check if unpolPDF exists or not            " << endl;
  ARG_LOG << "# -----------------------------------------------" << endl;
#endif // DEBUG_FITTING
  if( updf_ == NULL ){
    ARG_ERR << "# " << args.command() << endl;
    ARG_ERR << "#  requires unpolPDF initial distribuion." << endl;
    ARG_ERR << "#  please specify with one of following options." << endl;
   vector< string > opts = unpolPDF::Evo::options();
   for( int i = 0; i < opts.size(); i++ ){
     ARG_ERR << "#\t--" << opts[ i ] << "=[xml file]" << endl; 
   }
   ARG_LOG << "#  running in the fragXsecFit mode....." << endl;
   //   throw( 1 );
  } else {
    updf_->constNf( false );
  }
  
  // ------------------------------------------------------------
  //   check ff object 
  // ------------------------------------------------------------
#ifdef DEBUG_FITTING
  ARG_LOG << "# -----------------------------------------------" << endl;
  ARG_LOG << "#     check if FF objects exist or not            " << endl;
  ARG_LOG << "# -----------------------------------------------" << endl;
#endif // DEBUG_FITTING
  if( pion_ == NULL && kaon_ == NULL && proton_ == NULL && rest_ == NULL ){
    ARG_ERR << "# " << args.command() << endl;
    ARG_ERR << "# requires fragmentation functions at least for "
	    << "pion, kaon, proton, or rest." << flush;
    throw( 2 );
  } else {
    if( pion_ )   pion_->constNf( false );
    if( kaon_ )   kaon_->constNf( false );
    if( proton_ ) proton_->constNf( false );
    if( rest_ )   rest_->constNf( false );
    if( hadron_ ) hadron_->constNf( false );
  }
  
  // ------------------------------------------------------------
  //   prepare xsection model and Mellin inversion
  // ------------------------------------------------------------
#ifdef DEBUG_FITTING
  ARG_LOG << "# -----------------------------------------------" << endl;
  ARG_LOG << "#      Prepare xsection objects                  " << endl;
  ARG_LOG << "# -----------------------------------------------" << endl;
#endif // DEBUG_FITTING
  try {
    this->setup_FragXsec();
    this->setup_Multiplicity();
  }
  
  catch( int e ) {
    cerr << __FILE__ << ":" << __LINE__
	 << "\tcatch error (" << e << ")" << endl;
    throw( e );
  }
  
  // ------------------------------------------------------------
  //   reset multiplicity Mellin inversion length (obsoleated...)
  // ------------------------------------------------------------
#ifdef DEBUG_FITTING
  ARG_LOG << "# -----------------------------------------------" << endl;
  ARG_LOG << "#      Mellin inversion configuration            " << endl;
  ARG_LOG << "# -----------------------------------------------" << endl;
#endif // DEBUG_FITTING
  xXsecLength_[ 0 ] = 6.5;
  xXsecLength_[ 1 ] = 8.0;
  xXsecLength_[ 2 ] = 9.5;
  
  xXsecLength_ = args.get( "xXsecLength", ",", xXsecLength_ );

  // ------------------------------------------------------------
  //   calculate denominator for multiplicity
  // ------------------------------------------------------------
  this->calc_denominator();

  post_conf_judge_ = true; 
  // flag for post config process execution 
  // (for duplicate check & delete process)
  
}

void MultiFragFit::calc_denominator() {
  
  static int ip0 = MultiData::hType( "P0" );
  static int ipi = MultiData::hType( "PI" );
  
#ifdef DEBUG_FITTING
  ARG_LOG << endl;
  ARG_LOG << "# -----------------------------------------------" << endl;
  ARG_LOG << "#     calculate denominator for Multiplicity     " << endl;
  ARG_LOG << "# -----------------------------------------------" << endl;
#endif // DEBUG_FITTING
  
  vector< double > xx( useAverage_ ? 1 : 2 );
  
  for( int i = 0; i < fData_.size(); i++ ){
    
    MultiData *data = dynamic_cast< MultiData* >( fData_[ i ] );
    if( data == NULL ){
      ARG_DETAIL << "skip fragXsec data" << endl;
      continue; // skip fragXsec data
    }
    
    int& ih   = data->ih_;
    int& it   = data->it_;
    
    if( ih == ip0 ){
      ARG_WARN << "pi0 multiplicity is not yet implemented." << endl;
      continue;
    }
    
    if( xmdsec_[ it ] == NULL || fmd_[ it ] == NULL ) {
#ifdef DEBUG_FITTING
      ARG_DETAIL
	<< setw( 24 ) << data->expInfo_ 
	<< "   ---- NOT Available ----" << endl;
#endif // DEBUG_FITTING
      continue;
    }
    
    xmdsec_[ it ]->setS( data->s_ );      // set s  value
    xmdsec_[ it ]->setQ2( data->q2_ );    // set Q2 value
    
    if( useAverage_ ){
      xx[ 0 ] = data->xave_;
    } else {
      xx = data->vx_;
    }    
    
#ifdef DEBUG_FITTING
    ARG_DETAIL << setw( 35 ) << data->expInfo_ << flush;
    ARG_DETAIL << setw(6) << xx[ 0 ] << flush;
    ARG_DETAIL << setw(6) << ( useAverage_ ? 0.0 : xx[ 1 ] ) << flush;
    ARG_DETAIL << setw(6) << data->q2_ << flush;
#endif // DEBUG_FITTING
    
    data->denom_ = (*fmd_[ it ])( xx );   // calculate denominator
    fmd_[ it ]->moment()->clearCache();   // save memory for fitting
    
#ifdef DEBUG_FITTING
    ARG_DETAIL << setw( 12 ) << data->denom_ << endl;
#endif // DEBUG_FITTING
    
  }
  
#ifdef DEBUG_FITTING
  ARG_LOG << "# -----------------------------------------------" << endl;
  ARG_LOG << "#       end of denominator calculation           " << endl;
  ARG_LOG << "# -----------------------------------------------" << endl;
#endif // DEBUG_FITTING
  
};

bool MultiFragFit::calc( FitData* fd ){
  
  static int ip0 = FragData::hType( "P0" );
  static int ipi = FragData::hType( "PI" );
  
  static int ip0mul = MultiData::hType( "P0" );
  
  if( fd == NULL ) return false;
  int& it = fd->it_;
  int  ih = fd->ih_;   
  
  // xsection type
  FragData *fragD = dynamic_cast< FragData* >( fd );
  if( fragD ){
    
    bool fpi0;
    if( ( fpi0 = ( ih == ip0 ) ) == true ){
      ih  = ipi;
    };
    
    if( xsec_[ it ][ ih ] == NULL ) return false;
    
    // set Q2 value			    
    xsec_[ it ][ ih ]->setQ2( fragD->q2_ );
    
    // do not forget about factor 2 for the cross section 
    // to account for both charged hadrons
    // for c or b enriched sample there is additional factor 2
    fragD->fit_ = ( it == 0 ? 2.0 : 4.0 ) * (*f_[ it ][ ih ])( fragD->vz_ );
    
    // additional factor 0.5 for neutral pion
    if( fpi0 ) fragD->fit_ *= 0.5;
    
    return true;
  }
  
  MultiData *multD = dynamic_cast< MultiData* >( fd );
  if( multD ){
    
    if( ih == ip0mul ){
      ARG_ERR << "neutral pion is not yet implemented." << endl;
      return false;
    }
    
    if( xmnsec_[ it ][ ih ] == NULL ) return false;
    
    // set x and z
    if( useAverage_ ){
      xx_.resize( 1 ); xx_[ 0 ] = multD->xave_;
      zz_.resize( 1 ); zz_[ 0 ] = multD->zave_;
    } else {
      xx_ = multD->vx_;
      zz_ = multD->vz_; 
    }
    
    multD->fit_ = 
      this->sigma_h( it, ih, multD->q2_, multD->s_, xx_, zz_ ) / 
      multD->denom_;
    
    return true;
  }
  
  ARG_ERR << "Non FragData or MultData object was given. Check." << endl;
  return false;
}

bool MultiFragFit::calc( const int& i ){
  ARG_ERR << __FILE__ << " is desinged for using FitData object." << endl;
  ARG_ERR << "MultiFrafFit::calc( const int& i ) should not be called." << endl;
  return true;
}

void MultiFragFit::postLoop() {
  
#ifdef DEBUG_FITTING
  
  if( updf_ && updf_->dglapMatrix().cache_size() > 0  ){
    
    ARG_DETAIL
      << setw(6) << updf_->dglapMatrix().cache_size()
      << " F1h" << flush;    
    
    if( useXsecSub_ ){
      for( int i = 0; i < xmnsec0_.size(); i++ ){
	for( int j = 0; j < xmnsec0_[ i ].size(); j++ ){
	  if( xmnsec0_[ i ][ j ] ) {
	    ARG_DETAIL
	      << setw(6) << 
	      ( xmnsec0_[ i ][ j ]->cache2().size() +
		xmnsec1_[ i ][ j ]->cache2().size() +
		xmnsec2_[ i ][ j ]->cache2().size() ) / 3
	      << flush;
	  }
	}
      }
    } else {
      for( int i = 0; i < xmnsec_.size(); i++ ){
	for( int j = 0; j < xmnsec_[ i ].size(); j++ ){
	  if( xmnsec_[ i ][ j ] ) 
	    ARG_DETAIL 
	      << setw(6) << xmnsec_[ i ][ j ]->cache2().size()
	      << flush;
	}
      }
    }
  }
#endif // DEBUG_FITTING
  
}


void MultiFragFit::loadData( Exp::DataSet& ds ){
  
  string& dInfo = ds[ 0 ].info();
  
  bool frag = FragData::check( ds[ 0 ] );
  bool mult = MultiData::check( ds[ 0 ] );
  
  if( frag == false && mult == false ) return;
  
  int it = ( frag ? 
	     FragData::get_nType( dInfo ) : MultiData::get_nType( dInfo ) );
  
  int ih = ( frag ? 
	     FragData::get_hType( dInfo ) : MultiData::get_hType( dInfo ) );
  
#ifdef DEBUG_FITTING
  ARG_LOG 
    << "  INFO: " << setw(35)<< dInfo
    << setw(10) << ( frag ? "(FragXsec)" : "(Multi)" )
    << endl;
  
  ARG_LOG 
    << "  TYPE:"  << setw(3) << it 
    << "  HAD:"   << setw(3) << ih
    << "  NORM:"  << setw(12) << ds.norm()
    << "  NCOEF:" << setw(12) << ds.ncoef()
    << "  NCERR:" << setw(12) << ds.ncerr()
    << "  WEIGHT:" << setw(6) << ( ds.expdata() ? ds.expdata()->lambda() : 1.0 )
    << "  ( " << ds.ncstat() << ")"
    << endl;
#endif // DEBUG_FITTING
  
  if( frag ){
    fragData_[ it ][ ih ] = true;
  } else {
    multData_[ it ][ ih ] = true;
  }
  
  for( int id = 0; id < ds.size(); id++ ){
    
    DataPoint& d = ds[ id ];
    
    // ----------------------   Z     ----------------------- 
    double z1 = d.kine( "zmin" );
    double z2 = d.kine( "zmax" );
    
    int ih2 = ( frag && ih == FragData::hType( "P0" ) ?
		MultiData::hType( "P0" ) : ih );
    
    if( z1 < vzran_[ ih2 ][ 0 ] || z2 > vzran_[ ih2 ][ 1 ] ) continue;
    if( !frag && z2 > Arguments::ref().get( "MultiZlimit", 0.6 ) ) continue;
    if( !frag && z1 < Arguments::ref().get( "MultiZlimitL", 0.0 ) ) continue;
    
    
    FitData *data;
    
    if( frag ) data = new FragData( ds, d );
    else data = new MultiData( ds, d );
  
    // ignore data below Q2 threshold
    if( data->q2_ <= q2limit_ ) { 
      delete data;
      continue;
    }
    
    // store dat for fitting
    fData_.push_back( data );
    
    if( frag && fragc2w_.size() > 0 ){
      fData_.back()->c2weight_ = fragc2w_[ ih ];
    } else if( ! frag && multc2w_.size() > 0 ) {
      fData_.back()->c2weight_ = multc2w_[ ih ] ;
    } 
    
#ifdef DEBUG_FITTING
    if( frag )  ARG_DETAIL << dynamic_cast< FragData& >( *( fData_.back() ) )  << endl;
    else        ARG_DETAIL << dynamic_cast< MultiData& >( *( fData_.back() ) ) << endl;
#endif 
    
  }

}

void MultiFragFit::postUpdate() {
  
  // if none of FF or PDF are fitted, do nothing 
  if( this->kernels().size() == 0 ) return;
  
  for( int i = 0; i < xsec_.size(); i++ ) 
    for( int j = 0; j < xsec_[ i ].size(); j++ ) 
      if( xsec_[ i ][ j ] != NULL ) xsec_[ i ][ j ]->clearCache();
  
  for( int i = 0; i < xmnsec_.size(); i++ ) 
    for( int j = 0; j < xmnsec_[ i ].size(); j++ ) {
      if( xmnsec_[ i ][ j ]  ) xmnsec_[ i ][ j ]->clearCache();
      if( xmnsec0_[ i ][ j ] ) xmnsec0_[ i ][ j ]->clearCache();
      if( xmnsec1_[ i ][ j ] ) xmnsec1_[ i ][ j ]->clearCache();
      if( xmnsec2_[ i ][ j ] ) xmnsec2_[ i ][ j ]->clearCache();
    }
}

bool MultiFragFit::postFit( Exp::DataSet& ds ){
  
  if( ds.size() == 0 ) return true; // ignore empty set
  
  vector< double > xx( useAverage_ ? 1 : 2 );
  vector< double > zz( useAverage_ ? 1 : 2 );
  
  string& dInfo = ds[ 0 ].info();
  
  bool frag = FragData::check( ds[ 0 ] );
  bool mult = MultiData::check( ds[ 0 ] );
  
  if( frag == false && mult == false ) return true;
  
  int htyp = ( frag ? FragData::get_hType( dInfo ) : MultiData::get_hType( dInfo ) );
  int ih = ( frag && htyp == FragData::hType( "P0" ) ? FragData::hType( "PIp" ) : htyp );
  int it = ( frag ? FragData::get_nType( dInfo ) : MultiData::get_nType( dInfo ) );
  
  for( int id = 0; id < ds.size(); id++ ){
    
    DataPoint& d = ds[ id ];
    
    if( frag ) {
      
      if( xsec_[ it ][ ih ] == NULL ) continue;
      
      xsec_[ it ][ ih ]->setQ2( d.kine("Q^2") );
      
      // set z
      vector< double > fragZ( 2 );
      fragZ[ 0 ] = d.kine("zmin");
      fragZ[ 1 ] = d.kine("zmax");
      
      if( fragZ[ 0 ] == fragZ[ 1 ] ) fragZ.resize( 1 );
      
      // it is impossible to calculate for the case of z[ 0 ] == 0 
      if( fragZ[ 0 ] < 0.001 ) continue;
      
      double tmpvalue = ( htyp == FragData::hType( "P0" ) ? 0.5 : 1.0 ) *
	( it == 0 ? 2.0 : 4.0 ) * (*f_[ it ][ ih ])( fragZ );
      
      if( Arguments::ref().hasOpt( "scale2" ) ){
	d.scale2() = tmpvalue; 
      } else if( Arguments::ref().hasOpt( "scale1" ) ){
	d.scale1() = tmpvalue; 
      } else {
	d.model() = tmpvalue; 
      }
      
#ifdef DEBUG_FITTING
      ARG_DETAIL 
	<< "* " 
	<< setw(34) << ds.info() 
	<< setw(10) << d.kine("Q^2")
	<< setw(10) << fragZ[ 0 ]
	<< setw(10) << fragZ[ ( fragZ.size() > 1 ? 1 : 0 ) ]
	<< setw(20) << ""
	<< setw(16) << d.model()
	<< endl;
#endif // DEBUG_FITTING
      
    } else {
      
      if( htyp == MultiData::hType( "P0" ) ){
	ARG_ERR << "neutral pion is not yet implemented" << endl;
	continue;
      }
      
      if( xmnsec_[ it ][ ih ] == NULL ||
	  xmdsec_[ it ] == NULL ) continue;
      
      double qq2 = 0.0;
      
      if( ( ! useAverage_ ) && d.hasKine( "Q^2 max" ) && d.hasKine( "Q^2 min" ) ) {
	qq2 = 0.5 * ( d.kine( "Q^2 max" ) + d.kine( "Q^2 min" ) );
      } else {
	qq2 = ( d.hasKine( "Q^2 ave" ) ? d.kine( "Q^2 ave" ) : d.kine( "Q2" ) );
      }
      
      double s = d.kine( "s" );
      xmdsec_[ it ]->setS( s );
      xmdsec_[ it ]->setQ2( qq2 );
      
      // set x and z
      if( useAverage_ ){
	xx[ 0 ] = d.kine( "xave" );
	zz[ 0 ] = d.kine( "zave" );
      } else {
	
	xx[ 0 ] = d.hasKine( "xave" ) ? d.kine( "xave" ) : d.kine( "xmax" );
	xx[ 1 ] = d.hasKine( "xave" ) ? d.kine( "xave" ) : d.kine( "xmin" );
	zz[ 0 ] = d.kine( "zmax" ); 
	zz[ 1 ] = d.kine( "zmin" );
	
	if( xx[ 0 ] == xx[ 1 ] ) xx.resize( 1 );
	if( zz[ 0 ] == zz[ 1 ] ) zz.resize( 1 );

      }
      
      if( useXXsec_ ) 
	this->set_contour( d.kine( "x" ), xfmn_[ it ][ ih ] );
      
      double tmpvalue =
	this->sigma_h( it, ih, qq2, s, xx, zz ) / (*fmd_[ it ])( xx );
      
      if( Arguments::ref().hasOpt( "scale2" ) ){
	d.scale2() = tmpvalue; 
      } else if( Arguments::ref().hasOpt( "scale1" ) ){
	d.scale1() = tmpvalue; 
      } else {
	d.model() = tmpvalue; 
      }
      
#ifdef DEBUG_FITTING
      ARG_DETAIL
	<< "* " 
	<< setw(34) << ds.info() 
	<< setw(10) << qq2
	<< setw(10) << xx[ 0 ]
	<< setw(10) << xx[ ( xx.size() > 1 ? 1 : 0 ) ]
	<< setw(10) << zz[ 0 ]
	<< setw(10) << zz[ ( xx.size() > 1 ? 1 : 0 ) ]
	<< setw(16) << d.model()
	<< endl;
#endif // DEBUG_FITTING
      
    }
    
  }
  
  return true;
};

bool MultiFragFit::postErrorMatrix( Exp::DataSet& ds ){
  
  if( ds.size() == 0 ) return true; // ignore empty set

  vector< double > xx( useAverage_ ? 1 : 2 );
  vector< double > zz( useAverage_ ? 1 : 2 );
  
  string& dInfo = ds[ 0 ].info();
  
  bool frag = FragData::check( ds[ 0 ] );
  bool mult = MultiData::check( ds[ 0 ] );
  
  if( frag == false && mult == false ) return true;
  
  int htyp = ( frag ? FragData::get_hType( dInfo ) : MultiData::get_hType( dInfo ) );
  int ih = ( frag && htyp == FragData::hType( "P0" ) ? FragData::hType( "PIp" ) : htyp );
  int it = ( frag ? FragData::get_nType( dInfo ) : MultiData::get_nType( dInfo ) );
  
  for( int id = 0; id < ds.size(); id++ ){
    DataPoint& d = ds[id];
    
    if( frag ){
      
      if( xsec_[ it ][ ih ] == NULL ) continue;
      
      xsec_[ it ][ ih ]->setQ2( d.kine("Q^2") );
      
      // set z
      vector< double > fragZ( 2 );
      fragZ[ 0 ] = d.kine("zmin");
      fragZ[ 1 ] = d.kine("zmax");
      
      if( fragZ[ 0 ] == fragZ[ 1 ] ) fragZ.resize( 1 );
      
      // it is impossible to calculate for the case of z[ 0 ] == 0 
      if( fragZ[ 0 ] < 0.001 ) continue;
      
      ErrorCalc::ErrCalc eCalc( f_[ it ][ ih ] );
      // for normalization
      this->setNormMp( &eCalc, &ds );
      
      d.modelError() = 
	( htyp == FragData::hType( "P0" ) ? 0.5 : 1.0 ) *
	( it == 0 ? 2.0 : 4.0 ) * sqrt( eCalc( fragZ ) );
      
#ifdef DEBUG_FITTING
      ARG_DETAIL
	<< "* "
	<< setw(34) << ds.info() 
	<< setw(12) << d.model()
	<< " +- "
	<< setw(12) << d.modelError()
	<< endl;
#endif // DEBUG_FITTING
      
    } else {
      
      if( htyp == MultiData::hType( "P0" ) ){
	ARG_ERR << "neutral pion is not yet implemented" << endl;
	continue;
      }
      
      if( xmnsec_[ it ][ ih ] == NULL ||
	  xmdsec_[ it ] == NULL ) continue;
      
      double qq2 = 0.0;
      
      if( ( ! useAverage_ ) && d.hasKine( "Q^2 max" ) && d.hasKine( "Q^2 min" ) ) {
	qq2 = 0.5 * ( d.kine( "Q^2 max" ) + d.kine( "Q^2 min" ) );
      } else {
	qq2 = ( d.hasKine( "Q^2 ave" ) ? d.kine( "Q^2 ave" ) : d.kine( "Q2" ) );
      }

      double s = d.kine( "s" );
      xmdsec_[ it ]->setS( s );
      xmdsec_[ it ]->setQ2( qq2 );
      
      // set x and z
      if( useAverage_ ){
	xx[ 0 ] = d.kine( "xave" );
	zz[ 0 ] = d.kine( "zave" );
      } else {

	xx[ 0 ] = d.hasKine( "xave" ) ? d.kine( "xave" ) : d.kine( "xmax" );
	xx[ 1 ] = d.hasKine( "xave" ) ? d.kine( "xave" ) : d.kine( "xmin" );
	zz[ 0 ] = d.kine( "zmax" ); 
	zz[ 1 ] = d.kine( "zmin" );

	if( xx[ 0 ] == xx[ 1 ] ) xx.resize( 1 );
	if( zz[ 0 ] == zz[ 1 ] ) zz.resize( 1 );

      }
      
      d.modelError() = 
	this->dsigma_h( it, ih, qq2, s, xx, zz, &ds ) / (*fmd_[ it ])( xx );
      
#ifdef DEBUG_FITTING
      ARG_DETAIL 
	<< "* " << setw(34) << ds.info() 
	<< setw(12) << d.model()
	<< " +- "
	<< setw(12) << d.modelError()
	<< endl;
#endif // DEBUG_FITTING
      
    }
    
  }
  
  return true;
}

void MultiFragFit::zrange( const double& zmin, const double& zmax ){
  if( zmin < zmax ) {
    zrange_[0] = zmin;
    zrange_[1] = zmax;
  } else {
    zrange_[1] = zmin;
    zrange_[0] = zmax;
  }
}

string MultiFragFit::ofilename( const int& index ){
  
  ostringstream  ostr;
  
  Evolution::Kernel *kernel = this->kernels()[ index ];
  Fragmentation::Evo *ff    = dynamic_cast< Fragmentation::Evo* >( kernel );
  polPDF::Evo *ppdf         = dynamic_cast< polPDF::Evo* >( kernel );
  unpolPDF::Evo *updf       = dynamic_cast< unpolPDF::Evo* >( kernel );
  
  ostr << "Multi_";
  
  if( ppdf ) ostr << "pPDF_" ;
  if( updf ) ostr << "uPDF_" ;
  if( ff ){
    if( ff->hadron() == QCD::Flavor::PIp ){
      ostr << "FF_";
    } else if( ff->hadron() == QCD::Flavor::KAp ){
      ostr << "FFK_";
    } else if( ff->hadron() == QCD::Flavor::PRp ){
      ostr << "FFP_";
    } else if( ff->hadron() == QCD::Flavor::REp ){
      ostr << "FFR_";
    } else {
      ostr << "FFX_";
    }
  }
  
  ostr << ( kernel->alpha().scheme().order() == Scheme::NLO ? "NLO_" : "LO_" ) ;
  
  if( ff ){
    if( ff->hadron() == QCD::Flavor::PIp || 
	ff->hadron() == QCD::Flavor::KAp || 
	ff->hadron() == QCD::Flavor::PRp || 
	ff->hadron() == QCD::Flavor::REp    ){
      ostr << vzran_[ MultiData::hType( ff->hadron() ) ][ 0 ]
	   << "-" 
	   << vzran_[ MultiData::hType( ff->hadron() ) ][ 1 ];
    } else {
      ostr << zrange_[ 0 ] << "-" << zrange_[ 1 ] ;
    }
  } else {
    
  }
  
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

void MultiFragFit::data_summary( std::ostream& os ) {
  
  os 
    << "*******************  Fitting condition *********************\n"
    << "  z range: "
    << setw(5) <<  zrange_[ 0 ] << " - "
    << setw(5) <<  zrange_[ 1 ]
    << endl;
  
  
  for( int i = 0; i < vzran_.size(); i++ ){
    os << "      " 
       << setw(3) << MultiData::hName( i )
       << " :     " 
       << setw(5) << vzran_[ i ][ 0 ] 
       << " - " 
       << setw(5) << vzran_[ i ][ 1 ] 
       << endl;
  }
  
  if( fragc2w_.size() > 0 ){
    os << "*************** chi2 weight ********************" << endl;
    os << "  fragXsec data:  pion " 
       << setw(12) << fragc2w_[ FragData::hType( QCD::Flavor::PIp ) ] 
       << endl;
    os << "  fragXsec data:  kaon " 
       << setw(12) << fragc2w_[ FragData::hType( QCD::Flavor::KAp ) ] 
       << endl;
    os << "  fragXsec data:  proton " 
       << setw(12) << fragc2w_[ FragData::hType( QCD::Flavor::PRp ) ] 
       << endl;
    os << "  fragXsec data:  hadron " 
       << setw(12) << fragc2w_[ FragData::hType( QCD::Flavor::HAp ) ] 
       << endl;
    os << "  fragXsec data:  pi0 " 
       << setw(12) << fragc2w_[ FragData::hType( QCD::Flavor::PI0 ) ] 
       << endl;
    
    os << "  Multiplicity data:  pion " 
       << setw(12) << multc2w_[ MultiData::hType( QCD::Flavor::PIp ) ] 
       << setw(12) << multc2w_[ MultiData::hType( QCD::Flavor::PIm ) ] 
       << endl;
    os << "  Multiplicity data:  kaon " 
       << setw(12) << multc2w_[ MultiData::hType( QCD::Flavor::KAp ) ] 
       << setw(12) << multc2w_[ MultiData::hType( QCD::Flavor::KAm ) ] 
       << endl;
    os << "  Multiplicity data:  proton " 
       << setw(12) << multc2w_[ MultiData::hType( QCD::Flavor::PRp ) ] 
       << setw(12) << multc2w_[ MultiData::hType( QCD::Flavor::PRm ) ] 
       << endl;
    os << "  Multiplicity data:  hadron " 
       << setw(12) << multc2w_[ MultiData::hType( QCD::Flavor::HAp ) ] 
       << setw(12) << multc2w_[ MultiData::hType( QCD::Flavor::HAm ) ] 
       << endl;
    os << "  Multiplicity data:  pi0 " 
       << setw(12) << multc2w_[ MultiData::hType( QCD::Flavor::PI0 ) ] 
       << endl;
    
  }
  
  os << "*************** Mellin Inversion condition *****************"
     << endl;
  
  if( useXXsec_ ){
    os << "   with Xsection::xXsec" << endl;
    
    if( useXsecSub_ ){
      
      int i, j;
      for( i = 0; i < xfmn0_.size(); i++ ){
	for( j = 0; j < xfmn0_[ i ].size(); j++ ){
	  
	  if( xfmn0_[ i ][ j ] == NULL ) continue;
	  
	  vector< double > off0 = xfmn0_[ i ][ j ]->offset();
	  vector< double > ang0 = xfmn0_[ i ][ j ]->angle();
	  vector< double > len0 = xfmn0_[ i ][ j ]->length();
	  
	  vector< double > off1 = xfmn1_[ i ][ j ]->offset();
	  vector< double > ang1 = xfmn1_[ i ][ j ]->angle();
	  vector< double > len1 = xfmn1_[ i ][ j ]->length();
	  
	  vector< double > off2 = xfmn1_[ i ][ j ]->offset();
	  vector< double > ang2 = xfmn1_[ i ][ j ]->angle();
	  vector< double > len2 = xfmn1_[ i ][ j ]->length();
	  
	  os << "it: " << setw(2) << i
	     << " ih: " << setw(2) << j
	     << endl;
	  
	  os << "       "
	     << setw(16) << "Sub0"
	     << setw(16) << "Sub1"
	     << setw(16) << "Sub2"
	     << endl;
	  
	  os << "offset:"
	     << setw(8) << off0[ 0 ] << setw(8) << off0[ 1 ] 
	     << setw(8) << off1[ 0 ] << setw(8) << off1[ 1 ] 
	     << setw(8) << off2[ 0 ] << setw(8) << off2[ 1 ] 
	     << endl;
	  
	  os << "angle: "
	     << setw(8) << ang0[ 0 ] << setw(8) << ang0[ 1 ] 
	     << setw(8) << ang1[ 0 ] << setw(8) << ang1[ 1 ] 
	     << setw(8) << ang2[ 0 ] << setw(8) << ang2[ 1 ] 
	     << endl;
	  
	  os << "length:"
	     << setw(8) << len0[ 0 ] << setw(8) << len0[ 1 ] 
	     << setw(8) << len1[ 0 ] << setw(8) << len1[ 1 ] 
	     << setw(8) << len2[ 0 ] << setw(8) << len2[ 1 ] 
	     << endl;
	}
      }
      
    } else {
      
      int i, j;
      for( i = 0; i < xfmn_.size(); i++ ){
	for( j = 0; j < xfmn_[ i ].size(); j++ ){
	  if( xfmn_[ i ][ j ] == NULL ) continue;

	  vector< double > off = xfmn_[ i ][ j ]->offset();
	  vector< double > ang = xfmn_[ i ][ j ]->angle();
	  vector< double > len = xfmn_[ i ][ j ]->length();
	  
	  os << "it: " << setw(2) << i
	     << " ih: " << setw(2) << j
	     << endl;
	  
	  os << "offset:"
	     << setw(8) << off[ 0 ] << setw(8) << off[ 1 ] 
	     << endl;
	  
	  os << "angle: "
	     << setw(8) << ang[ 0 ] << setw(8) << ang[ 1 ] 
	     << endl;
	  
	  os << "length:"
	     << setw(8) << len[ 0 ] << setw(8) << len[ 1 ] 
	     << endl;
	}
      }
      
    }
  } else {
  }
  
  bool frag = true;;
  for( int i = 0; i < fData_.size(); i++ ){
    
    FragData  *fragD = dynamic_cast< FragData* >( fData_[ i ] );
    MultiData *multD = dynamic_cast< MultiData* >( fData_[ i ] );
    
    if( i == 0 ){
      if( fragD ) os << FragData::header() << endl;
      if( multD ) os << MultiData::header() << endl;
    }
    
    if( ! fData_[ i ]->sigUsed_ ) continue;
    
    if( fragD ) { 
      if( frag == false ){
	os << FragData::header() << endl;
	frag = true;
      }
      os << *fragD<< endl;
    }
    if( multD ) {
      if( frag == true ){
	os << MultiData::header() << endl;
	frag = false;
      }
      os << *multD<< endl;
    }
  }

}

void MultiFragFit::setup_FragXsec( const QCD::Flavor::HADRON& h ) throw( int ){
  
  if( h == QCD::Flavor::PIm || h == QCD::Flavor::KAm ||
      h == QCD::Flavor::PRm ||
      h == QCD::Flavor::REm || h == QCD::Flavor::HAm ){
    // for FragXsec, negative hadron is equivalent to positive hadron.
    // ignore.... ( at the moment )
    return;
  }
  
  int ih = FragData::hType( h );
  
  Evolution::Kernel *ff = NULL;
  switch( h ){
  case QCD::Flavor::PIp :
  case QCD::Flavor::PIm :  ff = pion_; break; 
  case QCD::Flavor::KAp :
  case QCD::Flavor::KAm :  ff = kaon_; break; 
  case QCD::Flavor::PRp :
  case QCD::Flavor::PRm :  ff = proton_; break; 
  case QCD::Flavor::REp :
  case QCD::Flavor::REm :  ff = rest_; break; 
  case QCD::Flavor::HAp :
  case QCD::Flavor::HAm :  ff = hadron_; break; 
  }
  
  if( ff == NULL ) return;
  
  vector< FragXsec_T::FragXsec* >& xs = xsec_[ ih ];
  
  try {
    
    if( fragData_[ 0 ][ ih ] ) {
      xsec_[ 0 ][ ih ] = new FragXsec_T::FragXsec( ff, h );
    }
    
    if( fragData_[ 1 ][ ih ] ) {
      xsec_[ 1 ][ ih ] = new FragXsec_T::FragXsec( ff, h );
      xsec_[ 1 ][ ih ]->setIndiv( Flavor::c );  // C enriched sample
    }
    
    if( fragData_[ 2 ][ ih ] ) {
      xsec_[ 2 ][ ih ] = new FragXsec_T::FragXsec( ff, h );
      xsec_[ 2 ][ ih ]->setIndiv( Flavor::b );  // B enriched sample
    }
    
  }
  
  catch( int error ){
    ARG_ERR << "catch error (" << error << ")" << endl;
    throw( error );
  }
  
}

void MultiFragFit::setup_FragXsec() throw( int ){
  
  Utility::Arguments& args = Utility::Arguments::ref();
  
  FragXsec_T::FragXsec *null = NULL;
  
  // resize container object
  xsec_.resize( FragData::nType( "ALL" ),
		vector< FragXsec_T::FragXsec* >( FragData::hType( "ALL" ),
						 null ) );
  f_.resize( FragData::nType( "ALL" ),
	     vector< Xsection::xXsec* >( FragData::hType( "ALL" ),
					 ( Xsection::xXsec* ) NULL ) );
  
  try {
    
    this->setup_FragXsec( QCD::Flavor::PIp );
    this->setup_FragXsec( QCD::Flavor::KAp );
    this->setup_FragXsec( QCD::Flavor::PRp );
    this->setup_FragXsec( QCD::Flavor::REp );
    this->setup_FragXsec( QCD::Flavor::HAp );
    
    this->setup_FragXsec_inverse();
  }
  
  catch( int error ){
    ARG_ERR << "catch error (" << error << ")" << endl;
    throw( error );
  }
  
}


void MultiFragFit::setup_FragXsec_inverse() throw( int ) {
  
  Utility::Arguments& args = Utility::Arguments::ref();
  
  // -----------   Mellin inversion of FragXsec   ------------- //
  double offset = args.get( "offset", 3.0  );
  double length = args.get( "length", 8.0  );
  double angle  = args.get( "angle",  0.55 );
  
  int leg1    = args.get( "leg1", 6 );
  int leg2    = args.get( "leg2", 8 );
  double prec = args.get( "prec", 5.0E-3 );
  int ngrid   = args.get( "ngrid", 2 );
  
  try {
    for( int i = 0; i < xsec_.size(); i++ ){
      for( int j = 0; j < xsec_[ i ].size(); j++ ) {
	if( f_[ i ][ j ] != NULL ) continue;
	f_[ i ][ j ] = new Xsection::xXsec( xsec_[ i ][ j ], leg1, leg2, prec );
	f_[ i ][ j ]->setGngrid( ngrid );      
	f_[ i ][ j ]->setParameter( offset, angle );
	f_[ i ][ j ]->upper( length );      
      }
    }
  }
  
  catch( int error ){
    ARG_ERR << "catch error (" << error << ")" << endl;
    throw( error );
  }
  
}

void MultiFragFit::setup_Multiplicity( const Xsection::Charge::TARGET& n,
				       const QCD::Flavor::HADRON& h ) 
  throw( int ) {  
  
  int it = MultiData::nType( n );
  int ih = MultiData::hType( h );
  
  if( multData_[ it ][ ih ] == false ) return;         // no data loaded
  
  Evolution::Kernel *ff = NULL;
  switch( h ){
  case QCD::Flavor::PIp :
  case QCD::Flavor::PIm :  ff = pion_; break; 
  case QCD::Flavor::KAp :
  case QCD::Flavor::KAm :  ff = kaon_; break; 
  case QCD::Flavor::PRp :
  case QCD::Flavor::PRm :  ff = proton_; break; 
  case QCD::Flavor::REp :
  case QCD::Flavor::REm :  ff = rest_; break; 
  case QCD::Flavor::HAp :
  case QCD::Flavor::HAm :  ff = hadron_; break; 
  }
  
  if( ff == NULL || updf_ == NULL ) return;
  
  try {

    xmnsec_[ it ][ ih ] = new unpolSIDISXsec::StrFunc( updf_, ff, n, h );
    if( Arguments::ref().hasOpt( "MULStrConstNf" ) )
      xmnsec_[ it ][ ih ]->xsecComp()->coeff()->constNf( true );

    if( useXsecSub_ ){
      xmnsec0_[ it ][ ih ] 
	= new unpolSIDISXsec::StrFuncSub( updf_, ff, 0, n, h );
      xmnsec1_[ it ][ ih ] 
	= new unpolSIDISXsec::StrFuncSub( updf_, ff, 1, n, h );
      xmnsec2_[ it ][ ih ] 
	= new unpolSIDISXsec::StrFuncSub( updf_, ff, 2, n, h );
      if( Arguments::ref().hasOpt( "MULStrConstNf" ) ){
	xmnsec0_[ it ][ ih ]->xsecComp()->coeff()->constNf( true );
	xmnsec1_[ it ][ ih ]->xsecComp()->coeff()->constNf( true );
	xmnsec2_[ it ][ ih ]->xsecComp()->coeff()->constNf( true );
      }
    }

  }
  
  catch( int error ){
    ARG_ERR << "catch error (" << error << ")" << endl;
    throw( error );
  }
  
}

void MultiFragFit::setup_Multiplicity( const Xsection::Charge::TARGET& n ) 
  throw( int ) {
  
  if( updf_ == NULL ) return;
  
  try {
    
    int it = MultiData::nType( n );
    
    bool status = false;
    for( int i = 0; i < multData_[ it ].size(); i++ )
      if( multData_[ it ][ i ] == true ) { status = true; break; }
    
    if( status == false ) return;                 // no data loaded
    
    xmdsec_[ it ] = new unpolDISXsec::StrFunc( updf_, n );
    
    this->setup_Multiplicity( n, QCD::Flavor::PIp );
    this->setup_Multiplicity( n, QCD::Flavor::PIm );
    this->setup_Multiplicity( n, QCD::Flavor::KAp );
    this->setup_Multiplicity( n, QCD::Flavor::KAm );
    this->setup_Multiplicity( n, QCD::Flavor::PRp );
    this->setup_Multiplicity( n, QCD::Flavor::PRm );
    this->setup_Multiplicity( n, QCD::Flavor::REp );
    this->setup_Multiplicity( n, QCD::Flavor::REm );
    this->setup_Multiplicity( n, QCD::Flavor::HAp );
    this->setup_Multiplicity( n, QCD::Flavor::HAm );
    
  }
  
  catch( int error ){
    ARG_ERR << "catch error (" << error << ")" << endl;
    throw( error );
  }
  
}

void MultiFragFit::setup_Multiplicity() throw( int ) {
  
  int nhType = MultiData::hType( "ALL" );
  int ndType = MultiData::nType( "ALL" );
  
  xmdsec_.resize( ndType, ( unpolDISXsec::StrFunc* ) NULL );
  fmd_.resize( ndType, ( Xsection::xXsec* ) NULL );
  
  xmnsec_.resize( ndType, vector< unpolSIDISXsec::StrFunc* >( nhType, ( unpolSIDISXsec::StrFunc* ) NULL ) );
  
  xmnsec0_.resize( ndType, vector< unpolSIDISXsec::StrFuncSub* >( nhType, ( unpolSIDISXsec::StrFuncSub* ) NULL ) );
  xmnsec1_.resize( ndType, vector< unpolSIDISXsec::StrFuncSub* >( nhType, ( unpolSIDISXsec::StrFuncSub* ) NULL ) );
  xmnsec2_.resize( ndType, vector< unpolSIDISXsec::StrFuncSub* >( nhType, ( unpolSIDISXsec::StrFuncSub* ) NULL ) );
  
  fmn_.resize( ndType, vector< Xsection::xzXsec* >( nhType, ( Xsection::xzXsec* ) NULL ) );
  fmn0_.resize( ndType, vector< Xsection::xzXsec* >( nhType, ( Xsection::xzXsec* ) NULL ) );
  fmn1_.resize( ndType, vector< Xsection::xzXsec* >( nhType, ( Xsection::xzXsec* ) NULL ) );
  fmn2_.resize( ndType, vector< Xsection::xzXsec* >( nhType, ( Xsection::xzXsec* ) NULL ) );
  
  xfmn_.resize( ndType, vector< Xsection::xXsec* >( nhType, ( Xsection::xXsec* ) NULL ) );
  xfmn0_.resize( ndType, vector< Xsection::xXsec* >( nhType, ( Xsection::xXsec* ) NULL ) );
  xfmn1_.resize( ndType, vector< Xsection::xXsec* >( nhType, ( Xsection::xXsec* ) NULL ) );
  xfmn2_.resize( ndType, vector< Xsection::xXsec* >( nhType, ( Xsection::xXsec* ) NULL ) );
  
  try {
    // create Xsection Model
    this->setup_Multiplicity( Xsection::Charge::p );
    this->setup_Multiplicity( Xsection::Charge::n );
    this->setup_Multiplicity( Xsection::Charge::d );
    this->setup_Multiplicity_inverse();
  }
  
  catch( int error ){
    ARG_ERR << "catch error (" << error << ")" << endl;
    throw( error );
  }
  
}

void MultiFragFit::setup_Multiplicity_inverse() throw( int ) {
  
  Utility::Arguments& args = Utility::Arguments::ref();
  
  double FFoffset = args.get( "FFoffset", 3.0  );  
  double FFangle  = args.get( "FFangle",  0.55 );  
  double FFlength = args.get( "FFlength", 8.0 );
  
  vector< int > Mleg1( 1, 6 );
  vector< int > Mleg2( 1, 8 );
  Mleg1 = args.get( "Mleg1", ",", Mleg1 );
  Mleg2 = args.get( "Mleg2", ",", Mleg2 );

  double Mprec = args.get( "Mprec", 5.0E-3 );
  int Mngrid   = args.get( "Mngrid", 2 );
  
  // -------------------------------------------------------------
  //                      for numerator
  // -------------------------------------------------------------
  
  double UPDFoffset  = args.get( "UPDFoffset", 4.5  );  
  double UPDFangle   = args.get( "UPDFangle",  0.6 );  
  double UPDFlength  = args.get( "UPDFlength", 8.0 );
  
  vector< double > offset2( 2, FFoffset );
  vector< double > angle2( 2,  FFangle );
  vector< double > length2( 2, FFlength );
  
  offset2[ 0 ] = UPDFoffset; // 1st element for xx integ
  angle2[ 0 ]  = UPDFangle;  // 1st element for xx integ
  length2[ 0 ] = UPDFlength; // 1st element for xx integ

  
#ifdef DEBUG_FITTING
  ARG_LOG << "******* Mellin inversion of unpolSIDISXsec " << endl;
  ARG_LOG << "        ( with DInverseMellin ) " << endl;
  ARG_LOG << " offset: " << setw(6) << offset2[ 0 ] 
	  << ", " << setw(6) << offset2[ 1 ] << endl; 
  ARG_LOG << " angle:  " << setw(6) << angle2[ 0 ] 
	  << ", " << setw(6) << angle2[ 1 ] << endl; 
  ARG_LOG << " length: " << setw(6) << length2[ 0 ] 
	  << ", " << setw(6) << length2[ 1 ] << endl; 
#endif 
  for( int i = 0; i < xmnsec_.size(); i++ ){
    for( int j = 0; j < xmnsec_[ i ].size(); j++ ){
      
      if( xmnsec_[ i ][ j ] ){ 
	
	Xsection::xzXsec *xz = 
	  new Xsection::xzXsec( xmnsec_[ i ][ j ], Mleg1[0], Mleg2[0], Mprec );
	
	xz->setGngrid( Mngrid );
	xz->setParameter( offset2, angle2 );
	xz->integrand().upper( UPDFlength ); // x
	xz->upper( FFlength ); // z
	
	fmn_[ i ][ j ] = xz;

      }
      
      if( xmnsec0_[ i ][ j ] ){ 
	
	Xsection::xzXsec *xz = 
	  new Xsection::xzXsec( xmnsec0_[ i ][ j ], Mleg1[0], Mleg2[0], Mprec );
	
	xz->setGngrid( Mngrid );
	xz->setParameter( offset2, angle2 );
	xz->integrand().upper( UPDFlength ); // x
	xz->upper( FFlength ); // z
	
	fmn0_[ i ][ j ] = xz;

      }
      
      if( xmnsec1_[ i ][ j ] ){ 
	
	Xsection::xzXsec *xz = 
	  new Xsection::xzXsec( xmnsec1_[ i ][ j ], Mleg1[0], Mleg2[0], Mprec );
	
	offset2[ 0 ] = UPDFoffset + 1.0; // 1st element for xx integ
	
	xz->setGngrid( Mngrid );
	xz->setParameter( offset2, angle2 );
	xz->integrand().upper( UPDFlength ); // x
	xz->upper( FFlength ); // z

	fmn1_[ i ][ j ] = xz;

      }
      
      if( xmnsec2_[ i ][ j ] ){ 
	
	Xsection::xzXsec *xz = 
	  new Xsection::xzXsec( xmnsec2_[ i ][ j ], Mleg1[0], Mleg2[0], Mprec );
	
	offset2[ 0 ] = UPDFoffset + 2.0; // 1st element for xx integ
	
	xz->setGngrid( Mngrid );
	xz->setParameter( offset2, angle2 );
	xz->integrand().upper( UPDFlength ); // x
	xz->upper( FFlength ); // z
	
	fmn2_[ i ][ j ] = xz;

      }
      
    }
  }

  // -------------------------------------------------------------
  //                    for xXsec inversion
  // -------------------------------------------------------------
  if( useXXsec_ ){
    
    angle2[ 0 ]  = 0.60; angle2[ 1 ] = 0.60;
    offset2[ 0 ] = 3.6;  offset2[ 1 ] = 3.0;
    length2[ 0 ] = 9.0;  length2[ 1 ] = 8.0;
    
    offset2 = args.get( "offset2", ",", offset2 );
    length2 = args.get( "length2", ",", length2 );
    angle2  = args.get( "angle2",  ",", angle2  );

    // for sub xsection
    vector< double > off0( 2, 0.0 );
    vector< double > ang0( 2, 0.0 );
    vector< double > len0( 2, 0.0 );

    vector< double > off1( 2, 0.0 );
    vector< double > ang1( 2, 0.0 );
    vector< double > len1( 2, 0.0 );
    
    vector< double > off2( 2, 0.0 );
    vector< double > ang2( 2, 0.0 );
    vector< double > len2( 2, 0.0 );

    off0[ 0 ] = 3.0;  off0[ 1 ] = 3.0;
    ang0[ 0 ] = 0.55; ang0[ 1 ] = 0.55;
    len0[ 0 ] = 8.0;  len0[ 1 ] = 12.0;
    
    off1[ 0 ] = 2.5;  off1[ 1 ] = 3.0;
    ang1[ 0 ] = 0.55; ang1[ 1 ] = 0.55;
    len1[ 0 ] = 6.0;  len1[ 1 ] = 12.0;

    off2[ 0 ] = 3.5;  off2[ 1 ] = 3.0;
    ang2[ 0 ] = 0.55; ang2[ 1 ] = 0.55;
    len2[ 0 ] = 6.0;  len2[ 1 ] = 12.0;
    
    off0 = args.get( "off0", ",", off0 );
    len0 = args.get( "len0", ",", len0 );
    ang0 = args.get( "ang0", ",", ang0 );
    
    off1 = args.get( "off1", ",", off1 );
    len1 = args.get( "len1", ",", len1 );
    ang1 = args.get( "ang1", ",", ang1 );
    
    off2 = args.get( "off2", ",", off2 );
    len2 = args.get( "len2", ",", len2 );
    ang2 = args.get( "ang2", ",", ang2 );
    
#ifdef DEBUG_FITTING
    ARG_LOG << "******* Mellin inversion of unpolSIDISXsec " << endl;
    ARG_LOG << "        ( with InverseMellin ) requires --xXsec" << endl;
    
    ARG_LOG << " offset: " << setw(6) << offset2[ 0 ] 
	    << ", " << setw(6) << offset2[ 1 ] << endl; 
    ARG_LOG << " angle:  " << setw(6) << angle2[ 0 ] 
	    << ", " << setw(6) << angle2[ 1 ] << endl; 
    ARG_LOG << " length: " << setw(6) << length2[ 0 ] 
	    << ", " << setw(6) << length2[ 1 ] << endl; 
    
    if( useXsecSub_ ){
      
      ARG_LOG
	<< "         "
	<< setw(12) << "Sub0"
	<< setw(12) << "Sub1"
	<< setw(12) << "Sub2"
	<< endl;
      
      ARG_LOG
	<< " offset: "
	<< setw(6) << off0[ 0 ] << setw(6) << off0[ 1 ]
	<< setw(6) << off1[ 0 ] << setw(6) << off1[ 1 ]
	<< setw(6) << off2[ 0 ] << setw(6) << off2[ 1 ] << endl;
      
      ARG_LOG 
	<< " angle:  "
	<< setw(6) << ang0[ 0 ] << setw(6) << ang0[ 1 ]
	<< setw(6) << ang1[ 0 ] << setw(6) << ang1[ 1 ]
	<< setw(6) << ang2[ 0 ] << setw(6) << ang2[ 1 ] << endl;
      
      ARG_LOG 
	<< " length: "
	<< setw(6) << len0[ 0 ] << setw(6) << len0[ 1 ]
	<< setw(6) << len1[ 0 ] << setw(6) << len1[ 1 ]
	<< setw(6) << len2[ 0 ] << setw(6) << len2[ 1 ] << endl;
      
    }
    
#endif 
    
    for( int i = 0; i < xmnsec_.size(); i++ ){
      for( int j = 0; j < xmnsec_[ i ].size(); j++ ){
	
	if( xmnsec_[ i ][ j ] ) {
	  
	  Xsection::xXsec *xf = 
	    new Xsection::xXsec( xmnsec_[ i ][ j ], Mleg1[0], Mleg2[0], Mprec );
	  
	  xf->setGngrid( Mngrid );
	  xf->setParameter( offset2, angle2 );
	  xf->upper( length2 );
	  xfmn_[ i ][ j ] = xf;

	  /*
	  Transform::GridIntegration* grid =
	    dynamic_cast< Transform::GridIntegration* >( xf->integration() );

	  grid->ngrid( 4, 2 );
	  
	  if( Mleg1.size() > 1 )
	    dynamic_cast< Transform::LegQuadrature* >
	      ( grid->method1() )->setngrid( Mleg1[0], Mleg1[1] );

	  if( Mleg2.size() > 1 )
	    dynamic_cast< Transform::LegQuadrature* >
	      ( grid->method2() )->setngrid( Mleg2[0], Mleg2[1] );
	  */
	}
	
	if( xmnsec0_[ i ][ j ] ) {
	  
	  Xsection::xXsec *xf = 
	    new Xsection::xXsec( xmnsec0_[ i ][ j ], Mleg1[0], Mleg2[0], Mprec );
	  
	  xf->setGngrid( Mngrid );
	  xf->setParameter( off0, ang0 );
	  xf->upper( len0 );
	  xfmn0_[ i ][ j ] = xf;

	  
	  Transform::GridIntegration* grid =
	    dynamic_cast< Transform::GridIntegration* >( xf->integration() );
	  
	  //	  grid->ngrid( 4, 2 );
	  
	  /*
	    dynamic_cast< Transform::LegQuadrature* >( grid->method1() )->setngrid( 8, 4 );
	  dynamic_cast< Transform::LegQuadrature* >( grid->method2() )->setngrid( 10, 6 );
	  */

	}
	
	if( xmnsec1_[ i ][ j ] ) {
	  
	  Xsection::xXsec *xf = 
	    new Xsection::xXsec( xmnsec1_[ i ][ j ], Mleg1[0], Mleg2[0], Mprec );
	  
	  xf->setGngrid( Mngrid );
	  xf->setParameter( off1, ang1 );
	  xf->upper( len1 );
	  xfmn1_[ i ][ j ] = xf;


	  Transform::GridIntegration* grid =
	    dynamic_cast< Transform::GridIntegration* >( xf->integration() );

	  //	  grid->ngrid( 4, 2 );

	  /*
	  dynamic_cast< Transform::LegQuadrature* >( grid->method1() )->setngrid( 6, 4 );
	  dynamic_cast< Transform::LegQuadrature* >( grid->method2() )->setngrid( 8, 6 );
	  */

	}
	
	if( xmnsec2_[ i ][ j ] ) {
	  
	  Xsection::xXsec *xf = 
	    new Xsection::xXsec( xmnsec2_[ i ][ j ], Mleg1[0], Mleg2[0], Mprec );
	  
	  xf->setGngrid( Mngrid );
	  xf->setParameter( off2, ang2 );
	  xf->upper( len2 );
	  xfmn2_[ i ][ j ] = xf;
	  
	  Transform::GridIntegration* grid =
	    dynamic_cast< Transform::GridIntegration* >( xf->integration() );

	  //	  grid->ngrid( 4, 2 );

	  /*
	  dynamic_cast< Transform::LegQuadrature* >( grid->method1() )->setngrid( 6, 4 );
	  dynamic_cast< Transform::LegQuadrature* >( grid->method2() )->setngrid( 8, 6 );
	  */

	}
	
      }
    }
  }
  
  // -------------------------------------------------------------
  //                      for denominator
  // -------------------------------------------------------------
  double UPDFoffsetN = args.get( "UPDFoffsetN", 4.5  );  
  double UPDFangleN  = args.get( "UPDFangleN",  0.60 );  
  double UPDFlengthN = args.get( "UPDFlengthN", 8.0 );
  
  for( int i = 0; i < xmdsec_.size(); i++ ){
    
    if( xmdsec_[ i ] == NULL ) continue;
    
    Xsection::xXsec *xx =
      new Xsection::xXsec( xmdsec_[ i ], Mleg1[0], Mleg2[0], Mprec );
    
    xx->setGngrid( Mngrid );
    xx->setParameter( UPDFoffsetN, UPDFangleN );
    xx->upper( UPDFlengthN );
    
    fmd_[ i ] = xx;
  }
  
}

void MultiFragFit::set_contour( const double& x, Xsection::xXsec* xsec ){
  
  vector< double > length = xsec->length();
  //  vector< double > angle  = xsec->angle();
  //  vector< double > offset = xsec->offset();
  if( x < 0.01 ){
    length[ 0 ] = xXsecLength_[ 2 ];
  } else if( x < 0.1 ) {
    length[ 0 ] = xXsecLength_[ 1 ];
  } else {
    length[ 0 ] = xXsecLength_[ 0 ];
  }
  //  xsec->setParameter( offset, angle );
  xsec->upper( length );

}

double MultiFragFit::sigma_h( const int& it, const int& ih,
			      const double& q2, 
			      const double& s,
			      const std::vector< double >& x,
			      const std::vector< double >& z ){
  
  if( useXsecSub_ ){
    xmnsec0_[ it ][ ih ]->setS( s );
    xmnsec1_[ it ][ ih ]->setS( s );
    xmnsec2_[ it ][ ih ]->setS( s );
    xmnsec0_[ it ][ ih ]->setQ2( q2 );
    xmnsec1_[ it ][ ih ]->setQ2( q2 );
    xmnsec2_[ it ][ ih ]->setQ2( q2 );
  } else {
    xmnsec_[ it ][ ih ]->setS( s );
    xmnsec_[ it ][ ih ]->setQ2( q2 );
  }
  
  double val = 0.0;

  /*
  cout 
    << setw(2) << it << setw(2) << ih
    << setw(12) << s
    << setw(12) << q2 
    << setw(12) << x[ 0 ]
    << setw(12) << ( x.size() == 1 ? x[ 0 ] : x[ 1 ] ) 
    << setw(12) << z[ 0 ]
    << setw(12) << ( z.size() == 1 ? z[ 0 ] : z[ 1 ] ) 
    << endl;
  */

  if( useXXsec_ ) {
    //    if( ! useXsecSub_ ) this->set_contour( xave_[ i ], xfmn_[ it ][ ih ] );
    val = ( useXsecSub_ ? 
	    (*xfmn0_[ it ][ ih ] )( x, z ) +
	    (*xfmn1_[ it ][ ih ] )( x, z ) +
	    (*xfmn2_[ it ][ ih ] )( x, z ) :
	    (*xfmn_[ it ][ ih ] )( x, z ) );
  
  } else {
    
    val = ( useXsecSub_ ?  
	    (*fmn0_[ it ][ ih ])( x, z ) +
	    (*fmn1_[ it ][ ih ])( x, z ) +
	    (*fmn2_[ it ][ ih ])( x, z ) :
	    (*fmn_[ it ][ ih ])( x, z ) );
  }

  //  cout << setw(12) << val << endl;

  return val;
}

double MultiFragFit::dsigma_h( const int& it, const int& ih,
			       const double& q2, 
			       const double& s, 
			       const std::vector< double >& x,
			       const std::vector< double >& z,
			       Exp::DataSet* ds ){
  
  double error = 0.0;
  
  if( useXsecSub_ ){
    xmnsec0_[ it ][ ih ]->setS( s );
    xmnsec1_[ it ][ ih ]->setS( s );
    xmnsec2_[ it ][ ih ]->setS( s );
    xmnsec0_[ it ][ ih ]->setQ2( q2 );
    xmnsec1_[ it ][ ih ]->setQ2( q2 );
    xmnsec2_[ it ][ ih ]->setQ2( q2 );
  } else { 
    xmnsec_[ it ][ ih ]->setS( s );
    xmnsec_[ it ][ ih ]->setQ2( q2 );
  }

  /*
  cout 
    << setw(2) << it << setw(2) << ih
    << setw(12) << s
    << setw(12) << q2 
    << setw(12) << x[ 0 ]
    << setw(12) << ( x.size() == 1 ? x[ 0 ] : x[ 1 ] ) 
    << setw(12) << z[ 0 ]
    << setw(12) << ( z.size() == 1 ? z[ 0 ] : z[ 1 ] ) 
    << endl;
  */
  
  if( useXXsec_ ){
    
    if( useXsecSub_ ){

      //! NOTE: correlations between different terms 
      //! must be also taken into account
      /*
      ErrorCalc::ErrCalc eCalc0( xfmn0_[ it ][ ih ] );
      ErrorCalc::ErrCalc eCalc1( xfmn1_[ it ][ ih ] );
      ErrorCalc::ErrCalc eCalc2( xfmn2_[ it ][ ih ] );
      // for normalization
      if( ds != NULL ){ 
	this->setNormMp( &eCalc0, ds );
	this->setNormMp( &eCalc1, ds );
	this->setNormMp( &eCalc2, ds );
      }
      error = sqrt( eCalc0( x, z ) + eCalc1( x, z ) + eCalc2( x, z ) );
      */

      // diagonal element
      ErrorCalc::XsecCorrCalc eCL00( xfmn0_[ it ][ ih ], xfmn0_[ it ][ ih ] );
      ErrorCalc::XsecCorrCalc eCL11( xfmn1_[ it ][ ih ], xfmn1_[ it ][ ih ] );
      ErrorCalc::XsecCorrCalc eCL22( xfmn2_[ it ][ ih ], xfmn2_[ it ][ ih ] );
      // off-diagonal element
      ErrorCalc::XsecCorrCalc eCL01( xfmn0_[ it ][ ih ], xfmn1_[ it ][ ih ] );
      ErrorCalc::XsecCorrCalc eCL02( xfmn0_[ it ][ ih ], xfmn2_[ it ][ ih ] );
      ErrorCalc::XsecCorrCalc eCL12( xfmn1_[ it ][ ih ], xfmn2_[ it ][ ih ] );

      vector< double > x1( x ), z1( z ); // for first xXsec
      vector< double > x2( x ), z2( z ); // for second xXsec

      // for normalization
      if( ds != NULL ){ 
	this->setNormMp( &eCL00, ds );
	this->setNormMp( &eCL11, ds );
	this->setNormMp( &eCL22, ds );
	this->setNormMp( &eCL01, ds );
	this->setNormMp( &eCL02, ds );
	this->setNormMp( &eCL12, ds );
      }

      error = sqrt( eCL00( x1, z1, x2, z2 ) + eCL11( x1, z1, x2, z2 ) + 
		    eCL22( x1, z1, x2, z2 ) + 
		    2.0 * ( eCL01( x1, z1, x2, z2 ) + eCL02( x1, z1, x2, z2 ) 
			    + eCL12( x1, z1, x2, z2 ) ) 
		    );

    } else {

      ErrorCalc::ErrCalc eCalc( xfmn_[ it ][ ih ] );
      // for normalization
      if( ds != NULL ) this->setNormMp( &eCalc, ds );
      error = sqrt( eCalc( x, z ) );

    }

  } else {

    if( useXsecSub_ ){

      //! NOTE: correlations between different terms 
      //! must be also taken into account
      /*
      ErrorCalc::ErrCalc eCalc0( fmn0_[ it ][ ih ] );
      ErrorCalc::ErrCalc eCalc1( fmn1_[ it ][ ih ] );
      ErrorCalc::ErrCalc eCalc2( fmn2_[ it ][ ih ] );
      // for normalization
      if( ds != NULL ){
	setNormMp( &eCalc0, ds );
	setNormMp( &eCalc1, ds );
	setNormMp( &eCalc2, ds );
      }
      error = sqrt( eCalc0( x, z ) + eCalc1( x, z ) + eCalc2( x, z ) );
      */

      // diagonal element
      ErrorCalc::XsecCorrCalc eCL00( fmn0_[ it ][ ih ], fmn0_[ it ][ ih ] );
      ErrorCalc::XsecCorrCalc eCL11( fmn1_[ it ][ ih ], fmn1_[ it ][ ih ] );
      ErrorCalc::XsecCorrCalc eCL22( fmn2_[ it ][ ih ], fmn2_[ it ][ ih ] );
      // off-diagonal element
      ErrorCalc::XsecCorrCalc eCL01( fmn0_[ it ][ ih ], fmn1_[ it ][ ih ] );
      ErrorCalc::XsecCorrCalc eCL02( fmn0_[ it ][ ih ], fmn2_[ it ][ ih ] );
      ErrorCalc::XsecCorrCalc eCL12( fmn1_[ it ][ ih ], fmn2_[ it ][ ih ] );

      vector< double > x1( x ), z1( z ); // for first xXsec
      vector< double > x2( x ), z2( z ); // for second xXsec

      // for normalization
      if( ds != NULL ){ 
	this->setNormMp( &eCL00, ds );
	this->setNormMp( &eCL11, ds );
	this->setNormMp( &eCL22, ds );
	this->setNormMp( &eCL01, ds );
	this->setNormMp( &eCL02, ds );
	this->setNormMp( &eCL12, ds );
      }

      error = sqrt( eCL00( x1, z1, x2, z2 ) + eCL11( x1, z1, x2, z2 ) + 
		    eCL22( x1, z1, x2, z2 ) + 
		    2.0 * ( eCL01( x1, z1, x2, z2 ) + eCL02( x1, z1, x2, z2 ) 
			    + eCL12( x1, z1, x2, z2 ) ) 
		    );

    } else {

      ErrorCalc::ErrCalc eCalc( fmn_[ it ][ ih ] );
      // for normalization
      if( ds != NULL ) this->setNormMp( &eCalc, ds );
      //      outMatrix( &eCalc );
      error = sqrt( eCalc( x, z ) );

    }

  }

  return error;
}

bool MultiFragFit::condition(){

  if( hadron_ ){
    hadron_->setN( 2.0 );
    hadron_->setQ2( hadron_->initialScale() );
    for( int i = -6; i < 7; i++ )
      if( abs( hadron_->q( Flavor::PARTON( i ) ) ) >= 0.5 ) 
	return false;
  }

  return true;
}

void MultiFragFit::config_vzrange(){
  
  Utility::Arguments& args = Utility::Arguments::ref();
  
  vzran_[ MultiData::hType( QCD::Flavor::PIp ) ] = args.get( "zrange-pion", ",", zrange_ );
  vzran_[ MultiData::hType( QCD::Flavor::PIm ) ] = args.get( "zrange-pion", ",", zrange_ );
  
  ARG_LOG << "   pion:   " 
	  << setw(6) << vzran_[ MultiData::hType( QCD::Flavor::PIp ) ][ 0 ]
	  << " - "
	  << setw(6) << vzran_[ MultiData::hType( QCD::Flavor::PIp ) ][ 1 ]
	  << endl;
  
  vzran_[ MultiData::hType( QCD::Flavor::KAp ) ] = args.get( "zrange-kaon", ",", zrange_ );
  vzran_[ MultiData::hType( QCD::Flavor::KAm ) ] = args.get( "zrange-kaon", ",", zrange_ );
  
  ARG_LOG << "   kaon:   " 
	  << setw(6) << vzran_[ MultiData::hType( QCD::Flavor::KAp ) ][ 0 ]
	  << " - "
	  << setw(6) << vzran_[ MultiData::hType( QCD::Flavor::KAp ) ][ 1 ]
	  << endl;
  
  vzran_[ MultiData::hType( QCD::Flavor::PRp ) ] = args.get( "zrange-proton", ",", zrange_ );
  vzran_[ MultiData::hType( QCD::Flavor::PRm ) ] = args.get( "zrange-proton", ",", zrange_ );
  
  ARG_LOG << "   proton: " 
	  << setw(6) << vzran_[ MultiData::hType( QCD::Flavor::PRp ) ][ 0 ]
	  << " - "
	  << setw(6) << vzran_[ MultiData::hType( QCD::Flavor::PRp ) ][ 1 ]
	  << endl;
  
  vzran_[ MultiData::hType( QCD::Flavor::REp ) ] = args.get( "zrange-rest", ",", zrange_ );
  vzran_[ MultiData::hType( QCD::Flavor::REm ) ] = args.get( "zrange-rest", ",", zrange_ );
  
  ARG_LOG << "   rest: " 
	  << setw(6) << vzran_[ MultiData::hType( QCD::Flavor::REp ) ][ 0 ]
	  << " - "
	  << setw(6) << vzran_[ MultiData::hType( QCD::Flavor::REp ) ][ 1 ]
	  << endl;
    
  vzran_[ MultiData::hType( QCD::Flavor::HAp ) ] = args.get( "zrange-hadron", ",", zrange_ );
  vzran_[ MultiData::hType( QCD::Flavor::HAm ) ] = args.get( "zrange-hadron", ",", zrange_ );
  
  ARG_LOG << "   hadron: " 
	  << setw(6) << vzran_[ MultiData::hType( QCD::Flavor::HAp ) ][ 0 ]
	  << " - "
	  << setw(6) << vzran_[ MultiData::hType( QCD::Flavor::HAp ) ][ 1 ]
	  << endl;
  
  vzran_[ MultiData::hType( QCD::Flavor::PI0 ) ] = args.get( "zrange-pi0", ",", zrange_ );
  
  ARG_LOG << "   pion0:  " 
	  << setw(6) << vzran_[ MultiData::hType( QCD::Flavor::PI0 ) ][ 0 ]
	  << " - "
	  << setw(6) << vzran_[ MultiData::hType( QCD::Flavor::PI0 ) ][ 1 ]
	  << endl;
}

void MultiFragFit::config_c2weight(){
  
  Utility::Arguments& args = Utility::Arguments::ref();
  
  if( ! args.hasOpt( "c2w_pion" ) &&
      ! args.hasOpt( "c2w_kaon" ) &&
      ! args.hasOpt( "c2w_proton" ) &&
      ! args.hasOpt( "c2w_hadron" ) &&
      ! args.hasOpt( "c2w_pi0" ) ){
    ARG_LOG << "Use chi2 weight in the ExpData objects." << endl;
    return;
  }
  
  fragc2w_.resize( FragData::hType( "ALL" ),  1.0 );
  multc2w_.resize( MultiData::hType( "ALL" ), 1.0 );

  double c2w_pion   = args.get( "c2w_pion", 1.0 );
  double c2w_kaon   = args.get( "c2w_kaon", 1.0 );
  double c2w_proton = args.get( "c2w_proton", 1.0 );
  double c2w_hadron = args.get( "c2w_hadron", 1.0 );
  double c2w_pi0    = args.get( "c2w_pi0", 1.0 );
  
  ARG_LOG << "  Hadron type dependent chi2 weights:" << endl;
  ARG_LOG << "    pion:"   << setw(6) << c2w_pion   << endl;
  ARG_LOG << "    kaon:"   << setw(6) << c2w_kaon   << endl;
  ARG_LOG << "    proton:" << setw(6) << c2w_proton << endl;
  ARG_LOG << "    hadron:" << setw(6) << c2w_hadron << endl;
  ARG_LOG << "    pi0:"    << setw(6) << c2w_pi0    << endl;
  
  fragc2w_[ FragData::hType( QCD::Flavor::PIp ) ] = c2w_pion;
  fragc2w_[ FragData::hType( QCD::Flavor::KAp ) ] = c2w_kaon;
  fragc2w_[ FragData::hType( QCD::Flavor::PRp ) ] = c2w_proton;
  fragc2w_[ FragData::hType( QCD::Flavor::HAp ) ] = c2w_hadron;
  fragc2w_[ FragData::hType( QCD::Flavor::PI0 ) ] = c2w_pi0;
  
  multc2w_[ MultiData::hType( QCD::Flavor::PIp ) ] = c2w_pion;
  multc2w_[ MultiData::hType( QCD::Flavor::PIm ) ] = c2w_pion;
  
  multc2w_[ MultiData::hType( QCD::Flavor::KAp ) ] = c2w_kaon;
  multc2w_[ MultiData::hType( QCD::Flavor::KAm ) ] = c2w_kaon;
  
  multc2w_[ MultiData::hType( QCD::Flavor::PRp ) ] = c2w_proton;
  multc2w_[ MultiData::hType( QCD::Flavor::PRm ) ] = c2w_proton;
  
  multc2w_[ MultiData::hType( QCD::Flavor::HAp ) ] = c2w_hadron;
  multc2w_[ MultiData::hType( QCD::Flavor::HAm ) ] = c2w_hadron;
  
  multc2w_[ MultiData::hType( QCD::Flavor::PI0 ) ] = c2w_pi0;
  
}
