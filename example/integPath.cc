#include <Tranform/InverseMellin.hh>
#include <FragXsec_T/FragXsec.hh>
#include <polDIS/StrFunc.hh>
#include <unpolDIS/StrFunc.hh>
#include <Xsec/xXsec.hh>

#include <iostream>
#include <iomanip>
#include <cmath>
#include <sstream>

#include <TFile.h>
#include <TH2.h>

using namespace std;
using namespace Utility;
using namespace Transform;

class CFunc : public ComplexFunction {
  
public:
  
  CFunc( ComplexFunction *g, const double& n = 0 ) : 
    f_( g ), n_( n ) {}
  virtual ~CFunc() {}
  
  ComplexFunction& x( const double& v ){ x_ = v; return *this; }
  
  complex< double > operator()( const complex< double >& z ){
    return ( pow( x_ + n_ , - z  ) * (*f_)( z + n_ ) ); 
  }
  
private:
  ComplexFunction *f_;
  double x_;
  double n_;
  
};

class Func : public RealFunction {
  
public:
  
  Func( ComplexFunction *g, const double& n = 0 ) : 
    f_( g ), n_( n ) {}
  virtual ~Func() {}
  
  RealFunction& x( const double& v ){ x_ = v; return *this; }
  
  double operator()( const double& x ){
    complex< double > z( x, 0.0 );
    return ( pow( x_ , - z  ) * (*f_)( z + n_ ) ).real(); 
  }
  
private:
  ComplexFunction *f_;
  double x_;
  double n_;
  
};


void dumpZ( ComplexFunction& xsec, const double& xbj ){
  
  CFunc cf2( &xsec );
  cf2.x( xbj );
  
  ostringstream ostr;
  ostr << "h_" << xbj << "^{-z} f(z)" ;
  
  double xmin = -2.0;
  double xmax =  2.0;
  
  double ymin = -2.0;
  double ymax =  2.0;
  
  TH2 *h2 = new TH2F( ostr.str().c_str(), ostr.str().c_str(), 
		      100, xmin, xmax, 100, ymin, ymax );  
  
  for( int i = 1; i <= h2->GetXaxis()->GetNbins(); i++ )
    for( int j = 1; j <= h2->GetYaxis()->GetNbins(); j++ ){
      double x = h2->GetXaxis()->GetBinCenter( i );
      double y = h2->GetYaxis()->GetBinCenter( j );
      complex< double > z( x, y );
      
      h2->Fill( x, y, cf2( z ).real() );
      
    }
  
}


void dump( ComplexFunction& xsec, const double& n, 
	   const double& xbj, const double& c0 ){
  
  cout << "c0:     " << c0 << endl;
  CFunc cf2( &xsec, n );
  cf2.x( xbj );
  
  for( int i = 1; i < 4; i++ ){
    cout 
      << "F(" << i << ")(c0): " 
      << cf2.differential( complex< double >( c0, 0.0 ), i ) 
      << endl;
  }    
  
  double c1 = 
    cf2.differential( complex< double >( c0, 0.0 ), 3 ).real() /
    cf2.differential( complex< double >( c0, 0.0 ), 2 ).real() / 6.0 ; 
  
  cout << "c1:     " << c1 << endl;
  
  //  if( c1 < 0 ){
    
    cout << "xbj = " << xbj << endl;
    
    cout 
      << setw(4)  << ""
      << setw(46) << "c0+c1*t^2+it"
      << setw(46) << "c0+it"
      << setw(46) << "c0+t*e(phi)"
      << endl;
    
    cout 
      << setw(4)  << "t"
      << setw(30) << "F(z)=x^{-z}f(z)"
      << setw(16) << "Im(dz F(z))"
      << setw(30) << "F(z)=x^{-z}f(z)"
      << setw(16) << "Im(dz F(z))"
      << setw(30) << "F(z)=x^{-z}f(z)"
      << setw(16) << "Im(dz F(z))"
      << endl;
    
    for( double t = 0; t < 5.0; t += 0.1 ){
      
      complex< double > z( c0 + c1 * t * t, t );
      complex< double > dz( 2.0 * c1 * t, 1.0 );
      
      complex< double > n( c0, t );
      complex< double > dn( 0, 1.0 );
      
      complex< double > z2 = 
	t * complex< double >( cos( 0.55 * M_PI ), 
			       sin( 0.55 * M_PI ) ) + 
	complex< double >( c0, 0.0 );
      
      complex< double > dz2 = 
	complex< double >( cos( 0.55 * M_PI ), sin( 0.55 * M_PI ) );
      
      cout 
	<< setw(4)  << t
	<< setw(30) << cf2( z )
	<< setw(16) << ( dz * cf2( z ) ).imag()
	<< setw(30) << cf2( n )
	<< setw(16) << ( dn * cf2( n ) ).imag()
	<< setw(30) << cf2( z2 )
	<< setw(16) << ( dz2 * cf2( z2 ) ).imag()
	<< endl;
      
    }
    
    
    //  }
  
}

