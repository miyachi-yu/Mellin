#include "A1hFitLM.hh"

#include <TMatrixDEigen.h>

/*
#include <cmath>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <algorithm>
*/

using namespace std;
using namespace QCD;
using namespace Utility;
using namespace Exp;
using namespace Fit;

A1hFitLM::A1hFitLM( Arguments& args ) throw( int ) : 
  Fit::A1hFit( args, false ), orthmx_(), orthmxinv_(), errmx_(),
  eigvmx_(), eigvmxinv_(), eigparams_( 0 ), parweights_( 0 ),
  dissig_( abs( args.get( "DisSig", 1 ) ) ), 
  malsig_( abs( args.get( "MalSig", 100 ) ) ),
  fpindx_( 2, vector< int >( 0 ) ),
  egvid_( args.get( "EgvID", 1 ) ), reffrag_( false ), reffvalue_( 0.0 ), 
  refforgchi2_( 0.0 ), refforgvalue_( 0.0 ), refferr_( 0.0 ),
  tfname_( args.get( "TCFname", "Tchi2_ev.dat" ) ), 
  dfname_( args.get( "DCFname", "Dchi2_ev.dat" ) )
{  

  if( ! args.hasOpt( "LagMultM" ) ){ 
    ARG_ERR << "# '--LagMultM' option needed for LM process" << endl;
    throw( 1 );
  }

  this->setlambda( args.get( "LagMultM", 0.0 ) );

  try {
    // conf process in Fitting class with over loaded defineParameters
    // conf process should precede post conf process
    this->config_process();
    // post conf process in A1hFit class (currently default)
    this->post_config_process();
    // post post conf process in this class
    this->ppost_config_process();
  }

  catch( int error ){
    ARG_ERR << "catch errror (" << error << ")" << endl;
    throw( error );
  }

}

A1hFitLM::~A1hFitLM(){
  // objects created config process are deleted in 
  // corresponing classes which have them as own objects
}

void A1hFitLM::ppost_config_process(){

  Arguments& args = Arguments::ref();

  if( ! args.hasOpt( "Minuit" ) ){

    double paramerr_ = eigparams_[ egvid_ - 1 ].error();
    bool msjg_ = args.hasOpt( "SIPXTolManip" ); // true -> Simplex

    ostringstream ost;
    ost << ( msjg_ ? "SIMplex": "MIGrad" ) << " " 
	<< args.get( "iteration", 10000 ) << ","
	<< ( msjg_ ? 0.001: 1.0 ) * args.get( "TolFact", 1.0 ) / paramerr_;
    //-----Migrad case-----//
    // DEFAULT
    // paramerr_ = 0.01 -> tolerance = 10^2 ~ EDM = 0.1 (Simplex EDM)
    //-----Simplex case-----//
    // DEFAULT
    // paramerr_ = 0.01 -> tolerance = 0.1 ~ EDM = 0.1 (Simplex EDM)

    // Migrad  EDM( 0.0001 ) = 0.001 * tolerance( 0.1 ) * UP( 1 )
    // Simplex EDM( 0.1 ) = 0.1 * UP( 1 )

    // TolFact > 1.0 -> Simplex, TolFact < 1.0 -> Migrad,

    args.set( "Minuit", ost.str() );

    this->minuitCmds( args.getOpt( "Minuit" ) );
    ARG_LOG 
      << "--- command set in A1hFitLM ---" << endl;
    ARG_LOG 
      << "call Minuit commands " << args.getOpt( "Minuit" ) 
      << endl;
    ARG_LOG 
      << "(EV." << egvid_ << " Err = " << paramerr_ << ")"
      << endl;
    ARG_LOG
      << "--- command set in A1hFitLM ---" << endl;
  }

}

void A1hFitLM::defineParameters(){

  // it will re-define fit parameters iteratively 
  // if new paramterers are registered

  // create error & eigen vector matrices with current parameters
  this->mxcreation();

  // set weights for direct calc
  this->setweights();

  // set eigparams with current parameter values (with weights)
  this->seteigparams();

  // ------------------------------------------------------------ //
  //            set eigen params as fit parameters                //
  // ------------------------------------------------------------ //
  //  not always needed eigen transform (can be default treatment)
  //  for easiness to see orthogonal behavior between parameters
  //  in this LM process (other parameters should be almost unchanged)

  for( int i = 0; i < eigparams_.size(); i++ ){

    // constructed only with free parameters
    Parameter param = eigparams_[ i ];
    this->DefineParameter( i, param.name().c_str(), 
			   param.value(), param.error(),
                           param.lower(), param.upper() );
  }

}

