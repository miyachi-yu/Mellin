#include "FragXsecPanel.hh"
#include "PdfGraph.hh"

#include <ExpData/DataSet.hh>
#include <Fragmentation/Evolution.hh>

#include <Xsec/Xsec.hh>
#include <Xsec/XsecComp.hh>

#include <TROOT.h>
#include <TLine.h>
#include <TLatex.h>

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;
using namespace Utility;
using namespace Exp;

FragXsecPanel::FragXsecPanel() throw( int ) : 
  TCanvas( "FragXsec", "Inclusive hadron production cross-section", 
	   800, 600 ),
  evolution_( NULL ),
  xsec_( 3, (Xsection::Xsec*) NULL ), 
  xxsec_( 3, (Xsection::xXsec*) NULL ), 
  h_( QCD::Flavor::PIp ),
  expr_( (ExpData*) NULL ),
  legeExp_( (TLegend*) NULL ),
  legeThe_( (TLegend*) NULL ),
  line_( new TLine() ),
  text_( new TLatex() ),
  frame_( (TH1*) NULL ), 
  x_( 2, 0.0 ), y_( 2, 0.0 ),
  q2_( 0 ),
  scale_( 0 ),
  graph_( 3, vector< PdfGraph* >( 0 ) ),
  c_( 3.0 ), phi_( 0.55 ), length_( 8.0 ) {  

  this->initialize();
  
}


FragXsecPanel::FragXsecPanel( Arguments& args ) throw( int ) : 
  TCanvas( "FragXsec", "Inclusive hadron production cross-section", 
	   args.get( "canvas_xsiz", 1200 ), args.get( "canvas_ysiz", 800 ) ),
  evolution_( NULL ),
  xsec_( 3, (Xsection::Xsec*) NULL ), 
  xxsec_( 3, (Xsection::xXsec*) NULL ), 
  h_( QCD::Flavor::PIp ),
  toBeDelXsec_( false ),
  expr_( (ExpData*) NULL ),
  toBeDelExpr_( false ),
  legeExp_( (TLegend*) NULL ),
  line_( new TLine() ),
  text_( new TLatex() ),
  frame_( (TH1*) NULL ), x_( 2, 0.0 ), y_( 2, 0.0 ),
  q2_( 0 ),
  scale_( 0 ),
  graph_( 3, vector< PdfGraph* >( 0 ) ),
  c_( 3.0 ), phi_( 0.55 ), length_( 8.0 )
{
  
  this->initialize();
  
  if( args.hasOpt( "expdata" ) ) {
    expr_ = new ExpData( args );
    toBeDelExpr_ = true;
  }
  
  try {
    
    if( this->ff( QCD::Flavor::PIp ) == NULL &&
	this->ff( QCD::Flavor::KAp ) == NULL &&
	this->ff( QCD::Flavor::REp ) == NULL &&
	this->ff( QCD::Flavor::PRp ) == NULL ) {
      
      ARG_ERR << "one has to give at least one FF initial distributios" 
	      << endl;
      throw( 1 );
      
    }
    
    this->xsections();     // create Mellin Inversion objects

    // change to pi0
    if( h_ == QCD::Flavor::PIp && args.hasOpt( "neutral" ) )
      h_ = QCD::Flavor::PI0;

    this->drawData();      // draw experiment data
    this->draw();          // draw theory line
    
  } 
  
  catch( int error ) {
    cerr << __FILE__ << ":" << __LINE__ << "errror( " << error << " )" << endl;
    throw( error );
  }
}

FragXsecPanel::~FragXsecPanel() {
  if( toBeDelXsec_ && evolution_ ) delete evolution_;
  if( expr_ && toBeDelExpr_ ) delete expr_; 
  for( int i = 0; i < xsec_.size();  i++ ) if( xsec_[ i ] )  delete xsec_[ i ];
  for( int i = 0; i < xxsec_.size(); i++ ) if( xxsec_[ i ] ) delete xxsec_[ i ];
}

