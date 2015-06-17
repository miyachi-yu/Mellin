#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <fstream>

#include <Utility/Arguments.hh>
#include <Fragmentation/Evolution.hh>
#include <Xsec/xXsec.hh>

#include "StrFunc.hh"
#include "XsecComp.hh"
#include "CoeffFunc.hh"

using namespace std;
using namespace Utility;

int main( int argc, char* argv[] ){
  
  Arguments& args = *( Arguments::instance( argc, argv ) );

  try{
    
    int nt=100, nx=100;
    double xmin=1.0e-3, xmax=1.0, dx=(log10(xmax)-log10(xmin))/nx;
    //    double q2minNLO=0.4, q2max=200.0; //, dlam0=0.248;
    double q2minNLO=0.4, q2max=10000.0; //, dlam0=0.248;
    double q2minLO=0.26, q2min;//q2max=200.0; //, dlam0=0.248;
    ((args.hasOpt( "order" ) && (args.getOpt( "order" ) == "LO")) ?
     q2min=q2minLO : q2min=q2minNLO);
    double tmin=log(q2min), tmax=log(q2max), dt=(tmax-tmin)/nt;

    IntegFragXsec::StrFunc xs( args, QCD::Flavor::HADRON( 1 ),
                               6, 8, 10E-2 );
    xs.xsecInteg()->coeff()->constNf( false );
    dynamic_cast< Fragmentation::Evo* >
      ( xs.xsecInteg()->vevo() [0] )->constNf( false );

    /*
    int icb[ icbi ] = { 4, 5 };
    for( int j = 0; j < 2; j++ )
      xs.setIndiv( Flavor::PARTON( icb[ j ] ) );
    */

    string filename = "csPI" + args.getOpt( "order" );
    //	 filename += "_" + tar_.tname ( QCD::Flavor::HADRON( k ) );
    filename += ".dat";
    
    ofstream outfile( filename.c_str() );
    
    if ( outfile ){
      
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
	      //	      if ( (jj >= 0 ) && ( jj <= nx ) ){
	      outfile << setw(16) << x << setw(16) << qq2  << setw(16)
		      << 2.0 * xs( x ) / x << endl;
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
  
  catch( int err ){
  }
  
}
