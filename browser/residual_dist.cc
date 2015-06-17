#include <iostream>
#include <sstream>
#include <string>
#include <list>
#include <algorithm>

#include <TROOT.h>
#include <TApplication.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TCanvas.h>
#include <TH1.h>
#include <TLine.h>
#include <TLegend.h>
#include <TLatex.h>
#include <TSystem.h>

#include <Utility/Arguments.hh>
#include <QCD/Flavor.hh>
#include <Fragmentation/Evolution.hh>
#include <unpolPDF/Evolution.hh>
#include <polPDF/Evolution.hh>
#include <ErrorCalc/ErrCalc.hh>
#include <Evolution/PDFx.hh>

using namespace std;

class MyApplication : public TApplication {
public:
  
  MyApplication( int argc, char* argv[] ) : 
    TApplication( "myappli", &argc, argv ), 
    labels_( 0 ), canvas_( NULL ), vtg_( 0 ), line_(), lg_( NULL ), addlg_( NULL ), 
    addvtg_( 0 ), dbname_( "" ), vq2_( 0 ), vflav_( 0 ), 
    ymaxmin_( 2, 0.0 ), xmaxmin_( 2, 0.0 ),
    vevo_( 0 ), vxmls_( 0 ), vname_( 0 )
  {
    
    Utility::Arguments& args = Utility::Arguments::ref();
    
    if( args.hasOpt( "help" ) ) MyApplication::Usage();
    
    try {
      
      vq2_ = args.get( "q2", ",", vq2_ );
      vflav_ = args.get( "flavor", ",", vflav_ );

      ymaxmin_[ 0 ] = args.get( "ymax", 3.0 );    
      ymaxmin_[ 1 ] = - ymaxmin_[ 0 ];
      xmaxmin_[ 0 ] = args.get( "xmax", 1.0 );
      xmaxmin_[ 1 ] = args.get( "xmin", 0.01 );
      
      labels_ = args.get( "xytitle", ",", labels_ ); 

      canvas_ = new TCanvas( "c" );
      if( args.hasOpt( "logx" ) ) canvas_->SetLogx();
      if( args.hasOpt( "logy" ) ) canvas_->SetLogy();
      
      line_.SetLineWidth( 2 ); // for boundary
      line_.SetLineStyle( 2 );
    }
    
    catch( int error ){
      ARG_ERR << "catch error " << error << endl;
    }
    
  }
  
  virtual ~MyApplication(){
    if( canvas_ ) delete canvas_;
  }
  
  static void Usage() {
    Utility::Arguments& args = Utility::Arguments::ref();
    args.usage( "help",  "", "show this help" );
    args.usage( "FF,polPDF,unpolPDF", "", "set xmls with ordinary options" );
    args.usage( "q2",  "", "set q2 values (separated by ,)" );
    args.usage( "flavor",      "", "set flavor IDs (separated by ,)" );
    args.usage( "boundary",  "", "x boundary position (separated by ,)" );
    args.usage( "xmin",      "0.01", "x-axis lower value" );
    args.usage( "xmax",      "1.0", "x-axis upper value" );
    args.usage( "xdiv",      "100", "num. of x division for plot" );
    args.usage( "ymax",     "3", "y-axis upper/lower value" );
    args.usage( "format", "png", "Draing format (png,eps,ps,gif,jpg)" );
    args.usage( "logx",      "", "set x-axis in log" );
    args.usage( "logy",      "", "set y-axis in log" );
    args.usage( "xytitle",    "", "set xy-axis title (separated by ,)" );
    args.usage( "gstyle", "Plain", "gROOT style (Plain|Pub|Default)" );
    args.usage( "nodisplay", "false", "gROOT batch mode active" );
    args.usage( "nolegend", "false", "draw legend inactive" );
    args.usage( "tolerance", "1.0", "tolerance as a factor for error" );
    args.usage( "fill", "", "fill for horizontal boundary region" );
  }
    
  virtual void Run( Bool_t retrn = kFALSE ){
    
    Utility::Arguments& args = Utility::Arguments::ref();

    for( int i = 0; i < vq2_.size(); i++ )
      for( int j = 0; j < vflav_.size(); j++ ){
	this->draw( vq2_[ i ], vflav_[ j ] );
	if( vtg_.size() ){
	  ostringstream ostr;
	  ostr << dbname_ << "-residual_" 
	       << vq2_[ i ] << "_" << vflav_[ j ]
	       << "." << args.get( "format", "png" );
	  canvas_->Print( ostr.str().c_str() );
	}
	this->initialize();
      }
  }

private:
  
  TCanvas *canvas_;
  vector< TGraph* > vtg_; 
  TLine line_;
  TLegend* lg_;
  TLegend* addlg_;
  vector< TGraph* > addvtg_; 

