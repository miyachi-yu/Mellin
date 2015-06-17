#include <stdio.h>
#include <iostream> // cout(cin)
#include <iomanip>  // setw
#include <fstream>  // o(i)fstream
#include <sstream>  // o(i)stringstream
#include <cmath>
#include <map>
#include <vector>

#include <Utility/Arguments.hh>

#include <TApplication.h>
#include <TROOT.h>
#include <TCanvas.h>
#include <TAxis.h>
#include <TArrow.h>
#include <TMarker.h>
#include <TLegend.h>
#include <TH1F.h>
#include <TGraph.h>
#include <TText.h>
#include <TLatex.h>
#include <Math/DistFunc.h>
//#include <Math/QuantFuncMathMore.h>

#include "TGraVmk.hh"

using namespace std;
using namespace Utility;

bool Tchi2plots( const string& dn );
bool Tsumplot( const TGraVmk& tgv );
bool Dchi2plots( const string& dn );
bool Dsumplot( const TGraVmk& tgv, const int& evid );

bool Echi2plots( const string& dn, const vector< string >& dlistv );
bool Esumplot( const vector< string >& dname_, const vector< double >& chi2_, 
	       const vector< double >& chi2err_ );
bool Esumplot2( const vector< string >& dname_, const vector< double >& chi2_, 
		const vector< double >& chi2err_ );

vector< double > setCLvals( const double& perc, const vector< int >& nfreev, 
			    const vector< double >& chi2_0v );

vector< vector < double > > getTolerance( const TGraVmk& tgv, 
					  const double& cl68,
					  const double& cl90 );

bool Toleplot( map< string, vector< vector< double > > > *tv, 
	       const int& evid, vector< double >& clv, 
	       vector< string >& clsv,
	       const vector< string >& dlistv );
//= vector< string >( 1, "" ) );

bool ALLToleplot( const vector< vector< double > >& clvv,
		  const vector< vector< string > >& clsvv );

bool Tchi2plots_2( const string& dn );
bool Tsumplot_2( const TGraVmk& tgv, const string& hadid );

string gethadname( const string& hadid );

string odirname_;

int main( int argc, char* argv[] ){

  Arguments& args = *( Arguments::instance( argc, argv ) );
  TApplication TA( "browser", &argc, argv ); //needed if you use root

  gROOT->SetStyle( args.get( "gstyle", "Plain" ).c_str() );
  gROOT->SetBatch( Arguments::ref().hasOpt( "nodisplay" ) );
  // or simply set '--b' for the batch mode 
  // (become active through TApplication)

  bool tchi2_( args.hasOpt( "TChi2" ) );
  bool dchi2_( args.hasOpt( "DChi2" ) );
  bool ffsidis_( args.hasOpt( "FFSIDIS" ) );  

  if( ! tchi2_ && ! dchi2_ ){
    cout << "Need --TChi2 or --DChi2 option" << endl;
    return 0;
  }

  if( ffsidis_ && dchi2_ ){
    cout << "--FFSIDIS requires --TChi2 option (!= DChi2)" << endl;
    return 0;
  }
  
  string dirname_ = ( tchi2_ ? args.get( "TChi2", "" ) 
		      : args.get( "DChi2", "" ) );

  odirname_ = args.get( "odir", "." );

  if( ! ffsidis_ ){
    if( tchi2_ && ! Tchi2plots( dirname_ ) )
      cout << "fail to create tchi2 plots" << endl;
    if( dchi2_ && ! Dchi2plots( dirname_ ) )
      cout << "fail to create dchi2 plots" << endl;    

    if( ! Echi2plots( dirname_, vector< string >( 1, "" ) ) )
      cout << "fail to create error chi2 plots" << endl;

  } else {
    if( tchi2_ && ! Tchi2plots_2( dirname_ ) )
      cout << "fail to create tchi2 plots" << endl;
  }

  return 0;
}

bool Tchi2plots( const string& dn ){
  int imax( 100 );

  Arguments& args = Arguments::ref();

  TGraVmk tgv_;
  tgv_.setbaseinfo( "Total Chi2 vs Eigen Vector (Tolerance)" );
  tgv_.setxlimit( Arguments::ref().get( "TGxlim", 20.0 ) );
  tgv_.setyfrag( true ); // dy > 0 && dy < (dx)^2 

  for( int i = 1; i < imax; i++ ){

    if( args.hasOpt( "xupperlim" ) )
      tgv_.setxupperlimit( args.get( "xupperlim", 0.0 ) );
    if( args.hasOpt( "xlowerlim" ) )
      tgv_.setxlowerlimit( args.get( "xlowerlim", 0.0 ) );

    ostringstream ost;

    ost << "xupperlim" << i;
    if( args.hasOpt( ost.str() ) )
      tgv_.setxupperlimit( args.get( ost.str(), 0.0 ) );
    ost.str( "" ); ost.clear();
    ost << "xlowerlim" << i;
    if( args.hasOpt( ost.str() ) )
      tgv_.setxlowerlimit( args.get( ost.str(), 0.0 ) );
    ost.str( "" ); ost.clear();

    ost << dn << "/TDATA/LM_EVID_" << i << ".dat";

    ifstream ifs( ost.str().c_str() );

    if( ! ifs ){
      cout << "fail to open file: " << ost.str() << endl;
      break;
    }
    
    cout << "opening file: " << ost.str() << endl;
    
    string strline;
    int n_( 0 );
    vector< double > x_( 0 ), y_( 0 );
    int id_; double lamb_;
    
    while( getline( ifs, strline ) ){
      
      istringstream iss( strline );
      x_.push_back( 0.0 ); y_.push_back( 0.0 );
      
      iss >> id_ >> lamb_ >> x_[ n_ ] >> y_[ n_ ];
      
      n_++;
    }
    
    if( ! tgv_.setvec( x_, y_ ) ){
      cout << "fail to create TGraph for " 
	   << ost.str() << endl;
      return false;
    }
    
    ostringstream ostt; ostt << "Eigen Vector No." << i;
    tgv_.setinfo( ostt.str() );
  }
  
  // set parabolic sample 
  tgv_.getfparv()[ 3 ] = 1.0;
  tgv_.setsampleTG();
  // set 0 line sample 
  tgv_.setsampleTG();

  if( tgv_.size() != tgv_.getvinfo().size() ){
    cout << "TGraph & info sizes inconsistensy" << endl;
    return false;
  }
  
  return Tsumplot( tgv_ );
}

bool Tsumplot( const TGraVmk& tgv ){

  try {

    int nplot_( 6 ); // number of plot in one sheet (int!)
    //    int nplot_( 1 ); // number of plot in one sheet (int!)

    // start sheet loop 
    for( int i = 0; i < ( tgv.size() - 1 ) / nplot_ + 1; i++ ){

      // Create Canvas & Divide//
      TCanvas c( "Tchi2 vs EigV" );
      c.SetTitle( tgv.getbaseinfo().c_str() );
      c.Divide( 2, int( nplot_ / 2.0 + 0.5 ) );
      //      c.Divide( 1, int( nplot_ / 1.0 + 0.5 ) );

      // Plot each TGraphs //
      vector< TLegend* > lv_( 0 ); // legend should exist til c.Draw()
      vector< TLatex* > tv_( 0 ); //  should exist til c.Draw()
      // start plot loop
      for( int j = 0; j < nplot_; j++ ){

	if( j + i * nplot_ > tgv.size() - 1 ) break;

	int pid_( j + i * nplot_ );

	c.cd( j + 1 );

	// Frame //
	double xlim_( tgv.getxlimit() );
	double ylim_( tgv.getylimit() );
	TH1F* frame 
	  = gPad->DrawFrame( - 1.2 * xlim_, - 0.1 * ylim_, 
			     1.2 * xlim_, ylim_ );

	// Titles //
	TAxis* ta = frame->GetXaxis();
	ta->SetLabelSize( 0.05 );
	ostringstream ost; 
	ost << "T_{" << j + i * nplot_ + 1 << "}";
	//	ost << " (#Delta_{EV}/#sigma_{" << j + i * nplot_ + 1 << "})";
	ta->SetTitle( ost.str().c_str() );
	ost.str( "" ); ost.clear();
	ta->SetTitleSize( 0.07 );
	ta->SetTitleOffset( 0.6 );
	ost << "#Delta #chi^{2}_{TOTAL}";
	ta = frame->GetYaxis();
	ta->SetLabelSize( 0.05 );
	ta->SetTitle( ost.str().c_str() );
	ost.str( "" ); ost.clear();
	ta->SetTitleSize( 0.07 );
	ta->SetTitleOffset( 0.7 );

	// Plots //
	// chi2 plot //
	tgv[ pid_ ]->SetMarkerSize( 1 );
	tgv[ pid_ ]->SetMarkerStyle( 20 );
	tgv[ pid_ ]->SetMarkerColor( 1 );
	tgv[ pid_ ]->Draw( "Psame" );
	// reference plot (para line)//
	tgv.getsampleTG()[ 0 ]->SetLineStyle( 2 );
	tgv.getsampleTG()[ 0 ]->SetLineColor( 2 );
	tgv.getsampleTG()[ 0 ]->Draw( "Csame" );
	// reference plot (zero line)//
	tgv.getsampleTG()[ 1 ]->SetLineStyle( 1 );
	tgv.getsampleTG()[ 1 ]->SetLineColor( 1 );
	tgv.getsampleTG()[ 1 ]->Draw( "Csame" );

	// Legend //
	TLegend* l = new TLegend( 0.12, 0.14, 0.30, 0.27 );
	l->SetFillColor( 0 );
	l->SetTextSize( 0.05 );
	//	l->AddEntry( tgv[ j + i * nplot_ ], "#Delta #chi2", "p" );
	l->AddEntry( tgv[ pid_ ], "  Lag. Multi.", "p" );
	l->AddEntry( tgv.getsampleTG()[ 0 ], "  Ideal", "l" );
	l->Draw();
	lv_.push_back( l );

	// Text
	TLatex* la = new TLatex( - 0.5 * xlim_, 0.8 * ylim_, 
			       tgv.getvinfo()[ pid_ ].c_str() );
	la->SetTextSize( 0.10 );
	la->Draw();
	tv_.push_back( la );

      }
      // end plot loop

      // draw process //
      if( &c ){
	//	if( ! Arguments::ref().hasOpt( "nodisplay" ) ) c.Draw(); 
	ostringstream ostf;
	ostf << odirname_ << "/TChi2_plot_" << i + 1 << ".";
	string png_( ostf.str() + Arguments::ref().get( "format", "png" ) );
	c.Print( png_.c_str() );
	string pdf_( ostf.str() + "pdf" );
	c.Print( pdf_.c_str() );
	//	c.Clear();
      }

      for( int k = 0; k < lv_.size(); k++ )
	delete lv_[ k ];
      for( int k = 0; k < tv_.size(); k++ )
	delete tv_[ k ];

    }
    // end sheet loop 

  }
  
  catch( int err ){
    cout << "fail to create sumplots at the stage " 
	 << err << endl;
    return false;
  }

  return true;
}

