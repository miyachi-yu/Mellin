/*
  view_emc

  
 */

// ----------------------  starndar c++ library  ------------------
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <sstream>
#include <cstdlib>

// ----------------------  ROOT library  --------------------------
#include <TApplication.h>
#include <TCanvas.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TH1.h>
#include <TLatex.h>
#include <TPDF.h>

// ----------------------  Mellin libarary ------------------------
#include <Utility/Arguments.hh>
#include <Fragmentation/Evolution.hh>
#include <unpolPDF/Evolution.hh>
#include <Evolution/KernelSum.hh>
#include <unpolDISXsec/StrFunc.hh>
#include <unpolSIDISXsec/StrFunc.hh>
#include <Xsec/xXsec.hh>
#include <Xsec/xzXsec.hh>
#include <ExpData/ExpData.hh>

using namespace std;

// ----------------------  MyApplication definition ---------------
class MyApplication : public TApplication {
public:
  MyApplication( int& argc, char* argv[] ) throw( int );
  virtual ~MyApplication();
  virtual void Run( Bool_t retrn = kFALSE ) throw( int );
  
  Xsection::Charge::TARGET n_;
  QCD::Flavor::HADRON      h_;
  int ixbin_;
    
private:
  
  TCanvas *main_;
  TH1     *frame_;
  TLatex  *latex_;

  unpolPDF::Evo           *pdf_;     // unpolPDF
  Fragmentation::Evo      *pion_;    // pion fragmentation
  Fragmentation::Evo      *kaon_;    // kaon fragmentation
  Fragmentation::Evo      *proton_;  // proton fragmentation
  Fragmentation::Evo      *rest_;  // proton fragmentation
  Evolution::KernelSum    *hadron_;  // hadron fragmentation
  
  unpolDISXsec::StrFunc   *sigp_;    // sigma_p
  unpolDISXsec::StrFunc   *sigd_;    // sigma_d
  
  unpolSIDISXsec::StrFunc *sigp_hp_; // sigma_p^h+
  unpolSIDISXsec::StrFunc *sigp_hm_; // sigma_p^h-
  unpolSIDISXsec::StrFunc *sigd_hp_; // sigma_d^h+
  unpolSIDISXsec::StrFunc *sigd_hm_; // sigma_d^h-
  
  Xsection::xXsec  *f_p_;
  Xsection::xXsec  *f_d_;
  
  Xsection::xzXsec *f_p_hp_;
  Xsection::xzXsec *f_p_hm_;
  Xsection::xzXsec *f_d_hp_;
  Xsection::xzXsec *f_d_hm_;

  Exp::ExpData     *expdata_;
  
  vector< double > xbin_;
  vector< vector< double > > zbin_;
  
};


int main( int argc, char* argv[] ){
  
  // prepare Arguments global object
  Utility::Arguments& args = Utility::Arguments::ref( argc, argv );
  
  try {
    
    // prepare Application object
    MyApplication app( argc, argv );
    
    // select ixbin to be drawn
    app.ixbin_ = args.get( "ixbin", 1 );
    
    // target selection
    app.n_ = ( args.get( "target", "p" ) == "p" ? 
	       Xsection::Charge::p : Xsection::Charge::d );
    
    // hadron selection
    app.h_ = ( args.get( "hadron", "hp" ) == "hp" ?
	       QCD::Flavor::HAp : QCD::Flavor::HAm );
    
    // start main routine
    app.Run();

  }
  
  catch ( int e ) {
    ARG_ERR << "catch error( " << e << " )" << endl;
  }
  
  return 0;
}