int main( int argc, char* argv[] ){
  
  Arguments& args = Arguments::ref( argc, argv );
  
  try {

    FragXsec_T::FragXsec  *xsec = 
      ( args.hasOpt( "FF" ) ? 
	new FragXsec_T::FragXsec( args, QCD::Flavor::PIp  ) :
	NULL );
    
    polDIS::StrFunc *g1 = 
      ( args.hasOpt( "polPDF" ) ?
	new polDIS::StrFunc( args, Xsection::Charge::p ) :
	NULL );
    
    unpolDIS::StrFunc *f1 = 
      ( args.hasOpt( "unpolPDF" ) || args.hasOpt( "MRST" ) ?
	new unpolDIS::StrFunc( args, Xsection::Charge::p ) :
	NULL );
    
    if( xsec ) xsec->setQ2( args.get( "Q2", 2.5 ) );
    if( g1 )   g1->setQ2( args.get( "Q2", 2.5 ) );
    if( f1 )   f1->setQ2( args.get( "Q2", 2.5 ) );
    
    //  x-space 
    Xsection::xXsec *xxsec = ( xsec ? new Xsection::xXsec( xsec ) : NULL );
    Xsection::xXsec *xg1   = ( g1   ? new Xsection::xXsec( g1 ) :   NULL );
    Xsection::xXsec *xf1   = ( f1   ? new Xsection::xXsec( f1 ) :   NULL );
    
    double xbj = args.get( "x", 0.01 );
    
    if( 1 ){
      
      if( xxsec ) xxsec->F().x( xbj );
      if( xg1 )   xg1->F().x( xbj );
      if( xf1 )   xf1->F().x( xbj );
      
      for( double x = 0.01; x < 4.0; x += 0.02 ){
	
	complex< double > z( x, 0.0 );
	
	cout << setw(5)  << x << flush;
	if( xxsec ) {
	  cout << setw(16) << xxsec->F()( z ).real() << flush;
	  cout << setw(16) << xxsec->F().differential( z, 1 ).real() << flush;
	}
	
	if( xf1 ){
	  cout << setw(16) << xf1->F()( z ).real()   << flush;
	  cout << setw(16) << xf1->F().differential( z, 1 ).real() << flush;
	}
	
	if( xg1 ){
	  cout << setw(16) << xg1->F()( z ).real()   << flush;
	  cout << setw(16) << xg1->F().differential( z, 1 ).real() << flush;
	}
	
	cout << endl;
      }
      

      if( xsec ){

	Func rxsec( xsec );
	rxsec.x( xbj );
	
	vector< double > range( 3, 0.0 );
	range[0] = 1.5;
	range[1] = 2.0;
	range[2] = 3.0;
	
	for( int i = 0; i < range.size(); i++ ) 
	  cout << setw(12) << rxsec( range[i] ) << flush;
	cout << setw(12) << rxsec.minimum( range, 1.0E-3 ) << endl;
	
      }
      
    }
    
    if( 0 ) {
      
      /*
	Func f1_0( g1, 0 );
	Func f1_1( g1, 1 );
	Func f2( xsec, 0 );
	//    Func f3( &f1, 0 );
	
	f1_0.x( xbj );
	f1_1.x( xbj );
      f2.x( xbj );
      //    f3.x( xbj );
      
      cout 
      << setw(5)  << "x"
      << setw(16) << "x^{-z}g1"
	<< setw(16) << "d(x^{-z}g1)"
	<< setw(16) << "x^{-z+1}g1"
	<< setw(16) << "x^{-z}xsec"
	<< setw(16) << "d(x^{-z}xsec)"
	//      << setw(16) << "x^{-z}f1"
	//      << setw(16) << "d(x^{-z}f1)"
	<< endl;
	
      for( double x = -1.5; x < 1.5; x += 0.02 ){
	
	complex< double > z( x, 0.0 );
	cout 
	  << setw(5)  << x
	  << setw(16) << f1_0(x)
	  << setw(16) << f1_0.differential(x)
	  << setw(16) << f1_1(x)
	  << setw(16) << f2(x)
	  << setw(16) << f2.differential(x)
	  //	<< setw(16) << f3(x)
	  //	<< setw(16) << f3.differential(x)
	  //	<< setw(18) << pow( xbj, -z )
	  //	<< setw(18) << g1( z )
	  << endl;

      */
    }
    
    if( xf1 ) { delete xf1; delete f1; }
    if( xg1 ) { delete xg1; delete g1; }
    if( xxsec ) { delete xxsec; delete xsec; }
    
  
  }
  
  catch( int e ){
  }
  
};