bool Dchi2plots( const string& dn ){

  // GET Data Set Information //
  vector< string > dlistv_( 0 );
  vector< int > usedv_( 0 ), freev_( 0 );
  vector< double > dc2_0v_( 0 );

  string fdlist_ = dn + "/DDATA/DSET_list.dat";
  ifstream ifsl( fdlist_.c_str() );
  if( ifsl ){
    string tmpds_;
    double used_, free_, dc2_0_;
    while( ifsl >> tmpds_ >> used_ >> free_ >> dc2_0_ ){
      dlistv_.push_back( tmpds_ );
      usedv_.push_back( int( used_ ) );
      freev_.push_back( int( free_ ) );
      dc2_0v_.push_back( dc2_0_ );
    }
  }

  if( ! dlistv_.size() ){
    cout << "could not find dset list in " << fdlist_ << endl;
    return false;
  } else {
    cout << "--- DATA SET LIST ---" << endl;
    for( int i = 0; i < dlistv_.size(); i++ )
      cout << dlistv_[ i ] << endl;
    cout << "---------------------" << endl;
  }

  // Set CL Values (Permissive Deviation from the Global Minima) //
  vector< double > CL90_( dlistv_.size(), 0.0 );
  vector< double > CL68_( dlistv_.size(), 0.0 );
  //  vector< double > CL84_( dlistv_.size(), 0.0 );
  CL90_ = setCLvals( 0.90, usedv_, dc2_0v_ );
  CL68_ = setCLvals( 0.68, usedv_, dc2_0v_ );
  //  CL84_ = setCLvals( 0.84, usedv_, dc2_0v_ );

  // For Total Tolerance Plot //
  vector< vector< double > > clvv_( 0 );
  vector< vector< string > > clsvv_( 0 );

  // PLOT Manipulation Start //
  int imax( 100 );

  Arguments& args = Arguments::ref();

  // Eigen Vector loop //
  for( int i = 1; i < imax; i++ ){

    TGraVmk tgv_;
    ostringstream oo; oo << "DataSet Chi2 vs Eig. Vec. No." << i;
    tgv_.setbaseinfo( oo.str() );
    tgv_.setxlimit( Arguments::ref().get( "TGxlim", 20.0 ) );
    tgv_.setyfrag( false ); // no cond. on y

    if( args.hasOpt( "xupperlim" ) )
      tgv_.setxupperlimit( args.get( "xupperlim", 0.0 ) );
    if( args.hasOpt( "xlowerlim" ) )
      tgv_.setxlowerlimit( args.get( "xlowerlim", 0.0 ) );

    ostringstream osttmp;

    osttmp << "xupperlim" << i;
    if( args.hasOpt( osttmp.str() ) )
      tgv_.setxupperlimit( args.get( osttmp.str(), 0.0 ) );
    osttmp.str( "" ); osttmp.clear();
    osttmp << "xlowerlim" << i;
    if( args.hasOpt( osttmp.str() ) )
      tgv_.setxlowerlimit( args.get( osttmp.str(), 0.0 ) );
    osttmp.str( "" ); osttmp.clear();

    // for tole plot for each eig vec //
    map< string, vector< vector < double > > > tolev_;
    // for tole plot for all eig vecs //
    vector< double > tmpclv_( 2, 0.0 );
    vector< string > tmpclsv_( 2, "" );

    // Dset Loop //
    for( int j = 0; j < dlistv_.size(); j++ ){

      // set ylimit for each dataset //
      tgv_.setylimit( CL90_[ j ] );

      ostringstream ost;
      ost << dn << "/DDATA/LM_EVID_" << i 
	  << "_" << dlistv_[ j ] << ".dat";
      
      ifstream ifs( ost.str().c_str() );
      
      if( ! ifs ){
	cout << "fail to open file: " << ost.str() << endl;
	goto dbrk;
      } 

      cout << "opening file: " << ost.str() << endl;
      
      string strline;
      int n_( 0 );
      vector< double > x_( 0 ), y_( 0 );
      int id_; double lamb_;
      
      while( getline( ifs, strline ) ){
	
	istringstream iss( strline );
	x_.push_back( 0.0 ); y_.push_back( 0.0 );
	
	iss >> id_ >> lamb_ >> x_[ n_ ] >> y_[ n_ ];
	
	n_++;
      }
      
      if( ! tgv_.setvec( x_, y_ ) ){
	cout << "fail to create TGraph for " 
	     << ost.str() << endl;
	return false;
      }

      /*      
      ostringstream ostt; ostt << dlistv_[ j ] << " (U/F=" 
			       << usedv_[ j ] << "/" 
			       << freev_[ j ] << ")";
      ostringstream ost2; ost2 << "Org Chi2=" << dc2_0v_[ j ];
      tgv_.setinfo( ostt.str() );
      tgv_.setinfo2( ost2.str() );
      */
      ostringstream ostt; ostt << dlistv_[ j ];
      tgv_.setinfo( ostt.str() );

      // set fit polynominal sample 
      tgv_.polyfitToTG( tgv_.size() - 1 );
      tolev_[ dlistv_[ j ] ] = getTolerance( tgv_, CL68_[ j ], CL90_[ j ] );
      //      tolev_[ ostt.str() ];
      tgv_.setsampleTG();
      // set 68% CL line sample 
      tgv_.getfparv()[ 0 ] = CL68_[ j ];
      tgv_.setsampleTG();
      // set 90% CL line sample 
      tgv_.getfparv()[ 0 ] = CL90_[ j ];
      tgv_.setsampleTG();
    }

    // set 0 line sample 
    tgv_.getfparv()[ 1 ] = 0.0;
    tgv_.setsampleTG();
    
    if( tgv_.size() != tgv_.getvinfo().size() ){
      cout << "TGraph & info sizes inconsistensy" << endl;
      return false;
    }

    if( ! Dsumplot( tgv_, i ) ){
      cout << "fail to make sumplot for Ev No." << i << endl;  
      return false;
    }

    //    if( ! ( args.hasOpt( "ORG" ) ? Toleplot( &tolev_, i, tmpclv_, tmpclsv_ )
    if( ! ( args.hasOpt( "ORG" ) ? Toleplot( &tolev_, i, tmpclv_, tmpclsv_, 
					     vector< string >( 1, "" ) )
	    : Toleplot( &tolev_, i, tmpclv_, tmpclsv_, dlistv_ ) ) ){
      cout << "fail to make tolerance plot for Ev No." << i << endl;  
      return false;
    }

    clvv_.push_back( tmpclv_ );
    clsvv_.push_back( tmpclsv_ );
  }

  dbrk:

  if( ! ALLToleplot( clvv_, clsvv_ ) ){
    cout << "fail to make total tolerance plot" << endl;  
    return false;
  }

  if( ! ( args.hasOpt( "ORG" ) ? Echi2plots( dn, vector< string >( 1, "" ) ) :
	  Echi2plots( dn, dlistv_ ) ) )
    cout << "fail to create error chi2 plots" << endl;
  
  return true;
}

