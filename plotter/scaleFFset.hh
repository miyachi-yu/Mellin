#ifndef _plotter_scaleFFset_hh_
#define _plotter_scaleFFset_hh_

#include <QCD/Flavor.hh>
#include <Utility/Arguments.hh>
#include <Fragmentation/Evolution.hh>
#include <ErrorCalc/ErrCalc.hh>
#include <Evolution/PDFx.hh>
#include <Evolution/PDF.hh>

#include <TCanvas.h>
#include <TGraph.h>
#include <TH1F.h>
#include <TLatex.h>
#include <TLegend.h>
#include <TSystem.h>
//#include <cmath>
//#include <ifstream>

using namespace std;
using namespace Utility;
using namespace QCD;

class scaleFFset {

  vector< string > vtitle_;

  vector< double > vscale_;
  
  vector< vector< double > > vchi2_; //! scale vs chi2 (w err)
  vector< vector< double > > vmoment_; //! scale vs moment (w err)

  vector< string > vxml_;

  map< double, map< int, vector< double > > > mapstatmom_;
  //! q2, flavor, momnet vector( center, stat err ) (scale=1.0)
  map< double, map< int, vector< double > > > mapscalemom_;
  //! q2, flavor, momnet vector( scaleerr up, scaleerr low )

  Utility::Arguments& args;
  int prec_;

  int offset_;
  TLegend* cl_;

public:
  scaleFFset() : vtitle_( 0 ), vscale_( 0 ), vchi2_( 0 ), vmoment_( 0 ),
		 vxml_( 0 ), offset_( 0 ), cl_( NULL ),
		 //		 mapstatmom_( 0 ), mapscalemom_( 0 ), 
		 args( Utility::Arguments::ref() ),
		 prec_( args.get( "precision", 3 ) ){};

  virtual ~scaleFFset(){};

  void plotvects_( const vector< string >& vtitle_,
		   const vector< double >& vscale_, 
		   const vector< vector< double > >& vchi2_ ){

    if( vscale_.size() != vchi2_.size() ) return;
    if( ! vscale_.size() || ! vchi2_.size() ) return;

    ostringstream ost, ostt;

    // TCanvas handling //
    TCanvas* c = new TCanvas( "c1" );
    if( ! args.hasOpt( "linear" ) ) c->SetLogx();

    // Frame handling //
    vector< double > vxylim_ = this->getframeparam( vscale_, vchi2_ );
    // xmin, ymin, xmax, ymax

    TH1F* frame = c->DrawFrame( vxylim_[ 0 ], vxylim_[ 1 ],
				vxylim_[ 2 ], vxylim_[ 3 ] );
    
    if( ! args.hasOpt( "noxtitle" ) )
      frame->GetXaxis()->SetTitle( vtitle_[ 0 ].c_str() );
    if( ! args.hasOpt( "noytitle" ) )
      frame->GetYaxis()->SetTitle( vtitle_[ 1 ].c_str() );

    // TGraph handling //
    vector< TGraph* > vtgraph_ = this->getTGraph( vscale_, vchi2_ );
    for( int i = 0; i < vtgraph_.size(); i++ ){
      ost << "noplot" << i + 1;
      if( ! args.hasOpt( ost.str().c_str() ) ){
	if( vtgraph_.size() == 2 && i == 1 ){
	  vtgraph_[ i ]->Draw( "FL" );
	  continue;
	}
	vtgraph_[ i ]->Draw( "L" );
      }
      ost.str( "" ); ost.clear();
    }

    vector< TGraph* > vaddtgraph_ = this->getaddTGraph( vxylim_ );
    for( int i = 0; i < vaddtgraph_.size(); i++ )
      vaddtgraph_[ i ]->Draw( "L" );

    // TLegend handling //
    //    TLegend* l = new TLegend( 0.65, 0.72, 0.89, 0.86 ); cl_ = l;
    TLegend* l = new TLegend( 0.70, 0.72, 0.89, 0.86 ); cl_ = l;
    l->SetFillColor( 0 );
    l->SetTextSize( 0.035 );
    if( vtitle_.size() > 3 ) l->SetHeader( vtitle_[ 3 ].c_str() );

    for( int i = 0; i < vtgraph_.size(); i++ ){
      ost << "legend" << i + 1;
      if( args.hasOpt( ost.str().c_str() ) ){
	ostt << "flegend" << i + 1;
	string pf_ = ( args.hasOpt( ostt.str().c_str() ) ? "f" : "l" );
	l->AddEntry( vtgraph_[ i ], args.get( ost.str().c_str(), "" ).c_str(), 
		     pf_.c_str() );
	ostt.str( "" ); ostt.clear();
      }
      ost.str( "" ); ost.clear();
    }
    if( l && !args.hasOpt( "nolegend" ) ) l->Draw();

    vector< TLatex* > vaddtlatex_ = this->getaddTLatex();
    for( int i = 0; i < vaddtlatex_.size(); i++ )
      vaddtlatex_[ i ]->Draw();

    if( c ) {
      string outsuffix_ = args.get( "suffix", "png" );
      string dirname_ = args.get( "dirname", "PLOT" );

      ost << dirname_ << "_" << vxylim_[ 0 ] << "-" << vxylim_[ 2 ];
      dirname_ = ost.str(); 
      ost.str( "" ); ost.clear();

      gSystem->mkdir( dirname_.c_str() );
      ost << dirname_ << "/" << vtitle_[ 2 ]
	  << "." << outsuffix_; 
      c->Print( ost.str().c_str() );
      ost.str( "" ); ost.clear();
    }

  };

