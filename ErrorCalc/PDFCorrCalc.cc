#include "PDFCorrCalc.hh"
#include <Xsec/Xsec.hh>
#include <Xsec/xXsec.hh>
#include <Xsec/xzXsec.hh>
#include <fstream>
#include <TGraph.h>
#include <TCanvas.h>
//#include <stdio.h>

using namespace std;
using namespace QCD;
using namespace Evolution;
using namespace Xsection;
using namespace Transform;
using namespace ErrorCalc;

PDFCorrCalc::PDFCorrCalc( Xsec* xsec ):
  ErrCalc( xsec ), vpdf_( 2, ( PDF* ) NULL ), vpdfx_( 2, ( PDFx* ) NULL ),
  //  parton1_( Flavor::PARTON( 0 ) ), parton2_( Flavor::PARTON( 0 ) )
  vparton_( 2, ( const Flavor::PARTON* ) NULL ),
  errmx_( 2, vector< double >( 2, 0.0 ) ),
  covmtx_( true ), outfile_( false ), outcurv_( true ), nom_( false ),
  NN_( complex< double >( 0.0, 0.0 ) ), 
  MM_( complex< double >( 0.0, 0.0 ) ), XX_( 0.0 ), ZZ_( 0.0 )
{
}

PDFCorrCalc::~PDFCorrCalc(){
  for( int i = 0; i < 2; i++ ){
  if( vpdf_[ i ] ) delete vpdf_[ i ];
  if( vpdfx_[ i ] ) delete vpdfx_[ i ];
  }
}

void PDFCorrCalc::setPID_1( const QCD::Flavor::PARTON& p )
{ vparton_[ 0 ] = &p; }
void PDFCorrCalc::setPID_2( const QCD::Flavor::PARTON& p )
{ vparton_[ 1 ] = &p; }


void PDFCorrCalc::covout( ComplexFunction* f, ComplexFunction* g ){
  if( !f || !g ) return;
  this->getN() = NN_; this->getM() = NN_;
  errmx_[ 0 ][ 0 ] = ( this->calcerror( f, f ) ).real();
  this->getN() = NN_; this->getM() = MM_;
  errmx_[ 0 ][ 1 ] = ( this->calcerror( f, g ) ).real();
  this->getN() = MM_; this->getM() = NN_;
  errmx_[ 1 ][ 0 ] = ( this->calcerror( g, f ) ).real(); 
  this->getN() = MM_; this->getM() = MM_;
  errmx_[ 1 ][ 1 ] = ( this->calcerror( g, g ) ).real();

  if( outfile_ ) this->fileout();
  if( outcurv_ ) this->curveout();
}

void PDFCorrCalc::covout( RealFunction* f, RealFunction* g ){
   if( !f || !g ) return;
   this->getX() = XX_; this->getZ() = XX_;
   errmx_[ 0 ][ 0 ] = this->calcerror( f, f );
   this->getX() = XX_; this->getZ() = ZZ_;
   errmx_[ 0 ][ 1 ] = this->calcerror( f, g );
   this->getX() = ZZ_; this->getZ() = XX_;
   errmx_[ 1 ][ 0 ] = this->calcerror( g, f );
   this->getX() = ZZ_; this->getZ() = ZZ_;
   errmx_[ 1 ][ 1 ] = this->calcerror( g, g );

   if( outfile_ ) this->fileout();
   if( outcurv_ ) this->curveout();
}

complex< double > PDFCorrCalc::operator()( const complex< double >& n ){
  return (*this)( n , n );
}

complex< double > PDFCorrCalc::operator()
  ( const complex< double >& n, const complex< double >& m ){
  
  NN_ = n; MM_ = m;
  
  for( int i = 0; i < 2; i++ ){
    if( !vpdf_[ i ] ) vpdf_[ i ] 
			= new PDF( getvevo()[ getivevo() ], *vparton_[ i ] );
    if( *vparton_[ i ] != vpdf_[ i ]->getPID() ) 
      vpdf_[ i ]->getPID() = *vparton_[ i ] ; 
  }

  if( covmtx_ ) this->covout( vpdf_[ 0 ], vpdf_[ 1 ] );

  this->getN() = NN_; this->getM() = MM_;
  return this->calcerror( vpdf_[ 0 ], vpdf_[ 1 ] );
}

double PDFCorrCalc::operator()( const double& x ){
  return (*this)( x, x );
}

double PDFCorrCalc::operator()( const double& x, const double& z ){

  XX_ = x; ZZ_ = z;

  for( int i = 0; i < 2; i++ ){
  if( !vpdfx_[ i ] ) vpdfx_[ i ] 
		       = new PDFx( getvevo()[ getivevo() ], *vparton_[ i ],
				   4, 6, 1.0E-4, 10.0, 2.0, 0.55 );
  if( *vparton_[ i ] != vpdfx_[ i ]->pdf().getPID() ) 
    vpdfx_[ i ]->pdf().getPID() = *vparton_[ i ]; 
  }

  if( covmtx_ ) this->covout( vpdfx_[ 0 ], vpdfx_[ 1 ] );

  this->getX() = XX_; this->getZ() = ZZ_;
  return this->calcerror( vpdfx_[ 0 ], vpdfx_[ 1 ] );

}

