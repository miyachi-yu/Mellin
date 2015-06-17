//
// testSub.cc
//  
// Made by Yoshiyuki Miyachi
// Login   <miyachi@yoshi-desktop>
// 
// Started on  Thu Nov 20 11:05:34 2008 Yoshiyuki Miyachi
// Started on  Thu Nov 20 11:05:34 2008 Yoshiyuki Miyachi
//

#include <iostream>
#include <Xsec/xXsec.hh>
#include <Utility/Timer.hh>
#include <Tranform/GridIntegration.hh>
#include <Tranform/LegQuadrature.hh>

#include "StrFunc.hh"
#include "StrFuncSub.hh"

using namespace std;

void setngrid( Xsection::xXsec& xxsec, const vector< int >& ngrid ){
  if( ngrid.size() == 0 ) return;
  Transform::GridIntegration *grid =
    dynamic_cast< Transform::GridIntegration* >( xxsec.integration() );
  if( grid == NULL ) return;
  if( ngrid.size() == 1 ) grid->ngrid( ngrid[ 0 ] );
  grid->ngrid( ngrid[ 0 ], ngrid[ 1 ] );
}

void setnleg( Transform::Integration* integ, const vector< int >& leg ){
  if( leg.size() < 2 ) return;
  Transform::LegQuadrature* meth = dynamic_cast< Transform::LegQuadrature* >( integ );
  if( meth ) meth->setngrid( leg[0], leg[1] );
}

void setnleg( Xsection::xXsec& xxsec, const vector< int >& leg1, const vector< int >& leg2 ){
  Transform::GridIntegration *grid =
    dynamic_cast< Transform::GridIntegration* >( xxsec.integration() );
  if( grid == NULL ) return;
  setnleg( grid->method1(), leg1 );
  setnleg( grid->method2(), leg2 );
}