  void addplotvects_( const vector< string >& vtitle_,
		      const vector< double >& vscale_, 
		      const vector< vector< double > >& vchi2_ ){

    if( vscale_.size() != vchi2_.size() ) return;
    if( ! vscale_.size() || ! vchi2_.size() ) return;

    ostringstream ost, ostt;
    TVirtualPad* c = gPad;

    vector< double > vxylim_ = this->getframeparam( vscale_, vchi2_ );
    // xmin, ymin, xmax, ymax

    // TGraph handling //
    vector< TGraph* > vtgraph_ = this->getTGraph( vscale_, vchi2_ );
    for( int i = 0; i < vtgraph_.size(); i++ ){
      ost << "noplot" << offset_ + i + 1;
      if( ! args.hasOpt( ost.str().c_str() ) ){
	if( vtgraph_.size() == 2 && i == 1 ){
	  vtgraph_[ i ]->Draw( "FLsame" );
	  continue;
	}
	vtgraph_[ i ]->Draw( "Lsame" );
      }
      ost.str( "" ); ost.clear();
    }

    // TLegend handling //
    TLegend* l = cl_;
    l->SetFillColor( 0 );
    if( l ){
      for( int i = 0; i < vtgraph_.size(); i++ ){
	ost << "legend" << offset_ + i + 1;
	if( args.hasOpt( ost.str().c_str() ) ){
	  ostt << "flegend" << offset_ + i + 1;
	  string pf_ = ( args.hasOpt( ostt.str().c_str() ) ? "f" : "l" );
	  l->AddEntry( vtgraph_[ i ], args.get( ost.str().c_str(), "" ).c_str(), 
		       pf_.c_str() );
	  ostt.str( "" ); ostt.clear();
	}
	ost.str( "" ); ost.clear();
      }
      if( ! args.hasOpt( "nolegend" ) ) l->Draw();
    }

    if( c ) {
      string outsuffix_ = args.get( "suffix", "png" );
      string dirname_ = args.get( "dirname", "PLOT" );

      ost << dirname_ << "_" << vxylim_[ 0 ] << "-" << vxylim_[ 2 ];
      dirname_ = ost.str(); 
      ost.str( "" ); ost.clear();

      gSystem->mkdir( dirname_.c_str() );
      ost << dirname_ << "/" << vtitle_[ 2 ]
	  << "." << outsuffix_; 
      c->Print( ost.str().c_str() ); // TPad overloaded 
      ost.str( "" ); ost.clear();
    }

  };