Fragmentation::Evo* FragXsecPanel::ff( const QCD::Flavor::HADRON& h ) 
  throw( int ) {
  
  Utility::Arguments& args = Utility::Arguments::ref();
  
  // check arguments for the given hadron type
  vector< string > opts = Fragmentation::Evo::options( h );
  vector< string >::iterator itr = args.hasOpt( opts );
  if( itr == opts.end() ) {
    ARG_LOG << "no FF found for hadron " << h << endl;
    for( int i = 0; i < opts.size(); i++ )
      ARG_LOG << "   --" << opts[ i ] << "=....." << endl;
    ARG_LOG << endl;
    return NULL;
  } else {
    ARG_LOG << "--" << *itr << " is found for " << h << endl;
  }
  
  Fragmentation::Evo *evo = NULL;
  
  try{
    
    evo = new Fragmentation::Evo( args, h, *itr );
  }
  
  catch( int error ){
    ARG_ERR << "catch error (" << error << ")" << endl;
    throw( error );
  }
  
  if( evo ){
    evo->constNf( false );
    toBeDelXsec_ = true;
    evolution_ = evo;
    h_ = h;
  }
  
  return evo;
}

void FragXsecPanel::xsections(){

  // prepare cross section
  xsec_[ 0 ] = new FragXsec_T::FragXsec( evolution_, h_ );
  
  // for c enriched sample
  xsec_[ 1 ] = new FragXsec_T::FragXsec( evolution_, h_ );
  xsec_[ 1 ]->setIndiv( QCD::Flavor::c );  
  
  // for b enriched sample
  xsec_[ 2 ] = new FragXsec_T::FragXsec( evolution_, h_ );
  xsec_[ 2 ]->setIndiv( QCD::Flavor::b );  
  
  // cross section in x-space
  for( int i = 0; i < xsec_.size(); i++ ){
    xxsec_[ i ] = new Xsection::xXsec( xsec_[ i ] );
    xxsec_[ i ]->setParameter( c_, phi_ );
    xxsec_[ i ]->upper( length_ );
  }
  
}


void FragXsecPanel::set( Fragmentation::Evo *evolution ) {
  
  // if the pointer does not change, do nothing
  if( evolution_ == evolution ) return; 
  
  // delete exsinting Xsec object and assign
  if( evolution_ != NULL && toBeDelXsec_ ) delete evolution_ ;
  evolution_ = evolution;
  toBeDelXsec_ = false;
  
  // get hadron type information
  h_ = evolution_->hadron();
  
  // create xXsec object
  this->xsections();
  
}

void FragXsecPanel::set( ExpData *data ){
  if( expr_ ) return;
  expr_ = data;
  toBeDelExpr_ = false;
  this->drawData();
}

void FragXsecPanel::initialize(){
  
  // set q2 values
  q2_.push_back( 29.0 * 29.0 ); scale_.push_back(  0.001 );
  q2_.push_back( 34.0 * 34.0 ); scale_.push_back(  0.01  );
  q2_.push_back( 44.0 * 44.0 ); scale_.push_back(  0.1  );
  q2_.push_back( 91.2 * 91.2 ); scale_.push_back( 1.0  );
  
  // set pointer container
  for( int i = 0; i < graph_.size(); i++ ) 
    graph_[i].resize( q2_.size(), (PdfGraph*) NULL );
  
  // ------------ canvas attribute -----------------
  this->drawFrame();
  this->Draw();
}

void FragXsecPanel::draw() {
  
  this->drawXsec();
  
  Utility::Arguments& args = Utility::Arguments::ref();

  string filename = "fragXsec_" ;
  filename += this->hadronName( h_ );
  if( args.hasOpt( "91.2" ))  filename += "_bc";
  filename += "." + args.get( "format", "png" );
  
  this->Print( filename.c_str() );

  //  this->WaitPrimitive(); 
  
}

