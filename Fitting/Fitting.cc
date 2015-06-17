// $Id: Fitting.cc,v 1.133 2013/06/14 06:26:16 miyachi Exp $
/*!
  \file    Fitting.cc
  \version $Revision: 1.133 $
  \author  $Author: miyachi $
  \date    $Date: 2013/06/14 06:26:16 $
  \brief   class implementation of Fitting class
 */
#include "Fitting.hh"
#include "FitData.hh"

#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <algorithm>
#include <ctime>

#include <TAxis.h>
#include <TFile.h>
#include <TMatrixT.h>

#include <QCD/PdfBase.hh>
#include <QCD/PdfParametrization.hh>
#include <QCD/HadronParam.hh>
#include <Tranform/GridIntegration.hh>
#include <Evolution/KernelBase.hh>
#include <Evolution/Kernel.hh>
#include <Xsec/Xsec.hh>
#include <Xsec/XsecComp.hh>

using namespace std;
using namespace Utility;
using namespace QCD;
using namespace Evolution;
using namespace Fit;
using namespace Exp;

//double Fitting::CHI2HUGE = 1.0E99;
double Fitting::CHI2HUGE = 1000.0;
//double Fitting::CHI2HUGE = Arguments::ref().get( "chi2huge", 1.0E99 );

Fitting::Fitting( Arguments& args, const bool& judge ) throw( int ):
  TMinuit( 100 ),
  expr_( 0 ),
  kernel_( 0 ),
  pdf_( 0 ),
  params_( 0 ),
  nam_( 0 ),
  iflags_( 6, 0 ),
  timer_(),
  commands_( 0 ),
  contFileName_( "" ),
  contparam_( 0 ),
  chi2_( 0.0 ),
  res_chi2_( 0.0 ),
  chi2scale_( args.get( "chi2scale", 1.0 ) ),
  useFData_( args.hasOpt("useFitData") ),
  fData_( 0 ),
  sig_( 0 ), dsig_( 0 ), q2_( 0 ), expInfo_( 0 ), fit_( 0 ), 
  sigUsed_( 0 ),
  c2_( 0 ), c2weight_( 0 ), dset_( 0 ), point_( 0 ),
  nData_( 0 ),
  q2limit_( 1.0 ),
  offset_( 0.0 ), angle_( 0.0 ), length_( 0.0 ),
  multiplier_( false ),
  lambda_( 0.0 ),
  emflag_( false ),
  fmin_( 0.0 ), fedm_( 0.0 ), errdef_( 0.0 ), npari_( 0 ), nparx_( 0 ), istat_( 0 ),
  pion_( NULL ), kaon_( NULL ), rest_( NULL ), proton_( NULL ), hadron_( NULL ), ffFit_( false ),
  updf_( NULL ), ppdf_( NULL ),
  o_pion_( NULL ), o_kaon_( NULL ), o_rest_( NULL ), o_proton_( NULL ), o_updf_( NULL ), o_ppdf_( NULL ),
  d_pion_( NULL ), d_kaon_( NULL ), d_rest_( NULL ), d_proton_( NULL ), d_updf_( NULL ), d_ppdf_( NULL ),
  toFit_( 0 ),
  emlevel_( 1 ),
  dataNormalization_( false ), rescaledData_( 0 ),
  postErrMatrix_( false ),
  ematrices_( 0 ),
  with_stat_corr_( args.hasOpt( "covMatrix" ) ),
  prefix_( args.get( "prefix", "" ) ),
  sigHalfSize_( 0 ),
  useThread_( false ),
  errorDirect_( args.hasOpt( "errordirect" ) ),
  scaleDirect_( args.hasOpt( "scaledirect" ) ),
  skipfit_( false ),
  checkFitCondition_( ! args.hasOpt( "NoCondition" ) ) ,
  difference_( args.hasOpt( "difference" ) ),
  diffPrec_( args.get( "difference", 0.1 ) ),
  conf_judge_( false ),
  pvalue_(),
  dumpParamLevel_( 0 )
{
  
#ifdef DEBUG_FITTING
  ARG_LOG << args << endl;
#endif 
  
  if( args.hasOpt( "help" ) || args.hasOpt( "?" ) ){

    args.usage( "expdata", "", "Path to the experiment data files, with \",\" separator." );
    args.usage( "oexpdata", "", "file name of output file for the experiment data" );
    args.usage( "fit", "", "parameter to be fitted. (pion,kaon,proton,rest,unpolPDF,polPDF,norm)" );
    args.usage( "NoCondition", "", "without fitting condition" );

    args.usage( "covMatrix", "", "use stat. covariance in chi2 calculation." );

    args.usage( "Minuit", "", "call Minuit commands (separated with |)" );
    args.usage( "MinuitPrintLevel", "0", "Set Minuit print level" );
    args.usage( "iteration", "10000", "maximum number of iteration" );
    args.usage( "difference", "0.1", "calculate chi2 difference (experiment)" );

    args.usage( "prefix", "", "use the given prefix for the output file name." );

    args.usage( "errordirect", "", "calculate error without fit using errmx already assigned in parameters" );
    args.usage( "scaledirect", "", "calculate scale effect without fit" );

    args.usage( "contour", "contour.root", "create chi2 contour plot" );
    args.usage( "ncontour", "20", "number of contour grid" );
    args.usage( "contparam", "", "parameter number for contour plot" );

    args.usage( "errorMatrix", "", "calculate error matrix from the fit" );
    args.usage( "postErrorMatrix", "", "enable postErrorMatrix call" );
    args.usage( "emlevel", "1", "minimum error matrix level for error matrix calculation" );

    args.usage( "verbose", "", "set verbose mode" );

    args.usage( "", "", "" );

    args.usage( "DataCacheMaxSize", "", "change DataCache max size" );
    args.usage( "DataCache2MaxSize", "", "change DataCache2 max size" );
    args.usage( "gridMaxDepth", "15", "Set maximum depth for grid integration");

    args.usage( "thread", "", "Enable thread (experimental)" );
    args.usage( "useFitData", "", "Using FitData object (experimental)." );

    args.usage( "offset", "2.00", "Set default offset value for Mellin inversion" );
    args.usage( "angle", "0.55", "Set default angle value for Mellin inversion" );
    args.usage( "length", "15.0", "Set default length value for Mellin inversion" );

    args.usage( "", "", "" );
    args.usage( "dumpParamLevel", "0", "parameter xml file output if <= istat." );
    args.usage( "?", "", "Show this massage" );
    args.usage( "help", "", "Show this massage" );
  
  
  }

  //config process 
  if( judge ){

    try {
      this->config_process();
    }

    catch( int error ){
      ARG_ERR << "catch errror (" << error << ")" << endl;
      throw( error );
    }
  }
  
  dumpParamLevel_ = args.get( "dumpParamLevel", 0 );
  if( dumpParamLevel_ < 0 ) dumpParamLevel_ = 0;
  if( dumpParamLevel_ > 3 ) dumpParamLevel_ = 3;

}

Fitting::~Fitting() {

  if( conf_judge_ ){
    this->delete_process();
  }
}

void Fitting::delete_process() {

  // delete created dist object in the constructor
  null_delete( hadron_ );

  null_delete( proton_ );
  null_delete( rest_ );
  null_delete( kaon_ );
  null_delete( pion_ );
  null_delete( updf_ );
  null_delete( ppdf_ );

  null_delete( o_proton_ );
  null_delete( o_rest_ );
  null_delete( o_kaon_ );
  null_delete( o_pion_ );
  null_delete( o_updf_ );
  null_delete( o_ppdf_ );

  null_delete( d_proton_ );
  null_delete( d_rest_ );
  null_delete( d_kaon_ );
  null_delete( d_pion_ );
  null_delete( d_updf_ );
  null_delete( d_ppdf_ );

  // delete stored Exp::ExpData objects
  for( int i = 0; i < expr_.size(); i++ ) delete expr_[ i ];

  // delete stored Fitting::FitData objects
  for( int i = 0; i < fData_.size(); i++ ) delete fData_[ i ];
}

void Fitting::config_process() throw( int ) {

  if( conf_judge_ ){
    ARG_ERR << "error: configuration process execution duplicated" << endl;
    throw( 1 );
  }

#ifdef DEBUG_FITTING
  ARG_LOG << " *** Local configuration" << endl;
#endif 
  this->config_local();          // setup 

#ifdef DEBUG_FITTING
  ARG_LOG << " *** MINUIT configuration" << endl;
#endif 
  this->config_minuit();         // setup MINUIT
  
  // Prepare data and parameters for fitting
  try {
#ifdef DEBUG_FITTING
  ARG_LOG << " *** ExpData configuration" << endl;
#endif 
    this->config_expdata();
    
#ifdef DEBUG_FITTING
    ARG_LOG << " *** Parameter configuration" << endl;
#endif 
    this->config_param();
  }
  
  catch( int error ){
    ARG_ERR << "catch errror (" << error << ")" << endl;
    throw( error );
  }
  
#ifdef DEBUG_FITTING
  ARG_LOG << " *** contour configuration" << endl;
#endif 
  this->config_contour();        // setup chi2 contour calculation

#ifdef DEBUG_FITTING
    ARG_LOG << " *** Mellin Inversion  configuration" << endl;
#endif 
  this->config_inversion();      // setup Mellin inversion

#ifdef DEBUG_FITTING
    ARG_LOG << " *** Error Matrix configuration" << endl;
#endif 
  this->config_errorMatrix();    // setup error matrix calculation

  timer_.start(); // start global timer

  conf_judge_ = true; 
  // flag for config process execution (for duplicate check & delete process)
}