bool Dsumplot( const TGraVmk& tgv, const int& evid ){

  try {

    Arguments& args = Arguments::ref();

    int nplot_( 6 ); // number of plot in one sheet (int!)

    // start sheet loop 
    for( int i = 0; i < ( tgv.size() - 1 ) / nplot_ + 1; i++ ){

      // Create Canvas & Divide//
      TCanvas c( "Dchi2 vs EigV", "", 700, 500 );
      c.SetTitle( tgv.getbaseinfo().c_str() );
      c.Divide( 2, int( nplot_ / 2.0 + 0.5 ) );

      // Plot each TGraphs //
      vector< TLegend* > lv_( 0 ); // legend should exist til c.Draw()
      vector< TLatex* > tv_( 0 ); //  should exist til c.Draw()
      // start plot loop
      for( int j = 0; j < nplot_; j++ ){

	if( j + i * nplot_ > tgv.size() - 1 ) break;

	int pid_( j + i * nplot_ );

	c.cd( j + 1 );
	//	gPad->SetTopMargin( 0.15 ); // wo text
       	gPad->SetTopMargin( 0.10 );
       	gPad->SetBottomMargin( 0.15 );
       	gPad->SetLeftMargin( 0.15 );
       	gPad->SetRightMargin( 0.0 );

	// Frame //
	double xlim_( tgv.getxlimit() );
	double ymin_( tgv.getvminmax()[ pid_ ][ 2 ] );

	double plotymax_ = tgv.getvminmax()[ pid_ ][ 3 ];
	double ntclymax_ = tgv.getsampleTG()[ 2 + 3 * pid_ ]->GetY()[ 0 ];
	double ymax_( ntclymax_ > plotymax_ ? ntclymax_ : plotymax_ );

	double ydif_ = ymax_ - ymin_;

	TH1F* frame 
	  = gPad->DrawFrame( - 1.2 * xlim_, ymin_ - 0.1 * ydif_, 
			     1.2 * xlim_, ymax_ + 0.1 * ydif_ );

	// Titles //
	TAxis* ta = frame->GetXaxis();
	//	ta->SetLabelSize( 0.05 );
	ta->SetLabelSize( 0.08 );
	ta->SetLabelOffset( 0.02 );
	ostringstream ost; 
	ost << "T_{" << evid << "}";
	//	ost << " (#Delta_{EV}/#sigma_{" << evid << "})";
	ta->SetTitle( ost.str().c_str() );
	ost.str( "" ); ost.clear();
	//	ta->SetTitleSize( 0.07 );
	//	ta->SetTitleOffset( 0.6 );
	ta->SetTitleSize( 0.10 );
	ta->SetTitleOffset( 0.6 );
	//	ostringstream ost;
	//	ost << "#Delta #chi^{2}_{DATA}";
	ost << "#Delta #chi^{2}_{DS}";
	ta = frame->GetYaxis();
	//	ta->SetLabelSize( 0.05 );
	ta->SetLabelSize( 0.08 );
	ta->SetLabelOffset( 0.02 );
	ta->SetTitle( ost.str().c_str() );
	ost.str( "" ); ost.clear();
	//	ta->SetTitleSize( 0.07 );
	//	ta->SetTitleOffset( 0.7 );
	ta->SetTitleSize( 0.12 );
	ta->SetTitleOffset( 0.55 );

	// Plots //
	// chi2 plot //
	//	tgv[ pid_ ]->SetMarkerSize( 0.8 );
	tgv[ pid_ ]->SetMarkerSize( 0.9 );
	tgv[ pid_ ]->SetMarkerStyle( 20 );
	tgv[ pid_ ]->SetMarkerColor( 1 );
	tgv[ pid_ ]->Draw( "Psame" );
	// reference plot (fit poly line)//
	tgv.getsampleTG()[ 0 + 3 * pid_ ]->SetLineStyle( 1 );
	tgv.getsampleTG()[ 0 + 3 * pid_ ]->SetLineColor( 1 );
	tgv.getsampleTG()[ 0 + 3 * pid_ ]->SetLineWidth( 2 );
	tgv.getsampleTG()[ 0 + 3 * pid_ ]->Draw( "Csame" );
	// reference plot (68% CL line)//
	tgv.getsampleTG()[ 1 + 3 * pid_ ]->SetLineStyle( 3 );
	tgv.getsampleTG()[ 1 + 3 * pid_ ]->SetLineColor( 4 );
	tgv.getsampleTG()[ 1 + 3 * pid_ ]->SetLineWidth( 2 );
	if( args.hasOpt( "with68" ) )
	  tgv.getsampleTG()[ 1 + 3 * pid_ ]->Draw( "Csame" );
	// reference plot (90% CL line)//
	tgv.getsampleTG()[ 2 + 3 * pid_ ]->SetLineStyle( 2 );
	tgv.getsampleTG()[ 2 + 3 * pid_ ]->SetLineColor( 2 );
	tgv.getsampleTG()[ 2 + 3 * pid_ ]->SetLineWidth( 2 );
	tgv.getsampleTG()[ 2 + 3 * pid_ ]->Draw( "Csame" );
	// reference plot (zero line)//
	int zid_( tgv.getsampleTG().size() - 1 );
	tgv.getsampleTG()[ zid_ ]->SetLineStyle( 1 );
	tgv.getsampleTG()[ zid_ ]->SetLineColor( 1 );
	tgv.getsampleTG()[ zid_ ]->SetLineWidth( 1 );
	tgv.getsampleTG()[ zid_ ]->Draw( "Csame" );

	if( ! j ){
	  // Legend //
	  TLegend* l = new TLegend( 0.20, 0.46, 0.50, 0.80 );
	  l->SetFillColor( 0 );
	  l->SetTextSize( 0.10 );
	  //	l->AddEntry( tgv[ j + i * nplot_ ], "#Delta #chi2", "p" );
	  // 	  l->AddEntry( tgv[ pid_ ], "  Lag. Multi.", "p" );
	  l->AddEntry( tgv.getsampleTG()[ 0 + 3 * pid_ ], "  Poly. Fit", "l" );
	  if( args.hasOpt( "with68" ) )
	    l->AddEntry( tgv.getsampleTG()[ 1 + 3 * pid_ ], "  68% CL", "l" );
	  l->AddEntry( tgv.getsampleTG()[ 2 + 3 * pid_ ], "  90% CL", "l" );
	  l->Draw();
	  lv_.push_back( l );
	}

	// Text
	TLatex* la = new TLatex( xlim_, 1.15 * ymax_, 
			       tgv.getvinfo()[ pid_ ].c_str() );
	la->SetTextSize( 0.10 );
	la->SetTextAlign( 31 );
	la->Draw();
	/*
	TLatex* la2 = new TLatex( - 0.5 * xlim_, 0.6 * ymax_, 
				tgv.getvinfo2()[ pid_ ].c_str() );
	la2->SetTextSize( 0.10 );
	la2->Draw();
	*/

	tv_.push_back( la );
	//	tv_.push_back( la2 );

      }
      // end plot loop

      // draw process //
      if( ! &c ) return false;
      
      // Write Title //
      c.cd( 0 );
      /*
      TLatex tit_ = TLatex( 0.25, 0.97, tgv.getbaseinfo().c_str() );
      tit_.SetTextSize( 0.045 );
      tit_.Draw();
      */ // wo text

      //      if( ! Arguments::ref().hasOpt( "nodisplay" ) ) c.Draw(); 
      ostringstream ostf;
      ostf << odirname_ << "/DChi2_plot_EVID_" << evid
	   << "_" << i + 1 << ".";
      string png_( ostf.str() + Arguments::ref().get( "format", "png" ) );
      c.Print( png_.c_str() );
      string pdf_( ostf.str() + "pdf" );
      c.Print( pdf_.c_str() );
      //	c.Clear();
      
      for( int k = 0; k < lv_.size(); k++ )
	delete lv_[ k ];
      for( int k = 0; k < tv_.size(); k++ )
	delete tv_[ k ];

    }
    // end sheet loop 

  }
  
  catch( int err ){
    cout << "fail to create sumplots at the stage " 
	 << err << endl;
    return false;
  }

  return true;
}

vector< double > setCLvals( const double& perc, const vector< int >& nfreev, 
			    const vector< double >& chi2_0v ){
  vector< double > tmpv_( 0 );
  for( int i = 0; i < nfreev.size(); i++ ){
    double c2perc = ROOT::Math::chisquared_quantile( perc, nfreev[ i ] );
    tmpv_.push_back( chi2_0v[ i ] * ( c2perc / nfreev[ i ] - 1 ) );
  } // most probable (peak) ~ number of freedom (nfreev)

  return tmpv_;
}

vector< vector < double > > 
getTolerance( const TGraVmk& tgv, const double& cl68, const double& cl90 ){

  double xlim_ = 1.5 * tgv.getxlimit(); // > 0
  double ymin_ = tgv.getvminmax()[ tgv.size() - 1 ][ 2 ]; // ymin of last TG

  int ndisc_( 1000 );

  double cl68xmin_( - xlim_ ), cl68xmax_( xlim_ );
  double cl90xmin_( - xlim_ ), cl90xmax_( xlim_ );

  // for finding clxmin_ (closest point to 0)
  // approach from left to 0
  for( int i = 1; i < ndisc_; i++ ){
    double x_ = - xlim_ + i * 2.0 * xlim_ / ndisc_;

    if( x_ < 0 ){ // find clxmin_
      if( cl68 < tgv( x_ ) ) cl68xmin_ = x_;  
      if( cl90 < tgv( x_ ) ) cl90xmin_ = x_;
    } else { 
      break;
    }
  }

  // for finding clxmax_ (closest point to 0)
  // approach from right to 0
  for( int i = 1; i < ndisc_; i++ ){
    double x_ = + xlim_ - i * 2.0 * xlim_ / ndisc_;

    if( x_ < 0 ){ 
      break;
    } else { // find clxmax_
      if( cl68 < tgv( x_ ) ) cl68xmax_ = x_;
      if( cl90 < tgv( x_ ) ) cl90xmax_ = x_;
    }
  }

  // find xmin_
  TGraph* tmptg = tgv[ tgv.size() - 1 ];
  int j( 0 );
  while( tmptg->GetY()[ j ] != ymin_ ) j++;
  double xmin_ = tmptg->GetX()[ j ];

  // ymin xval //
  vector< double > xval_( 1, xmin_ );
  // cl68 xval //
  vector< double > cl68xval_( 0 );
  cl68xval_.push_back( cl68xmin_ );
  cl68xval_.push_back( cl68xmax_ );
  // cl90 xval //
  vector< double > cl90xval_( 0 );
  cl90xval_.push_back( cl90xmin_ );
  cl90xval_.push_back( cl90xmax_ );

  vector< vector< double > > tmpvv_( 0 );
  tmpvv_.push_back( xval_ );
  tmpvv_.push_back( cl68xval_ );
  tmpvv_.push_back( cl90xval_ );

  return tmpvv_;
}

bool Echi2plots( const string& dn, const vector< string >& dlistv ){

  // data reading process //
  ostringstream ost;
  ost << dn << "/chi2_error.dat";

  ifstream ifs( ost.str().c_str() );

  if( ! ifs ){
    cout << "fail to open file: " << ost.str() << endl;
    return false;
  }
    
  cout << "opening file: " << ost.str() << endl;
    
  string strline;
  vector< string > vedname_( 0 ); 
  vector< double > vchi2_( 0 ), vchi2err_( 0 ); // reduced chi2s
  
  while( getline( ifs, strline ) ){
      
    int tfree_( 0 ), idisp_( 0 );
    string tname_( "" ), sdisp_( "" );
    double tchi2_( 0.0 ), terr_( 0.0 ), ddisp_( 0.0 );

    istringstream iss( strline );
    if( iss >> idisp_ >> tname_  >> tfree_ >> sdisp_ >> idisp_ >> ddisp_ >>
	tchi2_ >> sdisp_ >> terr_ >> sdisp_ )
      if( tname_ != "Reduced_Chi2" && tfree_ != 0 ){
	vedname_.push_back( tname_ );
	vchi2_.push_back( tchi2_ / tfree_ );
	vchi2err_.push_back( terr_ / tfree_ );
      }
  }

  // data plot process //
  if( vedname_.size() != dlistv.size() + 1 ){
    return Esumplot( vedname_, vchi2_, vchi2err_ );
  } else {
    vector< string > vedname_tmp_( 0 );
    vector< double > vchi2_tmp_( 0 ), vchi2err_tmp_( 0 );
    string total_( "" ); double total_chi2_( 0.0 ), total_chi2_err_( 0.0 );

    for( int i = 0; i < dlistv.size(); i++ )
      for( int j = 0; j < vedname_.size(); j++ ){
	if( dlistv[ i ] == vedname_[ j ] ){
	  vedname_tmp_.push_back( vedname_[ j ] );
	  vchi2_tmp_.push_back( vchi2_[ j ] );
	  vchi2err_tmp_.push_back( vchi2err_[ j ] );
	} else if ( vedname_[ j ] == "Total_Chi2" ){
	  //	  total_ = vedname_[ j ];
	  total_ = "TOTAL";
	  total_chi2_ = vchi2_[ j ];
	  total_chi2_err_ = vchi2err_[ j ];
	}
      }
    vedname_tmp_.push_back( total_ );
    vchi2_tmp_.push_back( total_chi2_ );
    vchi2err_tmp_.push_back( total_chi2_err_ );

    return Esumplot2( vedname_tmp_, vchi2_tmp_, vchi2err_tmp_ );
  }
 
}