void FragXsecPanel::drawXsec(){
  
  if( xxsec_[0] == NULL ) return;

 
  
  Utility::Arguments& args = Utility::Arguments::ref();
  vector< string > flavors( 3 );
  flavors[ 0 ] = "all";
  flavors[ 1 ] = "c";
  flavors[ 2 ] = "b";
  
  vector< string > keys( flavors );
  keys = args.get( "flavors", ",", keys );
  
  vector< int > colors( 0 );
  colors = args.get( "line_color", ",", colors );
  
  if( colors.size() < q2_.size() ){
    int ibegin = colors.size();
    colors.resize( q2_.size() );
    for( int i = ibegin; i < colors.size(); i++ ) colors[ i ] = i + 1;
  }
  
  
  vector< int > lstyle( 0 );
  lstyle = args.get( "line_style", ",", lstyle );
  if( lstyle.size() < xsec_.size() ){
    int ibegin = lstyle.size();
    lstyle.resize( xsec_.size() );
    for( int i = ibegin; i < lstyle.size(); i++ ) lstyle[ i ] = i + 1 ;
  }

  vector< int > pstyle( 0 );
  pstyle = args.get( "point_style", ",", pstyle );
  if( pstyle.size() < xsec_.size() ){
       int ibegin = pstyle.size();
       pstyle.resize( xsec_.size() );
    for( int i = ibegin; i < pstyle.size(); i++ ) pstyle[ i ] = i + 1 ;
  }
  

  if( colors.size() < q2_.size() ){
    int ibegin = colors.size();
    colors.resize( q2_.size() );
    for( int i = ibegin; i < q2_.size(); i++ ) colors[ i ] = i + 1;
  }
  

 
  for( int i = 0; i < xsec_.size(); i++ ){
	 for( int iq2 = q2_.size()-1; iq2 >-1; iq2-- ){

      if( find( keys.begin(), keys.end(), flavors[ i ] ) ==
	  keys.end() ) continue;
      
      xxsec_[ i ]->moment()->setQ2( q2_[ iq2 ]  );
      
      if( graph_[ i ][ iq2 ] == NULL ){
	
       // option 91.2 :to write  b and c on same plane
	 if( args.hasOpt( "91.2" ) ){
	      if(i==2)scale_[ q2_.size()-1 ]=0.1;
	      if(iq2!=q2_.size()-1)continue;}


	 // skip 34^2 GeV^2 in the case of PI0
	if( args.hasOpt( "neutral" ) && iq2==1){ scale_[ iq2 ]=0.01;continue;}
	if( args.hasOpt( "neutral" ) && iq2==0){ scale_[ iq2 ]=0.01;}
	
	// scale 2.0 accouns for positive and negative charged hadron
	// for neutral pion there is additional 0.5
	graph_[ i ][ iq2 ] = 
	  new PdfGraph( xxsec_[ i ], 0.005, 0.9, 30, 0.0, 
			( i == 0 ? 2.0 : 4.0 ) *
			( h_ == QCD::Flavor::PI0 ? 0.5 : 1.0 ) *
			scale_[ iq2 ] );

	graph_[ i ][ iq2 ]->SetLineWidth( 2.9 );

	//for lstyle option
	if( args.hasOpt( "lstyle" ))
	     graph_[ i ][ iq2 ]->SetLineStyle( lstyle[ i ]);
	else
	     graph_[ i ][ iq2 ]->SetLineStyle( (3*iq2)+3-1*(i));
	if(args.hasOpt( "lcolor"))
	     graph_[ i ][ iq2 ]->SetLineColor( colors[ iq2 ] );
	cout<<"lstyle "<<(3*iq2)+3+(i*2)<<endl
	    <<"iq2    "<<iq2<<endl
	    <<"i      "<<i<<endl;
	
	ostringstream ostr;

	
	ostr << " Q^{2} = " << sqrt( q2_[ iq2 ] )<<"^{2}" 
	     << " GeV^{2}"
	     << ( i == 0 ? "  " : ( i == 1 ? ",  c" : ",  b" ) )
	     <<" ( #times " << scale_[ iq2 ] << " )" ;
	
	legeThe_->SetTextSize( 0.028);
	legeThe_->AddEntry( graph_[ i ][ iq2 ], ostr.str().c_str(), "l" );
	
      } else {
	graph_[ i ][ iq2 ]->update();
      }
      
      graph_[ i ][ iq2 ]->Draw();
      this->Update();
    }
  }

  legeThe_->Draw();
  legeExp_->Draw();

  text_->SetTextSize( 0.06 );
  text_->SetTextAlign( 31 );

  double txtx = x_[1];
  double txty = pow( 10.0, 
		     0.01 * ( log10( y_[1] ) - log10( y_[0] ) ) + 
		     log10( y_[1] ) );

  ostringstream ostr;
  ostr << "e^{+} + e^{-} #rightarrow " << this->hadronPrintName( h_ ) << " + X";

  text_->DrawLatex( x_[ 1 ], txty, ostr.str().c_str() );

  this->Update();
}