  vector< string > labels_;
  string dbname_;
  string pdfname_;
  string xval_;
  string hadronname_;

  vector< double > vq2_;
  vector< int > vflav_;
  vector< double > ymaxmin_;
  vector< double > xmaxmin_;

  // moved from getTGraph
  vector< Evolution::KernelBase* > vevo_;
  vector< string > vxmls_;
  vector< string > vname_;

  void draw( const double& q2, const int& iflav ){
    
    Utility::Arguments& args = Utility::Arguments::ref();
    
    vector< string > vtag_( 0 ); // for legend
    vtg_ = this->getTGraphs( q2, iflav, vtag_ ); // get dbname also

    TH1 *frame = canvas_->DrawFrame( xmaxmin_[ 1 ], ymaxmin_[ 1 ], 
				     xmaxmin_[ 0 ], ymaxmin_[ 0 ] );

    if( labels_.size() == 2 ){
      TAxis* tax = frame->GetXaxis();
      tax->SetTitle( labels_[ 0 ].c_str() );
      tax->SetTitleSize( 0.052 );
      tax->SetTitleOffset( 0.50 );
      tax->SetLabelSize( 0.042 );
      tax->SetLabelOffset( 0.01 );
      TAxis* tay = frame->GetYaxis();
      tay->SetTitle( labels_[ 1 ].c_str() );
      tay->SetTitleSize( 0.052 );
      tay->SetTitleOffset( 0.70 );
      tay->SetLabelSize( 0.042 );
      tay->SetLabelOffset( 0.01 );
      // ( Dist - Fit ) / d(Fit_Err)
    }

    this->draw_boundary( vtag_ );

    this->setattributes();

    vector< int > tgorder_ = args.get( "tgorder", ",", vector< int >( 0 ) );
    if( ! tgorder_.size() ) for( int i = 0; i < vtg_.size(); i++ ) 
      tgorder_.push_back( i + 1 );

    for( int j = 0; j < tgorder_.size(); j++ )
      for( int i = 0; i < vtg_.size(); i++ )
	if( i + 1 == tgorder_[ j ] ){
	  ostringstream ost; ost << "noplot" << i + 1;
	  ostringstream ostt; ostt << "fillplot" << i + 1;
	  if( ! args.hasOpt( ost.str() ) )
	    ( args.hasOpt( ostt.str() ) ? vtg_[ i ]->Draw( "FL" ) 
	      : vtg_[ i ]->Draw() );
	  ost.str( "" ); ost.clear();
	  ostt.str( "" ); ostt.clear();
	}

    if( ! args.hasOpt( "nolegend" ) )
      this->setlegend( vtag_, get_header( q2, iflav ) );

    if( args.hasOpt( "addlegend" ) )
      this->setaddlegend( vtag_ );
    
    canvas_->Update();
  }

  void setlegend( const vector< string >& vtag_, const string& header_ ){

    Utility::Arguments& args = Utility::Arguments::ref();

    vector< double > vlpar_( 0 );
    vlpar_ = args.get( "legparam", ",", vlpar_ );
    if( vlpar_.size() != 4 ){
      lg_ = new TLegend( 0.12, 0.60, 0.42, 0.88 );
    } else {
      lg_ = new TLegend( vlpar_[ 0 ], vlpar_[ 1 ], 
			 vlpar_[ 2 ], vlpar_[ 3 ] ); 
    }
    lg_->SetFillColor( 0 );
    lg_->SetTextSize( args.get( "lgtxtsize", 0.038 ) );
    if( ! args.hasOpt( "noheader" ) )
      lg_->SetHeader( header_.c_str() );

    vector< int > lorder_ = args.get( "lorder", ",", vector< int >( 0 ) );
    if( ! lorder_.size() ) for( int i = 0; i < vtag_.size(); i++ ) 
      lorder_.push_back( i + 1 );
    
    for( int j = 0; j < lorder_.size(); j++ )
      for( int i = 0; i < vtag_.size(); i++ )
	if( i + 1 == lorder_[ j ] ){
	  ostringstream ost, ostt, ostf; 
	  ost << "nolegend" << i + 1; ostt << "legend" << i + 1; 
	  ostf << "flegend" << i + 1;
	  if( ! args.hasOpt( ost.str() ) ){
	    lg_->AddEntry( vtg_[ i ], args.get( ostt.str(), vtag_[ i ] ).c_str(),
			   ( args.hasOpt( ostf.str() ) ? "f" : "l" ) );
	  }
	  ost.str( "" ); ost.clear(); ostt.str( "" ); ostt.clear(); 
	  ostf.str( "" ); ostf.clear();
	}
    lg_->Draw();
  }