  // xmin, ymin, xmax, ymax
  vector< double > getframeparam( const vector< double >& vscale_, 
				  const vector< vector< double > >& vchi2_ ){
    vector< double > vxylim_( 4, 1.0 );
    for( int i = 0; i < vscale_.size(); i++ )
      if( vscale_[ i ] == 1.0 ){
	vxylim_[ 1 ] = vchi2_[ i ][ 0 ]; vxylim_[ 3 ] = vchi2_[ i ][ 0 ];
      }

    double& xmin_ = vxylim_[ 0 ]; double& ymin_ = vxylim_[ 1 ];
    double& xmax_ = vxylim_[ 2 ]; double& ymax_ = vxylim_[ 3 ];

    double ty_( 0.0 );

    for( int i = 0; i < vscale_.size(); i++ ){
      if( vscale_[ i ] < xmin_ ) xmin_ = vscale_[ i ];
      if( vscale_[ i ] > xmax_ ) xmax_ = vscale_[ i ];
      ty_ = ( vchi2_[ i ].size() == 1 ? vchi2_[ i ][ 0 ] : 
	      vchi2_[ i ][ 0 ] - vchi2_[ i ][ 1 ] );
      if( ty_ < ymin_ ) ymin_ = ty_;
      ty_ = ( vchi2_[ i ].size() == 1 ? vchi2_[ i ][ 0 ] : 
	      vchi2_[ i ][ 0 ] + vchi2_[ i ][ 1 ] );
      if( ty_ > ymax_ ) ymax_ = ty_;
    }

    double margr_ = args.get( "margratio", 0.1 );
    if( args.hasOpt( "linear" ) ){
      xmin_ -= margr_ * ( xmax_ - xmin_ );
      xmax_ += margr_ * ( xmax_ - xmin_ );
    } else {
      xmin_ /= pow( xmax_, margr_ ) / pow( xmin_, margr_ );
      xmax_ *= pow( xmax_, margr_ ) / pow( xmin_, margr_ );
    }
    double margry_ = args.get( "margratioy", 0.1 );
    ymin_ -= margry_ * ( ymax_ - ymin_ );
    ymax_ += margry_ * ( ymax_ - ymin_ );

    return vxylim_;
  };