int main( int argc, char* argv[] ){
  
  Utility::Arguments& args = ARG_REF2( argc, argv );
  
  // ---------------------------------------------------------------- //
  //   Q2                                                             //
  // ---------------------------------------------------------------- //
  vector< double > q2( 0 );
  q2.push_back( 1.0 );
  q2 = args.get( "Q2", ",", q2 );
  
  // ---------------------------------------------------------------- //
  //   moment (N,M)                                                   //
  // ---------------------------------------------------------------- //
  complex< double > N( 4.0 ), M( 4.0 );
  N = args.get( "N", N );
  M = args.get( "M", M );
  
  // ---------------------------------------------------------------- //
  //   x and zfffff                                                   //
  // ---------------------------------------------------------------- //
  vector< double > vx( 1, 0.1 ), vz( 1, 0.2 );
  vx = args.get( "x", ",", vx );
  vz = args.get( "z", ",", vz );

  // ---------------------------------------------------------------- //
  //   contour setting                                                //
  // ---------------------------------------------------------------- //
  vector< double > off0( 2, 0.0 ), off1( 2, 0.0 ), off2( 2, 0.0 ), off( 2, 0.0 );
  vector< double > ang0( 2, 0.0 ), ang1( 2, 0.0 ), ang2( 2, 0.0 ), ang( 2, 0.0 );
  vector< double > len0( 2, 0.0 ), len1( 2, 0.0 ), len2( 2, 0.0 ), len( 2, 0.0 );
  
  off[ 0 ] = 3.6; off[ 1 ] = 3.0;
  ang[ 0 ] = 0.6; ang[ 1 ] = 0.6;
  len[ 0 ] = 9.0; len[ 1 ] = 8.0;
  
  off = args.get( "offset", ",", off );
  ang = args.get( "angle",  ",", ang );
  len = args.get( "length", ",", len );

  len0 = len; len1 = len; len2 = len;
  off0 = off; off1 = off; off2 = off;
  ang0 = ang; ang1 = ang; ang2 = ang;

  off0[ 0 ] = 3.5;  off0[ 1 ] = 3.0;
  ang0[ 0 ] = 0.55; ang0[ 1 ] = 0.55;
  len0[ 0 ] = 8.0;  len0[ 1 ] = 12.0;
  
  off1[ 0 ] = 2.5;  off1[ 1 ] = 3.0;
  ang1[ 0 ] = 0.55; ang1[ 1 ] = 0.55;
  len1[ 0 ] = 6.0;  len1[ 1 ] = 12.0;
  
  off2[ 0 ] = 3.5;  off2[ 1 ] = 3.0;
  ang2[ 0 ] = 0.55; ang2[ 1 ] = 0.55;
  len2[ 0 ] = 6.0;  len2[ 1 ] = 12.0;
  
  off0 = args.get( "offset0", ",", off0 );
  ang0 = args.get( "angle0",  ",", ang0 );
  len0 = args.get( "length0", ",", len0 );

  off1 = args.get( "offset1", ",", off1 );
  ang1 = args.get( "angle1",  ",", ang1 );
  len1 = args.get( "length1", ",", len1 );
  
  off2 = args.get( "offset2", ",", off2 );
  ang2 = args.get( "angle2",  ",", ang2 );
  len2 = args.get( "length2", ",", len2 );
  
  // ---------------------------------------------------------------- //
  //   Mellin inversion                                               //
  // ---------------------------------------------------------------- //
  vector< int > leg1( 1, 6 );
  leg1 = args.get( "leg1", ",", leg1 );
  
  vector< int > leg2( 1, 8 );
  leg2 = args.get( "leg2", ",", leg2 );
  
  double prec = args.get( "precision", 2.0E-3 );
  
  // ---------------------------------------------------------------- //
  //   Mellin inversion                                               //
  // ---------------------------------------------------------------- //
  cout << args << endl;
  
  try {
    
    unpolSIDISXsec::StrFuncSub  xsec0( args, 0 );
    unpolSIDISXsec::StrFuncSub  xsec1( args, 1 );
    unpolSIDISXsec::StrFuncSub  xsec2( args, 2 );
    
    unpolSIDISXsec::StrFunc     xsec( args );
    
    Xsection::xXsec xxsec0( &xsec0, leg1[0], leg2[0], prec );
    Xsection::xXsec xxsec1( &xsec1, leg1[0], leg2[0], prec );
    Xsection::xXsec xxsec2( &xsec2, leg1[0], leg2[0], prec );
    Xsection::xXsec xxsec(  &xsec,  leg1[0], leg2[0], prec );
    
    xxsec0.setParameter( off0, ang0 );
    xxsec1.setParameter( off1, ang1 );
    xxsec2.setParameter( off2, ang2 );
    xxsec.setParameter( off, ang );
    
    // ngrid
    vector< int > ngrid( 2, 4 );
    ngrid = args.get( "ngrid", ",", ngrid );
    
    setngrid( xxsec0, ngrid );
    setngrid( xxsec1, ngrid );
    setngrid( xxsec2, ngrid );
    setngrid( xxsec,  ngrid );
    
    xxsec0.upper( len0 );
    xxsec1.upper( len1 );
    xxsec2.upper( len2 );
    xxsec.upper( len );
    
    leg1[0] = 8; leg1[1] = 4; leg2[0] = 10; leg2[1] = 6; setnleg( xxsec0, leg1, leg2 );
    leg1[0] = 6; leg1[1] = 4; leg2[0] = 8;  leg2[1] = 6; setnleg( xxsec1, leg1, leg2 );
    leg1[0] = 6; leg1[1] = 4; leg2[0] = 8;  leg2[1] = 6; setnleg( xxsec2, leg1, leg2 );
    //    setnleg( xxsec,  leg1, leg2 );
    
    if( args.hasOpt("xsecTest") ) { 
      cout 
	<< "// -------------------------------------------------------- //\n"
	<< "//                        Xsec Test                         //\n"
	<< "// -------------------------------------------------------- //"
	<< endl;

      cout 
	<< setw( 24 ) << "Sub0"
	<< setw( 24 ) << "Sub1"
	<< setw( 24 ) << "Sub2"
	<< setw( 24 ) << "Xsec"
	<< setw( 24 ) << "Diff"
	<< endl;
      
      for( int i = 0; i < q2.size(); i++ ){
	
	xsec0.setQ2( q2[ i ] );
	xsec1.setQ2( q2[ i ] );
	xsec2.setQ2( q2[ i ] );
	xsec.setQ2( q2[ i ] );
	
	complex< double > x0;
	complex< double > x1;
	complex< double > x2;
	complex< double > x;
	
	cout 
	  << setw(24) << ( x0 = xsec0( N, M ) ) << flush
	  << setw(24) << ( x1 = xsec1( N, M ) ) << flush
	  << setw(24) << ( x2 = xsec2( N, M ) ) << flush
	  << setw(24) << ( x  = xsec( N, M ) ) << flush
	  << setw(24) << x - x0 - x1 - x2
	  << endl;
	
      }
      
    }

    
    if( args.hasOpt( "xXsecTest" ) ) {
      cout 
	<< "// --------------------------------------------------------- //\n"
	<< "//                      xXsection test                       //\n"
	<< "// --------------------------------------------------------- //"
	<< endl;
      
      cout
	<< setw(16) << "Sub0"
	<< setw(16) << "Sub1"
	<< setw(16) << "Sub2"
	<< setw(16) << "Xsec"
	<< setw(16) << "Diff"
	<< endl;
      
      for( int i = 0; i < q2.size(); i++ ){
	
	xsec0.setQ2( q2[ i ] );
	xsec1.setQ2( q2[ i ] );
	xsec2.setQ2( q2[ i ] );
	xsec.setQ2( q2[ i ] );
	
	for( int ix = 0; ix < vx.size(); ix++ ){
	  for( int iz = 0; iz < vz.size(); iz++ ){
	    
	    double xx0;
	    double xx1;
	    double xx2;
	    double xx;
	    
	    cout << setw(16) << ( xx0 = xxsec0( vx[ ix ], vz[ iz ] ) ) << flush;
	    cout << setw(16) << ( xx1 = xxsec1( vx[ ix ], vz[ iz ] ) ) << flush;
	    cout << setw(16) << ( xx2 = xxsec2( vx[ ix ], vz[ iz ] ) ) << flush;
	    cout << setw(16) << ( xx  = xxsec( vx[ ix ], vz[ iz ]  ) ) << flush;
	    cout << setw(16) << xx - xx0 - xx1 - xx2 << endl;
	    
	  }
	}
      }
    }
    
      
    // for parameter optimization
    if( 1 )
    {
      Utility::Timer t;
      
      if( vx.size() == 1 && vz.size() == 1 ){
	vx.resize( args.get( "nx", 6 ) );
	double lxmin = args.get( "lxmin", -2.0 );
	double lxmax = args.get( "lxmax",  0.0 );
	for( int i = 0; i < vx.size(); i++ ){
	  double lx = lxmin + ( lxmax - lxmin ) / vx.size() * i;
	  vx[ i ] = pow( 10.0, lx );
	}
      }
      
      if( q2.size() == 1 ){
	q2[ 0 ] = 2.0;
	q2.push_back(   5.0 );
	q2.push_back(  10.0 );
	q2.push_back(  50.0 );
      }	q2.push_back( 100.0 );

      for( int i = 0; i < q2.size(); i++ ){
	
	xsec0.setQ2( q2[ i ] );
	xsec1.setQ2( q2[ i ] );
	xsec2.setQ2( q2[ i ] );
	xsec.setQ2( q2[ i ] );
	
	cout << setw(10) << "Q2: " << setw(10) << q2[ i ] << flush;
	
	for( int iz = 0; iz < vz.size(); iz++ ){
	  
	  cout << setw(10) << "z: " << setw(10) << vz[ iz ] << endl;
	  
	  vector< double > v0( vx.size(), 0.0 );
	  vector< double > v1( vx.size(), 0.0 );
	  vector< double > v2( vx.size(), 0.0 );
	  vector< double > v3( vx.size(), 0.0 );
	  
	  cout << setw(6)  << "x" << flush;
	  for( int ix = 0; ix < vx.size(); ix++ ) 
	    cout << setw(13) << vx[ ix ] << flush;
	  cout << endl;
	  
	  cout << setw(6) << "" << flush;
	  for( int ix = 0; ix < vx.size(); ix++ ) cout << setw(13) << "------------" << flush;
	  cout << endl;
	  
	  t.start();
	  cout << setw(6)  << "sub1" << flush;
	  for( int ix = 0; ix < vx.size(); ix++ ) 
	    cout << setw(13) << ( v0[ ix ] = xxsec0( vx[ ix ], vz[ iz ] ) ) << flush;
	  cout << setw(13) << t.check( Utility::Timer::Sec ) << " s"<< endl;
	  
	  t.start();
	  cout << setw(6) << "sub2" << flush;
	  for( int ix = 0; ix < vx.size(); ix++ ) 
	    cout << setw(13) << ( v1[ ix ] = xxsec1( vx[ ix ], vz[ iz ] ) ) << flush;
	  cout << setw(13) << t.check( Utility::Timer::Sec ) << " s"<< endl;
	  
	  t.start();
	  cout << setw(6) << "sub3" << flush;
	  for( int ix = 0; ix < vx.size(); ix++ )
	    cout << setw(13) << ( v2[ ix ] = xxsec2( vx[ ix ], vz[ iz ] ) ) << flush;
	  cout << setw(13) << t.check( Utility::Timer::Sec ) << " s"<< endl;
	  
	  t.start();
	  cout  << setw(6) << "full" << flush;
	  for( int ix = 0; ix < vx.size(); ix++ ) 
	    cout << setw(13) << ( v3[ ix ] = xxsec( vx[ ix ], vz[ iz ]  ) ) << flush;
	  cout << setw(13) << t.check( Utility::Timer::Sec ) << " s"<< endl;
	  
	  cout << setw(6) << "" << flush;
	  for( int ix = 0; ix < vx.size(); ix++ ) cout << setw(13) << "------------" << flush;
	  cout << endl;
	  
	  cout << setw(6) << "diff" << flush;
	  for( int ix = 0; ix < vx.size(); ix++ ) 
	    cout << setw(13) << ( v0[ ix ] + v1[ ix ] + v2[ ix ] - v3[ ix ] ) << flush;
	  cout << endl;
	  
	  cout << endl;
	}
      }
    }
    
  }

  catch ( int error ){
    ARG_ERR << "catch error (" << error << ")" << endl;
    return 1;
  }
  
  return 0;
}