bool Esumplot2( const vector< string >& dname_, const vector< double >& chi2_, 
		const vector< double >& chi2err_ ){

  Arguments& args = Arguments::ref();

  TCanvas c( "Reduced Chi2 vs DataSet" );
  c.SetBottomMargin( 0.30 );

  double xmarg_( 1.0 );
  double xmin_( 0.0 ), xmax_( 2.0 * xmarg_ + dname_.size() - 1.0 );
  double yscale_( 1.1 );
  //  double yabsmax_( 2.0 );
  double yabsmax_( 3.0 );
  double ymin_( - yscale_ * yabsmax_ );
  double ymax_( + yscale_ * yabsmax_ );

  TH1F* frame = c.DrawFrame( xmin_, ymin_, xmax_, ymax_ );

  // Titles //
  TAxis* ta = frame->GetXaxis();
  ta->SetLabelColor( 0 );
  ta->SetTickLength( 0.0 );
  ta = frame->GetYaxis();
  ta->SetLabelSize( 0.038 );
  ta->SetLabelOffset( 0.01 );
  ta->SetTitle( "( #tilde{#chi^{2}} - 1.0 ) / #sigma_{#tilde{#chi^{2}}}" );
  ta->SetTitleSize( 0.043 );
  ta->SetTitleOffset( 0.8 );

  // Lines & Markers //
  TLine zerol_( xmin_, 0.0, xmax_, 0.0 );
  zerol_.SetLineStyle( 1 );
  zerol_.SetLineWidth( 2 );
  zerol_.Draw();
  TLine onepl_( xmin_, +1.0, xmax_, +1.0 );
  TLine oneml_( xmin_, -1.0, xmax_, -1.0 );
  onepl_.SetLineStyle( 2 );  oneml_.SetLineStyle( 2 );
  onepl_.SetLineWidth( 1 );  oneml_.SetLineWidth( 1 );
  //  onepl_.SetLineColor( 4 );  oneml_.SetLineColor( 4 );
  onepl_.Draw();  oneml_.Draw();
  TLine twopl_( xmin_, +2.0, xmax_, +2.0 );
  TLine twoml_( xmin_, -2.0, xmax_, -2.0 );
  twopl_.SetLineStyle( 3 );  twoml_.SetLineStyle( 3 );
  twopl_.SetLineWidth( 1 );  twoml_.SetLineWidth( 1 );
  //  twopl_.SetLineColor( 2 );  twoml_.SetLineColor( 2 );
  twopl_.Draw();  twoml_.Draw();

  vector< TLatex* > dsname_( 0 );
  vector< TMarker* > tmk_( 0 );
  vector< TLine* > vbl_( 0 );

  vector< int > vborder_( 0 );
  vborder_.push_back( 9 );
  vborder_.push_back( 15 );
  vborder_.push_back( 19 );

  for( int i = 0; i < dname_.size(); i++ ){
    double xpos_ = xmin_ + xmarg_ + i;
    // dset name
    double ttymin_ = 1.05 * ymin_;
    dsname_.push_back( new TLatex( xpos_, ttymin_, dname_[ i ].c_str() ) );
    // reduced chi2
    tmk_.push_back( new TMarker( xpos_, ( chi2_[ i ] - 1.0 ) / chi2err_[ i ], 8 ) );
    for( int j = 0; j < vborder_.size(); j++ ){
      if( i + 1 == vborder_[ j ] ){
	vbl_.push_back( new TLine( xpos_ + 0.5, -3.0, xpos_ + 0.5, +3.0 ) );
      }
    }
  }

  // Draw Process //  
  for( int i = 0; i < dsname_.size(); i++ ){ 
    dsname_[ i ]->SetTextSize( 0.028 );
    dsname_[ i ]->SetTextAngle( 90 );
    dsname_[ i ]->SetTextAlign( 32 );
    dsname_[ i ]->Draw();
  }
  for( int i = 0; i < tmk_.size(); i++ ){ 
    tmk_[ i ]->SetMarkerSize( 0.8 );
    tmk_[ i ]->Draw(); 
  }
  for( int i = 0; i < vbl_.size(); i++ ){ 
    vbl_[ i ]->SetLineStyle( 1 );
    vbl_[ i ]->SetLineWidth( 1 );
    vbl_[ i ]->Draw(); 
  }

  if( ! &c ) return false;

  //  if( ! Arguments::ref().hasOpt( "nodisplay" ) ) c.Draw();  
  ostringstream ostf;
  ostf << odirname_ << "/Red_Chi2_NORM_plot" << ".";
  string png_( ostf.str() + Arguments::ref().get( "format", "png" ) );
  c.Print( png_.c_str() );
  string pdf_( ostf.str() + "pdf" );
  c.Print( pdf_.c_str() );
  //  c.Clear();

  for( int i = 0; i < dsname_.size(); i++ ) delete dsname_[ i ];
  for( int i = 0; i < tmk_.size(); i++ ) delete tmk_[ i ];
  for( int i = 0; i < vbl_.size(); i++ ) delete vbl_[ i ];

  return true;
}

bool Esumplot( const vector< string >& dname_, const vector< double >& chi2_, 
	       const vector< double >& chi2err_ ){

  Arguments& args = Arguments::ref();

  TCanvas c( "Reduced Chi2 vs DataSet" );
  ostringstream oo; oo << "Reduced Chi2 vs DataSet";
  c.SetTitle( oo.str().c_str() );
  c.SetTopMargin( 0.13 );
  c.SetBottomMargin( 0.4 );

  // find minimam & max //
  double tymin_( 1.0 ), tymax_( 1.0 );
  for( int i = 0; i < dname_.size(); i++ ){
    if( chi2_[ i ] + 2.0 * chi2err_[ i ] > tymax_ )
      tymax_ = chi2_[ i ] + 2.0 * chi2err_[ i ];
    if( chi2_[ i ] - 2.0 * chi2err_[ i ] < tymin_ )
      tymin_ = chi2_[ i ] - 2.0 * chi2err_[ i ];
  }
  
  // Frame //
  double xmarg_( 1.0 );
  double xmin_( 0.0 ), xmax_( 2.0 * xmarg_ + dname_.size() - 1.0 );   
  double yscale_( 1.2 );
  double ymin_( yscale_ * ( tymin_ - 1.0 ) + 1.0 );
  double ymax_( yscale_ * ( tymax_ - 1.0 ) + 1.0 );
    
  TH1F* frame = c.DrawFrame( xmin_, ymin_, xmax_, ymax_ );

  // Titles //
  TAxis* ta = frame->GetXaxis();
  ta->SetLabelColor( 0 );
  ta->SetTickLength( 0.0 );
  ta = frame->GetYaxis();
  ta->SetLabelSize( 0.035 );
  ta->SetTitle( "Reduced Chi2" );
  ta->SetTitleSize( 0.035 );
  ta->SetTitleOffset( 0.8 );

  // Lines & Markers //
  TLine zerol_( xmin_, 1.0, xmax_, 1.0 );
  zerol_.SetLineStyle( 1 );
  zerol_.SetLineWidth( 2 );
  zerol_.Draw();

  vector< TLatex* > dsname_( 0 );
  vector< TMarker* > tmk_( 0 );
  vector< TLine* > tlv1_( 0 );
  vector< TLine* > tlv2_( 0 );

  vector< TMarker* > mks68_( 0 );
  vector< TMarker* > mks90_( 0 );

  for( int i = 0; i < dname_.size(); i++ ){

    double xpos_ = xmin_ + xmarg_ + i;
    // dset name
    double ttymin_ = 1.05 * ( ymin_ - 1.0 ) + 1.0;
    dsname_.push_back( new TLatex( xpos_, ttymin_, dname_[ i ].c_str() ) );
    // reduced chi2
    tmk_.push_back( new TMarker( xpos_, chi2_[ i ], 8 ) );

    // 1-sigma CL line
    double ymin1_( chi2_[ i ] - 1.0 * chi2err_[ i ] );
    double ymax1_( chi2_[ i ] + 1.0 * chi2err_[ i ] );
    tlv1_.push_back( new TLine( xpos_, ymin1_, xpos_, ymax1_ ) );

    mks68_.push_back( new TMarker( xpos_, ymin1_, 23 ) );
    mks68_.push_back( new TMarker( xpos_, ymax1_, 22 ) );

    // 2-sigma CL line
    double ymin2_( chi2_[ i ] - 2.0 * chi2err_[ i ] );
    double ymax2_( chi2_[ i ] + 2.0 * chi2err_[ i ] );
    tlv2_.push_back( new TLine( xpos_, ymin2_, xpos_, ymax2_ ) );

    mks90_.push_back( new TMarker( xpos_, ymin2_, 23 ) );
    mks90_.push_back( new TMarker( xpos_, ymax2_, 22 ) );

  }

  // Draw Process //  
  for( int i = 0; i < dsname_.size(); i++ ){ 
    dsname_[ i ]->SetTextSize( 0.028 );
    dsname_[ i ]->SetTextAngle( 90 );
    dsname_[ i ]->SetTextAlign( 32 );
    dsname_[ i ]->Draw();
  }
  for( int i = 0; i < tmk_.size(); i++ ){ 
    tmk_[ i ]->SetMarkerSize( 0.8 );
    tmk_[ i ]->Draw(); 
  }
  for( int i = 0; i < tlv1_.size(); i++ ){ 
    tlv1_[ i ]->SetLineWidth( 1 );
    tlv1_[ i ]->Draw();
  }
  for( int i = 0; i < tlv2_.size(); i++ ){
    tlv2_[ i ]->SetLineWidth( 1 );
    tlv2_[ i ]->Draw();
  }
  for( int i = 0; i < mks68_.size(); i++ ){
    mks68_[ i ]->SetMarkerSize( 1.0 );
    mks68_[ i ]->SetMarkerColor( 4 ); 
    if( args.hasOpt( "with68" ) )
      mks68_[ i ]->Draw();
  }
  for( int i = 0; i < mks90_.size(); i++ ){ 
    mks90_[ i ]->SetMarkerSize( 1.0 ); 
    mks90_[ i ]->SetMarkerColor( 2 ); 
    mks90_[ i ]->Draw(); 
  }

  if( ! &c ) return false;

  // Write Title //
  TLatex tit_ = TLatex( 0.0 , 1.2 * ymax_, oo.str().c_str() );
  tit_.SetTextSize( 0.045 );
  tit_.Draw();

  //  if( ! Arguments::ref().hasOpt( "nodisplay" ) ) c.Draw();  
  ostringstream ostf;
  ostf << odirname_ << "/Red_Chi2_plot" << ".";
  string png_( ostf.str() + Arguments::ref().get( "format", "png" ) );
  c.Print( png_.c_str() );
  string pdf_( ostf.str() + "pdf" );
  c.Print( pdf_.c_str() );
  //  c.Clear();

  for( int i = 0; i < dsname_.size(); i++ ) delete dsname_[ i ];
  for( int i = 0; i < tmk_.size(); i++ ) delete tmk_[ i ];
  for( int i = 0; i < tlv1_.size(); i++ ) delete tlv1_[ i ];
  for( int i = 0; i < tlv2_.size(); i++ ) delete tlv2_[ i ];
  for( int i = 0; i < mks68_.size(); i++ ) delete mks68_[ i ];
  for( int i = 0; i < mks90_.size(); i++ ) delete mks90_[ i ];

  return true;
}