  // include fill handling & color style handling
  vector< TGraph* > getTGraph( const vector< double >& vscale_, 
			       const vector< vector< double > >& vchi2_ ){

    vector< TGraph* > vtg_( 0 );

    vector< double > tvy_( vscale_.size(), 0.0 );
    for( int i = 0; i < vscale_.size(); i++ ){
      tvy_[ i ] = vchi2_[ i ][ 0 ];
    }
    
    // center line //
    vtg_.push_back( new TGraph( vscale_.size(), &vscale_[ 0 ], &tvy_[ 0 ] ) );

    if( vchi2_[ 0 ].size() != 1 ){

      int vi_ = ( args.hasOpt( "fill" ) ? 1 : 2 );
      vector< vector< double > > 
	vyband_( vi_, vector< double >( 2 / vi_ * vscale_.size(), 0.0 ) );
      vector< double > vxband_( 2 / vi_ * vscale_.size(), 0.0 );

      for( int i = 0; i < vscale_.size(); i++ ){
	if( vi_ == 2 ){
	  vyband_[ 0 ][ i ] = vchi2_[ i ][ 0 ] + vchi2_[ i ][ 1 ];
	  vyband_[ 1 ][ i ] = vchi2_[ i ][ 0 ] - vchi2_[ i ][ 1 ];
	  vxband_[ i ] = vscale_[ i ];
	} else {
	  vyband_[ 0 ][ i ] = vchi2_[ i ][ 0 ] + vchi2_[ i ][ 1 ];
	  vyband_[ 0 ][ 2 * vscale_.size() - 1 - i ] 
	    = vchi2_[ i ][ 0 ] - vchi2_[ i ][ 1 ];
	  vxband_[ i ] = vscale_[ i ];
	  vxband_[ 2 * vscale_.size() - 1 - i ] = vscale_[ i ];
	}
      }
      
      // error line //
      for( int i = 0; i < vyband_.size(); i++ ){
	vtg_.push_back( new TGraph( vyband_[ i ].size(), 
				    &vxband_[ 0 ], &vyband_[ i ][ 0 ] ) );
      }
      
    }
    
    // set color & style
    for( int i = 0; i < vtg_.size(); i++ ){
      ostringstream ost, ostt; 
      
      ost << "setLC" << offset_ + i + 1;
      if( args.hasOpt( ost.str().c_str() ) )
	vtg_[ i ]->SetLineColor( args.get( ost.str().c_str(), 1 ) );
      
      if( args.hasOpt( "fill" ) || i == 1 ){
	vtg_[ i ]->SetFillColor( args.get( ost.str().c_str(), 1 ) );
	ostt << "setFS" << offset_ + i + 1;
	vtg_[ i ]->SetFillStyle( args.get( ostt.str().c_str(), 0 ) );
	ostt.str( "" ); ostt.clear();
      }
      ost.str( "" ); ost.clear();
      
      ost << "setLS" << offset_ + i + 1;
      if( args.hasOpt( ost.str().c_str() ) )
	vtg_[ i ]->SetLineStyle( args.get( ost.str().c_str(), 1 ) );      
      ost.str( "" ); ost.clear();
      
      ost << "setLW" << offset_ + i + 1;
      if( args.hasOpt( ost.str().c_str() ) )
	vtg_[ i ]->SetLineWidth( args.get( ost.str().c_str(), 1 ) );      
      ost.str( "" ); ost.clear();
    }
    
    return vtg_;
  };
  
  vector< TGraph* > getaddTGraph( const vector< double >& vxylim_ ){

    vector< TGraph* > vtg_( 0 );

    // additional hline
    vector< double > vhs_ = args.get( "addhline", ",", 
				     vector< double >( 0 ) );
    vector< double > xminmax_( 2, 0.0 ); 
    xminmax_[ 0 ] = vxylim_[ 0 ]; xminmax_[ 1 ] = vxylim_[ 2 ];

    if( vhs_.size() ){
      
      vector< int > vhlc_ = args.get( "addhl_LC", ",", vector< int >( 0 ) );
      vector< int > vhls_ = args.get( "addhl_LS", ",", vector< int >( 0 ) );
      vector< int > vhlw_ = args.get( "addhl_LW", ",", vector< int >( 0 ) );
      
      for( int i = 0; i < vhs_.size(); i++ ){
	vector< double > hline_( 2, vhs_[ i ] );
	
	TGraph* tg_ = new TGraph( 2, &xminmax_[ 0 ], &hline_[ 0 ] );
	if( vhlc_.size() ) tg_->SetLineColor( vhlc_[ i ] );
	if( vhls_.size() ) tg_->SetLineStyle( vhls_[ i ] );
	if( vhlw_.size() ) tg_->SetLineWidth( vhlw_[ i ] );
	
	vtg_.push_back( tg_ );
      }
      
    }

    // additional vline
    vector< double > vvs_ = args.get( "addvline", ",", 
				      vector< double >( 0 ) ); 
    vector< double > yminmax_( 2, 0.0 ); 
    yminmax_[ 0 ] = vxylim_[ 1 ]; yminmax_[ 1 ] = vxylim_[ 3 ];

    if( vvs_.size() ){
      
      vector< int > vvlc_ = args.get( "addvl_LC", ",", vector< int >( 0 ) );
      vector< int > vvls_ = args.get( "addvl_LS", ",", vector< int >( 0 ) );
      vector< int > vvlw_ = args.get( "addvl_LW", ",", vector< int >( 0 ) );
      
      for( int i = 0; i < vvs_.size(); i++ ){
	vector< double > vline_( 2, vvs_[ i ] );
	
	TGraph* tg_ = new TGraph( 2, &vline_[ 0 ], &yminmax_[ 0 ] );
	if( vvlc_.size() ) tg_->SetLineColor( vvlc_[ i ] );
	if( vvls_.size() ) tg_->SetLineStyle( vvls_[ i ] );
	if( vvlw_.size() ) tg_->SetLineWidth( vvlw_[ i ] );
	
	vtg_.push_back( tg_ );
      }
      
    }
    
    return vtg_;
  };

