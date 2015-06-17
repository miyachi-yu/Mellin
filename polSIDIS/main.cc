#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <fstream>
#include <iomanip>//setw
#include <Utility/Arguments.hh>
#include <Xsec/xzXsec.hh>
#include "StrFunc.hh"
#include "XsecComp.hh"
#include "CoeffFunc.hh"

using namespace std;
using namespace Utility;

int main( int argc, char* argv[] ){
  
  Arguments& args = *( Arguments::instance( argc, argv ) );

  try{
    
    polSIDIS::CoeffFunc* ckernel_;
    Xsection::Charge tar_( ckernel_ );
/*
    cout << tar_.tname(  Xsection::Charge::TARGET( 0 ) ) << " "
	 << tar_.tname(  Xsection::Charge::TARGET( 1 ) ) << " "
	 << tar_.tname(  Xsection::Charge::TARGET( 2 ) ) << " "
	 << tar_.tname(  Xsection::Charge::TARGET( 3 ) ) << endl;
*/
    int nt=100, nx=100;
    double xmin=1.0e-3, xmax=1.0, dx=(log10(xmax)-log10(xmin))/nx;
    double q2minNLO=0.4, q2max=200.0; //, dlam0=0.248;
    double q2minLO=0.26, q2min;//q2max=200.0; //, dlam0=0.248;
    ((args.hasOpt( "order" ) && (args.getOpt( "order" ) == "LO")) ?
     q2min=q2minLO : q2min=q2minNLO);
    double tmin=log(q2min), tmax=log(q2max), dt=(tmax-tmin)/nt;
    std::vector< double > x_( 1, 0.0 ), z_( 2, 0.0 );
    z_[ 0 ] = 0.8; z_[ 1 ] = 0.2;

//    for( int k = 0; k < 3; k++ ){
    for( int k = 0; k < 1; k++ ){
//    for( int k = 2; k < 3; k++ ){

    polSIDIS::StrFunc xs( args, Xsection::Charge::TARGET( k ) );
//    xs.xsecComp()->coeff()->constNf( false );
//    (xs.xsecComp()->vevo())[0]->constNf( false );
//    std::vector< Kernel* >& vevo_ = xs.xsecComp()->vevo();
//    for( int i = 0; i < vevo_.size(); i++ ) vevo_[i]->constNf( false );

    Xsection::xzXsec xxc( &xs, 4, 6, 1.0E-4, 10.0, 2.0, 0.55 );//NLO
//    Xsection::xXsec xxc( &xs, 4, 6, 1.0E-4, 10.0, 1.8, 0.6 );//LO

    std::vector< double > mom( 2, 0.0 );
    mom[ 1 ] = 1.0;
    xxc.setMomParam( mom );

	 string filename = "xg1h" + args.getOpt( "order" );
	 filename += "_" + tar_.tname ( Xsection::Charge::TARGET( k ) );
	 filename += "PIP.dat";

    ofstream outfile( filename.c_str() );

    if ( outfile ){

    for( int kk = 0; kk <= nt; kk+= 1){

    double qq2 = exp(tmin+dt*kk);

    if((( qq2 > 1.0) && ( qq2 <= 1.3)) || 
       (( qq2 > 2.0) && ( qq2 <= 2.3)) 
//){
|| 
       (( qq2 > 10.0) && (qq2 <= 13.0)) ||
       (( qq2 > 100.0) && ( qq2 <= 120.0)) ||
       (( qq2 > 190.0) && ( qq2 <= 200.0))){

	 xs.setQ2( qq2 );

	 for( double jj = 0; jj <= nx; jj += 1 ){
	      double x = xmin*pow( 10.0, jj*dx );
	      x_[ 0 ] = x;
	      if ( (jj > nx / 2 ) && ( jj < nx ) ){
//	      if ( (jj >= 0 ) && ( jj <= nx ) ){
	      outfile << setw(16) << x << setw(16) << qq2  << setw(16)
		      << x * xxc( x_, z_ ) * ( z_[ 0 ] - z_[ 1 ] ) / 2.0 
		      << endl;
	      }
	      else{
	      outfile << setw(16) << x << setw(16) << qq2 << setw(16) 
		      << 0.0 << endl;	 
	      }
	 }
	 
    }else{

	 for( double jj = 0; jj <= nx; jj += 1 ){
	      double x = xmin*pow( 10.0, jj*dx );
	      outfile << setw(16) << x << setw(16) << qq2 << setw(16) 
		      << 0.0 << endl;	 
	 }

    }
    
    }
    }
    outfile.close();
    
    }

  }
  
  catch( int err ){
  }
  
}