bool Toleplot( map< string, vector< vector< double > > > *tv, 
	       const int& evid, vector< double >& clv, 
	       vector< string >& clsv,
	       const vector< string >& dlistv ){
	       //	       = vector< string >( 1, "" ) ){

  Arguments& args = Arguments::ref();
  
  TCanvas c( "Tolerance vs DataSet" );
  ostringstream oo; oo << "Tolerance (E.V. No." << evid << ") vs DataSet";
  c.SetTitle( oo.str().c_str() );
  c.SetBottomMargin( 0.30 );

  // Frame //
  double xmarg_( 1.0 );
  double xmin_( 0.0 ), xmax_( 2.0 * xmarg_ + tv->size() - 1.0 );   
  double xlim = Arguments::ref().get( "TGxlim", 20.0 );
  double yscale_( 1.2 );
  double ymin_( - yscale_ * xlim ), ymax_( yscale_ * xlim );

  TH1F* frame = c.DrawFrame( xmin_, ymin_, xmax_, ymax_ );

  // Titles //
  TAxis* ta = frame->GetXaxis();
  ta->SetLabelColor( 0 );
  ta->SetTickLength( 0.0 );
  ta = frame->GetYaxis();
  ta->SetLabelSize( 0.035 );
  ta->SetLabelOffset( 0.01 );
  //  ta->SetTitle( "T" );
  ostringstream ost; ost << "T_{" << evid << "}"; 
  ta->SetTitle( ost.str().c_str() );
  ost.str( "" ); ost.clear();
  ta->SetTitleSize( 0.04 );
  ta->SetTitleOffset( 0.7 );

  // Lines & Markers //
  TLine zerol_( xmin_, 0.0, xmax_, 0.0 );
  zerol_.SetLineStyle( 1 );
  zerol_.SetLineWidth( 1 );
  zerol_.Draw();

  vector< TLatex* > dsname_( 0 );
  vector< TMarker* > tmk_( 0 );
  vector< TLine* > tlv1_( 0 );
  vector< TLine* > tlv2_( 0 );

  vector< TMarker* > mks68_( 0 );
  vector< TMarker* > mks90_( 0 );

  double cl68L_( ymax_ ), cl90L_( ymax_ );
  string cl68S_( "" ), cl90S_( "" );

  int n( 0 );

  for( int i = 0; i < dlistv.size(); i++ )
    for( map< string, vector< vector< double > > >::iterator itr = tv->begin();
	 itr != tv->end(); itr++ )
      if( ! dlistv[ i ].size() || dlistv[ i ] == itr->first ){
	double xpos_ = xmin_ + xmarg_ + n;
	// dset name
	dsname_.push_back( new TLatex( xpos_, ymin_ * 1.1, 
				       itr->first.c_str() ) );
	// ymin xval
	tmk_.push_back( new TMarker( xpos_, itr->second[ 0 ][ 0 ], 8 ) );
	
	double yminlim_( 0.0 ), ymaxlim_( 0.0 );
	bool minjd_( false ), maxjd_( false );
	// 68% CL xvals
	minjd_ = ( itr->second[ 1 ][ 0 ] <= ymin_ );
	maxjd_ = ( itr->second[ 1 ][ 1 ] >= ymax_ );
	yminlim_ = ( minjd_ ? ymin_ : itr->second[ 1 ][ 0 ] );
	ymaxlim_ = ( maxjd_ ? ymax_ : itr->second[ 1 ][ 1 ] );
	tlv1_.push_back( new TLine( xpos_, yminlim_, xpos_, ymaxlim_ ) );
	
	if( ! minjd_ )
	  //	  mks68_.push_back( new TMarker( xpos_, yminlim_, 23 ) );
	  mks68_.push_back( new TMarker( xpos_, yminlim_, 27 ) );
	if( ! maxjd_ )
	  //	  mks68_.push_back( new TMarker( xpos_, ymaxlim_, 22 ) );
	  mks68_.push_back( new TMarker( xpos_, ymaxlim_, 27 ) );
	
	if( fabs( yminlim_ ) < cl68L_ ){ 
	  cl68L_ = fabs( yminlim_ );
	  cl68S_ = itr->first;
	}
	if( fabs( ymaxlim_ ) < cl68L_ ){
	  cl68L_ = fabs( ymaxlim_ );
	  cl68S_ = itr->first;
	}
	// 90% CL xvals
	minjd_ = ( itr->second[ 2 ][ 0 ] <= ymin_ );
	maxjd_ = ( itr->second[ 2 ][ 1 ] >= ymax_ );
	yminlim_ = ( minjd_ ? ymin_ : itr->second[ 2 ][ 0 ] );
	ymaxlim_ = ( maxjd_ ? ymax_ : itr->second[ 2 ][ 1 ] );
	tlv2_.push_back( new TLine( xpos_, yminlim_, xpos_, ymaxlim_ ) );
	
	if( ! minjd_ )
	  //	  mks90_.push_back( new TMarker( xpos_, yminlim_, 23 ) );
	  mks90_.push_back( new TMarker( xpos_, yminlim_, 21 ) );
	if( ! maxjd_ )
	  //	  mks90_.push_back( new TMarker( xpos_, ymaxlim_, 22 ) );
	  mks90_.push_back( new TMarker( xpos_, ymaxlim_, 21 ) );
	
	if( fabs( yminlim_ ) < cl90L_ ){
	  cl90L_ = fabs( yminlim_ );
	  cl90S_ = itr->first;
	}
	if( fabs( ymaxlim_ ) < cl90L_ ){
	  cl90L_ = fabs( ymaxlim_ );
	  cl90S_ = itr->first;
	}
	
	n++;
      }
  
  clv[ 0 ] = cl68L_; clv[ 1 ] = cl90L_;  
  clsv[ 0 ] = cl68S_; clsv[ 1 ] = cl90S_;  
  
  // Draw Process //  
  TLine cl68lp_( xmin_, +cl68L_, xmax_, +cl68L_ );
  cl68lp_.SetLineStyle( 3 );
  cl68lp_.SetLineColor( 4 );
  cl68lp_.SetLineWidth( 3 );
  TLine cl68lm_( xmin_, -cl68L_, xmax_, -cl68L_ );
  cl68lm_.SetLineStyle( 3 );
  cl68lm_.SetLineColor( 4 );
  cl68lm_.SetLineWidth( 3 );
  ostringstream ost68; ost68 << "(" << clsv[ 0 ] << ")";
  //  TLatex tx68cl_ = TLatex( xmax_ + 0.5 * xmarg_, -cl68L_, ost68.str().c_str() );
  TLatex tx68cl_ = TLatex( xmax_ + 1.1 * xmarg_, - ymax_, ost68.str().c_str() );
  tx68cl_.SetTextSize( 0.035 );
  tx68cl_.SetTextColor( 4 );
  tx68cl_.SetTextAngle( 90 );
  //  tx68cl_.SetTextAlign( 32 );
  tx68cl_.SetTextAlign( 12 );
  TLatex tx68cl2_ = TLatex( xmax_ + 0.5 * xmarg_, - ymax_, "(68% CL)" );
  tx68cl2_.SetTextSize( 0.035 );
  tx68cl2_.SetTextColor( 4 );
  tx68cl2_.SetTextAngle( 90 );
  tx68cl2_.SetTextAlign( 12 );

  if( args.hasOpt( "with68" ) )
    if( cl68L_ < ymax_ ){ cl68lp_.Draw(); cl68lm_.Draw(); tx68cl_.Draw(); tx68cl2_.Draw(); }

  TLine cl90lp_( xmin_, +cl90L_, xmax_, +cl90L_ );
  cl90lp_.SetLineStyle( 2 );
  cl90lp_.SetLineColor( 2 );
  cl90lp_.SetLineWidth( 3 );
  TLine cl90lm_( xmin_, -cl90L_, xmax_, -cl90L_ );
  cl90lm_.SetLineStyle( 2 );
  cl90lm_.SetLineColor( 2 );
  cl90lm_.SetLineWidth( 3 );
  ostringstream ost90; ost90 << clsv[ 1 ];
  //  TLatex tx90cl_ = TLatex( xmax_ + 0.5 * xmarg_, +cl90L_, ost90.str().c_str() );
  TLatex tx90cl_ = TLatex( xmax_ + 1.1 * xmarg_, + ymax_, ost90.str().c_str() );
  tx90cl_.SetTextSize( 0.035 );
  tx90cl_.SetTextColor( 2 );
  tx90cl_.SetTextAngle( 90 );
  //  tx90cl_.SetTextAlign( 12 );
  //  tx90cl_.SetTextAlign( 22 );
  tx90cl_.SetTextAlign( 32 );
  TLatex tx90cl2_ = TLatex( xmax_ + 0.5 * xmarg_, + ymax_, "90% CL" );
  tx90cl2_.SetTextSize( 0.035 );
  tx90cl2_.SetTextColor( 2 );
  tx90cl2_.SetTextAngle( 90 );
  tx90cl2_.SetTextAlign( 32 );

  if( cl90L_ < ymax_ ){ cl90lp_.Draw(); cl90lm_.Draw(); tx90cl_.Draw(); tx90cl2_.Draw(); }

  for( int i = 0; i < dsname_.size(); i++ ){ 
    dsname_[ i ]->SetTextSize( 0.032 );
    dsname_[ i ]->SetTextAngle( 90 );
    dsname_[ i ]->SetTextAlign( 32 );
    dsname_[ i ]->Draw();
  }
  for( int i = 0; i < tmk_.size(); i++ ){ 
    tmk_[ i ]->SetMarkerSize( 1.0 );
    tmk_[ i ]->Draw(); 
  }
  /*
  for( int i = 0; i < tlv1_.size(); i++ ){ 
    tlv1_[ i ]->SetLineWidth( 1.5 );
    tlv1_[ i ]->Draw();
  }
  */
  for( int i = 0; i < tlv2_.size(); i++ ){
    tlv2_[ i ]->SetLineWidth( 2 );
    tlv2_[ i ]->Draw();
  }
  for( int i = 0; i < mks68_.size(); i++ ){
    mks68_[ i ]->SetMarkerSize( 2.0 );
    mks68_[ i ]->SetMarkerColor( 4 ); 
    if( args.hasOpt( "with68" ) )
      mks68_[ i ]->Draw();
  }
  for( int i = 0; i < mks90_.size(); i++ ){ 
    mks90_[ i ]->SetMarkerSize( 1.5 ); 
    mks90_[ i ]->SetMarkerColor( 2 ); 
    mks90_[ i ]->Draw(); 
  }

  //  TLegend l_( 0.40, 0.77, 0.60, 0.87 );
  TLegend l_( 0.10, 0.91, 0.27, 0.98 );
  l_.SetFillColor( 0 );
  l_.SetTextSize( 0.032 );
  l_.AddEntry( mks90_[ 0 ], "90% CL Lim.", "p" );
  l_.AddEntry( mks68_[ 0 ], "68% CL Lim.", "p" );
  l_.Draw();

  // OutPut Process //
  if( ! &c ) return false;

  // Write Title //
  /*
  TLatex tit_ = TLatex( 0.0 , 1.2 * ymax_, oo.str().c_str() );
  tit_.SetTextSize( 0.045 );
  tit_.Draw();
  */

  //  if( ! Arguments::ref().hasOpt( "nodisplay" ) ) c.Draw();  
  ostringstream ostf;
  ostf << odirname_ << "/Tol_plot_EVID_" << evid << ".";
  string png_( ostf.str() + Arguments::ref().get( "format", "png" ) );
  c.Print( png_.c_str() );
  string pdf_( ostf.str() + "pdf" );
  c.Print( pdf_.c_str() );
  //  c.Clear();

  for( int i = 0; i < dsname_.size(); i++ ) delete dsname_[ i ];
  for( int i = 0; i < tmk_.size(); i++ ) delete tmk_[ i ];
  for( int i = 0; i < tlv1_.size(); i++ ) delete tlv1_[ i ];
  for( int i = 0; i < tlv2_.size(); i++ ) delete tlv2_[ i ];
  for( int i = 0; i < mks68_.size(); i++ ) delete mks68_[ i ];
  for( int i = 0; i < mks90_.size(); i++ ) delete mks90_[ i ];

  return true;
}