  void setaddlegend( const vector< string >& vtag_ ){

    Utility::Arguments& args = Utility::Arguments::ref();

    vector< double > vlpar_( 0 );
    vlpar_ = args.get( "addlegparam", ",", vlpar_ );
    if( vlpar_.size() != 4 ) return;

    addlg_ = new TLegend( vlpar_[ 0 ], vlpar_[ 1 ], 
			  vlpar_[ 2 ], vlpar_[ 3 ] ); 
    addlg_->SetFillColor( 0 );
    addlg_->SetFillStyle( 0 );
    addlg_->SetBorderSize( 0 );
    addlg_->SetTextSize( args.get( "addlgtxtsize", 0.038 ) );
    
    vector< int > lorder_ = args.get( "addlorder", ",", vector< int >( 0 ) );
    if( ! lorder_.size() ) for( int i = 0; i < vtag_.size(); i++ ) 
      lorder_.push_back( i + 1 );
    
    for( int j = 0; j < lorder_.size(); j++ )
      for( int i = 0; i < vtag_.size(); i++ )
	if( i + 1 == lorder_[ j ] ){
	  ostringstream ost, ostt, ostf; 
	  ost << "noaddlegend" << i + 1; ostt << "addlegend" << i + 1; 
	  ostf << "addflegend" << i + 1;
	  if( ! args.hasOpt( ost.str() ) ){
	    addlg_->AddEntry( vtg_[ i ], args.get( ostt.str(), vtag_[ i ] ).c_str(),
			   ( args.hasOpt( ostf.str() ) ? "f" : "l" ) );
	  }
	  ost.str( "" ); ost.clear(); ostt.str( "" ); ostt.clear(); 
	  ostf.str( "" ); ostf.clear();
	}
    addlg_->Draw();
  }
  