// ----------------------------------------------------------------
//                           Implemnetaiton
// ----------------------------------------------------------------
MyApplication::MyApplication( int& argc, char* argv[] ) throw( int ) :
  TApplication( "MyApplication", &argc, argv ),
  n_( Xsection::Charge::p ), h_( QCD::Flavor::HAp ),
  main_( NULL ), frame_( NULL ), latex_( NULL ),
  pdf_( NULL ), pion_( NULL ), kaon_( NULL ), proton_( NULL ), rest_( NULL ),
  hadron_( NULL ),
  sigp_( NULL ), sigd_( NULL ),
  sigp_hp_( NULL ), sigp_hm_( NULL ), sigd_hp_( NULL ), sigd_hm_( NULL ),
  f_p_( NULL ), f_d_( NULL ),
  f_p_hp_( NULL ), f_p_hm_( NULL ), f_d_hp_( NULL ), f_d_hm_( NULL ),
  expdata_( NULL ),
  xbin_( 0 ), ixbin_( 1 ),
  zbin_( 0 )
{
  
  Utility::Arguments& args = Utility::Arguments::ref();
  
  try {
    

   //mellin parameters

       double UPDFlength = args.get("UPDFlength" ,12.0);
       double UPDFoffset = args.get("UPDFoffset" ,5.0);
       double UPDFangle  = args.get("UPDFangle" ,0.60);

       double FFlength   = args.get("FFlength" ,12.0);
       double FFoffset   = args.get("FFoffset" ,2.5);
       double FFangle    = args.get("FFangle" ,0.55);

    // unpol PDF
    ARG_LOG << "creating unppl PDF" << endl;
    pdf_    = new unpolPDF::Evo( args );
    
    // unpol DIS Xsection
    ARG_LOG << "creating unpol DIS cross sections for p and d targets." << endl;
    sigp_   = new unpolDISXsec::StrFunc( pdf_, Xsection::Charge::p );
    sigd_   = new unpolDISXsec::StrFunc( pdf_, Xsection::Charge::d );
    
    // Mellin Inversion
    ARG_LOG << "prepare Mellin inversion for DIS Xsec" << endl;
    f_p_    = new Xsection::xXsec( sigp_, 6, 8, 5.0E-4, UPDFlength, UPDFoffset, UPDFangle );
        f_d_    = new Xsection::xXsec( sigp_, 6, 8, 5.0E-4, UPDFlength, UPDFoffset, UPDFangle );
    
    // fragmentation functions
    ARG_LOG << "creating pion fragmentation function" << endl;
    pion_   = new Fragmentation::Evo( args, QCD::Flavor::PIp );

    ARG_LOG << "creating kaon fragmentation function" << endl;
    kaon_   = new Fragmentation::Evo( args, QCD::Flavor::KAp );

//not to use proton Fragmentation I coment out line 163 164 174  3Sep09

/*    ARG_LOG << "creating proton fragmentation function" << endl;
      proton_ = new Fragmentation::Evo( args, QCD::Flavor::PRp );*/
    
    ARG_LOG << "creating rest fragmentation function" << endl;
    rest_ = new Fragmentation::Evo( args, QCD::Flavor::REp );
    
    ARG_LOG << "creating hadron fragmentation function" << endl;
    hadron_ = new Evolution::KernelSum();
    
    if( pion_ )   hadron_->add( pion_ );
    if( kaon_ )   hadron_->add( kaon_ );
//    if( proton_ ) hadron_->add( proton_ );
    if( rest_ )   hadron_->add( rest_ );
    
    // unpol SIDIS Xsection
    sigp_hp_ = 
      new unpolSIDISXsec::StrFunc( pdf_, hadron_, 
				   Xsection::Charge::p, QCD::Flavor::HAp ); 
    sigp_hm_ = 
      new unpolSIDISXsec::StrFunc( pdf_, hadron_, 
				   Xsection::Charge::p, QCD::Flavor::HAm ); 
    sigd_hp_ = 
      new unpolSIDISXsec::StrFunc( pdf_, hadron_, 
				   Xsection::Charge::d, QCD::Flavor::HAp ); 
    sigd_hm_ = 
      new unpolSIDISXsec::StrFunc( pdf_, hadron_, 
				   Xsection::Charge::d, QCD::Flavor::HAm ); 

/*
    double kreamz = 0.114;
    sigp_->xsecComp()->coeff()->alpha().atMz( kreamz );
    sigd_->xsecComp()->coeff()->alpha().atMz( kreamz );
    sigp_hp_->xsecComp()->coeff()->alpha().atMz( kreamz );
    sigp_hm_->xsecComp()->coeff()->alpha().atMz( kreamz );
    sigd_hp_->xsecComp()->coeff()->alpha().atMz( kreamz );
    sigd_hm_->xsecComp()->coeff()->alpha().atMz( kreamz );
*/  
  
    // Mellin inversion
    /*
    f_p_hp_ = new Xsection::xzXsec( sigp_hp_, 6, 8, 5.0E-3, 8.0, 4.5, 0.6 );
    f_p_hm_ = new Xsection::xzXsec( sigp_hm_, 6, 8, 5.0E-3, 8.0, 4.5, 0.6 );
    f_d_hp_ = new Xsection::xzXsec( sigd_hp_, 6, 8, 5.0E-3, 8.0, 4.5, 0.6 );
    f_d_hm_ = new Xsection::xzXsec( sigd_hm_, 6, 8, 5.0E-3, 8.0, 4.5, 0.6 );
    */
    f_p_hp_ = new Xsection::xzXsec( sigp_hp_, 6, 8, 5.0E-4);
    f_p_hm_ = new Xsection::xzXsec( sigp_hm_, 6, 8, 5.0E-4);
    f_d_hp_ = new Xsection::xzXsec( sigd_hp_, 6, 8, 5.0E-4);
    f_d_hm_ = new Xsection::xzXsec( sigd_hm_, 6, 8, 5.0E-4);

    vector< double > length2_(2 , UPDFlength);
    vector< double > offset2_(2 , UPDFoffset);
    vector< double > phi2_(2 , UPDFangle);

    offset2_[1] = FFoffset;
    phi2_[1] = FFangle;
    

    for (int i =0;i<2 ;i++){
	 cout<<"#############################################"<<endl
	     << "i :"<<i<<endl
	     <<"length2_ :"<<length2_[i]<<endl
	     <<"offset2_ :"<<offset2_[i]<<endl
	     <<"phi2_ :"<<phi2_[i]<<endl;
    }

    f_p_hp_ -> upper( length2_ );
    f_p_hp_ -> setParameter( offset2_ ,phi2_ );

    f_p_hm_ -> upper( length2_ );
    f_p_hm_ -> setParameter( offset2_ ,phi2_ );

    f_d_hp_ -> upper( length2_ );
    f_d_hp_ -> setParameter( offset2_ ,phi2_ );

    f_d_hm_ -> upper( length2_ );    
    f_d_hm_ -> setParameter( offset2_ ,phi2_ );

    // ExpData
    expdata_ = new Exp::ExpData( args, "expdata" );
    //    if( expdata_ ) expdata_->summary( cout );
  } 
  catch( int e ){
    ARG_ERR << "catch error( " << e << " )" << endl; 
    throw( e );
  }

  // create canvas
  main_ = new TCanvas( "EMC", "EMC", 600, 800 );
  
  // create latex object
  latex_ = new TLatex();
  latex_->SetTextSize( 0.03 );
  
  // prepare x binning
  xbin_.push_back( 0.01 );
  xbin_.push_back( 0.02 );
  xbin_.push_back( 0.035 );
  xbin_.push_back( 0.09 );
  xbin_.push_back( 0.2 );
  xbin_.push_back( 0.4 );
  xbin_.push_back( 1.0 );
  
  // prepare z bin
  zbin_.resize( 6, vector< double >( 2, 0.0 ) );
  zbin_[ 0 ][ 0 ] = 0.10; zbin_[ 0 ][ 1 ] = 0.15; 
  zbin_[ 1 ][ 0 ] = 0.15; zbin_[ 1 ][ 1 ] = 0.25; 
  zbin_[ 2 ][ 0 ] = 0.25; zbin_[ 2 ][ 1 ] = 0.35; 
  zbin_[ 3 ][ 0 ] = 0.35; zbin_[ 3 ][ 1 ] = 0.45; 
  zbin_[ 4 ][ 0 ] = 0.45; zbin_[ 4 ][ 1 ] = 0.60; 
  zbin_[ 5 ][ 0 ] = 0.60; zbin_[ 5 ][ 1 ] = 1.00; 

}

