#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <fstream>
#include <iomanip>
using namespace std;

#include <Utility/Arguments.hh>
#include <Xsec/xXsec.hh>
#include "FragXsec.hh"
#include "XsecComp.hh"
#include "CoeffFunc.hh"

using namespace Utility;
using namespace QCD;

int main( int argc, char* argv[] ){
  
  Arguments& args = *( Arguments::instance( argc, argv ) );
  
  try{
    
    FragXsec_T::FragXsec xs( args );
    Xsection::xXsec xxc( &xs, 4, 6, 1.0E-4, 20.0, 2.0, 0.5 );//NLO
    //    Xsection::xXsec xxc( &xs, 4, 6, 1.0E-4, 10.0, 1.8, 0.6 );//LO
    // note that angle argument in SetParameter is not the unit of M_PI //
    
    int nt=100, nx=100;
    double xmin=1.0e-3, xmax=1.0, dx=(log10(xmax)-log10(xmin))/nx;
    double q2minNLO=0.4, q2max=10000.0; //, dlam0=0.248;
    double q2minLO=0.26, q2min;//q2max=200.0; //, dlam0=0.248;
    ((args.hasOpt( "order" ) && (args.getOpt( "order" ) == "LO")) ?
     q2min=q2minLO : q2min=q2minNLO);
    double tmin=log(q2min), tmax=log(q2max), dt=(tmax-tmin)/nt;
    
    ofstream charge( "testCharge.dat" );
    Flavor flav;
    int ntt = 400;

    for( int kk = 0; kk <= ntt; kk+= 1){
      double qq2 = exp(tmin+dt * nt / ntt *kk);
      
      //    for( double qq2 = 1.0; qq2 <= 10000; qq2++ ){
	 if ( charge ){
	      xs.setQ2( qq2 );

	      charge << setw( 10 ) << qq2 << flush;
	      for ( int i = 0; i <= 2; i++ )
//	 for ( int i = -6; i <= 6; i++ )
//        cout <<  flav.name( Flavor::PARTON( i ) ) << flush;
		   charge << setw( 10 ) 
			  << ( xs.xsecComp()->coeff()->vchar() )[ i + 6 ] 
			  << setw( 10 )<< flush;
	      charge << endl;
	 }
    }


    const int icbi = 2;
//    int icbi = 2;
    int icb[ icbi ] = { 4, 5 };

    for( int j = 0; j < icbi + 1; j++ ){
	 FragXsec_T::FragXsec xs( args );
	 Xsection::xXsec xxc( &xs, 4, 6, 1.0E-4, 20.0, 2.0, 0.5 );//NLO
	 string filename = "csPI" + args.getOpt( "order" );    
	 if( j != 0 ){
	 filename += "_" + flav.name( Flavor::PARTON( icb[ j - 1 ] ) );
	 xs.setIndiv( Flavor::PARTON( icb[ j - 1 ] ) );
	 }
	 filename += ".dat";

    ofstream outfile( filename.c_str() );

    if (outfile){

    for( int kk = 0; kk <= nt; kk+= 1){

    double qq2 = exp(tmin+dt*kk);
    double sq = sqrt( qq2 );

    if((( sq >= 3.0 ) && ( sq <= 3.2)) ||
      (( sq >= 1.0 ) && ( sq <= 1.1)) || 
      (( sq >= 20.0 ) && ( sq <= 21.0)) || 
       (( sq >= 50.0 ) && ( sq <= 60.0)) ||
       (( sq >= 80.0 ) && ( sq <= 100.0))){

	 xs.setQ2( qq2 );

	 for( double jj = 0; jj <= nx; jj += 1 ){
	      double x = xmin*pow( 10.0, jj*dx );
	      if ( (jj > nx / 2 ) && ( jj < nx ) ){
	      outfile << setw(16) << x << setw(16) << qq2  << setw(16)
		      << ( j == 0 ? 2.0 : 4.0 ) * xxc( x ) << endl;
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