  vector< TGraph* > getTGraphs( const double& q2, const int& iflav, 
				vector< string >& vtag ){

    vector< TGraph* > tgv_( 0 );
    vector< vector< double > > scalefillvy_( 0 ); 
    vector< string > vtag_( 0 );

    Utility::Arguments& args = Utility::Arguments::ref();

    //!----------------- evolution set --------------------//
    /* moved to private variable in MyApplication
    vector< Evolution::KernelBase* > vevo_( 0 );
    vector< string > vxmls_( 0 );
    vector< string > vname_( 0 );
    */

    vector< string > polpdf_opts = polPDF::Evo::options();
    vector< string > unpolpdf_opts = unpolPDF::Evo::options();
    vector< string > ff_opts( 0 );

    for( vector< string >::iterator itr = polpdf_opts.begin();
	 itr != polpdf_opts.end(); itr++ ){
      if( args.hasOpt( *itr ) ){
	vevo_.push_back( new polPDF::Evo( args, *itr ) ); // polpdf
	vxmls_.push_back( args.get( *itr, "" ) );
	vname_.push_back( *itr );
	vtag_.push_back( *itr );
	pdfname_ = "x#Delta q"; hadronname_ = ""; xval_ = "x";
      }

      for( int j = 0; j < 2; j++ ){
	ostringstream ost; ost << "SCALE" << j + 1;
	string tmpname_( *itr + ost.str() );
	if( args.hasOpt( tmpname_ ) ){
	  args.set( *itr, args.get( tmpname_, "" ) );
	  vevo_.push_back( new polPDF::Evo( args, *itr ) );
	  vxmls_.push_back( args.get( *itr, "" ) );
	  vname_.push_back( tmpname_ );
	  vtag_.push_back( tmpname_ );
	}
      }

      for( int j = 0; j < 10; j++ ){
	ostringstream ost; ost << "ADD" << j + 1;
	string tmpname_( *itr + ost.str() );
	if( args.hasOpt( tmpname_ ) ){
	  args.set( *itr, args.get( tmpname_, "" ) );
	  vevo_.push_back( new polPDF::Evo( args, *itr ) );
	  vxmls_.push_back( args.get( *itr, "" ) );
	  vname_.push_back( tmpname_ );
	  vtag_.push_back( tmpname_ );
	}
      }
    }

    for( vector< string >::iterator itr = unpolpdf_opts.begin();
	 itr != unpolpdf_opts.end(); itr++ ){
      if( args.hasOpt( *itr ) ){
	vevo_.push_back( new unpolPDF::Evo( args, *itr ) ); // unpolpdf
	vxmls_.push_back( args.get( *itr, "" ) );
	vname_.push_back( *itr );
	vtag_.push_back( *itr );
	pdfname_ = "xq"; hadronname_ = ""; xval_ = "x";
      }

      for( int j = 0; j < 2; j++ ){
	ostringstream ost; ost << "SCALE" << j + 1;
	string tmpname_( *itr + ost.str() );
	if( args.hasOpt( tmpname_ ) ){
	  args.set( *itr, args.get( tmpname_, "" ) );
	  vevo_.push_back( new unpolPDF::Evo( args, *itr ) );
	  vxmls_.push_back( args.get( *itr, "" ) );
	  vname_.push_back( tmpname_ );
	  vtag_.push_back( tmpname_ );
	}
      }

      for( int j = 0; j < 10; j++ ){
	ostringstream ost; ost << "ADD" << j + 1;
	string tmpname_( *itr + ost.str() );
	if( args.hasOpt( tmpname_ ) ){
	  args.set( *itr, args.get( tmpname_, "" ) );
	  vevo_.push_back( new unpolPDF::Evo( args, *itr ) );
	  vxmls_.push_back( args.get( *itr, "" ) );
	  vname_.push_back( tmpname_ );
	  vtag_.push_back( tmpname_ );
	}
      }
    }

    for( int i = 1; i < 4; i++ ){ // pi+,ka+,pr+ ff
      ff_opts = Fragmentation::Evo::options( QCD::Flavor::HADRON( i ) );
      for( vector< string >::iterator itr = ff_opts.begin();
	   itr != ff_opts.end(); itr++ ){
        if( args.hasOpt( *itr ) ){
          vevo_.push_back( new Fragmentation::
			   Evo( args, QCD::Flavor::HADRON( i ), *itr ) );
	  vxmls_.push_back( args.get( *itr, "" ) );
	  vname_.push_back( *itr );
          vtag_.push_back( *itr );
	  pdfname_ = "zD"; xval_ = "z";
	  hadronname_ = "#" 
	    + QCD::Flavor::name( QCD::Flavor::HADRON( i ), true );
        }
	
	for( int j = 0; j < 2; j++ ){
	  ostringstream ost; ost << "SCALE" << j + 1;
	  string tmpname_( *itr + ost.str() );
	  if( args.hasOpt( tmpname_ ) ){
	    args.set( *itr, args.get( tmpname_, "" ) );
	    vevo_.push_back( new Fragmentation::
			     Evo( args, QCD::Flavor::HADRON( i ), *itr ) );
	    vxmls_.push_back( args.get( *itr, "" ) );
	    vname_.push_back( tmpname_ );
	    vtag_.push_back( tmpname_ );
	  }
	}

	for( int j = 0; j < 10; j++ ){
	  ostringstream ost; ost << "ADD" << j + 1;
	  string tmpname_( *itr + ost.str() );
	  if( args.hasOpt( tmpname_ ) ){
	    args.set( *itr, args.get( tmpname_, "" ) );
	    vevo_.push_back( new Fragmentation::
			     Evo( args, QCD::Flavor::HADRON( i ), *itr ) );
	    vxmls_.push_back( args.get( *itr, "" ) );
	    vname_.push_back( tmpname_ );
	    vtag_.push_back( tmpname_ );
	  }
	}
      }
    }    

    if( ! vevo_.size() ){
      cerr << "options for polpdf, unpolpdf, or " << flush;
      cerr << "ff were not found" << endl;
      return tgv_; 
    }

    //!----------------- errcalc set --------------------//
    //!--- check evo with error ---//
    int i_werr( -1 );
    for( int i = 0; i < vevo_.size(); i++ )
      if( vevo_[ i ]->initialDistribution()->emNumber() > 0 )
	//	if( vxmls_[ i ].find( args.get( "scalekey", "yoshi" ) ) 
	//	    == vxmls_[ i ].npos )
	if( vname_[ i ].find( "SCALE" ) == vxmls_[ i ].npos )
	  if( vname_[ i ].find( "ADD" ) == vxmls_[ i ].npos )
	    i_werr = i;
    if( i_werr < 0 ){
      cerr << "one of xmls should contain error matrix" << endl;
      return tgv_;
    }
	
    ErrorCalc::ErrCalc ecal( vevo_[ i_werr ] ); // errorMX assign
    dbname_ = vtag_[ i_werr ]; // get dbname (for filename) also
    vtag_.erase( vtag_.begin() + i_werr );
    vtag = vtag_; // copy to reverence vtag in argument

    //!----------------- calc prepare --------------------//
    vector< double > vx_base( 0 );
    int nxdiv_( args.get( "xdiv", 100 ) );
    for( int i = 0; i < nxdiv_; i++ ){
      if( args.hasOpt( "logx" ) ){
	double dx_ = ( log10( xmaxmin_[ 0 ] ) - 
		       log10( xmaxmin_[ 1 ] ) ) / nxdiv_;
	vx_base.push_back( xmaxmin_[ 1 ] * pow( 10.0, i * dx_ ) );
      } else {
	double dx_ = ( xmaxmin_[ 0 ] - xmaxmin_[ 1 ] ) / nxdiv_;
        vx_base.push_back( xmaxmin_[ 1 ] + dx_ * i );
      }
    }

    //!----------------- calc dist --------------------//
    //!--- calc base (w error)---//
    Evolution::PDFx pdfx( vevo_[ i_werr ], QCD::Flavor::PARTON( iflav ),
			  4, 6, 1.0E-4, 15.0, 2.0, 0.55 );
    //		  	  4, 6, 1.0E-4, 15.0, 3.0, 0.55 );
    ecal.rfunccalc( true );
    ecal.setPDFcalc( false );
    ecal.setRealFunc( &pdfx );

    vector< vector< double > > vy_base( 2, vector< double >( vx_base.size(), 0.0 ) );

    double tole_( args.get( "tolerance", 1.0 ) );
    for( int i = 0; i < vx_base.size(); i++ ){
      vector< double > tmpy_( 2, 0.0 ); // basic central & error
      if( getvfile( vx_base[ i ], q2, iflav, vname_[ i_werr ], 
		    vxmls_[ i_werr ], tmpy_ ) ){
      } else {
	vevo_[ i_werr ]->setQ2( q2 ); // (iteration makes no effect)
	tmpy_[ 0 ] = vx_base[ i ] * pdfx( vx_base[ i ] );
	tmpy_[ 1 ] = vx_base[ i ] * sqrt( ecal( vx_base[ i ] ) );
	if( ! setvfile( vx_base[ i ], q2, iflav, vname_[ i_werr ], 
			vxmls_[ i_werr ], tmpy_ ) ) throw( 10 );
      }
      vy_base[ 0 ][ i ] = ( tmpy_[ 0 ] );
      vy_base[ 1 ][ i ] = ( tole_ * tmpy_[ 1 ] );
    }

    ecal.rfunccalc( false );
    ecal.setPDFcalc( true );

    //!--- calcs ---//
    for( int i = 0; i < vevo_.size(); i++ ){
      if( i == i_werr ) continue;

      Evolution::PDFx pdfx( vevo_[ i ], QCD::Flavor::PARTON( iflav ),
			    4, 6, 1.0E-4, 15.0, 2.0, 0.55 );
      //       		    4, 6, 1.0E-4, 15.0, 3.0, 0.55 );
      vector< double > vy_tmp( 0 );

      for( int j = 0; j < vx_base.size(); j++ ){
	vector< double > tmpy_( 1, 0.0 ); // central value
	if( getvfile( vx_base[ j ], q2, iflav, vname_[ i ], 
		      vxmls_[ i ], tmpy_ ) ){
	} else {
	  vevo_[ i ]->setQ2( q2 ); // (iteration makes no effect)
	  tmpy_[ 0 ] = vx_base[ j ] * pdfx( vx_base[ j ] );
	  if( ! setvfile( vx_base[ j ], q2, iflav, vname_[ i ], 
			  vxmls_[ i ], tmpy_ ) ) throw( 10 );
	}
	vy_tmp.push_back( ( tmpy_[ 0 ] - vy_base[ 0 ][ j ] ) / vy_base[ 1 ][ j ] );
      }

      tgv_.push_back( new TGraph( vx_base.size(), 
				  &*vx_base.begin(), &*vy_tmp.begin() ) );

      // scale & fill
      if( args.hasOpt( "scale" ) && args.hasOpt( "fill" ) ){
	//	if( vxmls_[ i ].find( args.get( "scalekey", "" ) ) 
	//	!= vxmls_[ i ].npos )
	if( vname_[ i ].find( "SCALE" ) != vxmls_[ i ].npos )
	  scalefillvy_.push_back( vy_tmp );
      }

    }

    for( int i = 0; i < vevo_.size(); i++ ) 
      delete vevo_[ i ]; // delete all evolutions

    // additional TGraph for scale fill
    if( args.hasOpt( "scale" ) && args.hasOpt( "fill" ) ){

      if( scalefillvy_.size() != 2 ) throw( 2 );
      if( args.hasOpt( "scalelist" ) )
	getmaxmin( q2, iflav, vx_base, vy_base, scalefillvy_ );

      vector< double > tmpx_( 2 * vx_base.size(), 0.0 );
      vector< double > tmpy_( tmpx_.size(), 0.0 );
      for( int i = 0; i < vx_base.size(); i++ ){
	tmpx_[ i ] = vx_base[ i ];
	tmpy_[ i ] = scalefillvy_[ 0 ][ i ];
      }
      for( int i = vx_base.size() - 1; i > -1; i-- ){
	tmpx_[ 2 * vx_base.size() - i - 1 ] = vx_base[ i ];
	tmpy_[ 2 * vx_base.size() - i - 1 ] = scalefillvy_[ 1 ][ i ];
      }

      tgv_.push_back( new TGraph( tmpx_.size(), 
				  &*tmpx_.begin(), &*tmpy_.begin() ) );
      vtag_.push_back( "SCALE_FILL" ); vtag = vtag_; // copy to vtag again
    }

    // zero line
    vector< double > vzero_( xmaxmin_.size(), 0.0 );
    tgv_.push_back( new TGraph( xmaxmin_.size(),
				&*xmaxmin_.begin(), &*vzero_.begin() ) );
    vtag_.push_back( "ZERO_LINE" ); vtag = vtag_; // copy to vtag again

    if( tgv_.size() != vtag_.size() ) throw( 0 );
    return tgv_;
  }