bool ALLToleplot( const vector< vector< double > >& clvv, 
		  const vector< vector< string > >& clsvv ){

  Arguments& args = Arguments::ref();

  bool c2av_( ! args.hasOpt( "chi2nonave" ) );
  
  TCanvas c( "Tolerance vs Eigen Vectors" );
  ostringstream oo; oo << "Tolerance vs Eigen Vectors";
  c.SetTitle( oo.str().c_str() );
  c.SetBottomMargin( 0.2 );

  // Frame //
  double xmarg_( 1.0 );
  double xmin_( 0.0 ), xmax_( 2.0 * xmarg_ + clvv.size() - 1.0 );   
  //  double xlim = args.get( "TGxlim", 20.0 );
  double xlim = 10.0;
  double txtposi = 1.05 * xlim;
  double yscale_( 1.2 );
  double ymin_( - yscale_ * xlim ), ymax_( yscale_ * xlim );

  TH1F* frame = c.DrawFrame( xmin_, ymin_, xmax_, ymax_ );

  // Titles //
  TAxis* ta = frame->GetXaxis();
  ta->SetLabelColor( 0 );
  ta->SetTickLength( 0.0 );
  ta = frame->GetYaxis();
  ta->SetLabelSize( 0.035 );
  ta->SetLabelOffset( 0.01 );
  //  ta->SetTitle( "Tolerance: T" );
  ta->SetTitle( "T_{i}" );
  ta->SetTitleSize( 0.040 );
  ta->SetTitleOffset( 0.8 );

  // Lines & Markers //
  TLine zerol_( xmin_, 0.0, xmax_, 0.0 );
  zerol_.SetLineStyle( 1 );
  zerol_.SetLineWidth( 1 );
  zerol_.Draw();

  vector< TLatex* > evname_( 0 );
  vector< TMarker* > tmk_( 0 );
  vector< TLine* > tlv68_( 0 );
  vector< TLine* > tlv90_( 0 );

  vector< TMarker* > mks68_( 0 );
  vector< TMarker* > mks90_( 0 );
  vector< TLatex* > txt68_( 0 );
  vector< TLatex* > txt90_( 0 );

  double cl68L_( ymax_ ), cl90L_( ymax_ );
  string cl68S_( "" ), cl90S_( "" );

  double cl68Lave_( 0.0 ), cl90Lave_( 0.0 );

  for( int i = 0; i < clvv.size(); i++ ){

    double xpos_ = xmin_ + xmarg_ + i;
    // eig vec name
    ostringstream ostev; ostev << "E.V. No." << i + 1;
    evname_.push_back( new TLatex( xpos_, ymin_ * 1.05, 
				  ostev.str().c_str() ) );
    // center 
    //    tmk_.push_back( new TMarker( xpos_, 0.0, 8 ) );

    double yminlim_( 0.0 ), ymaxlim_( 0.0 );
    double tyminlim_( 0.0 ), tymaxlim_( 0.0 );
    bool minjd_( false ), maxjd_( false );
    // 90% CL xvals
    maxjd_ = ( clvv[ i ][ 1 ] >= ymax_ );
    tymaxlim_ = ymaxlim_ = ( maxjd_ ? ymax_ : clvv[ i ][ 1 ] );
    tyminlim_ = yminlim_ = - ymaxlim_;
    tlv90_.push_back( new TLine( xpos_, yminlim_, xpos_, ymaxlim_ ) );
    cl90Lave_ += ymaxlim_;

    if( ! maxjd_ ){
      //      mks90_.push_back( new TMarker( xpos_, ymaxlim_, 22 ) );
      //      mks90_.push_back( new TMarker( xpos_, yminlim_, 23 ) );
      mks90_.push_back( new TMarker( xpos_, ymaxlim_, 21 ) );
      mks90_.push_back( new TMarker( xpos_, yminlim_, 21 ) );
      //      txt90_.push_back( new TLatex( xpos_, ymaxlim_ + ymax_ / 20, 
      //				   clsvv[ i ][ 1 ].c_str() ) );
      ostringstream ost90; ost90 << "" << clsvv[ i ][ 1 ] << "";
      txt90_.push_back( new TLatex( xpos_, + txtposi,
				    ost90.str().c_str() ) );
    }
    if( fabs( ymaxlim_ ) < cl90L_ ){ 
      cl90L_ = fabs( ymaxlim_ );
      cl90S_ = ostev.str();
    }
    // 68% CL xvals
    maxjd_ = ( clvv[ i ][ 0 ] >= ymax_ );
    ymaxlim_ = ( maxjd_ ? ymax_ : clvv[ i ][ 0 ] );
    yminlim_ = - ymaxlim_;
    tlv68_.push_back( new TLine( xpos_, yminlim_, xpos_, ymaxlim_ ) );
    cl68Lave_ += ymaxlim_;

    if( ! maxjd_ ){
      //      mks68_.push_back( new TMarker( xpos_, ymaxlim_, 22 ) );
      //      mks68_.push_back( new TMarker( xpos_, yminlim_, 23 ) );
      mks68_.push_back( new TMarker( xpos_, ymaxlim_, 27 ) );
      mks68_.push_back( new TMarker( xpos_, yminlim_, 27 ) );
      if( tyminlim_ > ymin_ ){
	//	txt68_.push_back( new TLatex( xpos_, tyminlim_ - ymax_ / 20, 
	//				      clsvv[ i ][ 0 ].c_str() ) );
	ostringstream ost68; ost68 << "(" << clsvv[ i ][ 0 ] << ")";
	txt68_.push_back( new TLatex( xpos_, - txtposi,
				      ost68.str().c_str() ) );
      }
    }
    if( fabs( ymaxlim_ ) < cl68L_ ){
      cl68L_ = fabs( ymaxlim_ );
      cl68S_ = ostev.str();
    }
  }

  cl90Lave_ /= clvv.size();
  cl68Lave_ /= clvv.size();
  cl90Lave_ = rint( cl90Lave_ * pow( 10.0, 2 ) ) / pow( 10.0, 2 );
  cl68Lave_ = rint( cl68Lave_ * pow( 10.0, 2 ) ) / pow( 10.0, 2 );

  /*
  double chi2_68_( pow( cl68L_, 1 ) ), chi2_90_( pow( cl90L_, 1 ) );
  ostringstream ost68; ost68 << "( #sqrt{#Delta#chi^{2}}=" << chi2_68_ << ")";
  ostringstream ost90; ost90 << "( #sqrt{#Delta#chi^{2}}=" << chi2_90_ << ")";
  double chi2_68_( pow( cl68Lave_, 1 ) ), chi2_90_( pow( cl90Lave_, 1 ) );
  ostringstream ost68; ost68 << "( #bar{#sqrt{#Delta#chi^{2}}}=" 
			     << chi2_68_ << ")";
  ostringstream ost90; ost90 << "( #bar{#sqrt{#Delta#chi^{2}}}=" 
			     << chi2_90_ << ")";
  */

  double chi2_68_( ( c2av_ ? cl68Lave_ : cl68L_ ) ); 
  double chi2_90_( ( c2av_ ? cl90Lave_ : cl90L_ ) );
  ostringstream ost68; ostringstream ost90; 
  if( c2av_ ){
    //    ost68 << "( #bar{#sqrt{#Delta#chi^{2}}}=" 
    ost68 << "( #bar{T}=" 
	  << chi2_68_ << ")";
    //    ost90 << " #bar{#sqrt{#Delta#chi^{2}}}=" 
    ost90 << " #bar{T}=" 
	  << chi2_90_ << "";
  } else {
    //    ost68 << "( #sqrt{#Delta#chi^{2}}=" << chi2_68_ << ")";
    ost68 << "( T=" << chi2_68_ << ")";
    //    ost90 << " #sqrt{#Delta#chi^{2}}=" << chi2_90_ << "";
    ost90 << " T=" << chi2_90_ << "";
  }

  // Draw Process //
  // for CL 68%
  TLine cl68lp_( xmin_, ( c2av_ ? +cl68Lave_ : +cl68L_ ), 
		 xmax_, ( c2av_ ? +cl68Lave_ : +cl68L_ ) );
  cl68lp_.SetLineStyle( 3 );
  cl68lp_.SetLineColor( 4 );
  cl68lp_.SetLineWidth( 2 );
  TLine cl68lm_( xmin_, ( c2av_ ? -cl68Lave_ : -cl68L_ ), 
		 xmax_, ( c2av_ ? -cl68Lave_ : -cl68L_ ) );
  cl68lm_.SetLineStyle( 3 );
  cl68lm_.SetLineColor( 4 );
  cl68lm_.SetLineWidth( 2 );

  TLatex c2c68_ = TLatex( xmax_ + 0.9 * xmarg_, -txtposi, 
			  ost68.str().c_str() );
  c2c68_.SetTextSize( 0.033 );
  c2c68_.SetTextColor( 4 );
  c2c68_.SetTextAngle( 90 );
  //  c2c68_.SetTextAlign( ( c2av_ ? 22 : 32 ) );
  //  c2c68_.SetTextAlign( 32 );
  c2c68_.SetTextAlign( 12 );
  TLatex c2c682_ = TLatex( xmax_ + 0.3 * xmarg_, -txtposi, "(68% CL)" );
  c2c682_.SetTextSize( 0.033 );
  c2c682_.SetTextColor( 4 );
  c2c682_.SetTextAngle( 90 );
  c2c682_.SetTextAlign( 12 );

  // text for non-ave
  TLatex tx68cl_ = TLatex( xmax_ - 0.3 * xmarg_, -cl68L_, cl68S_.c_str() );
  //  tx68cl_.SetTextSize( 0.032 );
  tx68cl_.SetTextSize( 0.028 );
  tx68cl_.SetTextColor( 4 );
  tx68cl_.SetTextAngle( 90 );
  tx68cl_.SetTextAlign( 32 );
    
  if( args.hasOpt( "with68" ) )
    if( ( c2av_ ? cl68Lave_ : cl68L_ ) < ymax_ ){ 
      cl68lp_.Draw(); cl68lm_.Draw(); c2c68_.Draw(); c2c682_.Draw();
      if( ! c2av_ ) tx68cl_.Draw();
    }

  // for CL 90%
  TLine cl90lp_( xmin_, ( c2av_ ? +cl90Lave_ : +cl90L_ ), 
		 xmax_, ( c2av_ ? +cl90Lave_ : +cl90L_ ) );
  cl90lp_.SetLineStyle( 2 );
  cl90lp_.SetLineColor( 2 );
  cl90lp_.SetLineWidth( 2 );
  TLine cl90lm_( xmin_, ( c2av_ ? -cl90Lave_ : -cl90L_ ), 
		 xmax_, ( c2av_ ? -cl90Lave_ : -cl90L_ ) );
  cl90lm_.SetLineStyle( 2 );
  cl90lm_.SetLineColor( 2 );
  cl90lm_.SetLineWidth( 2 );

  TLatex c2c90_ = TLatex( xmax_ + 0.9 * xmarg_, +txtposi, ost90.str().c_str() );
  c2c90_.SetTextSize( 0.033 );
  c2c90_.SetTextColor( 2 );
  c2c90_.SetTextAngle( 90 );
  //  c2c90_.SetTextAlign( ( c2av_ ? 22 : 12 ) );
  //  c2c90_.SetTextAlign( 12 );
  c2c90_.SetTextAlign( 32 );
  TLatex c2c902_ = TLatex( xmax_ + 0.3 * xmarg_, +txtposi, "90% CL" );
  c2c902_.SetTextSize( 0.033 );
  c2c902_.SetTextColor( 2 );
  c2c902_.SetTextAngle( 90 );
  c2c902_.SetTextAlign( 32 );

  // text for non-ave
  TLatex tx90cl_ = TLatex( xmax_ - 0.3 * xmarg_, +cl90L_, cl90S_.c_str() );
  //  tx90cl_.SetTextSize( 0.032 );
  tx90cl_.SetTextSize( 0.028 );
  tx90cl_.SetTextColor( 2 );
  tx90cl_.SetTextAngle( 90 );
  tx90cl_.SetTextAlign( 12 );
    
  if( ( c2av_ ? cl90Lave_ : cl90L_ ) < ymax_ ){ 
    cl90lp_.Draw(); cl90lm_.Draw(); c2c90_.Draw(); c2c902_.Draw();
    if( ! c2av_ ) tx90cl_.Draw();
  }
    
  for( int i = 0; i < evname_.size(); i++ ){ 
    evname_[ i ]->SetTextAngle( 90 );
    //    evname_[ i ]->SetTextSize( 0.032 );
    evname_[ i ]->SetTextSize( 0.030 );
    evname_[ i ]->SetTextAlign( 32 );
    evname_[ i ]->Draw();
  }
  for( int i = 0; i < tmk_.size(); i++ ){ 
    //    tmk_[ i ]->SetMarkerSize( 1.2 );
    tmk_[ i ]->SetMarkerSize( 1.0 );
    tmk_[ i ]->Draw(); 
  }
  /*
  for( int i = 0; i < tlv68_.size(); i++ ){ 
    tlv68_[ i ]->SetLineWidth( 1 );
    if( args.hasOpt( "with68" ) )
      tlv68_[ i ]->Draw();
  }
  */
  for( int i = 0; i < tlv90_.size(); i++ ){
    tlv90_[ i ]->SetLineWidth( 2 );
    tlv90_[ i ]->Draw();
  }
  for( int i = 0; i < mks68_.size(); i++ ){
    //    mks68_[ i ]->SetMarkerSize( 1.5 );
    mks68_[ i ]->SetMarkerSize( 1.3 );
    mks68_[ i ]->SetMarkerColor( 4 ); 
    if( args.hasOpt( "with68" ) )
      mks68_[ i ]->Draw();
  }
  for( int i = 0; i < mks90_.size(); i++ ){ 
    //    mks90_[ i ]->SetMarkerSize( 1.5 ); 
    mks90_[ i ]->SetMarkerSize( 1.0 ); 
    mks90_[ i ]->SetMarkerColor( 2 ); 
    mks90_[ i ]->Draw(); 
  }
  for( int i = 0; i < txt68_.size(); i++ ){ 
    txt68_[ i ]->SetTextAngle( 90 );
    txt68_[ i ]->SetTextSize( 0.022 );
    //    txt68_[ i ]->SetTextSize( 0.018 );
    txt68_[ i ]->SetTextColor( 4 );
    //    txt68_[ i ]->SetTextAlign( 32 );
    txt68_[ i ]->SetTextAlign( 12 );
    if( args.hasOpt( "with68" ) )
      txt68_[ i ]->Draw();
  }
  for( int i = 0; i < txt90_.size(); i++ ){ 
    txt90_[ i ]->SetTextAngle( 90 );
    txt90_[ i ]->SetTextSize( 0.022 );
    //    txt90_[ i ]->SetTextSize( 0.018 );
    txt90_[ i ]->SetTextColor( 2 );
    //    txt90_[ i ]->SetTextAlign( 12 );
    txt90_[ i ]->SetTextAlign( 32 );
    txt90_[ i ]->Draw();
  }

  TLegend l_( 0.10, 0.91, 0.27, 0.98 );
  l_.SetFillColor( 0 );
  l_.SetTextSize( 0.030 );
  l_.AddEntry( mks90_[ 0 ], "90% CL Lim.", "p" );
  l_.AddEntry( mks68_[ 0 ], "68% CL Lim.", "p" );
  l_.Draw();

  // OutPut Process //
  if( ! &c ) return false;

  //  if( ! args.hasOpt( "nodisplay" ) ) c.Draw();  
  ostringstream ostf;
  ostf << odirname_ << "/ALL_Tol_plot" << ".";
  string png_( ostf.str() + Arguments::ref().get( "format", "png" ) );
  c.Print( png_.c_str() );
  string pdf_( ostf.str() + "pdf" );
  c.Print( pdf_.c_str() );
  //  c.Clear();

  for( int i = 0; i < evname_.size(); i++ ) delete evname_[ i ];
  for( int i = 0; i < tmk_.size(); i++ ) delete tmk_[ i ];
  for( int i = 0; i < tlv68_.size(); i++ ) delete tlv68_[ i ];
  for( int i = 0; i < tlv90_.size(); i++ ) delete tlv90_[ i ];
  for( int i = 0; i < mks68_.size(); i++ ) delete mks68_[ i ];
  for( int i = 0; i < mks90_.size(); i++ ) delete mks90_[ i ];
  for( int i = 0; i < txt68_.size(); i++ ) delete txt68_[ i ];
  for( int i = 0; i < txt90_.size(); i++ ) delete txt90_[ i ];

  return true;
}