void A1hFitLM::dumpParameters(){
  
  for( int i = 0; i < eigparams_.size(); i++ )
    this->GetParameter( i, eigparams_[ i ].value(), 
			eigparams_[ i ].error() );

  this->mirror( eigparams_ );
  // updatePdf done in mirror op.

  // ------------------------------------------------------------ //
  //                 Dump parameter information                   //
  // ------------------------------------------------------------ //
  if( dumpParamLevel_ <= istat_ ){
    for( int i = 0; i < this->kernels().size(); i++ ){
      PdfParametrization *init = this->kernels()[ i ]->initialDistribution();
      ofstream ofs( ( prefix_ + this->ofilename( i ) ).c_str() );
      ofs << *init << endl;
      this->summary_basic( i, ofs );      
      ofs.close();
    }
  }
}

double A1hFitLM::referenceFunction(){

  //# note eigen value calc from updated params_ & rescaleData_ 
  // ( through mirror() )
  //# egvid_ refers Param NO. (for vector index, egvid_ - 1)
  //# NOTE:: mirroring has been done before this operation
  //# -> weight manipulation is already taken into account (in direct calc) 
  return  reffvalue_ = this->geteigvvalue( egvid_ - 1 );
}

bool A1hFitLM::condition(){
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

  // from A1hFit.cc
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
      
      //      if( xpol < 0.001 ) continue;
      if( xpol < 0.01 ) continue;
      //      if( ( xpol - xunpol ) / xpol > 0.05 ){
      if( ( xpol - xunpol ) / xpol > 0.10 ){
	cout << i << " : " << x_ << " : " 
	     << xpol << " < " << xunpol << endl; 
	return false;
      }
    }

  }

  // some fundamental checkings 
  if( egvid_ > eigvmx_.GetNrows() ) throw( 1 );
  if( this->GetNumFreePars() != eigparams_.size() ) throw( 1 );

  return true;
}

void A1hFitLM::file_out(){

  // for total chi2 vs eigvector
  ofstream ofsT( tfname_.c_str(), ios::out | ios::app );

  double tlamb_ = this->getlambda();
  double tchi2_ = this->getchi2() 
    + tlamb_ * reffvalue_;
  double cchi2_ = this->getchi2();

  double dcchi2_ = this->getchi2() - refforgchi2_;
  double deigvc_ = reffvalue_ - refforgvalue_;
  double deigvc_n_ = deigvc_ / refferr_;
  /*
  if( abs( dcchi2_ ) < 1.0E-5 ) dcchi2_ = 0.0;
  if( abs( deigvc_ ) < 1.0E-5 ) deigvc_ = 0.0;
  if( abs( deigvc_n_ ) < 1.0E-5 ) deigvc_n_ = 0.0;
  */

  ofsT << setw( 8 )  << egvid_
       << setw( 8 )  << tlamb_
       << setw( 12 ) << tchi2_
       << setw( 12 ) << cchi2_
       << setw( 12 ) << reffvalue_
       << setw( 12 ) << dcchi2_
       << setw( 12 ) << deigvc_
       << setw( 12 ) << deigvc_n_
       << endl;
  ofsT.close();

  // for each chi2 vs eigvector
  ofstream ofsD( dfname_.c_str(), ios::out | ios::app );
  vector< ExpData* >& vexpr_ = this->vexpr();
  int nds_( 0 ); // count total dataset size
  // dataset contribution
  for( int i = 0; i < vexpr_.size(); i++ )
    for( int j = 0; j < vexpr_[ i ]->data().size(); j++ ){
      DataSet& ds = vexpr_[ i ]->data()[ j ];
      ofsD << setw( 8 )  << egvid_
	   << setw( 8 )  << tlamb_ 
	   << setw( 12 ) << deigvc_n_
	   << setw( 25 ) << ds.info() 
	   << setw( 4 )  << ds.fittedDataPoint()
	   << setw( 4 )  << ds.size()
	   << setw( 12 ) << ds.chiSquare()
	   << setw( 2 )  << ( ds.enable() ? "E" : "D" )
	   << endl;
      //      if( ds.enable() ) nds_++;
      nds_++;
    }
  // rescale param contribution
  ofsD << setw( 8 )  << egvid_
       << setw( 8 )  << tlamb_ 
       << setw( 12 ) << deigvc_n_
       << setw( 25 ) << "RESCALE_PARAMS"
       << setw( 4 )  << rescaledData_.size()
       << setw( 4 )  << nds_
       << setw( 12 ) << this->getreschi2()
       << setw( 2 )  << ( dataNormalization_ ? "E" : "D" )
       << endl;
  ofsD.close();

}