void PDFCorrCalc::fileout(){
  string output = "./covmtx/covmtx_";
  output += name() + ".dat";
  ofstream covm( output.c_str() );
  covm << errmx_[ 0 ][ 0 ] << setw( 16 ) 
       << errmx_[ 0 ][ 1 ] << endl;
  covm << errmx_[ 1 ][ 0 ] << setw( 16 ) 
       << errmx_[ 1 ][ 1 ] << endl;
  covm.close();
}

void PDFCorrCalc::curveout(){
  vector< vector< double > > tem = errmx_;

  if( nom_ ){ // for inverse matrix
    tem[ 0 ][ 0 ] = tem[ 0 ][ 0 ] / errmx_[ 0 ][ 0 ];
    tem[ 0 ][ 1 ] = tem[ 0 ][ 1 ] / sqrt( errmx_[ 0 ][ 0 ] * errmx_[ 1 ][ 1 ] );
    tem[ 1 ][ 0 ] = tem[ 1 ][ 0 ] / sqrt( errmx_[ 0 ][ 0 ] * errmx_[ 1 ][ 1 ] );
    tem[ 1 ][ 1 ] = tem[ 1 ][ 1 ] / errmx_[ 1 ][ 1 ];
  }
  /*
  cout << tem[ 0 ][ 0 ] << setw( 8 ) << tem[ 0 ][ 1 ] << endl;
  cout << tem[ 1 ][ 0 ] << setw( 8 ) << tem[ 1 ][ 1 ] << endl;
  */

  //inverse of err matrix { (a, b), (b,c) } / d 
  double d = tem[ 0 ][ 0 ] * tem[ 1 ][ 1 ] 
    - tem[ 0 ][ 1 ] * tem[ 1 ][ 0 ]; // determinant //
  if( d == 0.0 ) throw( 8 );

  double a = tem[ 1 ][ 1 ];
  double b = - tem[ 0 ][ 1 ]; // - tem[ 1 ][ 0 ];
  double c = tem[ 0 ][ 0 ];

  // d = a * x^2 + 2 * b * x * y + c * y^2
  // y = - b / c * x +- ( ( d - ( a - b^2 / c ) * x^2 ) / c )^{ 1 / 2 }

  // plot //
  TCanvas* c1 = new TCanvas("c1","A Simple Graph Example",
			    200,10,
			    700,500);
  
  double x[ 1000 ], y[ 1000 ];
  int n = 200;

  double xsigma = ( nom_ ? 1.0 : sqrt( errmx_[ 0 ][ 0 ] ) );
  double ysigma = ( nom_ ? 1.0 : sqrt( errmx_[ 1 ][ 1 ] ) );
  double dx = 2.0 * xsigma / n;  

  for( int i = 0; i < n + 1 ; i++ ) {
    x[ 2*i ] = - xsigma + i * dx;
    x[ 2*i + 1 ] = x[ 2*i ];
    y[ 2*i ] = - b / c * x[ 2*i ]
      + sqrt( ( d - ( a - b * b / c ) * x[ 2*i ] * x[ 2*i ] ) / c );
    y[ 2*i + 1 ] = - b / c * x[ 2*i + 1 ]
      - sqrt( ( d - ( a - b * b / c ) * x[ 2*i + 1 ] * x[ 2*i + 1 ] ) / c );
  }

  TGraph* gr = new TGraph( 2 * ( n + 1 ), x, y );
  gr->SetMarkerStyle( 20 );
  gr->SetMarkerSize( 0.6 );
  gr->SetMarkerColor( 2 );
  gr->Draw("AP");

  double xx[ 5 ], yy[ 5 ];
  xx[ 0 ] = xsigma; xx[ 1 ] = xsigma;
  yy[ 0 ] = - ysigma; yy[ 1 ] = ysigma;
  xx[ 2 ] = - xsigma; xx[ 3 ] = - xsigma;
  yy[ 2 ] = ysigma; yy[ 3 ] = - ysigma;
  xx[ 4 ] = xx[ 0 ]; yy[ 4 ] = yy[ 0 ];
  TGraph* ln = new TGraph( 5, xx, yy );
  ln->Draw( "L" ); //ln->Draw( "Lsame" );

  if( c1 ){
    string output = "./chi2cv/chi2cv_";
    output += name() + ".png";
    c1->Print( output.c_str() );
  }
  
}

string PDFCorrCalc::name(){
  char tmpchar[ 10 ];
  sprintf( tmpchar, "%1.1f", getvevo()[ getivevo() ]->Q2() );

  string output = Flavor::name( *vparton_[ 0 ] ) + "_" 
    + Flavor::name( *vparton_[ 1 ] ) + "_" 
    + string( tmpchar );
  //  output + =;
  return output;
}