  vector< TLatex* > getaddTLatex(){
    
    vector< TLatex* > vtl_( 0 );

    // additional hline
    vector< string > vstr_ = args.get( "addlatex", ",", 
				     vector< string >( 0 ) );
    vector< double > vlxx_ = args.get( "addlx_x", ",", vector< double >( 0 ) );
    vector< double > vlxy_ = args.get( "addlx_y", ",", vector< double >( 0 ) );

    vector< double > vlxTS_ = args.get( "addlx_TS", ",", vector< double >( 0 ) );
    
    if( vstr_.size() == vlxx_.size() == vlxy_.size() == vlxTS_.size() ){
      for( int i = 0; i < vstr_.size(); i++ ){
	vtl_.push_back( new TLatex( vlxx_[ i ], vlxy_[ i ], vstr_[ i ].c_str() ) );
	vtl_[ vtl_.size() - 1 ]->SetTextSize( vlxTS_[ i ] );
      }
    }

    return vtl_;

  }

  void insertxml( const string& xmlfile = "", const double& scale = 0.0 ){
    if( xmlfile.size() && scale ){
      vxml_.push_back( xmlfile );
      //      vscale_.push_back( scale );
      vscale_.push_back( 1.0 / scale );
    }
  };

  void plotchi2( const string& filename ){

    if( ! filename.size() ) return;

    ifstream ifs_( filename.c_str() );

    if( ifs_ ){
      double tscale_( 0.0 );
      double tdof_( 0.0 );
      double tchi2_( 0.0 );
      string tstr_( "" );

      while( getline( ifs_, tstr_ ) ){

	istringstream iss( tstr_ );
	if( iss >> tscale_ >> tdof_ >> tchi2_ ){
	  //	  vscale_.push_back( tscale_ );
	  vscale_.push_back( 1.0 / tscale_ );

	  vector< double > tvchi2_( 1, tchi2_ );
	  if( args.hasOpt( "werror" ) )
	    tvchi2_.push_back( sqrt( 2.0 / tdof_ ) );
	  vchi2_.push_back( tvchi2_ );
	}

      }

      // title handling
      //      vtitle_.push_back( "Q^{2} / #mu_{F=R}^{2}" );
      //      vtitle_.push_back( "#mu_{F=R}^{2} / Q^{2}" );
      vtitle_.push_back( "#mu_{F}^{2} / Q^{2}" );
      //      vtitle_.push_back( "#tilde{#chi}_{Total}^{2}" );
      //      vtitle_.push_back( "#tilde{#chi}^{2}" );
      vtitle_.push_back( "Reduced #chi^{2}" );
      vtitle_.push_back( "rchi2_plot" );

      // plot process
      this->plotvects_( vtitle_, vscale_, vchi2_ );
    }

    vscale_.resize( 0 );
    vchi2_.resize( 0 );
    vtitle_.resize( 0 );
  };

