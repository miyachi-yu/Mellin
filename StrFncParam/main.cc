#include "ALLM91.hh"
#include "ALLM97.hh"
#include "DoLara.hh"
#include "F1Param.hh"
#include "F2Param.hh"
#include "R1990.hh"
#include "R1998.hh"
#include "F2pSMC.hh"
#include "F2nSMC.hh"
#include "F2dSMC.hh"
#include "GD07P.hh"
#include "GD11P.hh"

#include "Utility/Arguments.hh"

#include <iostream>
#include <iomanip>
#include <cmath>
#include <fstream>
#include <string>

using namespace std;

    /*

  StrFncParam::R1990  r1990;
  StrFncParam::R1998  r1998;
  
  StrFncParam::F2pSMC  f2psmc;
  StrFncParam::F2nSMC  f2nsmc;
  StrFncParam::F2dSMC  f2dsmc;
  
  StrFncParam::F2Param f2al91( al97, r1998 );
  StrFncParam::F1Param f1al91( al97 );

  for( double lq2 = 0; lq2 < 2; lq2 += 0.2 ){
    double q2 = pow( 10.0, lq2 );
    double x = 0.015;
    cout << "R(" << x << ", " << setw(10) << q2 << ") = "
	 << setw(12) << r1990( x, q2 )
	 << " +- "
	 << setw(12) << r1990.error( x, q2 )
	 << setw(12) << r1998( x, q2 )
	 << " +- "
	 << setw(12) << r1998.error( x, q2 )
	 << endl;
  }
  */

void calc( StrFncParam::PhotoAbsorbXsec& xsec ){
  
  Utility::Arguments& args = Utility::Arguments::ref();
  double q2min = args.get( "Q2min",    0.1 );
  double q2max = args.get( "Q2max", 1000.0 );
  int    nq2   = args.get( "nQ2",    400 );

  vector< double > q2;
  double lq2min = log10( q2min );
  double lq2max = log10( q2max );
  double dlq2   = ( lq2max - lq2min ) / nq2;
  
  for( int i = 0; i < nq2; i++ )
    q2.push_back( pow( 10, lq2min + dlq2 * i ) );
  
  double xmin = args.get( "xmin", 1.0E-4 );
  double xmax = args.get( "xmax", 1.0    );
  int    nx   = args.get( "nx",   400    );
  
  vector< double > x;
  double lxmin = log10( xmin );
  double lxmax = log10( xmax );
  double dlx   = ( lxmax - lxmin ) / nx;
  
  for( int i = 0; i < nx; i++ )
    x.push_back( pow( 10, lxmin + dlx * i ) );
  
  string ofname = args.get( "output", "sigma-x-q2.dat" );
  ofstream ofs( ofname.c_str() );
  int colw = args.get( "colw", 11 );

  ofs << "####################################################" << endl;
  ofs << "# photo-absoption cross section sigma(x,Q^2)" << endl;
  ofs << "# Parameterization: " << xsec.name() << endl;
  ofs << "# " << endl;
  ofs << "# Format: " << endl;
  ofs << "# the first line: x values" << endl;
  ofs << "# from the second line: Q2_j  sigma(x_i, Q2_j) for i = 1, .... n"
      << endl;
  ofs << "####################################################" << endl;
  ofs << setw(colw) << "x"
      << setw(colw) << nx
      << setw(colw) << xmin
      << setw(colw) << xmax
      << endl;
  ofs << setw(colw) << "q2"
      << setw(colw) << nq2
      << setw(colw) << q2min
      << setw(colw) << q2max
      << endl;
  ofs << "####################################################" << endl;
  
  ofs << setprecision( 5 ) << flush;
  ofs << setw( colw ) << "";
  for( int ix = 0; ix < x.size(); ix++ ){
    ofs << setw( colw ) << x[ ix ] << flush;
  }
  ofs << endl;
  
  for( int iq2 = 0; iq2 < q2.size(); iq2++ ){
    ofs << setw( colw ) << q2[ iq2 ] << flush;
    for( int ix = 0; ix < x.size(); ix++ ){
      double w2 = xsec.W2( x[ ix ] );
      ofs << setw( colw )
	  << ( w2 > 4.0 ? xsec( w2, q2[ iq2 ] ) : 0.0 ) << flush;
    }
    ofs << endl;
  }
}


int main( int argc, char* argv[] ){
  
  Utility::Arguments& args = Utility::Arguments::ref( argc, argv );
  if( args.hasOpt( "help" ) || args.hasOpt( "?" ) ){
    cout << "Possible options" << endl;
    args.usage( "Q2min",    0.1,   "Lower edge of Q2 region" );
    args.usage( "Q2max", 1000.0, "Upper edge of Q2 region" );
    args.usage( "nQ2",    400, "sampling size for Q2" );
    return 1;
  }

  StrFncParam::ALLM91 al91;
  StrFncParam::ALLM97 al97;
  StrFncParam::GD07P  gd07p;
  StrFncParam::GD11P  gd11p;

  calc( gd11p );
  
  
  /*
    
  cout 
    << setw(10) << "W^2"
    << setw(5) << "Q^2"
    << setw(12) << "ALLM91"
    << setw(12) << "ALLM97"
    << setw(12) << "DL2007"
    << setw(12) << "x"
    << setw(12) << "F1"
    << setw(12) << "R1990"
    << setw(12) << "R1998"
    << setw(12) << "F2"
    << setw(12) << "F2pSMC"
    << setw(12) << "F2nSMC"
    << setw(12) << "F2dSMC"
    << endl;
  
  for( double lw2 = 1.0; lw2 < 5.0; lw2 += 0.1 ){ 

    double w2 = pow( 10.0, lw2 );
    double x  = al91.x( w2 );
    
    cout 
      << setw(10) << w2
      << setw(5)  << q2
      << setw(12) << al91( w2, q2 ) 
      << setw(12) << al97( w2, q2 ) 
      << setw(12) << dl07( w2, q2 ) 
      << setw(12) << x
      << setw(12) << f1al91( x, q2 )
      << setw(12) << r1990( x, q2 )
      << setw(12) << r1998( x, q2 )
      << setw(12) << f2al91( x, q2 )
      << setw(12) << f2psmc( x, q2 )
      << setw(12) << f2nsmc( x, q2 )
      << setw(12) << f2dsmc( x, q2 )
      << endl;
    
  }
  
  
  */

  return 0;
}