bool Tchi2plots_2( const string& dn ){

  vector< string > HADID_( 0 );
  HADID_.push_back( "PION" );
  HADID_.push_back( "KAON" );
  HADID_.push_back( "PROTON" );
  HADID_.push_back( "REST" );
  HADID_.push_back( "ALL" );

  string hadid_;

  TGraVmk tgv_;
  //  tgv_.setbaseinfo( "Total Chi2(FF) vs Total Chi2(SIDIS)" );
  tgv_.setbaseinfo( "Total Chi2(FF) vs Total Chi2(PHD)" );
  tgv_.setxlimit( Arguments::ref().get( "TGxlim", 20.0 ) );
  if( Arguments::ref().hasOpt( "xupperlim" ) )
    tgv_.setxupperlimit( Arguments::ref().get( "xupperlim", 0.0 ) );
  if( Arguments::ref().hasOpt( "xlowerlim" ) )
    tgv_.setxlowerlimit( Arguments::ref().get( "xlowerlim", 0.0 ) );
  tgv_.setyfrag( true ); // dy > 0 && dy < (dx)^2 
  //  tgv_.setyfrag( false ); // no cond. on y

  for( vector< string >::iterator itr = HADID_.begin(); 
       itr != HADID_.end(); itr++ ){

    ostringstream ost;
    ost << dn << "/TDATA/LM_HADID_" << *itr << ".dat";

    ifstream ifs( ost.str().c_str() );

    if( ! ifs ){
      cout << "fail to open file: " << ost.str() << endl;
      continue;
    }
    
    cout << "opening file: " << ost.str() << endl;
    hadid_ = *itr;

    string strline;
    int n_( 0 );
    vector< double > x_( 0 ), y_( 0 );
    string hadid_; double lamb_;
    
    while( getline( ifs, strline ) ){
      
      istringstream iss( strline );
      x_.push_back( 0.0 ); y_.push_back( 0.0 );
      
      iss >> hadid_ >> lamb_ >> x_[ n_ ] >> y_[ n_ ];
      
      n_++;
    }
    
    if( ! tgv_.setvec( x_, y_ ) ){
      cout << "fail to create TGraph for " 
	   << ost.str() << endl;
      return false;
    }
    
    ostringstream ostt; 
    //    ostt << "#Delta #chi^{2}_{" << *itr << "} vs  #Delta #chi^{2}_{A1H}";
    ostt << "#Delta #chi^{2}_{" << *itr << "} vs  #Delta #chi^{2}_{PHD}";
    //    ostt << "#Delta #chi^{2}_{FF} vs  #Delta #chi^{2}_{PHD}";
    //    ostt << "Chi2_" << *itr << " vs Chi2_A1H";
    tgv_.setinfo( ostt.str() );
  }
  
  if( tgv_.size() == 0 ) return false;

  // set fit polynominal sample 
  tgv_.polyfitToTG( tgv_.size() - 1 );
  //      tolev_[ ostt.str() ];
  tgv_.setsampleTG();

  /*
  // set parabolic sample 
  tgv_.getfparv()[ 3 ] = 1.0;
  tgv_.setsampleTG();
  */
  // set 0 line sample 
  tgv_.setsampleTG();

  if( tgv_.size() != tgv_.getvinfo().size() ){
    cout << "TGraph & info sizes inconsistensy" << endl;
    return false;
  }
  
  return Tsumplot_2( tgv_, hadid_ );
}