  void setattributes(){

    Utility::Arguments& args = Utility::Arguments::ref();
    // set line attributes
    for( int i = 0; i < vtg_.size(); i++ ){
      ostringstream ost; 

      ost << "setLC" << i + 1;
      vtg_[ i ]->SetLineColor( args.get( ost.str(), 1 ) );
      ost.str( "" ); ost.clear();
      ost << "setLS" << i + 1;
      vtg_[ i ]->SetLineStyle( args.get( ost.str(), 1 ) );
      ost.str( "" ); ost.clear();
      ost << "setLW" << i + 1;
      vtg_[ i ]->SetLineWidth( args.get( ost.str(), 1 ) );
      ost.str( "" ); ost.clear();

      ost << "setFC" << i + 1;
      vtg_[ i ]->SetFillColor( args.get( ost.str(), 1 ) );
      ost.str( "" ); ost.clear();
      ost << "setFS" << i + 1;
      vtg_[ i ]->SetFillStyle( args.get( ost.str(), 3002 ) );
      ost.str( "" ); ost.clear();
    }

  }
  
  void getmaxmin( const double& q2, const int& iflav,
		  const vector< double >& vx_base,
		  const vector< vector< double > >& vy_base,
		  vector< vector< double > >& scalefillvy_ ){

    Utility::Arguments& args = Utility::Arguments::ref();

    ifstream ifs_( args.get( "scalelist", "" ).c_str() );

    if( ifs_ ){
      string scalexml_;
      vector< double >& y_max_ = scalefillvy_[ 0 ];
      vector< double >& y_min_ = scalefillvy_[ 1 ];

      cout << "SCALELIST LOADING:" << endl;

      while( ifs_ >> scalexml_ ){

	vector< string > polpdf_opts = polPDF::Evo::options();
	vector< string > unpolpdf_opts = unpolPDF::Evo::options();
	vector< string > ff_opts( 0 );

	string scale_base( "" );

	Evolution::KernelBase* evo_;

	for( vector< string >::iterator itr = polpdf_opts.begin();
	     itr != polpdf_opts.end(); itr++ ){
	  for( int j = 0; j < 2; j++ ){
	    ostringstream ost; ost << "SCALE" << j + 1;
	    string tmpname_( *itr + ost.str() );
	    if( args.hasOpt( tmpname_ ) ){
	      args.set( scale_base = *itr, scalexml_ );
	      evo_ = new polPDF::Evo( args, *itr );
	      break;
	    }
	  }
	}

	for( vector< string >::iterator itr = unpolpdf_opts.begin();
	     itr != unpolpdf_opts.end(); itr++ ){
	  for( int j = 0; j < 2; j++ ){
	    ostringstream ost; ost << "SCALE" << j + 1;
	    string tmpname_( *itr + ost.str() );
	    if( args.hasOpt( tmpname_ ) ){
	      args.set( scale_base = *itr, scalexml_ );
	      evo_ = new unpolPDF::Evo( args, *itr );
	      break;
	    }
	  }
	}

	for( int i = 1; i < 4; i++ ){ // pi+,ka+,pr+ ff
	  ff_opts = Fragmentation::Evo::options( QCD::Flavor::HADRON( i ) );
	  for( vector< string >::iterator itr = ff_opts.begin();
	       itr != ff_opts.end(); itr++ ){
	    for( int j = 0; j < 2; j++ ){
	      ostringstream ost; ost << "SCALE" << j + 1;
	      string tmpname_( *itr + ost.str() );
	      if( args.hasOpt( tmpname_ ) ){
		args.set( scale_base = *itr, scalexml_ );
		evo_ = new Fragmentation::
		  Evo( args, QCD::Flavor::HADRON( i ), *itr );
		break;
	      }
	    }
	  }
	}

	Evolution::PDFx pdfx( evo_, QCD::Flavor::PARTON( iflav ),
			      4, 6, 1.0E-4, 15.0, 2.0, 0.55 );
	//	4, 6, 1.0E-4, 15.0, 3.0, 0.55 );

	for( int j = 0; j < vx_base.size(); j++ ){
	  vector< double > tmpy_( 1, 0.0 ); // central value	  
	  if( getvfile( vx_base[ j ], q2, iflav, scale_base + "_SCALELIST",
			scalexml_, tmpy_ ) ){
	  } else {
	    evo_->setQ2( q2 ); // (iteration makes no effect)
	    tmpy_[ 0 ] = vx_base[ j ] * pdfx( vx_base[ j ] );
	    if( ! setvfile( vx_base[ j ], q2, iflav, scale_base + "_SCALELIST",
			    scalexml_, tmpy_ ) ) throw( 10 );
	  }
	  tmpy_[ 0 ] -= vy_base[ 0 ][ j ]; 
	  tmpy_[ 0 ] /= vy_base[ 1 ][ j ];
	  if( tmpy_[ 0 ] > y_max_[ j ] ) y_max_[ j ] = tmpy_[ 0 ];
	  if( tmpy_[ 0 ] < y_min_[ j ] ) y_min_[ j ] = tmpy_[ 0 ];
	}

	if( evo_ ) delete evo_;
      }
    }

  }