void Fitting::config_minuit(){

  Utility::Arguments& args = Utility::Arguments::ref();
  
  // ------------------------------------------------------------ //
  //                      MINUIT configration                     //
  // ------------------------------------------------------------ //
  //   set Minuit print level ( -1, 0, 1, 2, 3 )
#ifdef DEBUG_FITTING
  ARG_LOG << "Set Minuit Log Level:" << args.get( "MinuitPrintLevel", 0 ) 
	  << endl;
#endif 
  this->SetPrintLevel( args.get( "MinuitPrintLevel", 0 ) ); 
  
  //   analyze minuit command
  if( args.hasOpt( "Minuit" ) ) {
    this->minuitCmds( args.getOpt( "Minuit" ) );
#ifdef DEBUG_FITTING
    ARG_LOG 
      << "call Minuit commands " << args.getOpt( "Minuit" ) 
      << endl;
#endif 
  }
  
  //   set maximum iteration number
  this->SetMaxIterations( args.get( "iteration", 10000 ) );
  
}

void Fitting::config_contour(){
  
  Utility::Arguments& args = Utility::Arguments::ref();
  
  // ------------------------------------------------------------ //
  //                    chi^2 contour calculation                 //
  // ------------------------------------------------------------ //
  if( args.hasOpt( "contour" ) ){
    contFileName_ = args.get( "contour", "contour.root" ) ;
#ifdef DEBUG_FITTING
    ARG_LOG << "calculate chi-2 contour after minimization." << endl;
#endif
    if( args.hasOpt( "contparam" ) ){
      contparam_ = args.get( "contparam", ",", contparam_ );
#ifdef DEBUG_FITTING
      ARG_LOG << "-- contour will be calculated for " << flush;
      for( int i = 0; i < contparam_.size(); i++ )
	ARG_LOG << setw(3) << contparam_[i] << flush;
      ARG_LOG << endl;
#endif
    } else {
#ifdef DEBUG_FITTING
      ARG_LOG << "-- contour will be calculated for all parameters" << endl;
#endif
    }
  }
  
}

void Fitting::config_inversion(){
  
  Utility::Arguments& args = Utility::Arguments::ref();
  
  // ------------------------------------------------------------ //
  //                 Mellin Inversion configuration               //
  // ------------------------------------------------------------ //
  // get integration path information for Mellin Inversion
  offset_ = args.get( "offset", 2.00 );
  angle_  = args.get( "angle",  0.55 );
  length_ = args.get( "length", 15.0 );
  
}

void Fitting::config_errorMatrix() {
  
  Utility::Arguments& args = Utility::Arguments::ref();
  
  // ------------------------------------------------------------ //
  //                 Error matrix configuration                   //
  // ------------------------------------------------------------ //
  // error matrix output flag
  if( emflag_ = args.hasOpt( "errorMatrix" ) ){
#ifdef DEBUG_FITTING
    ARG_LOG << "Enabled error matrix calculation routine:" << endl;
#endif 
    // error matrix level
    emlevel_ = args.get( "emlevel", emlevel_ );
    if( emlevel_ < 0 ) emlevel_ = 0;
    if( emlevel_ > 3 ) emlevel_ = 3;
#ifdef DEBUG_FITTING
    ARG_LOG << "  error matrix calculation level set to "
	    << setw( 2 ) << emlevel_ 
	    << ": " << Fitting::emLevelTag( emlevel_ )
	    << endl;
#endif 
  }
}

void Fitting::config_expdata() throw( int ) {
  
  Utility::Arguments& args = Utility::Arguments::ref();
  
  // ------------------------------------------------------------ //
  //                    Experiment Data setting                   //
  // ------------------------------------------------------------ //
  // expriment data
  if( ! args.hasOpt( "expdata" ) ){
    ARG_ERR << "--expdata=[data file path] is required."
	    << endl;
    throw( 1 );
  }
  
  vector< string > path( 0 );
  path = args.get( "expdata", ",", path );
  
  if( path.size() == 0 ){
    ARG_ERR << "--expdata=[data file path] requires values."
	    << endl;
    throw( 2 );
  }
  
  for( int i = 0; i < path.size(); i++ ){
    try {
#ifdef DEBUG_FITTING
      ARG_LOG << "Load ExpData File : " << path[ i ] << endl;
#endif 
      expr_.push_back( new ExpData( path[ i ] ) );
    } 
    catch( int error ){
      ARG_ERR << "catch error (" << error << ")" << endl;
      throw( error );
    }
  }
  
  if( expr_.size() == 0 ){
    ARG_ERR << "Fail to create any ExpData objects" << endl;
    throw( 3 );
  }
  
  // reset variables in ExpData
  if( ! errorDirect_ && ! scaleDirect_ )
    for( int iexp = 0; iexp < expr_.size(); iexp++ )
      for( int i = 0; i < expr_[ iexp ]->data().size(); i++ ){

	Exp::DataSet& dset = expr_[ iexp ]->data()[ i ];
	
        if( ! Arguments::ref().hasOpt( "LagMultM" ) ){
	  dset.emid()     = -1;
	  dset.emNumber() =  0;
	  dset.ems().clear();
	}

	for( int j = 0; j < dset.size(); j++ ){

	  Exp::DataPoint& p = dset[ j ];

	  if( ! Arguments::ref().hasOpt( "LagMultM" ) ){
	    p.model()      = 0.0;
	    p.modelError() = 0.0;
	  }
	  p.fitted()     = false;
	}
      }
  
#ifdef DEBUG_FITTING
  for( int i = 0; i < expr_.size(); i++ ) expr_[ i ]->summary( ARG_LOG );
#endif 
  
}

void Fitting::config_param() throw( int ) {
  
  Utility::Arguments& args = Utility::Arguments::ref();
  
  // ------------------------------------------------------------ //
  //      PDF, FF parameterizations and Data Normalization        //
  // ------------------------------------------------------------ //
  //   check what should be fitted.
  toFit_ = args.get( "fit", ",", toFit_ );
#ifdef DEBUG_FITTING
  ARG_LOG << "-- objects to be fitted: " << flush;
  for( int i = 0 ; i < toFit_.size(); i++ ){
    ARG_LOG << setw(8) << toFit_[ i ] << flush;
  }
  ARG_LOG << endl;
#endif
  
  //   prepare fragmentation function and register to TMinuit    
  //   preparation  : need options like --FF,DSSFF,...
  //   registration : need options like --fit=pion,kaon,...
#ifdef DEBUG_FITTING
  ARG_LOG << "-- create FF for the given options:" << endl;
#endif
  int nff = 0;
  
  if( ( pion_   = this->ff( QCD::Flavor::PIp ) ) != NULL ) {
    if( difference_ && 
	find( toFit_.begin(), toFit_.end(), "pion" ) != toFit_.end() ) {
      o_pion_ = this->ff( QCD::Flavor::PIp, true );
      d_pion_ = new Evolution::KernelDiff( pion_, o_pion_ );
    }
    nff++;
  }
  
  if( ( kaon_   = this->ff( QCD::Flavor::KAp ) ) != NULL ) {
    if( difference_ && 
	find( toFit_.begin(), toFit_.end(), "kaon" ) != toFit_.end() ) {
      o_kaon_ = this->ff( QCD::Flavor::KAp, true );
      d_kaon_ = new Evolution::KernelDiff( kaon_, o_kaon_ );
    }
    nff++;
  }
  
  if( ( proton_ = this->ff( QCD::Flavor::PRp ) ) != NULL ) {
    if( difference_ && 
	find( toFit_.begin(), toFit_.end(), "proton" ) != toFit_.end() ) {
      o_proton_ = this->ff( QCD::Flavor::PRp, true );
      d_proton_ = new Evolution::KernelDiff( proton_, o_proton_ );
    }
    nff++;
  }
  
  if( ( rest_   = this->ff( QCD::Flavor::REp ) ) != NULL ) {
    if( difference_ && 
	find( toFit_.begin(), toFit_.end(), "rest" ) != toFit_.end() ) {
      o_rest_ = this->ff( QCD::Flavor::REp, true );
      d_rest_ = new Evolution::KernelDiff( rest_, o_rest_ );
    }
    nff++;
  }
  
  if( nff > 1 && ( hadron_ = new Evolution::KernelSum() ) != NULL ){
#ifdef DEBUG_FITTING
    ARG_LOG << "-- create FF for the charged hadron" << endl;
#endif
    
    try {
      
      if( pion_ ) { 
#ifdef DEBUG_FITTING
	ARG_LOG << "-- add pion to hadron FF" << endl;
#endif
	hadron_->add( pion_ );
      }
      
      if( kaon_ ) {
#ifdef DEBUG_FITTING
	ARG_LOG << "-- add kaon to hadron FF" << endl;
#endif
	hadron_->add( kaon_ );
      }
      
      if( rest_ ) {
#ifdef DEBUG_FITTING
	ARG_LOG << "-- add rest to hadron FF" << endl;
#endif
	hadron_->add( rest_ );
      }
      
      if( proton_ ) {
#ifdef DEBUG_FITTING
	ARG_LOG << "-- add proton to hadron FF" << endl;
#endif
	hadron_->add( proton_ );
      }
    }
    
    catch( int error ){
      ARG_ERR << "catch errror (" << error << ")" << endl;
      throw( error );
    }
  }
  
  //   prepare PDFs and register to TMinuit    
  //   preparation  : need options like --MSTW,DSSV,...
  //   registration : need options like --fit=unpolPDF,polPDF...
  this->unpolPDF();
  this->polPDF();
  
  //   check data set to be re-normalzied
  if( find( toFit_.begin(), toFit_.end(), "norm" ) != toFit_.end() ){
    this->config_data_renormalization();
  }
  
  //   check if there is at least one Evolution::Kernel is set.
  if( this->kernels().size() == 0 && dataNormalization_ == false ){
    ARG_ERR << "You must specify fragmentation func to be fit" << endl;
    ARG_ERR << "\t--fit=pion,kaon,rest,unpolPDF,polPDF,norm" << endl;
    ARG_ERR << "\t             (separated with \",\")" << endl;
    throw( 2 );
  }
  
}

