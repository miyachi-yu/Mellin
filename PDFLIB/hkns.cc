#include "HKNS07.hh"
#include <Utility/Arguments.hh>
#include <iostream>
#include <iomanip>
#include <cmath>

using namespace std;

int main( int argc, char* argv[] ){
  
  Utility::Arguments &args =  Utility::Arguments::ref( argc, argv );
  
  // default is pion NLO for positive charge
  PDFLIB::HKNS07 hkns07( args.get( "iset", 2 ),
			 args.get( "icharge", 1 ) ); 
  
  int nq2 = args.get( "nq2", 10 );
  double q2min = args.get( "q2min", 1.0 );
  double q2max = args.get( "q2max", 10.0 );
  double lq2min = log10( q2min );
  double lq2max = log10( q2max );
  double dlq2   = ( lq2max - lq2min ) / nq2;

  int nz = args.get( "nz", 10 );
  double zmin = args.get( "zmin", 0.01 );
  double zmax = args.get( "zmax", 1.0 );
  double lzmin = log10( zmin );
  double lzmax = log10( zmax );
  double dlz   = ( lzmax - lzmin ) / nz;

  for( double iq2 = 0; iq2 < nq2; iq2++ ){
    
    double q2 = pow( 10.0, iq2 * dlq2 + lq2min );
    
    hkns07.setQ2( q2 );
    cout << "------------------------------------------------------" << endl;
    cout << "Q2: " << setw(4) << q2 << " GeV^2" << endl;
    cout << setw(12) << "z"
	 << setw(12) << "u-bar"
	 << setw(12) << "d"
	 << setw(12) << "s"
	 << setw(12) << "u-bar/d"
	 << setw(12) << "d/s"
	 << endl;
    cout << setw(12) << "-----------"
	 << setw(12) << "-----------"
	 << setw(12) << "-----------"
	 << setw(12) << "-----------"
	 << setw(12) << "-----------"
	 << setw(12) << "-----------"
	 << endl;

    for( double iz = 0; iz < nz; iz++ ){
      
      double z = pow( 10.0, iz * dlz + lzmin );
      
      double u, d, s;
      cout << setw(12) << z << flush;
      hkns07.setPID( QCD::Flavor::ubar );
      cout << setw(12) << ( u = z * hkns07( z ) ) << flush;
      hkns07.setPID( QCD::Flavor::d );
      cout << setw(12) << ( d = z * hkns07( z ) ) << flush;
      hkns07.setPID( QCD::Flavor::s );
      cout << setw(12) << ( s = z * hkns07( z ) ) << flush;
      cout << setw(12) << ( d != 0.0 ? u/d : 0.0 ) << flush;
      cout << setw(12) << ( s != 0.0 ? d/s : 0.0 ) << flush;
      cout << endl;
    }
  }
  



  return 0;
}