  bool getvfile( const double& x, const double& q2, const int& iflav, 
		 const string& name, const string& xml, 
		 vector< double >& tmpy ){
    bool werr_( tmpy.size() == 2 );

    gSystem->mkdir( name.c_str() );

    vector< string > tmpvdir_( 2, "" );
    getvdir( xml, tmpvdir_ );
    ostringstream ost; ost << name << "/" << q2 << "_" << iflav 
			   << "__" << tmpvdir_[ 0 ] << "__" << tmpvdir_[ 1 ] 
			   << "_.dat";
    ifstream ifs_( ost.str().c_str() );

    if( ifs_ ){
      string strline_;
      double tmpx_, tmpy_, tmpyerr_;

      while( getline( ifs_, strline_ ) ){
	istringstream iss( strline_ );
	iss >> tmpx_ >> tmpy_;
	if( werr_ ) iss >> tmpyerr_;

	if( abs( x - tmpx_ ) < 1.0E-6 ){
	  //	if( x == tmpx_ ){
	  tmpy[ 0 ] = tmpy_;
	  if( werr_ ) tmpy[ 1 ] = tmpyerr_;
	  return true;
	}
      }
    }

    ost.str( "" ); ost.clear();
    return false;
  }

  bool setvfile( const double& x, const double& q2, const int& iflav, 
		 const string& name, const string& xml, 
		 const vector< double >& tmpy ){
    bool werr_( tmpy.size() == 2 );

    vector< string > tmpvdir_( 2, "" );
    getvdir( xml, tmpvdir_ );
    ostringstream ost; ost << name << "/" << q2 << "_" << iflav 
			   << "__" << tmpvdir_[ 0 ] << "__" << tmpvdir_[ 1 ] 
			   << "_.dat";
    //    ofstream ofs_( ost.str().c_str(), ios::app );
    //    ofstream ofs_( ost.str().c_str(), ios::ate );
    ofstream ofs_( ost.str().c_str(), ios::out | ios::app );

    if( ofs_ ){
      cout << "writing to " << ost.str() << endl;
      ofs_ << setw( 12 ) << x << setw( 12 ) << tmpy[ 0 ];
      if( werr_ ) ofs_ << setw( 12 ) << tmpy[ 1 ];
      ofs_ << endl;
      return true;
    }

    ost.str( "" ); ost.clear();
    return false;
  }