MyApplication::~MyApplication(){

  if( pdf_ )     delete pdf_;
  if( pion_ )    delete pion_;
  if( kaon_ )    delete kaon_;
  if( proton_ )  delete proton_;
  if( rest_ )    delete rest_;
  if( sigp_ )    delete sigp_;
  if( sigd_ )    delete sigd_;
  if( sigp_hp_ ) delete sigp_hp_;
  if( sigp_hm_ ) delete sigp_hm_;
  if( sigd_hp_ ) delete sigd_hp_;
  if( sigd_hm_ ) delete sigd_hm_;
  if( f_p_ )     delete f_p_;
  if( f_d_ )     delete f_d_;
  if( f_p_hp_ )  delete f_p_hp_;
  if( f_p_hm_ )  delete f_p_hm_;
  if( f_d_hp_ )  delete f_d_hp_;
  if( f_d_hm_ )  delete f_d_hm_;
  if( expdata_ ) delete expdata_;
}

void MyApplication::Run( Bool_t retrn ) throw( int ) {
  
  Utility::Arguments& args = Utility::Arguments::ref();
  
  // string output = args.get( "output", "view_emc.pdf" );
  // TPDF *tpdf = new TPDF( output.c_str() );
  
  // check hadron type 
  if( h_ != QCD::Flavor::HAp && h_ != QCD::Flavor::HAm ){
    ARG_ERR << "Hadron type must be HAp or HAm." << endl;
    return;
  }
  
  // check target type
  if( n_ != Xsection::Charge::p && n_ != Xsection::Charge::d){
    ARG_ERR << "Target type must be p or d." << endl;
    return;
  }
  
  // prepare key for DataPoint
  string key( "MultiQ2_" );
  key += ( n_ == Xsection::Charge::p ? "PRO" : "DEU" );
  key += "_"; 
  key += ( h_ == QCD::Flavor::HAp    ? "Hp" : "Hm" );
  
  if( ixbin_ > ( xbin_.size() - 2 ) ) {
    ARG_ERR << "ixbin should be less than " << xbin_.size() - 1 << endl;
    return ;
  }

  // store DataPoint to be drawn
  vector< vector< Exp::DataPoint* > > 
    points( 6, vector< Exp::DataPoint* >( 0 ) );

  for( vector< Exp::DataSet >::iterator itr = expdata_->data().begin();
       itr != expdata_->data().end(); 
       itr++ ){
    
    for( Exp::DataSet::iterator jtr = itr->begin(); 
	 jtr != itr->end(); 
	 jtr++ ){

      if( jtr->info() != key ) continue;
      
      double x = jtr->kine( "x ave" );
      int ix;
      for( ix = 0; ix < ( xbin_.size() - 1 ); ix++ )
	if( xbin_[ ix ] < x && x < xbin_[ ix + 1 ] ) break;
      points[ ix ].push_back( &(*jtr) );
      
    }
  }
  
  // TGraph Objects
  vector< Exp::DataPoint* >& vp = points[ ixbin_ ];
  if( vp.size() == 0 ){
    ARG_ERR << "No data point stored" << endl;
    return;
  }
  
  main_->SetLogx();
  main_->SetLogy();
  frame_ = main_->DrawFrame( 1.0, 0.04, 200.0, 40.0 );
  frame_->GetXaxis()->SetTitle( "Q^{2} (GeV^{2})" );
  frame_->GetYaxis()->SetTitle( "Hadron Multiplicity" );
  
  // --------------------------------------------------------- //
  //                          Label                            //
  // --------------------------------------------------------- //  
  latex_->DrawLatex( 20.0, 30.0, "Hadron Multiplicity: " );
  
  ostringstream ostr;
  
  ostr.str( "" );
  ostr << "Target: " << ( n_ == Xsection::Charge::p ? "P" : "D" ) ;
  latex_->DrawLatex( 20.0, 25.0, ostr.str().c_str() );
  
  ostr.str( "" );
  ostr << "Hadron: " << ( h_ == QCD::Flavor::HAp ? "h^{+}" : "h^{-}" );
  latex_->DrawLatex( 60.0, 25.0, ostr.str().c_str() );
  
  ostr.str( "" );
  ostr << xbin_[ ixbin_ ] << " < x < " << xbin_[ ixbin_ + 1 ] ;
  latex_->DrawLatex( 1.5, 30.0, ostr.str().c_str() );
  
  main_->Update();

  bool isScaled = args.hasOpt( "scale" );
  
  // --------------------------------------------------------- //
  //                     model calculation                     //
  // --------------------------------------------------------- //
  Xsection::xXsec *xf1   = NULL;
  Xsection::xzXsec *xf1h = NULL;
  if( n_ == Xsection::Charge::p ){
    xf1  = f_p_ ;
    xf1h = ( h_ == QCD::Flavor::HAp ? f_p_hp_ : f_p_hm_ );
  } else {
    xf1  = f_d_ ;
    xf1h = ( h_ == QCD::Flavor::HAp ? f_d_hp_ : f_d_hm_ );
  }


  // --------------------------------------------------------- //
  //                     loop over z bin                       //
  // --------------------------------------------------------- //
  for( int izbin = 0; izbin < 6; izbin++ ){
    
    int icolor = pow( 2.0, izbin );

    ostr.str( "" );
    ostr << zbin_[ izbin ][ 0 ] << " < z < " << zbin_[ izbin ][ 1 ] ;
    latex_->SetTextColor( icolor );
    latex_->SetTextSize( 0.025 );
    latex_->DrawLatex( 2.0, 
		       pow( 12.0, log10( 20.0 ) - 0.05 * izbin ), 
		       ostr.str().c_str() );
    

    TGraphErrors *gExp = new TGraphErrors( 0 );
    gExp->SetMarkerStyle( 20 );
    gExp->SetMarkerSize( 1.0 );
    gExp->SetMarkerColor( icolor );
    gExp->SetLineStyle( 1 );
    gExp->SetLineWidth( 2 );
    gExp->SetLineColor( icolor );

    TGraph *g = new TGraph( 0 );
    g->SetLineStyle( 3 );
    g->SetLineWidth( 2 );
    g->SetLineColor( icolor );
    
    int nBin = gExp->GetN();
    for( int i = 0; i < vp.size(); i++ ){
      
      double zave = vp[ i ]->kine( "z ave" );
      if( zbin_[ izbin ][ 0 ] > zave || 
	  zbin_[ izbin ][ 1 ] < zave ) continue;
      
      int iBin = gExp->GetN();
      gExp->Set( nBin = ( iBin + 1 ) ); // increment TGraph
      g->Set( nBin );                   // increment TGraph
      
      Exp::DataPoint& dp = *vp[ i ];
      
      // ------ Experiment Data Point
      double scale = 1.0;
      
      if( isScaled && dp.dataSet() != NULL ){
	scale = dp.dataSet()->normalization();
	cout << "scale:" << setw(12) << scale << endl; 
      }
      
      gExp->SetPoint( iBin, 
		      dp.kine( "Q^2 ave" ), 
		      dp.data() / scale );
      
      gExp->SetPointError( iBin, 
			   0, 
			   dp.stat() / scale );
      
      // ------- Model calculation
      // set Q2
      double Q2 = dp.kine( "Q^2 ave" );
      xf1->moment()->setQ2( Q2 );
      xf1h->moment()->setQ2( Q2 );
      // set x
      vector< double > x( 1, dp.kine( "x ave" ) );
      
      cout << setw(6) << x[0] << setw(6) << Q2 << flush;
      cout << " | " << flush;
      
      double f1v = (*xf1)( x );
      cout << setw(10) << f1v << flush;
      cout << " / " << flush;

      double f1hv = (*xf1h )( x, zbin_[ izbin ] );
      cout << setw(10) << f1hv << flush;
      cout << " = " << flush;
      cout << setw(10) << f1hv / f1v << flush;
      cout << " | " << flush;
      cout << setw(10) << dp.data() << flush;
      cout << setw(10) << scale << flush;
      
      g->SetPoint( iBin, Q2, ( f1v > 0.0 ? f1hv/f1v : 1.0E-7 ) );

      g->Draw( "L" );
      gExp->Draw( "P" );
      main_->Update();
      
      cout << endl;
      
    }
    
  }
  
  latex_->SetTextSize( 0.03 );
  
  main_->Draw();

  ostr.str( "" );
  ostr << "emc_multi_" 
       << ( n_ == Xsection::Charge::p ? "p" : "d" ) 
       << "_"
       << ( h_ == QCD::Flavor::HAp ? "hp" : "hm" )
       << "_"
       << ixbin_ ;
  //       << ".png";
  
  main_->Print( ( ostr.str() + ".png" ).c_str() );
  main_->Print( ( ostr.str() + ".pdf" ).c_str() );

}