void FragXsecPanel::drawData(){
  
  if( expr_ == NULL ) return;
  
  Utility::Arguments& args = Utility::Arguments::ref();
  vector< string > flavors( 3 );
  flavors[ 0 ] = "all";
  flavors[ 1 ] = "c";
  flavors[ 2 ] = "b";
  flavors = args.get( "flavors", ",", flavors );
  
  static string key = "e+e-_";
  key += this->hadronName( h_ );
  
  vector< DataSet >& data = expr_->data();
  
  vector< int > cid( 0 );
  cid = args.get( "data_color", ",", cid );
  if( cid.size() < data.size() ){
    int ibegin = cid.size();
    cid.resize( data.size() );
    for( int i = ibegin; i < cid.size(); i++ ) {
      switch( i + 1 ){
      case 5: cid[ i ] = 50; break;
      case 10: cid[ i ] = 49; break;
      case 19: cid[ i ] = 48; break;
      default: cid[ i ] = i + 1;
      }
    }
  }
  
  int icol = 0;
  for( int i = 0; i < data.size(); i++ ){
    DataSet& set = data[ i ]; 
    if( set.size() == 0 ) continue;
    string& dinfo = set[0].info();
    
    if( dinfo.substr( 0, key.size() ) != key ) continue;
    
    if( dinfo == key ) {
      if( find( flavors.begin(), flavors.end(), "all" ) == flavors.end() ) 
	continue;
    } else {
      if( find( flavors.begin(), flavors.end(), dinfo.substr( 8, 1 )  ) == 
	  flavors.end() ) continue;
    }


    int j = 0;
    for( j = 0; j < q2_.size(); j++ ){
	 if( q2_[ j ] == set[0].kine("Q^2") ) break;}

    double weight=1.0;
    double norm=1.0;

    cout<<j<<endl;
    if(args.hasOpt( "normalization" )) {norm =1/set.normalization();}
 
    if( args.hasOpt( "91.2" ) && i==data.size()-1)weight=0.1;
    if( args.hasOpt( "neutral" )&& j==0)weight=10;
    //hello
    cout<<"data.size() :"<<data.size()<<endl
	<<"j           :"<<j<<endl
	<<"i           :"<<i<<endl
	<<"norm        :"<<norm<<endl
	<<"weight      :"<<weight<<endl;

    PdfGraph *gExp =    new PdfGraph( set, 0.0, ( j < q2_.size() ? scale_[ j ]*weight*norm : 1.0*weight*norm ) );

    //    gExp->Print();
    int p=20 + i;
    if(p>27)p=p-7;
    cout<<"p :"<<p<<endl;
    gExp->SetMarkerColor( cid[ icol++ ] );
    gExp->SetMarkerStyle( p );
    float msize=1.0;
//    msize = args.get( "msize", ",", msize );
    gExp->SetMarkerSize( msize );
    gExp->Draw("P");
    
    //draw line
    cout<<"draw line"<<endl;
    TLine* l1 = new TLine(0.05 ,0.0001,0.05,900);
    l1 -> SetLineStyle(2);
    l1 -> SetLineWidth(3);
    l1-> Draw();
    
    TLine* l2 =new TLine(0.8 ,0.0001,0.8,900);
    l2 -> SetLineStyle(2);
    l2 -> SetLineWidth(3);
    l2-> Draw();

    if( args.hasOpt( "91.2" ))    legeExp_->SetHeader( "   Enriched Data" );
    legeExp_->AddEntry( gExp, this->convertExpLeg( set.abbreviation() ).c_str(), "p" );
    
    this->Update();
  }
  legeExp_->SetTextSize( 0.028);
  legeExp_->Draw();
  legeThe_->Draw();

  this->Update();
  
}