  void plotchi2( const vector< string >& vfilename ){

    if( ! vfilename.size() ) return;

    for( int i = 0; i < vfilename.size(); i++ ){

      ifstream ifs_( vfilename[ i ].c_str() );

      if( ifs_ ){
	double tscale_( 0.0 );
	double tdof_( 0.0 );
	double tchi2_( 0.0 );
	string tstr_( "" );
	
	while( getline( ifs_, tstr_ ) ){
	  
	  istringstream iss( tstr_ );
	  if( iss >> tscale_ >> tdof_ >> tchi2_ ){
	    //	    vscale_.push_back( tscale_ );
	    vscale_.push_back( 1.0 / tscale_ );
	    
	    vector< double > tvchi2_( 1, tchi2_ );
	    if( args.hasOpt( "werror" ) )
	      tvchi2_.push_back( sqrt( 2.0 / tdof_ ) );
	    vchi2_.push_back( tvchi2_ );
	  }
	  
	}
	
	// title handling
	//	vtitle_.push_back( "Q^{2} / #mu_{F=R}^{2}" );
	//	vtitle_.push_back( "#mu_{F=R}^{2} / Q^{2}" );
	vtitle_.push_back( "#mu_{F}^{2} / Q^{2}" );
	//	vtitle_.push_back( "#tilde{#chi}_{Total}^{2}" );
	//	vtitle_.push_back( "#tilde{#chi}^{2}" );
	vtitle_.push_back( "Reduced #chi^{2}" );
	vtitle_.push_back( "rchi2_plot" );
	
	// plot process
	if( ! i ){
	  this->plotvects_( vtitle_, vscale_, vchi2_ );
	  offset_ += ( args.hasOpt( "werror" ) ? 2 : 1 );
	} else {
	  this->addplotvects_( vtitle_, vscale_, vchi2_ );
	  offset_ += ( args.hasOpt( "werror" ) ? 2 : 1 );
	}
	
      }
      
      vscale_.resize( 0 );
      vchi2_.resize( 0 );
      vtitle_.resize( 0 );
    }

    offset_ = 0;
  };