void Fitting::config_data_renormalization(){
  for( int ie = 0; ie < expr_.size(); ie++ ){
    vector< DataSet >& data = expr_[ ie ]->data();
    for( int i = 0; i < data.size(); i++ ){
      if( data[ i ].norm() != 0.0 && data[ i ].ncstat() != "fixed" ) 
	rescaledData_.push_back( &( data[ i ] ) );
    }
    if( rescaledData_.size() > 0 ){
      this->defineParameters();
      dataNormalization_ = true;
#ifdef DEBUG_FITTING
      ARG_LOG << "Enabled data Normalization fitting:" << endl;
      for( int i = 0; i < rescaledData_.size(); i++ ){
	ARG_LOG 
	  << setw( 6 ) << i << setw( 28 ) << rescaledData_[ i ]->info()
	  << endl;
      }
#endif
    }
  }
}

void Fitting::config_local(){
  
  Utility::Arguments& args = Utility::Arguments::ref();
  
  // ------------------------------------------------------------ //
  //                        set verbose                           //
  // ------------------------------------------------------------ //
  this->verbose( ( args.hasOpt( "verbose" ) ? true : false ) );
  
  // ------------------------------------------------------------ //
  //                User function configuration                   //
  // ------------------------------------------------------------ //
  postErrMatrix_ = args.hasOpt( "postErrorMatrix" ); 
  
  // ------------------------------------------------------------ //
  //                     DataCache configuration                  //
  // ------------------------------------------------------------ //
  Transform::DataCache::maxsize_ = 
    args.get( "DataCacheMaxSize", Transform::DataCache::maxsize_ );
  
  Transform::DataCache2::maxsize_ = 
    args.get( "DataCache2MaxSize", Transform::DataCache2::maxsize_ );
  
  // ------------------------------------------------------------ //
  //                  Grid integration max depth                  //
  // ------------------------------------------------------------ //
  Transform::GridIntegration::maxDepth_ =
    args.get( "gridMaxDepth", Transform::GridIntegration::maxDepth_ );
  
  // ------------------------------------------------------------ //
  //                    calculate in two thread                   //
  // ------------------------------------------------------------ //
  useThread_ = args.hasOpt( "thread" );
  
}

void Fitting::minuitCmds( std::string cmds ){
  
  ifstream ifs( cmds.c_str() );
  
  if( ifs.good() ){      // it seems that given option is file name....
    int size = 1024;
    char buff[ size ];
    while( ifs.getline( buff, size ) ){
      if( buff[ 0 ] == '*' || buff[ 0 ] == '#' || buff[ 0 ] == '\0' ) 
	continue; // ignore comments
      commands_.push_back( buff );
    }
  } else {
    int start = 0;
    int end   = 0;
    do {
      end = cmds.find( '|', start );
      int size = ( end == string::npos ? cmds.size() : end ) - start;
      commands_.push_back( cmds.substr( start, size ) );
      start = end + 1;
    } while( end != string::npos );
  }
}

void Fitting::dumpParameters(){
  
  // it looks like one has to get parameter again....

  // ------------------------------------------------------------ //
  //                         QCD::Parameter                       //
  // ------------------------------------------------------------ //
  for( int i = 0; i < params_.size(); i++ ){
    this->GetParameter( i, params_[ i ]->value(), params_[ i ]->error() ); 
  }
  
  // ------------------------------------------------------------ //
  //                         Exp::DataSet                         //
  // ------------------------------------------------------------ //
  for( int i = 0; i < rescaledData_.size(); i++ ){
    this->GetParameter( i + params_.size(), 
			rescaledData_[ i ]->ncoef(),
			rescaledData_[ i ]->ncerr() );
  }

  this->updatePdf();
  
  // ------------------------------------------------------------ //
  //                 Dump parameter information                   //
  // ------------------------------------------------------------ //
  if( dumpParamLevel_ <= istat_ ){
    for( int i = 0; i < kernel_.size(); i++ ){
      PdfParametrization *init = kernel_[i]->initialDistribution();
      ofstream ofs( ( prefix_ + this->ofilename( i ) ).c_str() );
      ofs << *init << endl;
      this->summary_basic( i, ofs );      
      ofs.close();
    }
  }  
}

void Fitting::baseinfo_summary( std::ostream& os ){
  
  os << "Number of iterations:" << endl;
  //  for( int i = 0; i < iflags_.size(); i++ )
  os  << "  iflag = " << setw(2) << 4 << " : " 
      << setw(10) << iflags_[ 4 ] << endl;
  
  os << "  Total chi^2 = " 
     << setw(10) << chi2_ << endl;
  
  os << "  data #      = " 
     << setw(10)  << this->numberOfData()   << endl;
  
  os << "  param #     = " 
     << setw(10)  << this->GetNumFreePars() << endl;
  
  os << "  red. chi^2  = " 
     << setw(10)  << chi2_ / this->dof()    << endl;
  
  os << "  istat       = " 
     << setw(10) << istat_ 
     << "  (" << Fitting::emLevelTag( istat_ ) << ")" << endl;
  
}

void Fitting::em_assignment( std::ostream& os ){

  Arguments& args = Arguments::ref();

  if( emflag_ ){

    //
    QCD::PdfErrMatrix em = this->errorMatrix();
    em.setprec( args.get( "errMXout_prec", 3 ) );

    if( istat_ >= emlevel_ ){
      os << "#----------------- error matrix ------------------------" << endl;
      os << em << endl;
    }
    
    if( args.hasOpt( "errMXout" ) || args.hasOpt( "NerrMXout" ) ){

      ostringstream ost; 

      ost << "errorMX_" << prefix_ << ".dat";
      ofstream ofs_( ost.str().c_str() );
      ofs_ << em << endl;
      ost.str( "" ); ost.clear();

      if( args.hasOpt( "NerrMXout" ) ){ 

	em.setnormflag( true );
	ost << "NerrorMX_" << prefix_ << ".dat";
	ofstream nofs_( ost.str().c_str() );
	nofs_ << em << endl;
	ost.str( "" ); ost.clear();
	em.setnormflag( false );

      }
    }

  }
}

void Fitting::summary( std::ostream& os ){
  
  Utility::Arguments& args = Utility::Arguments::ref();

  this->baseinfo_summary( os );
  
  this->em_assignment( os );

  // ------------------- model data comparisons ---------------------- //
  this->data_summary( os );
  
  // ------------------- print out present parameter value ---------- //
  this->dumpParameters();
}

void Fitting::runCommands() {
  for( int i = 0; i < commands_.size(); i++ ){
    this->Command( commands_[ i ].c_str() );
  }
}

