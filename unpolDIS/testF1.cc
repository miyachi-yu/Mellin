/*!
  \file  testF1.cc
  \brief test program for polarized structure functions

  This is a test and an example code for polarized structure functions
  implemented in libpolDIS.so.

  You may get some feeling how to use polDIS::StructureFunction class.

 */
#include <Utility/Timer.hh>

#include <StrFncParam/ALLM97.hh>
#include <StrFncParam/F1Param.hh>
#include <StrFncParam/F2pSMC.hh>
#include <StrFncParam/F2nSMC.hh>
#include <StrFncParam/R1998.hh>

#include <Xsec/xXsec.hh>

#include <iostream>
#include <iomanip>
#include <vector>

#include "StrFunc.hh"

using namespace std;
using namespace Utility;


int main( int argc, char* argv[] ) {
  
  Arguments& args = *( Arguments::instance( argc, argv ) );
  
  // use some parameterization as a reference
  StrFncParam::ALLM97  al97;
  StrFncParam::F2pSMC  f2p;
  StrFncParam::F2nSMC  f2n;
  StrFncParam::R1998   r1998;
  StrFncParam::F1Param f1a( al97 );          // reference 1
  StrFncParam::F1Param f1pb( f2p, r1998 );   // reference 2
  StrFncParam::F1Param f1nb( f2n, r1998 );   // reference 2
  
  try {
    
    Timer timer;

    // prepare q2 values
    vector< double > q2;
    //    q2.push_back( 1.0 );    // lower edge of the pol DIS data
    q2.push_back( 1.3 );    // CTEQ6 initial scale
    q2.push_back( 2.5 );    // HERMES average
    //    q2.push_back( 2.0 ); 
    q2.push_back( 5.0 );    // average value of pol DIS
    q2.push_back( 10.0 );   // average value of SMC/COMPASS
    q2.push_back( 20.0 );   // large Q2 sample in the fixed target exp
    q2.push_back( 50.0 );   // just see how PDF changes in the large Q2 
    q2.push_back( 100.0 );  //      region....
    q2.push_back( 200.0 );
    q2.push_back( 500.0 );
    
    // --------------------------------------------------------------
    //                       new implementation
    // --------------------------------------------------------------
    unpolDIS::StrFunc f1p( args, Xsection::Charge::p );
    Xsection::xXsec f1px( &f1p );                  // MellinInversion
    f1px.setParameter( 2.0, 0.50 );
    f1px.upper( 22.0 );
    
    unpolDIS::StrFunc f1n( args, Xsection::Charge::n );
    Xsection::xXsec f1nx( &f1n );                  // MellinInversion
    f1nx.setParameter( 2.0, 0.50 );
    f1nx.upper( 22.0 );

    // --------------------------------------------------------------
    //     performance in moment calculation
    // --------------------------------------------------------------
    complex< double > n( 2.0, 0.0 );
    if( args.hasOpt( "CTEQ6" ) )
      // calculate 2nd moment in case of CTEQ6
      n = complex< double >( 2.0, 0.0 );
    
    // x values
    vector< double > x;
    for( double lx = -2.0; lx < 0.0; lx += 0.2 )
      x.push_back( pow( 10.0, lx ) );
    
    cout << setw( 4 )  << "Q^2" 
	 << "(N=" 
	 << setw( 11 ) 
	 << n.real() << ")" ;
    
    for( int ix = 0; ix < x.size(); ix++ ) 
      cout << setw( 12 ) << x[ ix ] << flush;
    
    cout << endl;
    
    for( int iq2 = 0; iq2 < q2.size(); iq2++ ){
      
      cout << "-----------------------------------------------------"
	   << endl;
      
      // --------------------------------------------------------- //
      //    value from parameterization                            //
      // --------------------------------------------------------- //
      {
	cout << setw(4) << q2[ iq2 ] << setw(15) << "F1p No.1" << flush;
	for( int ix = 0; ix < x.size(); ix++ ) 
	  cout << setw( 12 ) << f1a( x[ ix ], q2[ iq2 ] ) << flush;
	cout << endl;
      }
      
      {
	cout << setw(4) << q2[ iq2 ] << setw(15) << "F1p No.2" << flush;
	for( int ix = 0; ix < x.size(); ix++ ) 
	  cout << setw( 12 ) << f1pb( x[ ix ], q2[ iq2 ] ) << flush;
	cout << endl;
      }
      
      // --------------------------------------------------------- //
      //    performance in x space :  new implementation
      // --------------------------------------------------------- //
      {
	f1p.setQ2( q2[ iq2 ] );
	// 1st moment
	cout << setw(4) << q2[ iq2 ] 
	  //	     << setw(15) << f1p( n ) 
	     << setw(15) << ""
	     << flush;
	timer.start();
	// at the moment, xSec returns 2.0 * F_1
	for( int ix = 0; ix < x.size(); ix++ ) 
	  cout << setw( 12 ) << 0.5 * f1px( x[ ix ] ) << flush;
	timer.stop();
	cout << setw(10) << timer.check( Timer::Sec ) << " sec" << endl;;
      }

      cout << "-" << endl;
      // --------------------------------------------------------- //
      //    value from parameterization                            //
      // --------------------------------------------------------- //
      {
	cout << setw(4) << q2[ iq2 ] << setw(15) << "F1n No.2" << flush;
	for( int ix = 0; ix < x.size(); ix++ ) 
	  cout << setw( 12 ) << f1nb( x[ ix ], q2[ iq2 ] ) << flush;
	cout << endl;
      }
      
      // --------------------------------------------------------- //
      //    performance in x space :  new implementation
      // --------------------------------------------------------- //
      {
	f1n.setQ2( q2[ iq2 ] );
	// 1st moment
	cout << setw(4) << q2[ iq2 ] 
	  //	     << setw(15) << f1n( n ) 
	     << setw(15) << ""
	     << flush;
	timer.start();
	// at the moment, xSec returns 2.0 * F_1
	for( int ix = 0; ix < x.size(); ix++ ) 
	  cout << setw( 12 ) << 0.5 * f1nx( x[ ix ] ) << flush;
	timer.stop();
	cout << setw(10) << timer.check( Timer::Sec ) << " sec" << endl;;
      }
      
    }
  }
  
  catch ( int error ) {
  }
  
  return 0;
}