void A1hFitLM::chi2err_calc(){

  vector< double > vchi2_err_( 0 );
  vector< double > vchi2_org_( 0 );

  vector< ExpData* >& vexpr_ = this->vexpr();
  //------ org chi2 set -------//
  this->mirror( eigparams_ );
  this->setchi2( this->chi2() );
  // update chi2 values for consistency orgs and deviated values
  // (to resolve discrepancies maybe from cache manipulation)

  for( int i = 0; i < vexpr_.size(); i++ )
    for( int j = 0; j < vexpr_[ i ]->data().size(); j++ ){
      DataSet& ds = vexpr_[ i ]->data()[ j ];
      vchi2_org_.push_back( ds.chiSquare() );   // dataset chi2s
    }
  vchi2_org_.push_back( this->getreschi2() );   // rescale chi2s
  vchi2_org_.push_back( this->getchi2() );   // total chi2s
  //------ org chi2 set -------//

  vchi2_err_.resize( vchi2_org_.size(), 0.0 );
  double devi_( Arguments::ref().get( "DEVI", 0.001 ) );

  if( ! Arguments::ref().hasOpt( "DerrCalc" ) ){
    // 2.0 * d.o.f. (expected variance of chi2 dist.)

    int i_( 0 );
    //------ error calc start -------//
    for( int i = 0; i < vexpr_.size(); i++ )
      for( int j = 0; j < vexpr_[ i ]->data().size(); j++ ){
	DataSet& ds = vexpr_[ i ]->data()[ j ];
	// dataset chi2s
	vchi2_err_[ i_ ] = 2.0 * ds.fittedDataPoint();
	i_++;
      }
    // rescale chi2s
    vchi2_err_[ i_ ] = 2.0 * rescaledData_.size(); i_++;
    // total chi2s
    vchi2_err_[ i_ ] = 2.0 * this->dof(); i_++;

    if( i_ != vchi2_org_.size() ) throw( 10 );
    //------ error calc end -------//

  } else {

    //------ error calc start -------//
    for( int k = 0; k < eigparams_.size(); k++ ){
      
      //------ calc partial derivatives -------//
      eigparams_[ k ].value() += devi_;
      this->mirror( eigparams_ );
      // update chi2 values
      this->setchi2( this->chi2() );
      
      vector< double > vchi2_dev_( vchi2_err_.size(), 0.0 );
      int i_( 0 );
      
      for( int i = 0; i < vexpr_.size(); i++ )
	for( int j = 0; j < vexpr_[ i ]->data().size(); j++ ){
	  DataSet& ds = vexpr_[ i ]->data()[ j ];
	  // dataset chi2s
	  vchi2_dev_[ i_ ] = ds.chiSquare() - vchi2_org_[ i_ ];
	  vchi2_dev_[ i_ ] /= devi_;
	  i_++;
	}
      // rescale chi2s
      vchi2_dev_[ i_ ] = this->getreschi2() - vchi2_org_[ i_ ];
      vchi2_dev_[ i_ ] /= devi_; i_++;
      // total chi2s
      vchi2_dev_[ i_ ] = this->getchi2() - vchi2_org_[ i_ ];   
      vchi2_dev_[ i_ ] /= devi_; i_++;
      
      if( i_ != vchi2_org_.size() ) throw( 10 );
      
      eigparams_[ k ].value() -= devi_;
      this->mirror( eigparams_ );
      // retrieve original chi2 values
      this->setchi2( this->chi2() );
      //------ calc partial derivatives -------//
      
      //------ calc chi2 errors -------//
      for( int i = 0; i < vchi2_err_.size(); i++ ){
	vchi2_err_[ i ] += eigvmx_[ k ][ k ] * pow( vchi2_dev_[ i ], 2 );
      }
      
    }
    //------ error calc end -------//
    
  }

  //------ take square root ------//
  for( int i = 0; i < vchi2_err_.size(); i++ ){
    vchi2_err_[ i ] = sqrt( vchi2_err_[ i ] );
  }

  //------ error out put --------//
  this->chi2err_out( ARG_LOG, vchi2_org_, vchi2_err_ );

  if( egvid_ == 1 && this->getlambda() == 0.0 ){
    ostringstream oss; oss << "chi2_error.dat";
    ofstream ofs( Arguments::ref().get( "C2ECFname", oss.str() ).c_str() );
    this->chi2err_out( ofs, vchi2_org_, vchi2_err_ );
    ofs.close();    
  }

}