Int_t Fitting::Eval( Int_t npar, Double_t* grad, 
		     Double_t& fval, Double_t* par, Int_t flag ){
  
  // increment iflag counter
  iflags_[ flag ]++;
  
  // prepare error status variable
  int err( 0 );
  
#ifdef DEBUG_FITTING
  /*
    for( int i = 0; i < par_.size(); i++ ){
    if( *( par_[ i ] ) != par[ i ] ){
    ARG_LOG
    << "   ***"
    << setw(10) << nam_[ i ] 
    << setw(16) << *( par_[ i ] )
    << " --> "
    << setw(16) << par[ i ]
    << endl;
    }
    }
  */
#endif 
  
  // initialization if( flag == 1 ){}
  
  // calculate first derivatives of FVAL if( flag == 2 ){}
  
  // mirroring Minuit parameters to initial distributions
  this->mirror( npar, par );
  //  this->mirror();
  
#ifdef DEBUG_FITTING
  Utility::Timer ltimer;
  ARG_LOG
    << setw( 5 ) << iflags_[ 4 ] 
    << " [" 
    << ltimer.local().substr(  4, 3 ) << "/" 
    << ltimer.local().substr(  8, 2 ) << " "
    << ltimer.local().substr( 11, 8 ) << "]" << flush;
  ltimer.start();
#endif // DEBUG_FITTING
  
  // chi2 calculation
  fval = chi2_ = this->chi2();
  
  // for Lagrange Multiplier method
  if( multiplier_ ) fval += lambda_ * this->referenceFunction();
  
#ifdef DEBUG_FITTING
  // stop timer
  ltimer.stop();
#endif   
  
  // call postLoop method
  this->postLoop();
  
#ifdef DEBUG_FITTING
  
  pvalue_.k( this->dof() );
  
  ARG_LOG 
    << setw(12)  << fval
    << " /"
    << setw(4)  << this->dof()
    << " ="
    << setw(10)  << fval / this->dof() 
    << " ("
    << setw(12) << setprecision(4) << pvalue_( fval )
    << ")"
    << flush;
  
  if( floor( ltimer.check( Utility::Timer::Min ) ) > 0 ) {
    ARG_LOG
      << setw( 5 ) << floor( ltimer.check( Utility::Timer::Min ) )
      << " m "
      << flush;
  } else if( floor( ltimer.check( Utility::Timer::Sec ) ) > 0  ) {
    ARG_LOG
      << setw( 5 ) << floor( ltimer.check( Utility::Timer::Sec ) )
      << " s "
      << flush;
  } else {
    ARG_LOG
      << setw( 5 ) << floor( ltimer.check( Utility::Timer::Msec ) )
      << " ms"
      << flush;
  }
  
  ARG_LOG 
    << setw( 5 ) << 
    floor( ltimer.check( Utility::Timer::Msec ) / this->numberOfData() )
    << " ms/p" 
    << flush;
  
  if( difference_ ){
    if( d_pion_ ) ARG_DETAIL << setw(6) << d_pion_->dglapMatrix().cache_size() << flush;
    if( d_kaon_ ) ARG_DETAIL << setw(6) << d_kaon_->dglapMatrix().cache_size() << flush;
    if( d_proton_ ) ARG_DETAIL << setw(6) << d_proton_->dglapMatrix().cache_size() << flush;
    if( d_rest_ ) ARG_DETAIL << setw(6) << d_rest_->dglapMatrix().cache_size() << flush;
    if( d_updf_ ) ARG_DETAIL << setw(6) << d_updf_->dglapMatrix().cache_size() << flush;
    if( d_ppdf_ ) ARG_DETAIL << setw(6) << d_ppdf_->dglapMatrix().cache_size() << flush;
  } else {
    for( int i = 0; i < kernel_.size(); i++ ) {
      ARG_DETAIL << setw(6) << kernel_[ i ]->dglapMatrix().cache_size() << flush;
    }
  }
  ARG_LOG << endl;
  
#endif // DEBUG_FITTING
  
  // ---------------------------------------------------------------------------- //
  //                    Dump information of this iteration                        //
  // ---------------------------------------------------------------------------- //
#ifndef DEBUG_FITTING
  int niter = 100;
  int n_100 = niter * ( iflags_[ 4 ]  / niter );
  if( n_100 == iflags_[ 4 ] )
#endif // DEBUG_FITTING
    {
      // save present parameterization ( for future investigation )
      for( int i = 0; i < kernel_.size(); i++ ){
	
	// cast to DGLAP base evolution kernel
	ostringstream ostr;
	ostr << "last_params_" << i << ".xml";
	ofstream ofs( ostr.str().c_str() );
	
	ofs << *( kernel_[ i ]->initialDistribution() ) << endl;
	this->summary_basic( i, ofs );      
      }
      
#ifndef DEBUG_FITTING
      if( dataNormalization_ ){
#endif // DEBUG_FITTING
	for( int i = 0; i < expr_.size(); i++ ){
	  ostringstream ostr;
	  ostr << "last_expdata" ;
	  if( expr_.size() > 1 ) ostr << "_" << i ;
	  ostr << ".xml";
	  ofstream ofs( ostr.str().c_str() );
	  ofs << *( expr_[ i ] )<< endl;
	}
#ifndef DEBUG_FITTING
      }
#endif // DEBUG_FITTING
      
    }
  
  // fit was converged. do something else... if( flag == 3 ){ }

  // IMPLICIT chi2 scaling 
  fval *= chi2scale_;
  
  return 0;
}

void Fitting::defineParameters(){
  
  // it will re-define parameter if there paramterers already exist...
  
  // ------------------------------------------------------------ //
  //                         QCD::Parameter                       //
  // ------------------------------------------------------------ //
  for( int i = 0; i < params_.size(); i++ ){
    
    Parameter* param = params_[ i ];
    
    this->DefineParameter( i, nam_[i].c_str(), 
			   param->value(), param->error(),
			   param->lower(), param->upper() );
    
    // if error is 0.0, TMinuit treat as constant value.
    if( param->error() != 0.0 && param->fixed() == true ) 
      this->FixParameter( i );
  }
  
  // ------------------------------------------------------------ //
  //                         Exp::DataSet                         //
  // ------------------------------------------------------------ //
  for( int i = 0; i < rescaledData_.size(); i++ ){
    this->DefineParameter( params_.size() + i,
			   rescaledData_[ i ]->info().c_str(),
			   rescaledData_[ i ]->ncoef(),
			   rescaledData_[ i ]->ncerr(),
			   -1.0E+4, 1.0E+4 );
    
    // if error is 0.0, TMinuit treat as constant value.
    if( rescaledData_[ i ]->ncerr() != 0.0 && 
	rescaledData_[ i ]->ncstat() == "fixed" ) 
      this->FixParameter( i );
  }
  
}

void Fitting::setModel( Xsection::Xsec *m ){
  
  if( m == NULL ) return; // if NULL pointer is given, do nothing.
  
  // loop over all PdfBase object in Xsec model to handle 
  // in TMinuit as fitting parameter
  vector< KernelBase* >& vevo  = m->xsecComp()->vevo();
  for( int i = 0; i < vevo.size(); i++ ){
    // cast to DGLAP base evolution kernel
    Kernel* k = dynamic_cast< Kernel* >( vevo[ i ] );
    if( k ) this->set( k );
  }
  
}

void Fitting::set( Kernel* k ){
  kernel_.push_back( k );
  vector< PdfBase* >& pdfs = k->initialDistribution()->pdfs();
  for( int i = 0; i < pdfs.size(); i++ ) this->insert( pdfs[ i ] );
  
  // assing parameters in PdfBase as TMinuit fitting parameter
  this->defineParameters();
  
}

void Fitting::mirror( const int& n, double* par ){
  
  // ------------------------------------------------------------ //
  //                         QCD::Parameter                       //
  // ------------------------------------------------------------ //
  for( int i = 0; i < params_.size(); i++ ) {
    params_[ i ]->value() = par[ i ];
  }
  
  // ------------------------------------------------------------ //
  //                         Exp::DataSet                         //
  // ------------------------------------------------------------ //
  for( int i = 0; i < rescaledData_.size(); i++ )
    rescaledData_[ i ]->ncoef() = par[ params_.size() + i ];
  
  this->updatePdf();
}

void Fitting::mirror(){
  
  // ------------------------------------------------------------ //
  //                         QCD::Parameter                       //
  // ------------------------------------------------------------ //
  for( int i = 0; i < params_.size(); i++ ){
    this->GetParameter( i, params_[ i ]->value(), params_[ i ]->error() ); 
  }
  
  // ------------------------------------------------------------ //
  //                         Exp::DataSet                         //
  // ------------------------------------------------------------ //
  for( int i = 0; i < rescaledData_.size(); i++ ){
    this->GetParameter( i + params_.size(), 
			rescaledData_[ i ]->ncoef(),
			rescaledData_[ i ]->ncerr() );
  }
  
  this->updatePdf();
}

void Fitting::updatePdf(){
  
  // call update process for each PDF object
  for( int i = 0; i < pdf_.size(); i++ ) pdf_[ i ]->update();
  
  // clear cached information in KernelBase object.
  for( int i = 0; i < kernel_.size(); i++ ) kernel_[ i ]->clearCache();
  
  if( ffFit_ == true && hadron_ != NULL ) hadron_->clearCache();
  
  // difference mode
  if( difference_ ){
    if( d_pion_ ) d_pion_->clearCache();
    if( d_kaon_ ) d_kaon_->clearCache();
    if( d_rest_ ) d_rest_->clearCache();
    if( d_proton_ ) d_proton_->clearCache();
    if( d_updf_ ) d_updf_->clearCache();
    if( d_ppdf_ ) d_ppdf_->clearCache();
  }
  
  // post update......
  this->postUpdate();
  
  // ---------------------------------------------------------------------------- //
  //                    Print out present parameterization                        //
  // ---------------------------------------------------------------------------- //
#ifndef DEBUG_FITTING
  int niter = 100;
  int n_100 = niter * ( iflags_[ 4 ]  / niter );
  if( n_100 == iflags_[ 4 ] )
#endif // DEBUG_FITTING
    {
      // save present parameterization ( for future investigation )
      for( int i = 0; i < kernel_.size(); i++ ){
	// cast to DGLAP base evolution kernel
	ostringstream ostr;
	ostr << "present_params_" << i << ".xml";
	ofstream ofs( ostr.str().c_str() );
	ofs << *( kernel_[ i ]->initialDistribution() ) << endl;
      }
    }
}

void Fitting::summary_basic( const int& i, std::ostream& os ){
  
  os << "<!--    Fitting status" 
     << setw(27) 
     << string( "[" + timer_.local().substr( 0, 24 ) + "]" ) << endl;
  
  os << "   original configuration file  ["
     << kernel_[ i ]->initialDistribution()->path()
     << "]" << endl;
  
  os << endl;
  
  Utility::Arguments& args = Utility::Arguments::ref();
  os << " ===========================================================" << endl;
  os << "   Arguments of the operation" << endl;
  os << "     command: " << args.command() << endl;
  
  map< string, string > opts = args.options();
  for( map< string, string >::iterator itr = opts.begin();
       itr != opts.end();
       itr++ ){
    os << "           " << itr->first << flush;
    if( itr->second != "" && itr->second != Arguments::NO_VALUE ) 
      os << "=" << itr->second << flush;
    os << endl;
  }
  
  vector< string > argv = args.arguments();
  for( int i = 0; i < argv.size(); i++ ){
    os << "           " << argv[ i ] << endl;
  }
  
  os << " ===========================================================" << endl;
  double sec = timer_.check( Utility::Timer::Sec );
  double hour = floor( sec / 3600.0 );
  sec -= hour * 3600.0;
  double min  = floor( sec / 60.0 ); 
  sec -= min * 60.0;
  
  os << "   elapsed time        " 
     << setw( 10 ) << hour << ":"
     << setw( 2 ) << min  << ":"
     << setw( 2 ) << floor( sec ) << endl;
  
  os << "   number of iteration " 
     << setw(16) << iflags_[ 4 ] << endl;
  
  os << "   total chi2          " 
     << setw(16) << chi2_ << endl;
  
  os << "   number of data point" 
     << setw(16) << this->numberOfData() << endl;
  
  os << "   number of free param" 
     << setw(16) << this->GetNumFreePars() << endl;
  
  os << "   d.o.f               " 
     << setw(16) << this->dof() << endl;
  
  os << "   reduced chi2        " 
     << setw(16) << chi2_ / this->dof() << endl;
  
  os << endl;
  this->data_summary( os );
  os << endl;
  
  os << "-->" << endl;
}