string FragXsecPanel::convertExpLeg( const std::string& str ){


  if( str == "SLD__inclusive_91.2_1999_PIpm" )   return "SLD (91.2^{2} GeV^{2})";

  if( str == "SLD_c_91.2_1999_PIpm" )   return "SLD(c) (91.2^{2} GeV^{2})";

  if( str == "SLD__b_91.2_1999_PIpm" )   return "SLD(b) (91.2^{2} GeV^{2})";

  if( str == "ALEPH__91.2_1995_PIpm" ||
      str == "ALEPH__91.2_2000_PI0" )   return "ALEPH (91.2^{2} GeV^{2})";

  if( str == "OPAL_91.2_1994_PIpm" ||
      str == "OPAL_91.2_1998_PI0" )  return "OPAL (91.2^{2} GeV^{2})";
  
  if( str == "DELPHI_inclusive_91.2_1998_PIpm" ||
      str == "DELPHI_91.2_1996_PI0" )   return "DELPHI (91.2^{2} GeV^{2})";

  if( str == "DELPHI_b_91.2_1998_PIpm" )   return "DELPHI(b) (91.2^{2} GeV^{2})";
  
  if( str == "L3_91.2_1994_PI0" )  return "L3 (91.2^{2} GeV^{2})";

    if( str == "TPC_29_1988_PIpm" ||
      str == "TPC_29_1985_PI0" )  return "TPC (29^{2} GeV^{2})";

  if( str == "TASSO_44_1989_PIpm" || 
      str == "TASSO_44_1989_PI0" )   return "TASSO (44^{2} GeV^{2})";

  if( str == "TASSO_34_1989_PIpm" ) return "TASSO (34^{2} GeV^{2})";


  return str;
}

void FragXsecPanel::drawFrame(){
  
  Utility::Arguments& args = Utility::Arguments::ref();
  x_[ 0 ] = 1.0E-2;
  x_[ 1 ] = 1.0;
  
  y_[ 0 ] = 1.0E-4;
  y_[ 1 ] = 1.0E+3;
  
  this->SetLogx();
  this->SetLogy();
  
  frame_ = this->DrawFrame( x_[ 0 ], y_[ 0 ], x_[ 1 ], y_[ 1 ] );
  frame_->GetXaxis()->SetTitle( "z_{H}" );
  frame_->SetTitleSize(0.07 ,"X");
  frame_->SetTitleOffset(0.5 ,"X");
  frame_->GetYaxis()->SetTitle( "(1/#sigma_{tot}) d#sigma / dz_{H}" ); 
  frame_->SetTitleSize(0.06 ,"Y");
  frame_->SetTitleOffset(0.67 ,"Y");

  if( args.hasOpt( "91.2" ) )
       legeExp_ = new TLegend( 0.12, 0.12, 0.307, 0.28 );  
  else legeExp_ = new TLegend( 0.12, 0.12, 0.340, 0.40 );  
  legeExp_ ->SetFillColor( 0 );
  legeThe_ = new TLegend( 0.60, 0.70, 0.89, 0.89 );  
  legeThe_ ->SetFillColor( 0 );
}

void FragXsecPanel::update(){
  if( evolution_ ) evolution_->clearCache();
  for( int i = 0; i < xsec_.size(); i++ )
    if( xsec_[ i ] ) xsec_[ i ]->clearCache();
}

string FragXsecPanel::hadronName( const QCD::Flavor::HADRON& h ) {
  switch( h ){
  case QCD::Flavor::PIp :
  case QCD::Flavor::PIm : return "PI";
  case QCD::Flavor::KAp :
  case QCD::Flavor::KAm : return "KA";
  case QCD::Flavor::REp :
  case QCD::Flavor::REm : return "RE";
  case QCD::Flavor::HAp :
  case QCD::Flavor::HAm : return "HA";
  case QCD::Flavor::PI0 : return "P0";
  }
  return "";
}

string FragXsecPanel::hadronPrintName( const QCD::Flavor::HADRON& h ) {
  switch( h ){
  case QCD::Flavor::PIp :
  case QCD::Flavor::PIm : return "#pi^{#pm}";
  case QCD::Flavor::KAp :
  case QCD::Flavor::KAm : return "K^{#pm}";
  case QCD::Flavor::REp :
  case QCD::Flavor::REm : return "Rest";
  case QCD::Flavor::HAp :
  case QCD::Flavor::HAm : return "h^{#pm}";
  case QCD::Flavor::PI0 : return "#pi^{0}";
  }
  return "";
}