void A1hFitLM::chi2err_out( ostream& os, 
				  const vector< double >& vchi2_org_,
				  const vector< double >& vchi2_err_ ){

  os << "----------------------------------" << endl;
  // dataset chi2
  vector< ExpData* >& vexpr_ = this->vexpr();
  int nds_( 0 );
  for( int i = 0; i < vexpr_.size(); i++ )
    for( int j = 0; j < vexpr_[ i ]->data().size(); j++ ){
      DataSet& ds = vexpr_[ i ]->data()[ j ];
      os 
	<< setw( 3 )  << nds_
	<< setw( 35 ) << ds.info() 
	<< setw( 4 )  << ds.fittedDataPoint() << "/" 
	<< setw( 4 )  << ds.size()
	<< setw( 12 ) << ds.chiSquare()
	<< setw( 12 ) << vchi2_org_[ nds_ ]
	<< setw( 3 )  << "+-"
	<< setw( 12 ) << vchi2_err_[ nds_ ]
	<< setw( 2 )  << ( ds.enable() ? "E" : "D" )
	<< endl;
      //      if( ds.enable() ) nds_++;
      nds_++;
    }
  os << "----------------------------------" << endl;
  // rescale chi2
  os
    //    << setw( 3 )  << ""
    << setw( 3 )  << nds_
    << setw( 35 ) << "Chi2_From_Rescale_Params"
    << setw( 4 )  << rescaledData_.size() << "/"
    << setw( 4 )  << nds_
    << setw( 12 ) << this->getreschi2()
    << setw( 12 ) << vchi2_org_[ nds_ ]
    << setw( 3 )  << "+-"
    << setw( 12 ) << vchi2_err_[ nds_ ]
    << setw( 2 )  << ( dataNormalization_ ? "E" : "D" )
    << endl;
  nds_++;
  os << "----------------------------------" << endl;
  // total chi2
  os
    //    << setw( 3 )  << ""
    << setw( 3 )  << nds_
    << setw( 35 ) << "Total_Chi2"
    //    << setw( 4 )  << this->dof()
    //    << setw( 4 )  << ""
    << setw( 4 )  << this->dof() << "/"
    << setw( 4 )  << this->dof()
    << setw( 12 ) << this->getchi2()
    << setw( 12 ) << vchi2_org_[ nds_ ]
    << setw( 3 )  << "+-"
    << setw( 12 ) << vchi2_err_[ nds_ ]
    << setw( 2 )  << "E"
    << endl;
  nds_++;
  // reduced chi2
  os
    //    << setw( 3 )  << ""
    << setw( 3 )  << nds_
    << setw( 35 ) << "Reduced_Chi2"
    //    << setw( 4 )  << ""
    //    << setw( 4 )  << ""
    << setw( 4 )  << this->dof() << "/"
    << setw( 4 )  << this->dof()
    << setw( 12 ) << this->getchi2() / this->dof()
    << setw( 12 ) << vchi2_org_[ nds_ - 1 ] / this->dof()
    << setw( 3 )  << "+-"
    << setw( 12 ) << vchi2_err_[ nds_ - 1 ] / this->dof()
    << setw( 2 )  << "E"
    << endl;
  nds_++;
  os << "----------------------------------" << endl;
}