void Fitting::insert( PdfBase *pdf ){
  for( PdfBase::iterator i = pdf->begin(); i != pdf->end(); i++ ){
    Parameter& p = i->second;
    // reset error matrix elements
    if( ! errorDirect_ && 
	! Arguments::ref().hasOpt( "LagMultM" ) ){
    p.ems().clear();
    p.emid() = -1;
    }

    // If Parameter is defined as fix, just ignore....
    // ( in which fixed, add, and multi states are included )
    // This case, emid will be -1. So no error matrix will be assigned.
    if( p.fixed() ) continue; 

    // register pointer to Parameter object to control by Minuit
    params_.push_back( &p );
    
    nam_.push_back( pdf->name() + "-"+  p.name() );
  }
  pdf_.push_back( pdf );
}

QCD::PdfErrMatrix Fitting::errorMatrix(){
  
  // get number of free parameters
  int nfree = this->GetNumFreePars();
  
  // if there is no free parameter, just return.
  if( nfree == 0 ) return QCD::PdfErrMatrix();
  
  // assign error matrix id and parameter id
  vector< int > index( 0 );
  for( int i = 0; i < params_.size(); i++ ){
    if( params_[ i ]->free() && 
	params_[ i ]->error() != 0.0 ) index.push_back( i );
  }
  
  // assign error matrix id and normalization parameter id
  for( int i = 0; i < rescaledData_.size(); i++ ){
    if( rescaledData_[ i ]->ncerr() != 0.0 &&
	rescaledData_[ i ]->ncstat() != "fixed" ) 
      index.push_back( i + params_.size() );
  }
  
  if( nfree != index.size() ){
    ARG_ERR << "Inconsistent number of free parameters: "
	    << setw(3) << nfree << " vs "
	    << setw(3) << index.size() 
	    << endl;
    for( int i = 0; i < index.size(); i++ ) 
      ARG_ERR << setw(3) << index[i] << flush;
    ARG_ERR << endl;
    return QCD::PdfErrMatrix();
  }
  
#ifdef DEBUG_FITTING
  ARG_LOG << "Error matrix assigment:" << endl;
  for( int i = 0; i < index.size(); i++ ) 
    ARG_LOG << setw(2) << i << " --> "
	    << setw(3) << index[i] << endl;
#endif
  
  // memory allocation to copy error matrix content from Minuit
  double *buff;
  if( ( buff = 
	( double* ) malloc( sizeof( double[ nfree ][ nfree ] ) ) ) 
      == NULL ) {
    ARG_ERR << "fail to allocate memory." << endl;
    return QCD::PdfErrMatrix();
  }

  // copy error matrix from TMinuit to local buffer
  this->mnemat( (double*) buff, nfree );
  
  // ----------------------------------------------------------- //
  //        generating error matrix ID with radom number         //
  // ----------------------------------------------------------- //
  srandom( time( NULL ) );                       // set random seed
  QCD::PdfErrMatrix em( random() );
  
  // resize error matrix with nunber of paramters in the fitting
  int n = params_.size() + rescaledData_.size();
  em.resetMatrixSize( n );
  
  // copy error matrix element from buffer to STL vector.
  // assign only in fitted parameter sector ( index[ i ] )
 for( int i = 0; i < nfree; i++ )
    for( int j = 0; j < nfree; j++ ) {
      em[ index[ i ] ][ index[ j ] ] = *( buff + i * nfree + j );
    }
  
  // free allocated memory
  free( buff );
  
  // ----------------------------------------------------------- //
  //      assign error matrix elements to Parameter objects        //
  // ----------------------------------------------------------- //
  for( int i = 0; i < params_.size(); i++ ){
    Parameter& par = *( params_[ i ] );
    par.emNumber() = em.ID();
    par.ems()      = em[ ( par.emid() = i ) ];
    //    em.info()[ par.emid() ] = par.name();
    em.info()[ par.emid() ] = nam_[ i ];
  }
  
  // ----------------------------------------------------------- //
  //      assign error matrix elements to DataSet objects        //
  // ----------------------------------------------------------- //
  for( int i = 0; i < rescaledData_.size(); i++ ){
    Exp::DataSet& data = *( rescaledData_[ i ] );
    data.emNumber() = em.ID();
    data.ems() = em[ ( data.emid() = i + params_.size() ) ];
    em.info()[ data.emid() ] = data.info();
  }
  
  return em;
}


TH2* Fitting::contour( int par1, int par2, int n ){
  
  if( par1 == par2 ) {
    return NULL;
  }
  
  if( ! ( par1 < params_.size() ) ) return NULL;
  if( ! ( par2 < params_.size() ) ) return NULL;
  
  if( params_[ par1 ]->error() == 0.0 || params_[ par2 ]->error() == 0.0 ){
    return NULL;
  }
  
  if( contparam_.size() > 0 && 
      ( find( contparam_.begin(), contparam_.end(), par1 + 1 ) == 
	contparam_.end() || 
	find( contparam_.begin(), contparam_.end(), par2 + 1 ) == 
	contparam_.end() ) 
      ) {
    return NULL;
  }
  
  ostringstream ostr;
  
  ostr << nam_[ par1 ] << " vs " << nam_[ par2 ];
  string title = ostr.str();
  
  ostr.str( "" );
  ostr << "corr" << par1 << "-" << par2 ;
  string name = ostr.str();
  
  double nsigma = 1.0;
  
  // backup all of the existing parameters
  vector< double > par( params_.size() );
  vector< double > err( params_.size() );
  for( int i = 0; i < params_.size(); i++ ) {
    par[ i ] = params_[ i ]->value();
    err[ i ] = params_[ i ]->error();
  }
  
  // keep the parameters and their errors
  double& p1 = params_[ par1 ]->value();
  double& p2 = params_[ par2 ]->value();
  double e1 = params_[ par1 ]->error();
  double e2 = params_[ par2 ]->error();
  
  // set controled parameter to fixed state
  params_[ par1 ]->status( QCD::Parameter::FIXED );
  params_[ par2 ]->status( QCD::Parameter::FIXED );
  
  ARG_LOG << "Create contour plot for parameters "  << endl;
  
  ARG_LOG << "\t" 
	  << setw( 10 ) << nam_[ par1 ]
	  << setw( 10 ) << p1 << " +- " 
	  << setw( 2 ) << nsigma << " * " << setw( 10 ) << e1 << endl;
  
  ARG_LOG << "\t" 
	  << setw( 10 ) << nam_[ par2 ]
	  << setw( 10 ) << p2 << " +- " 
	  << setw( 2 ) << nsigma << " * " << setw( 10 ) << e2 << endl;
  
#ifdef DEBUG_FITTING
  ARG_LOG << "\tcreate histogram: " << name.c_str() << endl;
#endif //

  TH2 *h2 = 
    new TH2F( name.c_str(), title.c_str(), 
	      n, p1 - nsigma * e1, p1 + nsigma * e1, 
	      n, p2 - nsigma * e2, p2 + nsigma * e2 ); 
  
  if( h2 == NULL ) {
    ARG_ERR << "Fail to create histogram: " << name.c_str() << endl;
    return NULL;
  }
  
  h2->GetXaxis()->SetTitle( nam_[ par1 ].c_str() );
  h2->GetYaxis()->SetTitle( nam_[ par2 ].c_str() );
  
  // set contour level
  double chi2 = this->chi2();
  h2->SetContour( 20 );
  double lstep = 0.5; 
  for( int i = 0; i < 20; i++ ) {
    h2->SetContourLevel( i, chi2 + pow( lstep * i, 2 ) );
  }
  
  for( int ip1 = 1; ip1 <= h2->GetXaxis()->GetNbins(); ip1++ ){
    
    ARG_LOG << setw(3) << ip1 << "/" 
	    << setw(3) << h2->GetXaxis()->GetNbins() 
	    << "  "
	    << flush;
    
    for( int ip2 = 1; ip2 <= h2->GetYaxis()->GetNbins(); ip2++ ){
      
      // set parameters to bin center, and update PDFs
      p1 = h2->GetXaxis()->GetBinCenter( ip1 );
      p2 = h2->GetYaxis()->GetBinCenter( ip2 );
      
      // re-define the parameters
      this->updatePdf();
      this->defineParameters();
      this->Migrad();
      //      this->Command( "SIMPLEX" );
      
      // fill 2D histogram
      h2->Fill( p1, p2, chi2_ );
      ARG_LOG << "*" << flush;
    }
    
    ARG_LOG << " done" << endl;
  }
  
  // restore parameters to the original values
  for( int i = 0; i < params_.size(); i++ ) {
    params_[ i ]->value() =  par[ i ];
    params_[ i ]->error() =  err[ i ];
  }
  
  // set controled parameter to free state
  params_[ par1 ]->status( QCD::Parameter::FREE );
  params_[ par2 ]->status( QCD::Parameter::FREE );
  
  this->updatePdf();
  this->defineParameters();  
  
  return h2; 
}

