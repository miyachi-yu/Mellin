#include <iostream>
#include <iomanip>
#include <cmath>

#include <Utility/Arguments.hh>
#include <PDFLIB/HKNS07.hh>
#include <Fragmentation/Evolution.hh>
#include <Evolution/PDFx.hh>

using namespace std;

int main( int argc, char* argv[] ){
  
  Utility::Arguments &args =  Utility::Arguments::ref( argc, argv );
  
  // check
  if( ! args.hasOpt( "FF" ) || args.get( "FF", "" ) != "HKNS07_FF.xml" ){
    args.err() << "requires --FF=HKNS07_FF.xml" << endl;
    return 0;
  }
  
  Fragmentation::Evo 
    *ff = new Fragmentation::Evo( args, QCD::Flavor::PIp );
  
  double angle  = args.get( "angle",  0.50 );
  double length = args.get( "length", 18.0 );
  double offset = args.get( "offset", 2.0  );
  vector< int > dim( 2 );
  dim[ 0 ] = 4;
  dim[ 1 ] = 6;
  dim = args.get( "ndim", ",", dim );
  double prec = args.get( "precision", 0.001 );

  Evolution::PDFx UBAR( ff, QCD::Flavor::ubar, dim[ 0 ], dim[ 1 ], prec, length, offset, angle  );
  Evolution::PDFx D(    ff, QCD::Flavor::d,    dim[ 0 ], dim[ 1 ], prec, length, offset, angle  );
  Evolution::PDFx S(    ff, QCD::Flavor::s,    dim[ 0 ], dim[ 1 ], prec, length, offset, angle  );
  
  // default is pion NLO for positive charge
  PDFLIB::HKNS07 hkns07( args.get( "iset", 2 ), args.get( "icharge", 1 ) ); 
  
  int nq2       = args.get( "nq2", 10 );
  double q2min  = args.get( "q2min", 1.0 );
  double q2max  = args.get( "q2max", 100.0 );
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
    ff->setQ2( q2 );

    cout << "" << endl;
    cout << "Q2: " << setw(4) << q2 << " GeV^2" << endl;
    cout << setw(12) << "z|"
	 << setw(12) << ""
	 << setw(12) << "u-bar|"
	 << setw(12) << ""
	 << setw(12) << "d|"
	 << setw(12) << ""
	 << setw(12) << "s|"
	 << setw(12) << ""
	 << setw(12) << "u-bar/d|"
	 << setw(12) << ""
	 << setw(12) << "d/s|"
	 << endl;

    cout << setw(12) << "|"
	 << setw(12) << "Grid |"
	 << setw(12) << "Evo|"
	 << setw(12) << "Grid |"
	 << setw(12) << "Evo|"
	 << setw(12) << "Grid |"
	 << setw(12) << "Evo|"
	 << setw(12) << "Grid |"
	 << setw(12) << "Evo|"
	 << setw(12) << "Grid |"
	 << setw(12) << "Evo|"
	 << endl;

    cout << setw(12) << "----------+"
	 << setw(12) << "----------+"
	 << setw(12) << "----------+"
	 << setw(12) << "----------+"
	 << setw(12) << "----------+"
	 << setw(12) << "----------+"
	 << setw(12) << "----------+"
	 << setw(12) << "----------+"
	 << setw(12) << "----------+"
	 << setw(12) << "----------+"
	 << setw(12) << "----------+"
	 << endl;

    for( double iz = 0; iz < nz; iz++ ){
      
      double z = pow( 10.0, iz * dlz + lzmin );
      
      double u1, d1, s1;
      double u2, d2, s2;
      cout << setw(12) << z << flush;
      hkns07.setPID( QCD::Flavor::ubar );
      cout << setw(12) << ( u1 = z * hkns07( z ) ) << flush;
      cout << setw(12) << ( u2 = z * UBAR( z ) )   << flush;
      hkns07.setPID( QCD::Flavor::d );
      cout << setw(12) << ( d1 = z * hkns07( z ) ) << flush;
      cout << setw(12) << ( d2 = z * D( z ) )      << flush;
      hkns07.setPID( QCD::Flavor::s );
      cout << setw(12) << ( s1 = z * hkns07( z ) ) << flush;
      cout << setw(12) << ( s2 = z * S( z ) )      << flush;
      cout << setw(12) << ( d1 != 0.0 ? u1/d1 : 0.0 ) << flush;
      cout << setw(12) << ( d2 != 0.0 ? u2/d2 : 0.0 ) << flush;
      cout << setw(12) << ( s1 != 0.0 ? d1/s1 : 0.0 ) << flush;
      cout << setw(12) << ( s2 != 0.0 ? d2/s2 : 0.0 ) << flush;
      cout << endl;
    }
  }
  



  return 0;
}