void A1hFitLM::param_out( std::ostream& os ){

  os << "--  orignal param out  --" << endl;
  os << setw( 8 )  << "param_ID" << setw( 12 ) << "name"
     << setw( 12 ) << "center" << setw( 12 ) << "error"
     << setw( 12 ) << "lower lim" << setw( 12 ) << "upper lim" << endl;
  for( int j = 0; j < params_.size(); j++ ){
    os << setw( 8 )  << j + 1
       << setw( 12 ) << nam_[ fpindx_[ 0 ][ j ] ] 
       << setw( 12 ) << params_[ fpindx_[ 0 ][ j ] ]->value() 
       << setw( 12 ) << params_[ fpindx_[ 0 ][ j ] ]->error() 
       << setw( 12 ) << params_[ fpindx_[ 0 ][ j ] ]->lower() 
       << setw( 12 ) << params_[ fpindx_[ 0 ][ j ] ]->upper() 
       << endl; 
  }  
  for( int j = 0; j < rescaledData_.size(); j++ ){
    os << setw( 8 )  << j + params_.size() + 1
       << setw( 12 ) << rescaledData_[ fpindx_[ 1 ][ j ] ]->info() 
       << setw( 12 ) << rescaledData_[ fpindx_[ 1 ][ j ] ]->ncoef()
       << setw( 12 ) << rescaledData_[ fpindx_[ 1 ][ j ] ]->ncerr()
      //       << setw( 12 ) << rescaledData_[ fpindx_[ 1 ][ j ] ]->lower()
      //       << setw( 12 ) << rescaledData_[ fpindx_[ 1 ][ j ] ]->upper()
       << setw( 12 ) << "-"
       << setw( 12 ) << "-"
       << endl;
  }
  os << "-------------------------" << endl;
  os << "-- eigvector param out --" << endl;
  this->seteigparams();
  os << setw( 8 )  << "param_ID" << setw( 12 ) << "name"
     << setw( 12 ) << "center" << setw( 12 ) << "error"
     << setw( 12 ) << "lower lim" << setw( 12 ) << "upper lim" << endl;
  for( int j = 0; j < eigparams_.size(); j++ ){
    os << setw( 8 )  << j + 1 
       << setw( 12 ) << eigparams_[ j ].name()
       << setw( 12 ) << eigparams_[ j ].value()
       << setw( 12 ) << eigparams_[ j ].error()
       << setw( 12 ) << eigparams_[ j ].lower()
       << setw( 12 ) << eigparams_[ j ].upper()
       << endl;
  }
  os << "-------------------------" << endl;

}

void A1hFitLM::param_corr_out( std::ostream& os ){

  vector< string > row_( 0 ), column_( 0 );

  for( int j = 0; j < params_.size(); j++ )
    column_.push_back( nam_[ fpindx_[ 0 ][ j ] ] );
  for( int j = 0; j < rescaledData_.size(); j++ )
    column_.push_back( rescaledData_[ fpindx_[ 1 ][ j ] ]->info() );
  row_.push_back( "Error" );
  for( int j = 0; j < eigparams_.size(); j++ )
    row_.push_back( eigparams_[ j ].name() );

  TMatrix tmpmx_( row_.size(), column_.size() );
  tmpmx_.style( TMatrix::STYLE( Arguments::ref().get( "ECFstyle", 1 ) ) );
  tmpmx_.precision( 3 );
  //  Arguments::ref().set( "ECFsquare", "" );

  for( int j = 0; j < column_.size(); j++ ){

    double denom_( 0.0 ); 
    for( int i = 0; i < row_.size() - 1; i++ )
      denom_ += pow( orthmx_[ j ][ i ], 2 ) * eigvmx_[ i ][ i ];
    if( ! Arguments::ref().hasOpt( "ECFsquare" ) )
      denom_ = sqrt( denom_ );

    //    cout << denom_ << " " << sqrt( errmx_[ j ][ j ] ) << endl;
    tmpmx_[ 0 ][ j ] = sqrt( errmx_[ j ][ j ] );

    for( int i = 0; i < row_.size() - 1; i++ ){
      tmpmx_[ i + 1 ][ j ] = pow( orthmx_[ j ][ i ], 2 ) * eigvmx_[ i ][ i ];
      if( ! Arguments::ref().hasOpt( "ECFsquare" ) )
	tmpmx_[ i + 1 ][ j ] = sqrt( tmpmx_[ i + 1 ][ j ] );
      tmpmx_[ i + 1 ][ j ] /= denom_;
    }

  }

  ostringstream oss; oss << "error_corr" << tmpmx_.suffix();

  ofstream ofs( Arguments::ref().get( "ECFname", oss.str() ).c_str() );
  tmpmx_.print( ofs, row_, column_ );
  ofs.close();

  //  if( this->verbose() ) tmpmx_.print( os, row_, column_ );
  os << "---- err. cor. in org & eigv ----" << endl;
  if( Arguments::ref().hasOpt( "ECFsquare" ) )
    os << "-- (squared corr. mx. summed up to 1) --" << endl;
  tmpmx_.print( os, row_, column_ );
  os << "---------------------------------" << endl;

}