  void plot( const int& mom, const double& Q2, const int& iflav ){

    vector< vector< double > > vmoment_;
    string flname_ = Flavor::name( Flavor::PARTON( iflav ) );
    string sort_ = args.get( "SORT", "FF" );
    int had_ = args.get( "HAD", 1 );
    string hname_ = ( abs( had_) < 2 ? "#" : "" ) +
      Flavor::name( Flavor::HADRON( had_ ), true );
    vector< double > tvx_( 0 );

    ostringstream ost;

    // making pdf data
    for( int i = 0; i < vscale_.size(); i++ ){

      args.set( sort_, vxml_[ i ] );

      Fragmentation::Evo evo( args, Flavor::HADRON( had_ ) );

      if( ! args.hasOpt( "partial" ) ){

	Evolution::PDF pdf( &evo, Flavor::PARTON( iflav ) );

	ErrorCalc::ErrCalc ecal( &evo ); // errorMX assign
	//	ecal.setPID( Flavor::PARTON( iflav ) );
	ecal.cfunccalc( true );
	ecal.setPDFcalc( false );
	ecal.setCompFunc( &pdf );
	
	evo.setQ2( Q2 );

	complex< double > cmom_( mom, 0.0 );

	vector< double > tvmom_( 1, pdf( cmom_ ).real() );
	if( args.hasOpt( "werror" ) )
	  tvmom_.push_back( sqrt( ecal( cmom_ ).real() ) );
	vmoment_.push_back( tvmom_ );

	ecal.cfunccalc( false );
	ecal.setPDFcalc( true );

      } else {

	vector< double > vx_ 
	  = args.get( "partial", ",", vector< double >( 0 ) );
	if( ! vx_.size() ) return;
	tvx_ = vx_;

	//	Evolution::PDFx pdfx( &evo, Flavor::PARTON( iflav ) );
	Evolution::PDFx pdfx( &evo, Flavor::PARTON( iflav ), 
			      4, 6, 1.0E-4, 15.0, 3.0, 0.55 );
        //      	      4, 6, 1.0E-4, 15.0, 2.0, 0.55 );
	pdfx.setMomParam( vector< double >( 1, mom ) );

	ErrorCalc::ErrCalc ecal( &evo ); // errorMX assign
	//	ecal.setPID( Flavor::PARTON( iflav ) );
	//	ecal.getPDFx()->setMomParam( vector< double >( 1, mom ) );
	ecal.rfunccalc( true );
	ecal.setPDFcalc( false );
	ecal.setRealFunc( &pdfx );

	evo.setQ2( Q2 );
	
	vector< double > tvmom_( 1, pdfx( vx_ ) );
	if( args.hasOpt( "werror" ) )
	  tvmom_.push_back( sqrt( ecal( vx_ ) ) );
	vmoment_.push_back( tvmom_ );

	ecal.rfunccalc( false );
	ecal.setPDFcalc( true );

      }

    }

    // for mapping
    if( args.hasOpt( "sumtable" ) ){
      this->setmaps( Q2, iflav, vscale_, vmoment_ );
      vmoment_.resize( 0 );
      return;
    }

    // title handling
    //    vtitle_.push_back( "#mu_{F=R}^{2}" );
    //    vtitle_.push_back( "#mu_{F}^{2}" );
    vtitle_.push_back( "#mu_{F}^{2} / Q^{2}" );

    ost << "#tilde{D}_{" << flname_ << "}^{" << hname_ << "}"
      //	<< "(" << mom << ", Q^{2}=" << Q2 << ")";
	<< "(" << mom << ", #mu_{F}^{2}=" << Q2 << ")";
    vtitle_.push_back( ost.str().c_str() );
    ost.str( "" ); ost.clear();

    ost << sort_ << "_" << flname_ << "_m" << mom << "_" << Q2;
    if( args.hasOpt( "partial" ) ) 
      ost << "_part_" << tvx_[ 0 ] << "-" << tvx_[ 1 ];
    vtitle_.push_back( ost.str().c_str() );
    ost.str( "" ); ost.clear();

    // plot process
    this->plotvects_( vtitle_, vscale_, vmoment_ );

    vscale_.resize( 0 );
    vmoment_.resize( 0 );
    vtitle_.resize( 0 );

  };

  void setmaps( const double& Q2, const int& iflav, 
		const vector< double >& vscale_, 
		const vector< vector< double > > vmoment_ ){

    vector< double > vscale_max_min_( 2, vmoment_[ 0 ][ 0 ] );
    // max, min in center values: vmoment[ i ][ 0 ]

    vector< int > si_( 2, 0 );
    // indices of max, min in vscale

    for( int i = 0; i < vscale_.size(); i++ ){

      if( vscale_[ i ] == 1.0 )
	mapstatmom_[ Q2 ][ iflav ] = vmoment_[ i ];

      // vmoment checking
      if( vmoment_[ i ][ 0 ] > vscale_max_min_[ 0 ] )
	vscale_max_min_[ 0 ] = vmoment_[ i ][ 0 ];
      if( vmoment_[ i ][ 0 ] < vscale_max_min_[ 1 ] )
	vscale_max_min_[ 1 ] = vmoment_[ i ][ 0 ];

      // vscale value checking
      if( vscale_[ i ] > vscale_[ si_[ 0 ] ] ) si_[ 0 ] = i;
      if( vscale_[ i ] < vscale_[ si_[ 1 ] ] ) si_[ 1 ] = i;

    }

    if( args.hasOpt( "scaledirect" ) ){
      vscale_max_min_[ 0 ] = ( vmoment_[ si_[ 0 ] ][ 0 ] > vmoment_[ si_[ 1 ] ][ 0 ] ? 
			       vmoment_[ si_[ 0 ] ][ 0 ] : vmoment_[ si_[ 1 ] ][ 0 ] );
      vscale_max_min_[ 1 ] = ( vmoment_[ si_[ 0 ] ][ 0 ] < vmoment_[ si_[ 1 ] ][ 0 ] ? 
			       vmoment_[ si_[ 0 ] ][ 0 ] : vmoment_[ si_[ 1 ] ][ 0 ] );
    }

    mapscalemom_[ Q2 ][ iflav ] = vscale_max_min_;

  };