void Fitting::contour( const int& n ) {
  
  // it looks like one has to get parameter again....
  int total = 0;
  for( int i = 0; i < params_.size(); i++ ){

    this->GetParameter( i, params_[ i ]->value(), params_[ i ]->error() ); 

    if( params_[ i ]->error() != 0.0 ) {
      if( contparam_.size() == 0 || 
	  find( contparam_.begin(), contparam_.end(), i + 1 ) != 
	  contparam_.end() ) {
	
	ARG_LOG 
	  << setw(12) << nam_[ i ] << " : "
	  << setw(12) << params_[ i ]->value() << " +- "
	  << setw(12) << params_[ i ]->error() << endl;
	
	total++;
      }
    }
  }
  total = total * ( total - 1 ) / 2;
  
  ARG_LOG 
    << " will be used to make contour plot."
    << endl
    << endl;

  if( contFileName_ == "" ) {
    ARG_ERR
      << "\toutput file must be specified with --contour option"
      << endl;
    return;
  }
  
  // set print level to quiet
  this->SetPrintLevel( -1 );
  
  ARG_LOG << "create ROOT file " << contFileName_ << endl;
  TFile tfile( contFileName_.c_str(), "RECREATE" );
  
  int count = 0;
  
  Utility::Timer ltimer;
  ltimer.start();
  double start = ltimer.check( Utility::Timer::Min );
  for( int i = 0; i < params_.size(); i++ )
    for( int j = i + 1; j < params_.size(); j++ ){
      
      double sec = ltimer.check( Utility::Timer::Sec );
      this->contour( i, j, n );
      if( floor( ltimer.check( Utility::Timer::Sec ) - sec  ) > 0 ){
	count++;
	double min = ltimer.check( Utility::Timer::Min ) - start;
	ARG_LOG << "*** Time elapsed:         " 
		<< setw( 4 ) << floor( min ) << " min"
		<< "  (" << setw(2) << count << " finished)" << endl;
	
	ARG_LOG << "*** Expected remain time: " << flush;
	
	double rest = min * ( total - count ) / count;
	double h = floor( rest / 60.0 );
	ARG_LOG << setw( 3 ) << h << ":" << flush;
	ARG_LOG << setw( 2 ) << floor( rest - 60.0 * h ) << " "
		<< "  (" << setw(2) << total - count  << " to be done)\n"
		<< endl;
      }
    }
  
  tfile.Write();
  tfile.Close();
  
  // set print level to quiet
  this->SetPrintLevel( Utility::Arguments::ref().get( "MinuitPrintLevel", 0 ) ); 
  
}

TGraph* Fitting::lagrangeMultiplier( const int& n, 
				     const double& min, const double& max ){
  TGraph *g = new TGraph( n );
  double step = ( max - min ) / n;
  this->multiplier( true );
  for( int i = 0; i < n; i++ ){
    this->multiplier( min + step * i );  // set lambda value
    this->Migrad();                      // find local minima
    //    this->Command( "SIMPLEX" );
    g->SetPoint( i, chi2_, reffnc_ );
  }
  this->multiplier( false );
  return g;
}

bool Fitting::startMinuit() {

  Utility::Arguments& args = Utility::Arguments::ref();
  
#ifdef DEBUG_FITTING
  ARG_LOG 
    << "*****************************************************************\n"
    << "*                       Start Minimization                      *\n"
    << "*****************************************************************\n"
    << endl;
#endif 
  
  if( args.hasOpt( "Minuit" ) ) {
    this->runCommands();
  } else {
    this->Migrad();
  }
  
#ifdef DEBUG_FITTING
  ARG_LOG
    << "*****************************************************************\n"
    << "*                       End Minimization                        *\n"
    << "*****************************************************************\n"
    << endl;
#endif
  // get MINUIT status
  this->mnstat( fmin_, fedm_, errdef_, npari_, nparx_, istat_ );
  this->summary( ARG_LOG );
  
}

bool Fitting::start() {
  
  Utility::Arguments& args = Utility::Arguments::ref();
  
  // --------------------------------------------------------------- //
  //              call MINUIT for the chi2 minimization              //
  // --------------------------------------------------------------- //
  if( ! errorDirect_ && ! scaleDirect_ && ! skipfit_ ) 
    this->startMinuit();

  if( skipfit_ ) this->skipmanip();

  // --------------------------------------------------------------- //
  //              call postFit methdo in any case                    //
  // --------------------------------------------------------------- //
#ifdef DEBUG_FITTING
  ARG_LOG << "  --- call postFit method" << endl;
#endif  
  for( int ie = 0; ie < expr_.size(); ie++ )
    for( int i = 0; i < expr_[ ie ]->data().size(); i++ ) 
      this->postFit( expr_[ ie ]->data()[ i ] );
  
  // --------------------------------------------------------------- //
  //              call postErrorMatrix methdo                        //
  // --------------------------------------------------------------- //
  if( errorDirect_ ||                        // --errordirect
      ( postErrMatrix_ && istat_ >= emlevel_ && emflag_ ) ) {
#ifdef DEBUG_FITTING
    ARG_LOG << "  --- call postErrorMatrix method" << endl;
#endif
    for( int ie = 0; ie < expr_.size(); ie++ )
      for( int i = 0; i < expr_[ ie ]->data().size(); i++ ){ 
	this->postErrorMatrix( expr_[ ie ]->data()[ i ] );
#ifdef DEBUG_FITTING
	ARG_LOG << "  -- " << expr_[ ie ]->data()[ i ].info() 
		<< " done --" << endl;
#endif
      }
  }
  
  // --------------------------------------------------------------- //
  //      in case --contour given, chi2 conour will be produced      //
  // --------------------------------------------------------------- //
  if( args.hasOpt( "contour" ) ) {
#ifdef DEBUG_FITTING
    ARG_LOG << "  --- call chi2 contour calculation method" << endl;
#endif
    this->contour( args.get( "ncontour", 20 ) );
  }
  
  // --------------------------------------------------------------- //
  //      produce experimental data XML file                         //
  // --------------------------------------------------------------- //
  if( dumpParamLevel_ <= istat_ ){
    ostringstream ostr;
    ostr << "expdata";
    if( errorDirect_ || scaleDirect_ ) ostr << "_direct";
    if( emflag_ )                      ostr << "_werr";
  
    string ofilename = ostr.str();
    ofilename = args.get( "oexpdata", ofilename );
#ifdef DEBUG_FITTING
    ARG_LOG << "  --- creating " << ofilename  << endl;
#endif

    for( int i = 0; i < expr_.size(); i++ ){
      ostr.clear(); ostr.str( "" ); // clear 
      int pos = ofilename.size() - 4;
      ostr 
	<< ( ofilename.substr( pos, 4 ) == ".xml" ?
	     ofilename.substr( 0, pos ) : ofilename );
      if( expr_.size() > 1 ) ostr << "_" << i ;
      ostr << ".xml";
      ofstream ofs( ( prefix_ + ostr.str() ).c_str() );
      ofs << *( expr_[ i ] ) << endl;
    }
  }

  // --------------------------------------------------------------- //
  //      print out summary report at the end                        //
  // --------------------------------------------------------------- //
  ARG_LOG << "  --- MINUIT summary" << endl;
  this->Command( "SHOW PARameters" );
  
  ARG_LOG << "  --- Fit basic info. summary" << endl;
  this->baseinfo_summary( ARG_LOG );
  
  ARG_LOG << "  --- Fitted data summary" << endl;
  this->fit_data_summary( ARG_LOG );

  // if postFit_summary returns false, quit 'start' process here
  if( ! this->postFit_summary( ARG_LOG ) ) return true;

  return true;

}

void Fitting::fit_data_summary( std::ostream& os ){

  for( int i = 0; i < expr_.size(); i++ )
    expr_[ i ]->summary( os );
  
  int nds_( 0 );
  for( int i = 0; i < expr_.size(); i++ )
    for( int j = 0; j < expr_[ i ]->data().size(); j++ )
      //      if( expr_[ i ]->data()[ j ].enable() ) nds_++;
      nds_++;
  int fds_( rescaledData_.size() );

  os << setw( 3 ) << "" << setw( 35 )
     << "Chi2 From Rescale Params"
     << setw( 4 ) << fds_ << "/"
     << setw( 4 ) << nds_
     << setw(12) << res_chi2_
     << endl;

  os << "Final Reduced Chi2: " 
     << setw(12) << chi2_ 
     << "/ " 
     << setw(4) << this->dof() 
     << " = "
     << setw(12) <<  chi2_  / this->dof() 
     << endl;
}

bool Fitting::postFit_summary( std::ostream& os ){

  Utility::Arguments& args = Utility::Arguments::ref();

  // --------------------------------------------------------------- //
  //      print out chi2 summary for LM error consideration          //
  // --------------------------------------------------------------- //
  if( args.hasOpt( "LMerror" ) ){
    if( expr_.size() != 2 ) return false;

    int DSIND_ = args.get( "LMerror", 0 );

    vector< double > vchi2_( 0 );
    for( int i = 0; i < expr_.size(); i++ )
      vchi2_.push_back( expr_[ i ]->chiSquare() );

    //lambda value manipulation always be in the second expdata
    double tmplambda_( expr_[ 1 ]->lambda() );
    string dsetname_( expr_[ 1 ]->data()[ 0 ].info() );
    
    os << endl;
    os << setw(44) << "Name"
       << setw(8)  << "DS_ID"
       << setw(8)  << "Lambda"
       << setw(12) << "Total"
       << setw(12) << "Part1"
       << setw(12) << "Part2"
       << endl;
    os << "LM chi2: "
       << setw(35) << dsetname_
       << setw(8)  << DSIND_
       << setw(8)  << tmplambda_
       << setw(12) << chi2_
       << setw(12) << vchi2_[ 0 ]
       << setw(12) << vchi2_[ 1 ]
       << endl;
  }

  return true;
  
}