bool A1hFitLM::postFit_summary( std::ostream& os ){

  /*
  os << "-- tmp expdata out --" << endl;
  this->data_summary( os );
  */
  this->param_out( os ); 
  // dump final parameters

  this->param_corr_out( os ); 
  // show correlation matrix (parameter independent)

  this->file_out();
  // dump chi2 values 

  this->chi2err_calc();
  //  // calc err. of chi2s

  os << endl;
  os << setw( 8 )  << "EV_ID"
     << setw( 8 )  << "Lambda"
     << setw( 12 ) << "Total"
     << setw( 12 ) << "Chi2A"
     << setw( 12 ) << "Chi2P"
     << setw( 12 ) << "SubAL"
     << setw( 12 ) << "SubPT"
     << setw( 12 ) << "NomSP"
     << endl;

  double tlamb_ = this->getlambda();
  double tchi2_ = this->getchi2() 
    + tlamb_ * reffvalue_;
  
  os << setw( 8 )  << egvid_
     << setw( 8 )  << tlamb_
     << setw( 12 ) << tchi2_
     << setw( 12 ) << this->getchi2()
     << setw( 12 ) << reffvalue_
     << setw( 12 ) << this->getchi2() - refforgchi2_
     << setw( 12 ) << reffvalue_ - refforgvalue_
     << setw( 12 ) << ( reffvalue_ - refforgvalue_ ) / refferr_
     << endl;

  return true;
}

void A1hFitLM::mirror( const std::vector< QCD::Parameter >& par ){

  double parm_[ par.size() ];

  for( int i = 0; i < par.size(); i++ )
    parm_[ i ] = par[ i ].value();

  this->mirror( par.size(), parm_ );
}

void A1hFitLM::mirror( const int& n, double* par ){

  if( n != eigparams_.size() ) throw( 1 );

  if( ! reffrag_ ) this->setrefs();

  for( int j = 0; j < fpindx_[ 0 ].size(); j++ ){
    double r( 0.0 );
    for( int k = 0; k < eigparams_.size(); k++ )
      r += orthmx_[ j ][ k ] * par[ k ];
    params_[ fpindx_[ 0 ][ j ] ]->value() = r;
    //    cout << j + 1 << " " << r << endl;
  }
  
  for( int j = 0; j < fpindx_[ 1 ].size(); j++ ){
    double r( 0.0 );
    for( int k = 0; k < eigparams_.size(); k++ )
      r += orthmx_[ j + fpindx_[ 0 ].size() ][ k ] * par[ k ];
    rescaledData_[ fpindx_[ 1 ][ j ] ]->ncoef() = r;
    //    cout << j + fpindx_[ 0 ].size() + 1 << " " << r << endl;
  }

  this->updatePdf();
  //  cout << this->referenceFunction() << endl;
}

void A1hFitLM::setrefs(){

  // original chi2 all value;
  // NOTE:: setrefs() is operated once before mirror()
  if( ! refforgchi2_ )
    refforgchi2_ = this->chi2();
  // original eigen value ( Param No. egvid_ )
  // NOTE:: setrefs() is operated once before mirror()
  if( ! refforgvalue_ ) 
    refforgvalue_ = this->geteigvvalue( egvid_ - 1 );
  // for its normalization ( Param No. egvid_ )
  if( ! refferr_ ) 
    refferr_ = sqrt( abs( eigvmx_[ egvid_ - 1 ][ egvid_ - 1 ] ) );
  
  reffrag_ = true;
}

void A1hFitLM::skipmanip(){
  // for direct calculation
  // NOTE:: Command( "SHOW PARameters" ) in start() performs fit process
  //        once in case of no fit routine.
  //        -> roughly speaking, all the below processes can be canceled out.
  //        However, for several processes between skipmanip() and 
  //        Command( "SHOW PARameters" ) in start(), perform following 
  //        parameter mirroring at this step.

  //  /*
  // mirroring of deviated parameters (eig vec's) to original ones
  // NOTE:: setrefs() works before the mirroring
  this->mirror( eigparams_ );

  this->setchi2( this->chi2() ); // for updated chi2_
  this->postLoop(); // just in case...
  this->referenceFunction(); // for updated reffvalue_
  //  */
}

