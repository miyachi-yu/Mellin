#include "XsecComp.hh"
#include "CoeffFunc.hh"
#include "Charge.hh"
#include <iostream>
#include <iomanip>

using namespace std;
using namespace Utility;
using namespace QCD;

int main( int argc, char* argv[] ){
  
  Arguments& args = *( Arguments::instance( argc, argv ) );
  
  try{ 
    
    QCD::Flavor::HADRON h = ( args.hasOpt( "FF" ) ? Flavor::PIp : Flavor::KAp );
    FragXsec_T::XsecComp xc( args, h );
    FragXsec_T::Charge& e = 
      dynamic_cast< FragXsec_T::Charge& >( *( xc.coeff()->charge() ) );
    
    for( double q2 = 1.0 ; q2 <= 10000 ; q2++ ){
      xc.setQ2( q2 );
      
      cout << setw(5) << q2 << flush;
      
    for( int i = 0; i <= 2; i++ ) {
      double ch = e( Flavor::PARTON( i ) );
      //double ch = e.bare( Flavor::PARTON( i ) );
      cout << setw(12) << ch << flush;
    }
    cout << endl;
    }  

  }

  catch ( int e ) {
    cerr << e << endl;
  }

  return 0;
}