Fragmentation::Evo* Fitting::ff( const QCD::Flavor::HADRON& h,
				 const bool& original ) throw( int ) {
  
  string hadron;

  switch( h ){
    
  case QCD::Flavor::PIp:
  case QCD::Flavor::PIm: hadron = "pion"; break;
    
  case QCD::Flavor::KAp:
  case QCD::Flavor::KAm: hadron = "kaon"; break;
    
  case QCD::Flavor::REp:
  case QCD::Flavor::REm: hadron = "rest"; break;

  case QCD::Flavor::PRp:
  case QCD::Flavor::PRm: hadron = "proton"; break;

  }
  
  Utility::Arguments& args = Utility::Arguments::ref();
  vector< string > opts = Fragmentation::Evo::options( h );
  vector< string >::iterator itr = args.hasOpt( opts );
  
  if( itr == opts.end() ) {
#ifdef DEBUG_FITTING
    ARG_LOG << " ---- skip " << hadron  << endl;
#endif
    return NULL;
  } else {
#ifdef DEBUG_FITTING
    ARG_LOG << " ---- create " << *itr  << endl;
#endif
  }
  
  Fragmentation::Evo *evo = NULL;
  
  try {
    
    if( evo = new Fragmentation::Evo( args, h, *itr ) ) {
      ARG_LOG 
	<< "create FF for " << hadron 
	<< " with --" << *itr << "=" << args.getOpt( *itr ) 
	<< flush;

      if( ! original ){
	if( find( toFit_.begin(), toFit_.end(), hadron ) != toFit_.end() ) {
	  ARG_LOG << "(FIT)" << endl;
	  this->set( evo );
	  ffFit_ = true;
	} else {
	  ARG_LOG << endl;
	}
      } else {
	ARG_LOG << endl;
      }
    }
  }
  
  catch( int error ){
    ARG_ERR << "catch error(" << error << ")" << endl;
    throw( error );
  }
  
  return evo;
}

unpolPDF::Evo* Fitting::unpolPDF() throw( int ) {
  
  if( updf_ ) return updf_;
  
  Utility::Arguments& args = Utility::Arguments::ref();
  
  try {
    
    if( updf_ = new unpolPDF::Evo( args ) ){
      if( find( toFit_.begin(), toFit_.end(), "unpolPDF" ) != toFit_.end() ) {
	ARG_LOG << "(FIT)" << endl;
	this->set( updf_ );
	if( difference_ ) {
	  o_updf_ = new unpolPDF::Evo( args );
	  d_updf_ = new Evolution::KernelDiff( updf_, o_updf_ );
	}
      } else {
	ARG_LOG << endl;
      }
    }
  }
  
  catch( int error ){
    ARG_ERR << "catch error(" << error << ")" << endl;
    throw( error );
  }
  
  return updf_;
}

polPDF::Evo* Fitting::polPDF() throw( int ) {
  
  if( ppdf_ ) return ppdf_;
  
  Utility::Arguments& args = Utility::Arguments::ref();
  vector< string > opts = polPDF::Evo::options();
  vector< string >::iterator itr = args.hasOpt( opts );
  
  if( itr == opts.end() ) return NULL;
  
  try {
    if( ppdf_ = new polPDF::Evo( args, *itr ) ){
      ARG_LOG 
	<< "create polPDF with --" << *itr << "=" << args.getOpt( *itr ) 
	<< flush;
      if( find( toFit_.begin(), toFit_.end(), "polPDF" ) != toFit_.end() ) {
	ARG_LOG << "(FIT)" << endl;
	this->set( ppdf_ );
	if( difference_ ) {
	  o_ppdf_ = new polPDF::Evo( args, *itr );
	  d_ppdf_ = new Evolution::KernelDiff( ppdf_, o_ppdf_ );
	}
	
      } else {
	ARG_LOG << endl;
      }
      
    }
  }
  
  catch( int error ){
    ARG_ERR << "catch error(" << error << ")" << endl;
    throw( error );
  }
  
  return ppdf_;
}

bool Fitting::condition() {
  return true;
}

double Fitting::chi2() {
  
  if( checkFitCondition_ && ! this->condition() ){
#ifdef DEBUG_FITTING
    ARG_LOG << " [FAIL TO FULLFILL THE FITTING CONDITION] " << false;
#endif
    return Fitting::CHI2HUGE;
  }
  
  // loop over the signal for chi2 calculation
  if( useThread_ ){
    this->chi2( 0, sigHalfSize_ );
    this->joinThread( this->newThread() );
  } else {
    this->chi2( 0, ( useFData_ ? fData_.size() : sig_.size() ) );
  }
  
  // sum up chi2
  double value( 0.0 );
  nData_ = 0;

  if( useFData_ ){                       // experimental implementation
    
    for( int i = 0; i < fData_.size(); i++ ){
      
      FitData* fd = fData_[ i ];
      if( ! fd->sigUsed_ ) continue;     // igonore
      
      value += fd->c2weight_ * fd->c2_;  // add diagonal elements at first,
      nData_++;                          // increment number of data point
      
      if( fd->point_ == NULL ) continue; // no DataPoint pointer
      
      fd->point_->chiSquare() = fd->c2_; // set present chi2 to DataPoint
      fd->point_->fitted() = true;       // change fitted status
      fd->point_->model() = fd->fit_; 
      
      if( with_stat_corr_ == false ||    // normal mode
	  fd->point_->emid() < 0   ||    // no matrix element
	  fd->point_->emNumber() < 0 ){  // wrong error matrix ID
	continue;
      } 
      
      // find error matrix
      vector< QCD::PdfErrMatrix >::iterator itr =
	find( ematrices_.begin(), ematrices_.end(), fd->point_->emNumber() );
      
      if( itr == ematrices_.end() ) continue; // can not find error matrix
      
      // -------- calculate chi2 with statistical correlation ----------
      value -= fd->c2weight_ * fd->c2_;       // remove diagonal elements
      
      // calculate using inverted error matrix
      QCD::PdfErrMatrix& em = *itr;
      for( int j = i; j < fData_.size(); j++ ){
	
	FitData* fdj = fData_[ j ];
	if( ! fdj->sigUsed_         ||        // igonore
	    fdj->point_ == NULL     ||        // no DataPoint pointer
	    fdj->point_->emid() < 0 ||        // no error matrix element 
	    fdj->point_->emNumber() != fd->point_->emNumber() // diff. matrix
	    ) continue; 
	
	value += 
	  ( i == j ? 1.0 : 2.0 ) *
	  em[ fd->point_->emid() ][ fdj->point_->emid() ] *
	  sqrt( fd->c2weight_ ) * 
	  ( fd->sig_ - 
	    ( fd->dset_ ? fd->dset_->normalization() : 1.0 ) * fd->fit_ ) * 
	  sqrt( fdj->c2weight_ ) *
	  ( fdj->sig_ - 
	    ( fdj->dset_ ? fdj->dset_->normalization() : 1.0 ) * fdj->fit_ );
	
      }
      
    }
    
  } else {
    
    int sigSize =  sig_.size();
    
    for( int i = 0; i < sigSize; i++ ){
      
      if( ! sigUsed_[ i ] ) continue;
      value += c2weight_[ i ] * c2_[ i ]; // calculate chi2
      nData_++;                           // increment number of data 
      
      Exp::DataPoint *pi = point_[ i ];
      if( pi == NULL ) continue;          // No DataPoint pointer
     
      pi->chiSquare() = c2_[ i ];         // set chi2 value to DataPoint
      pi->fitted()    = true;             // change fitting status
      pi->model()     = fit_[ i ];

      if( with_stat_corr_ == false ||     // normal mode
	  pi->emid() < 0           ||     // no error matrix elements
	  pi->emNumber() < 0              // wrong matrix ID
	  ) continue;
      
      // find error matrix
      vector< QCD::PdfErrMatrix >::iterator itr =
	find( ematrices_.begin(), ematrices_.end(), pi->emNumber() );
      
      if( itr == ematrices_.end() ) continue; // no error matrix
      
      // calculate using inverted error matrix
      QCD::PdfErrMatrix& em = *itr;
      
      // -------- calculate chi2 with statistical correlation ----------
      value -= c2weight_[ i ] * c2_[ i ];    // remove diagonal part
      
      for( int j = i; j < sigSize; j++ ){
	if( ! sigUsed_[ j ] ) continue;      // ignore
	Exp::DataPoint *pj = point_[ j ];
	if( pj == NULL     ||                // no DataPoint pointer
	    pj->emid() < 0 ||                // no error matrix element 
	    pj->emNumber() != pi->emNumber() // diff. matrix
	    ) continue; 
	
	double v = em[ pi->emid() ][ pj->emid() ];
	
	v *= 
	  sqrt( c2weight_[ i ] ) *
	  ( sig_[ i ] - 
	    ( dset_[ i ] ? dset_[ i ]->normalization() : 1.0 ) * fit_[ i ] );
	
	v *= 
	  sqrt( c2weight_[ j ] ) *
	  ( sig_[ j ] - 
	    ( dset_[ j ] ? dset_[ j ]->normalization() : 1.0 ) * fit_[ j ] );
	
	v *= ( i == j ? 1.0 : 2.0 );
	
	value += v;
      }
    }
  }
  
  // add chi2 contribution due to data normalization handling
  double tmpreschi2_( 0.0 );
  if( dataNormalization_ ){
    for( int i = 0; i < rescaledData_.size(); i++ ){
      tmpreschi2_ += pow( rescaledData_[ i ]->ncoef(), 2 );
    }
  }
  res_chi2_ = tmpreschi2_;
  value += res_chi2_;

  return value;
}