void A1hFitLM::mxcreation(){

  errmx_ = this->createErrMx();
  int mxsz = errmx_.size();

  // first resize TMatrixD objects (necessary)
  orthmx_.ResizeTo( mxsz, mxsz );
  orthmxinv_.ResizeTo( mxsz, mxsz );
  eigvmx_.ResizeTo( mxsz, mxsz );
  eigvmxinv_.ResizeTo( mxsz, mxsz );

  TMatrix tmpmx_( errmx_ );
  TMatrixDEigen de( tmpmx_ );
  //  TMatrixDEigen de( TMatrix( errmx_ ) );

  // ---------------------------------------------
  //              Eigen Vector Matrices
  // ---------------------------------------------
  /*
  *( dynamic_cast< TMatrixD* >( &orthmx_ ) ) 
    = de.GetEigenVectors();
  */
  orthmx_ = TMatrix( de.GetEigenVectors() ); 
  orthmx_.Invert();
  orthmxinv_ = orthmx_;
  orthmx_.Invert();

  // ---------------------------------------------
  //              Eigen Value Matrices
  // ---------------------------------------------
  /*
  *( dynamic_cast< TMatrixD* >( &eigvmx_ ) ) 
    = de.GetEigenValues();
  */
  eigvmx_ = TMatrix( de.GetEigenValues() );
  eigvmx_.Invert();
  eigvmxinv_ = eigvmx_;
  eigvmx_.Invert();

  this->matrix_output();
}

void A1hFitLM::matrix_output(){

  // ---------------------------------------------
  //              Matrix Out Put
  // ---------------------------------------------
  ARG_LOG << "# -------------------------- #" << endl;
  ARG_LOG << "# ----- Matrix Out Put ----- #" << endl;
  ARG_LOG << "# -------------------------- #" << endl;
  ARG_LOG << "--- Basic Error Matrix ---" << endl;
  ARG_LOG << errmx_ << endl;
  ARG_LOG << "--- Eigen Vector Matrix ---" << endl;
  ARG_LOG << orthmx_ << endl;
  ARG_LOG << "--- Eigen Vector Inverse Matrix ---" << endl;
  ARG_LOG << orthmxinv_ << endl;
  ARG_LOG << "--- Eigen Value Matrix ---" << endl;
  ARG_LOG << eigvmx_ << endl;
  ARG_LOG << "--- Eigen Value Inverse Matrix ---" << endl;
  ARG_LOG << eigvmxinv_ << endl;

}

//PdfErrMatrix& A1hFitLM::createErrMx(){
PdfErrMatrix A1hFitLM::createErrMx(){

  // -------------------------------------
  //     free parameter check first
  // -------------------------------------

  vector< int > tmpindxv1_( 0 );
  for( int j = 0; j < params_.size(); j++ )
    if( params_[ j ]->error() != 0.0 && params_[ j ]->free() )
      tmpindxv1_.push_back( j );
  
  vector< int > tmpindxv2_( 0 );
  for( int j = 0; j < rescaledData_.size(); j++ )
    if( rescaledData_[ j ]->ncerr() != 0.0 
	&& rescaledData_[ j ]->ncstat() != "fixed" ) 
      tmpindxv2_.push_back( j );

  fpindx_[ 0 ] = tmpindxv1_;
  fpindx_[ 1 ] = tmpindxv2_;

  int nfree = fpindx_[ 0 ].size() + fpindx_[ 1 ].size();

  // -------------------------------------
  //     error matrix construction
  // -------------------------------------
  // free parameter element (non-zero element) only!
  // error matrix must be always square matrix even temporarily!

  QCD::PdfErrMatrix em_;
  em_.resetMatrixSize( nfree );

  int nn0_ = 0;

  for( int j = 0; j < params_.size(); j++ )
    if( params_[ j ]->error() != 0.0 && params_[ j ]->free() ){

      int nn_( 0 );

      for( int i = 0; i < params_[ j ]->ems().size(); i++ )
	if( params_[ j ]->ems()[ i ] != 0 && nn_ < nfree ){
	  em_[ nn0_ ][ nn_ ] = params_[ j ]->ems()[ i ];
	  nn_++;
	}
      if( nn_ != nfree ) throw( 1 );
      //nn_ (+1 by additional incrementation)
      nn0_++;
    }
  
  for( int j = 0; j < rescaledData_.size(); j++ )
    if( rescaledData_[ j ]->ncerr() != 0.0 
	&& rescaledData_[ j ]->ncstat() != "fixed" ){

      int nn_( 0 );

      for( int i = 0; i < rescaledData_[ j ]->ems().size(); i++ )
	if( rescaledData_[ j ]->ems()[ i ] != 0 && nn_ < nfree ){
	  em_[ nn0_ ][ nn_ ] = rescaledData_[ j ]->ems()[ i ];
	  nn_++;
	}
      if( nn_ != nfree ) throw( 1 );
      //nn_ (+1 by additional incrementation)
      nn0_++;
    }

  //nn0_ (+1 by additional incrementation)
  if( nn0_ != nfree ){
  //  if( em_.size() != nfree ){
    ARG_ERR << "--- EM construction error ---" << endl;
    ARG_ERR << "--- load from xml files failed ---" << endl;
    throw( 1 );
  }

  return em_;
}