  void sum_table( const int& mom ){

    if( ! args.hasOpt( "sumtable" ) ) return;

    vector< int > vflav_ = args.get( "flavlist", ",", vector< int >( 0 ) );
    vector< double > vq2_ = args.get( "q2list", ",", vector< double >( 0 ) );

    for( int i = 0; i < vq2_.size(); i++ )
      for( int j = 0; j < vflav_.size(); j++ )
	this->plot( mom, vq2_[ i ], vflav_[ j ] );

    // out put process
    ostringstream ost;

    string outsuffix_ = args.get( "suffix", "dat" );
    string dirname_ = args.get( "dirname", "TABLE" );
    string filename_( "" );

    // dir name
    vector< vector< double > > 
      tmpvv_( vscale_.size(), vector< double >( 1, 0.0 ) );
    vector< double > vxylim_ = this->getframeparam( vscale_, tmpvv_ );
    ost << dirname_ << "_" << vxylim_[ 0 ] << "-" << vxylim_[ 2 ];
    if( args.hasOpt( "scaledirect" ) )
      ost << "_scaledirect";
    dirname_ = ost.str(); 
    ost.str( "" ); ost.clear();
    gSystem->mkdir( dirname_.c_str() );

    // filename
    ost << "sumtable_D(" << mom << ")";
    if( args.hasOpt( "partial" ) ){
	vector< double > vx_ 
	  = args.get( "partial", ",", vector< double >( 0 ) );
	ost << "_part_" << vx_[ 0 ] << "-" << vx_[ 1 ];
    }
    filename_ = ost.str();
    ost.str( "" ); ost.clear();

    // write process
    ost << dirname_ << "/" << filename_ << "." << outsuffix_; 
    ofstream ofs( ost.str().c_str() );

    if( ofs ){
      ofs << mom << "th moment list" 
	  << " (center, stat err, scale upper, scale lower)" << endl;

      ofs << setw( 6 ) << "Q2" << flush;
      for( int i = 0; i < vflav_.size(); i++ ){
	ofs << setw( 4 * ( prec_ + 3 ) + 10 ) 
	    << Flavor::name( Flavor::PARTON( vflav_[ i ] ) )
	    << flush;
      }
      ofs << endl;

      for( int i = 0; i < vq2_.size(); i++ ){
	ofs << setw( 6 ) << vq2_[ i ] << flush;

	for( int j = 0; j < vflav_.size(); j++ ){
	  vector< double > tmpstatv_ 
	    = mapstatmom_[ vq2_[ i ] ][ vflav_[ j ] ];
	  vector< double > tmpscalev_ 
	    = mapscalemom_[ vq2_[ i ] ][ vflav_[ j ] ];

	  ofs << setw( prec_ + 3 ) << round( tmpstatv_[ 0 ] )
	      << " +- " 
	      << setw( prec_ + 3 ) << flush;
	  ofs << ( tmpstatv_.size() != 1 && round( tmpstatv_[ 0 ] ) ? 
		   round( tmpstatv_[ 1 ] ) : 0.0 )
	      << flush;

	  ofs << " + " 
	      << setw( prec_ + 3 ) << round( tmpscalev_[ 0 ] - tmpstatv_[ 0 ] )
	      << " - " 
	      << setw( prec_ + 3 ) << round( tmpstatv_[ 0 ] - tmpscalev_[ 1 ] )
	      << flush;
	}
	ofs << endl;
      }

    }
    ofs.close();
    ost.str( "" ); ost.clear();

  };

  double round( const double& val ){
    return rint( val * pow( 10.0, prec_ ) ) / pow( 10.0, prec_ );
  };
  
};

#endif // _plotter_scaleFFset_hh_