void Fitting::chi2( const int& begin, const int& end ) {
  double factor = 1.0;
  if( useFData_ ){ // test
    for( int i = begin; i < end; i++ ){
      FitData* fd = fData_[ i ];
      if( ( fd->sigUsed_ = this->calc( fd ) ) == false ) continue;
      Exp::DataSet *ds = fd->dset_;
      factor = ( ds != NULL ? ds->normalization() : 1.0 );
      fd->c2_ = pow( ( fd->sig_ - factor * fd->fit_ ) / fd->dsig_, 2.0 );
    }
  } else {   // default mode
    for( int i = begin; i < end; i++ ){
      if( ( sigUsed_[ i ] = this->calc( i ) ) == false ) continue;
      Exp::DataSet *ds = dset_[ i ];
      factor = ( ds != NULL ? ds->normalization() : 1.0 );
      c2_[ i ] = pow( ( sig_[ i ] - factor * fit_[ i ] ) / dsig_[ i ], 2.0 );
    }
  }
}

void Fitting::del( vector< vector< Xsection::xzXsec* > >& f ){
  for( int i = 0; i < f.size(); i++ ) this->del( f[ i ] );
}

void Fitting::del( vector< Xsection::xzXsec* >& f ){
  for( int i = 0; i < f.size(); i++ ) 
    if( f[ i ] ) { delete  f[ i ]; f[ i ] = NULL; }
}

void Fitting::del( vector< vector< Xsection::xXsec* > >& f ){
  for( int i = 0; i < f.size(); i++ ) this->del( f[ i ] );
}

void Fitting::del( vector< Xsection::xXsec* >& f ){
  for( int i = 0; i < f.size(); i++ ) 
    if( f[ i ] ) { delete f[ i ] ; f[ i ] = NULL; }
}

void Fitting::del( vector< vector< unpolSIDISXsec::StrFuncSub* > >& f ){
  for( int i = 0; i < f.size(); i++ ) this->del( f[ i ] );
}

void Fitting::del( vector< unpolSIDISXsec::StrFuncSub* >& f ){
  for( int i = 0; i < f.size(); i++ ) 
    if( f[ i ] ) { delete f[ i ]; f[ i ] = NULL; }
}

void Fitting::del( vector< vector< unpolSIDISXsec::StrFunc* > >& f ){
  for( int i = 0; i < f.size(); i++ ) this->del( f[ i ] );
}

void Fitting::del( vector< unpolSIDISXsec::StrFunc* >& f ){
  for( int i = 0; i < f.size(); i++ ) 
    if( f[ i ] ) { delete f[ i ]; f[ i ] = NULL; }
}

void Fitting::del( vector< vector< unpolDISXsec::StrFunc* > >& f ){
  for( int i = 0; i < f.size(); i++ ) this->del( f[ i ] );
}

void Fitting::del( vector< unpolDISXsec::StrFunc* >& f ){
  for( int i = 0; i < f.size(); i++ ) 
    if( f[ i ] ) { delete f[ i ]; f[ i ] = NULL; }
}

void Fitting::del( vector< vector< unpolDIS::StrFunc* > >& f ){
  for( int i = 0; i < f.size(); i++ ) this->del( f[ i ] );
}

void Fitting::del( vector< unpolDIS::StrFunc* >& f ){
  for( int i = 0; i < f.size(); i++ ) 
    if( f[ i ] ) { delete f[ i ]; f[ i ] = NULL; }
}

void Fitting::del( vector< vector< unpolSIDIS::StrFunc* > >& f ){
  for( int i = 0; i < f.size(); i++ ) this->del( f[ i ] );
}

void Fitting::del( vector< unpolSIDIS::StrFunc* >& f ){
  for( int i = 0; i < f.size(); i++ ) 
    if( f[ i ] ) { delete f[ i ]; f[ i ] = NULL; }
}

void Fitting::del( vector< vector< polDIS::StrFunc* > >& f ){
  for( int i = 0; i < f.size(); i++ ) this->del( f[ i ] );
}

void Fitting::del( vector< polDIS::StrFunc* >& f ){
  for( int i = 0; i < f.size(); i++ ) 
    if( f[ i ] ) { delete f[ i ]; f[ i ] = NULL; }
}

void Fitting::del( vector< vector< polSIDIS::StrFunc* > >& f ){
  for( int i = 0; i < f.size(); i++ ) this->del( f[ i ] );
}

void Fitting::del( vector< polSIDIS::StrFunc* >& f ){
  for( int i = 0; i < f.size(); i++ ) 
    if( f[ i ] ) { delete f[ i ]; f[ i ] = NULL; }
}

void Fitting::del( vector< vector< FragXsec_T::FragXsec* > >& f ){
  for( int i = 0; i < f.size(); i++ ) this->del( f[ i ] );
}

void Fitting::del( vector< FragXsec_T::FragXsec* >& f ){
  for( int i = 0; i < f.size(); i++ ) 
    if( f[ i ] ) { delete f[ i ]; f[ i ] = NULL; }
}

string Fitting::emLevelTag( const int& i ){
  switch( i ){
  case 0: return "not calculated"; break;
  case 1: return "approximation only"; break;
  case 2: return "forced positive-definite"; break;
  case 3: return "full accurate covariance matrix"; break;
  }
  return "Not defined";
}

int Fitting::runInThread(){
  this->chi2( sigHalfSize_, ( useFData_ ? fData_.size() : sig_.size() ) );
  return 0;
}

void Fitting::setNormMp( ErrorCalc::ErrCalc* ecal, Exp::DataSet* ds ){
  if( Arguments::ref().hasOpt( "NoNormErr" ) ||
      ecal == NULL || ds == NULL ||
      ds->emNumber() == -1 ) return;
  
  for( int i = 0; i < expr_.size(); i++ ) ecal->addErrMatrix( expr_[ i ] );
  ecal->setDataSet( ds );
}

void Fitting::outMatrix( ErrorCalc::ErrCalc* ecal ){
  QCD::PdfErrMatrix em = ecal->ematrices()[ ecal->getDataID() ];
  
  ARG_LOG << "******** Total Error Matrix Confirmation ********" << endl;
  for( int i = 0; i < em.size() ; i++ ){
    for( int j = i; j < em[ i ].size() ; j++ )
      ARG_LOG << em[ i ][ j ] << " " << flush;
    ARG_LOG << endl;
  }
}

void Fitting::loadData() {
  
#ifdef DEBUG_FITTING
  ARG_LOG << "# -----------------------------------------------" << endl;
  ARG_LOG << "#  Load Experimental Data" << endl;
  ARG_LOG << "# -----------------------------------------------" << endl;
#endif // DEBUG_FITTING
  
  ematrices_.clear();                      //  clear error matrix 
  for( int ie = 0; ie < expr_.size(); ie++ ) this->loadData( expr_[ ie ] );

  // resize several vectors (fit_, c2_, etc.) before chi2 calc
  this->resize_vects();
}

void Fitting::loadData( Exp::ExpData* edata ){
  for( int i = 0; i < edata->data().size(); i++ ){
    Exp::DataSet& dset = edata->data()[ i ];
    if( dset.enable() && dset.size() > 0 ) this->loadData( dset );
    if( with_stat_corr_ ) this->loadErrorMatrix( dset );
  }
  this->invertErrorMatrix();
}

void Fitting::resize_vects(){

  if( useFData_ ){
    sigHalfSize_ = fData_.size() / 2;
  } else {
    // resize container variables
    if( fit_.size() != sig_.size() ){
      sigHalfSize_ = sig_.size()  / 2;
      sigUsed_.resize( sig_.size(), false );
      fit_.resize( sig_.size(), 0.0 );
      c2_.resize(  sig_.size(), 0.0 );
    }
    
    if( c2weight_.size() != sig_.size() ) 
      c2weight_.resize( sig_.size(), 1.0 );
    
    if( dset_.size() != sig_.size() ) 
      dset_.resize( sig_.size(), (Exp::DataSet*) NULL );
    
    if( point_.size() != sig_.size() ) 
      point_.resize( sig_.size(), (Exp::DataPoint*) NULL );
    
  }
}

void Fitting::loadErrorMatrix( Exp::DataSet& dset ){
  // Loading Error Matrix of DataPoint
  for( int j = 0; j < dset.size(); j++ ){
    Exp::DataPoint& p = dset[ j ];
    if( p.emid() < 0 || p.emNumber() < 0 ) continue;   // no error matrix
    vector< QCD::PdfErrMatrix >::iterator itr =
      find( ematrices_.begin(), ematrices_.end(), p.emNumber() );
    if( itr != ematrices_.end() ){
      itr->load( p );
    } else {
      ematrices_.push_back( QCD::PdfErrMatrix( p ) );
    }
  }
}

void Fitting::invertErrorMatrix(){
  
  // inver error matrix for chi2 calculation
  for( vector< QCD::PdfErrMatrix >::iterator itr = ematrices_.begin();
       itr != ematrices_.end(); itr++ ){
    
    QCD::PdfErrMatrix& em = *itr;
    int emsize = em.size();
    
    TMatrixT< double > m( emsize, emsize );
    for( int emi = 0; emi < emsize; emi++ ) 
      for( int emj = 0; emj < emsize; emj++ )
	m[ emi ][ emj ] = em[ emi ][ emj ];
    
    TMatrixT< double > minv = m;
    minv.Invert();
    for( int emi = 0; emi < emsize; emi++ )
      for( int emj = 0; emj < emsize; emj++ )
	em[ emi ][ emj ] = minv[ emi ][ emj ];
    
    //    ARG_DETAIL << "check if the matrix is successfully inverted." << endl;
    //    TMatrixT< double > unit = m * minv;
    //    unit.Print();
    
    ARG_LOG << "Load and Invert ErrorMatrix in DataPoint objects:" << endl;
    ARG_LOG << *itr << endl;
  }

}