void A1hFitLM::seteigparams(){

  // constructed only with free parameters
  eigparams_.resize( fpindx_[ 0 ].size() + fpindx_[ 1 ].size(), Parameter() );

  Arguments& args = Arguments::ref();

  for( int i = 0; i < eigparams_.size(); i++ ){
    // eigen param value from params_ & rescaledData_
    eigparams_[ i ].value() = this->geteigvvalue( i );
    // standard error of original eigen parameter
    eigparams_[ i ].error() = sqrt( abs( eigvmx_[ i ][ i ] ) );
    // upper & lower boundaries in unit of param error (or direct setting)
    eigparams_[ i ].upper() = eigparams_[ i ].value()
      + ( args.hasOpt( "MalDct" )? 1.0 : abs( eigparams_[ i ].error() ) ) 
      * malsig_;
    eigparams_[ i ].lower() = eigparams_[ i ].value()
      - ( args.hasOpt( "MalDct" )? 1.0 : abs( eigparams_[ i ].error() ) ) 
      * malsig_;
    // some weights on value for direct calc (setweights)
    // (should follow boundary settings)
    eigparams_[ i ].value() *= parweights_[ i ];
    // set weight error if needed (should follow boundary settings)
    eigparams_[ i ].error() /= dissig_;
    // set parameter name
    ostringstream nname; nname << "Eig Vec: " << i + 1;
    eigparams_[ i ].name() = nname.str();
  }

}

void A1hFitLM::setweights(){

  // constructed only with free parameters
  parweights_.resize( fpindx_[ 0 ].size() + fpindx_[ 1 ].size(), 1.0 );

  Arguments& arg = Arguments::ref();

  // currently, weight manipulation is always with "EgvID" option
  //  for( int i = 0; i < parweights_.size(); i++ ){
  int i = egvid_ - 1;
  ostringstream ost; ost << "PWeight" << i + 1;

  if( arg.hasOpt( ost.str().c_str() ) ){

    double tmplamb_ = arg.get( ost.str().c_str(), 0.0 );
    
    if( abs( tmplamb_ ) < malsig_ ){
      ARG_ERR << "weight value (PWeight: " << tmplamb_ 
	      << ") should be smaller than" << endl;
      ARG_ERR << "magnitude of upper & lower boundaries (MalSig: " 
	      << malsig_ << ")" << endl;
      throw( 1 );
    }

    // reset lambda value to PWeight value //
    this->setlambda( tmplamb_ );

    double orgval_ = this->geteigvvalue( i );
    double orgerr_ = sqrt( abs( eigvmx_[ i ][ i ] ) );
    parweights_[ i ] 
      = ( orgval_ + tmplamb_ * orgerr_ ) / orgval_;
    // skip fit process for direct calc
    this->skipfit_ = true;
    // INSTEAD!! -> 
    // Set iteration number at 1 through SetMaxIterations (TMinuit)
    //    this->SetMaxIterations( 1 );

    ARG_LOG << "---- Param Weights for Eigparams_ ----" << endl;
    for( int i = 0; i < parweights_.size(); i++ )
      ARG_LOG << i + 1 << " " << parweights_[ i ] << endl;
    ARG_LOG << "--------------------------------------" << endl;
  }

}

double A1hFitLM::geteigvvalue( const int& i ){
  // index i refers vector index (not parameter ID)

  double r( 0.0 );
  
  // note error matrix is constructed by the order of ( params, rescale )
  // manipulated only with free parameters
  for( int j = 0; j < fpindx_[ 0 ].size(); j++ )
      r += orthmxinv_[ i ][ j ] * params_[ fpindx_[ 0 ][ j ] ]->value();
  
  for( int j = 0; j < fpindx_[ 1 ].size(); j++ )
      r += orthmxinv_[ i ][ j + fpindx_[ 0 ].size() ] 
	* rescaledData_[ fpindx_[ 1 ][ j ] ]->ncoef();
  
  return r;
}
