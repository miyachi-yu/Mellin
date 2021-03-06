#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <fstream>

#include <Utility/Arguments.hh>
#include <unpolPDF/Evolution.hh>
#include <Fragmentation/Evolution.hh>
#include <Xsec/xXsec.hh>

#include "StrFunc.hh"
#include "XsecComp.hh"
#include "CoeffFunc.hh"
#include "PartialInteg.hh"

using namespace std;
using namespace Utility;

int main( int argc, char* argv[] ){
  
  Arguments& args = *( Arguments::instance( argc, argv ) );

  try{
    
    IntegUnpolSIDIS::CoeffFunc* kernel_;
    IntegXsec::Charge tar_( kernel_ );
/*
    cout << tar_.tname(  IntegXsec::Charge::TARGET( 0 ) ) << " "
	 << tar_.tname(  IntegXsec::Charge::TARGET( 1 ) ) << " "
	 << tar_.tname(  IntegXsec::Charge::TARGET( 2 ) ) << " "
	 << tar_.tname(  IntegXsec::Charge::TARGET( 3 ) ) << endl;
*/
    int nt=100, nx=100;
    double xmin=1.0e-3, xmax=1.0, dx=(log10(xmax)-log10(xmin))/nx;
    double q2minNLO=0.4, q2max=200.0; //, dlam0=0.248;
    double q2minLO=0.26, q2min;//q2max=200.0; //, dlam0=0.248;
    ((args.hasOpt( "order" ) && (args.getOpt( "order" ) == "LO")) ?
     q2min=q2minLO : q2min=q2minNLO);
    double tmin=log(q2min), tmax=log(q2max), dt=(tmax-tmin)/nt;

    //    for( int k = 0; k < 3; k++ ){
    for( int k = 0; k < 1; k++ ){

      /*
    IntegUnpolSIDIS::StrFunc xxs( args, IntegXsec::Charge::TARGET( k ),
				QCD::Flavor::PIp, 6, 8, 1.0E-2 );
    IntegUnpolSIDIS::PartialInteg xs( &xxs, 6, 8, 1.0E-2 );
    xs.setPIntegRange( 0.2, 0.8 );
      */
    IntegUnpolSIDIS::StrFunc xxs( args, IntegXsec::Charge::p, QCD::Flavor::PIp,
                                   4, 0, 1.0E-2, 10.0, 2.0, 0.55 );
// note default is 10.0, 1.8, 0.6. FF is so sensitive to angle ( 0.6 -> 0.55 )
    xxs.setGngrid( 4 ); 
    //    xxs.setloginteg( true ); 
    IntegUnpolSIDIS::PartialInteg xs( &xxs, 4, 0, 1.0E-2 );
    xs.setGngrid( 4 );
    xs.setPIntegRange( 0.2, 0.8 );

    string filename = "xf1h" + args.getOpt( "order" );
    filename += "_" + tar_.tname ( IntegXsec::Charge::TARGET( k ) );
    filename += ".dat";
    
    ofstream outfile( filename.c_str() );

    if ( outfile ){

    for( int kk = 0; kk <= nt; kk+= 1){

    double qq2 = exp(tmin+dt*kk);

    if((( qq2 >= 1.0) && ( qq2 <= 1.3)) || 
       (( qq2 >= 2.0) && ( qq2 <= 2.3)) || 
       (( qq2 >= 10.0) && (qq2 <= 13.0)) ||
       (( qq2 >= 100.0) && ( qq2 <= 120.0)) ||
       (( qq2 >= 190.0) && ( qq2 <= 200.0))){

	 xxs.setQ2( qq2 );

	 for( double jj = 0; jj <= nx; jj += 1 ){
	      double x = xmin*pow( 10.0, jj*dx );
	      if ( (jj > nx / 2 ) && ( jj < nx ) ){
//	      if ( (jj >= 0 ) && ( jj <= nx ) ){
	      outfile << setw(16) << x << setw(16) << qq2  << setw(16)
		      << xs( x ) / 2.0 << endl;
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