  void getvdir( const string& xml, vector< string >& tmpvdir_ ){
    if( tmpvdir_.size() != 2 ) throw( 3 );

    string fname_ = xml.substr( xml.rfind( "/" ) + 1 ); 
    tmpvdir_[ 1 ] = fname_.substr( 0, fname_.size() - 4 ); //base file name
    int size_ = fname_.size() + 1;
    string part_ = xml.substr( 0, xml.size() - size_ );
    tmpvdir_[ 0 ] = part_.substr( part_.rfind( "/" ) + 1 ); //base dir name
  }

  void draw_boundary( vector< string >& vtag_ ){

    Utility::Arguments& args = Utility::Arguments::ref();

    // horizontal boundary
    if( ! args.hasOpt( "fill" ) ){

      double tole_( args.get( "tolerance", 1.0 ) ); 
      line_.SetLineColor( tole_ != 1.0 ? 3 : 2 );
      line_.DrawLine( xmaxmin_[ 1 ], -1.0, xmaxmin_[ 0 ], -1.0 );
      line_.DrawLine( xmaxmin_[ 1 ], +1.0, xmaxmin_[ 0 ], +1.0 );
      line_.SetLineColor( 1 );
      
      if( tole_ != 1.0 ){
	line_.SetLineColor( 2 );
	line_.DrawLine( xmaxmin_[ 1 ], -1.0 / tole_, 
			xmaxmin_[ 0 ], -1.0 / tole_ );
	line_.DrawLine( xmaxmin_[ 1 ], +1.0 / tole_, 
			xmaxmin_[ 0 ], +1.0 / tole_ );
	line_.SetLineColor( 1 );
      }

    } else {

      vector< double > tmpx_( 4, 0.0 );
      vector< double > tmpy_( 4, 0.0 );
      
      double tole_( args.get( "tolerance", 1.0 ) ); 
      tmpx_[ 0 ] = xmaxmin_[ 1 ]; tmpx_[ 1 ] = xmaxmin_[ 0 ];
      tmpx_[ 2 ] = xmaxmin_[ 0 ]; tmpx_[ 3 ] = xmaxmin_[ 1 ];
      tmpy_[ 0 ] = +1.0; tmpy_[ 1 ] = +1.0;
      tmpy_[ 2 ] = -1.0; tmpy_[ 3 ] = -1.0;
      TGraph* tmptg_ = new TGraph( tmpx_.size(), &*tmpx_.begin(),
				   &*tmpy_.begin() );
      /*
      tmptg_->SetFillColor( tole_ != 1.0 ? 3 : 2 );
      tmptg_->SetFillStyle( args.get( "shade1", 3006 ) );
      tmptg_->Draw( "FL" );
      */
      vtg_.push_back( tmptg_ );
      vtag_.push_back( "TOL_FILL" );
      //      addvtg_.push_back( tmptg_ );
      
      if( tole_ != 1.0 ){
	tmpy_[ 0 ] = +1.0 / tole_; tmpy_[ 1 ] = +1.0 / tole_;
	tmpy_[ 2 ] = -1.0 / tole_; tmpy_[ 3 ] = -1.0 / tole_;
	TGraph* tmptg2_ = new TGraph( tmpx_.size(), &*tmpx_.begin(),
				     &*tmpy_.begin() );
	/*
	tmptg2_->SetFillColor( 2 );
	tmptg2_->SetFillStyle( args.get( "shade2", 3001 ) );
	tmptg2_->Draw( "FL" );
	*/
	vtg_.push_back( tmptg2_ );
	vtag_.push_back( "ERROR_FILL" );
	//	addvtg_.push_back( tmptg2_ );
      }

    }

    // vertical boundary
    if( args.hasOpt( "boundary" ) ){
      line_.SetLineColor( 6 );
      vector< double > xb( 0 );
      xb = args.get( "boundary", ",", xb );
      for( int i = 0; i < xb.size(); i++ )
	line_.DrawLine( xb[ i ], ymaxmin_[ 1 ], xb[ i ], ymaxmin_[ 0 ] );
      line_.SetLineColor( 1 );
    }
    
  }

