#include <Evolution/PDFx.hh>
#include <QCD/Flavor.hh>
#include <Utility/Arguments.hh>
#include <polPDF/Evolution.hh>
#include <Fragmentation/Evolution.hh>
#include <ErrorCalc/ErrCalc.hh>

#include <cmath>
#include <iostream>
#include <iomanip>
#include <fstream>

using namespace std;
using namespace Utility;
using namespace Evolution;
using namespace QCD;
using namespace ErrorCalc;

int main( int argc, char* argv[] ){
  
  Arguments& args = *( Arguments::instance( argc, argv ) );

  try{
    Fragmentation::Evo evo( args );

    ErrCalc ecal( &evo );
    evo.setQ2( 10.0 );

    ecal.setPID( Flavor::PARTON( 0 ) );
    double x( 0.5 );
    ecal( x ); // square of the error of delta Dg //
    cout << x * sqrt( ecal( x ) ) << endl;

    int nt=100, nx=100;
    double xmin=1.0e-3, xmax=1.0, dx=(log10(xmax)-log10(xmin))/nx;
    double q2minNLO=0.4, q2max=10000.0; //, dlam0=0.248;
    double q2minLO=0.26, q2min;//q2max=200.0; //, dlam0=0.248;
//    double tmin=log(q2min/pow(dlam0,2)), tmax=log(q2max/pow(dlam0,2));
    ((args.hasOpt( "order" ) && (args.getOpt( "order" ) == "LO")) ?
	q2min=q2minLO : q2min=q2minNLO);
    double tmin=log(q2min), tmax=log(q2max), dt=(tmax-tmin)/nt;

    Flavor flav;
    int i = 1;
    const int ii = Flavor::PARTON( i );
    cout << - Flavor::ubar + 5 << " " << Flavor::PARTON( i ) + 2 << " "
	 << flav.id( flav.name( Flavor::PARTON( i ) ) )<< endl;
    cout << ii + 8.0 << endl;

    return 0;

    for( int kk = 0; kk <= nt; kk+= 1){

    double qq2 = exp(tmin+dt*kk);
    evo.setQ2( qq2 );
    cout << qq2 << setw( 8 ) << evo.anomalousDimension()->nf() << endl;

    }

  }


  catch( int err ){
  }

  return 0;
}