bool Tsumplot_2( const TGraVmk& tgv, const string& hadid ){

  try {

    Arguments& args = Arguments::ref();

    //    int nplot_( 6 ); // number of plot in one sheet (int!)
    int nplot_( 1 ); // number of plot in one sheet (int!)

    // start sheet loop 
    for( int i = 0; i < ( tgv.size() - 1 ) / nplot_ + 1; i++ ){

      // Create Canvas & Divide//
      TCanvas c( "Chi2_FF vs Chi2_A1H" );
      c.SetTitle( tgv.getbaseinfo().c_str() );
      //      c.Divide( 2, int( nplot_ / 2.0 + 0.5 ) );
      c.Divide( 1, int( nplot_ / 1.0 + 0.5 ) );

      // Plot each TGraphs //
      vector< TLegend* > lv_( 0 ); // legend should exist til c.Draw()
      vector< TLatex* > tv_( 0 ); //  should exist til c.Draw()
      // start plot loop
      for( int j = 0; j < nplot_; j++ ){

	if( j + i * nplot_ > tgv.size() - 1 ) break;

	int pid_( j + i * nplot_ );

	c.cd( j + 1 );
	gPad->SetTopMargin( 0.1 );
	gPad->SetBottomMargin( 0.15 );

	// Frame //
	double xlim_( tgv.getxlimit() );
	double xmarg_ = args.get( "xmargin", 0.05 );

	double ymin_ = args.get( "ymin", tgv.getvminmax()[ pid_ ][ 2 ] );
	double ymax_ = args.get( "ymax", tgv.getvminmax()[ pid_ ][ 3 ] );
	double ymarg_ = args.get( "ymargin", 0.1 );
	double ydif_ = ymax_ - ymin_;

	TH1F* frame 
	  = gPad->DrawFrame( - ( 1.0 + xmarg_ ) * xlim_, ymin_ - ymarg_ * ydif_, 
			     + ( 1.0 + xmarg_ ) * xlim_, ymax_ + ymarg_ * ydif_ );

	// Titles //
	TAxis* ta = frame->GetXaxis();
	ta->SetLabelSize( 0.05 );
	//	ta->SetTitle( "#Delta #chi^{2}_{A1H}" );
	ta->SetTitle( "#Delta #chi^{2}_{PHD}" );
	ta->SetTitleSize( 0.07 );
	//	ta->SetTitleColor( 2 );
	ta->SetTitleOffset( 0.70 );
	ostringstream ost;
	//	ost << "#Delta #chi^{2}_{FF}";
	ost << "#Delta #chi^{2}_{ FF(" << gethadname( hadid ) << ")}";
	ta = frame->GetYaxis();
	ta->SetLabelSize( 0.05 );
	ta->SetTitle( ost.str().c_str() );
	ta->SetTitleSize( 0.07 );
	//	ta->SetTitleColor( 2 );
	ta->SetTitleOffset( 0.60 );

	// Plots //
	// chi2 plot //
	tgv[ pid_ ]->SetMarkerSize( 1 );
	tgv[ pid_ ]->SetMarkerStyle( 20 );
	tgv[ pid_ ]->SetMarkerColor( 1 );
	tgv[ pid_ ]->Draw( "Psame" );
	// reference plot (para line)//
	tgv.getsampleTG()[ 0 ]->SetLineStyle( 2 );
	tgv.getsampleTG()[ 0 ]->SetLineColor( 1 );
	tgv.getsampleTG()[ 0 ]->Draw( "Csame" );
	// reference plot (zero line)//
	tgv.getsampleTG()[ 1 ]->SetLineStyle( 1 );
	tgv.getsampleTG()[ 1 ]->SetLineColor( 1 );
	tgv.getsampleTG()[ 1 ]->Draw( "Csame" );

	// Legend //
	TLegend* l = new TLegend( 0.14, 0.35, 0.35, 0.43 );
	l->SetFillColor( 0 );
	l->SetTextSize( 0.04 );
	l->AddEntry( tgv[ pid_ ], "  Lag. Multi.", "p" );
	//	l->AddEntry( tgv.getsampleTG()[ 0 ], "  Fit", "l" );
	l->Draw();
	lv_.push_back( l );

	// Text
	TLatex* la = new TLatex( - 0.6 * xlim_, 0.75 * ymax_, 
				 tgv.getvinfo()[ pid_ ].c_str() );
	la->SetTextSize( 0.09 );
	la->SetTextColor( 2 );
	//	la->Draw();
	tv_.push_back( la );

      }
      // end plot loop

      // draw process //
      if( &c ){
	//	if( ! args.hasOpt( "nodisplay" ) ) c.Draw(); 
	ostringstream ostf;
	ostf << odirname_ << "/TChi2_plot_" << i + 1 << ".";
	string png_( ostf.str() + args.get( "format", "png" ) );
	c.Print( png_.c_str() );
	string pdf_( ostf.str() + "pdf" );
	c.Print( pdf_.c_str() );
	//	c.Clear();
      }

      for( int k = 0; k < lv_.size(); k++ )
	delete lv_[ k ];
      for( int k = 0; k < tv_.size(); k++ )
	delete tv_[ k ];

    }
    // end sheet loop 

  }
  
  catch( int err ){
    cout << "fail to create sumplots at the stage " 
	 << err << endl;
    return false;
  }

  return true;
}

string gethadname( const string& hadid ){
  if( hadid == "PION" ) return "#pi+";
  if( hadid == "KAON" ) return "K+";
  if( hadid == "PROTON" ) return "p+";
  if( hadid =="REST" ) return "r+";
  if( hadid == "ALL" ) return "h+";
  return "HAD";
}