  string get_header( const double& q2, const int& iflav ){
    ostringstream ost;
    ost << pdfname_ << "_{"
	<< QCD::Flavor::name( QCD::Flavor::PARTON( iflav ) )
	<< "}^{" << hadronname_ << "}( " 
	<< xval_ << ", #mu_{F}^{2}=" << q2 << " GeV^{2} )" << endl;
    return ost.str();
  }

  void initialize(){
    for( int i = 0; i < vtg_.size(); i++ ) 
      delete vtg_[ i ]; // delete all tgraphs created in getTGraphs
    vtg_.resize( 0 );
    if( lg_ ) delete lg_;
    lg_ = NULL;
    if( addlg_ ) delete addlg_;
    addlg_ = NULL;
    for( int i = 0; i < addvtg_.size(); i++ ) 
      delete addvtg_[ i ]; // delete all tgraphs created in draw_boundary
    addvtg_.resize( 0 );
  }

};

int main( int argc, char* argv[] ){

  Utility::Arguments& args = Utility::Arguments::ref( argc, argv );
  gROOT->SetStyle( args.get( "gstyle", "Plain" ).c_str() );
  gROOT->SetBatch( args.hasOpt( "nodisplay" ) );
  // or simply set '--b' for the batch mode 
  // (become active through TApplication)

  MyApplication app( argc, argv );
  app.Run();
  
  return 0;
}

