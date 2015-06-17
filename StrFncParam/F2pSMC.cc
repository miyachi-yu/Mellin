#include "F2pSMC.hh"
#include <cmath>

using namespace std;
using namespace StrFncParam;

F2pSMC::F2pSMC() :
  F2ParamBase(),
  lm2( 0.25 * 0.25 ),
  q02( 20.0 ),
  a( 7, 0.0 ),  b( 4, 0.0 ),  c( 4, 0.0 ),
  au( 7, 0.0 ), bu( 4, 0.0 ), cu( 4, 0.0 ),
  ad( 7, 0.0 ), bd( 4, 0.0 ), cd( 4, 0.0 ) 
{

  a[0] =  -0.24997;
  a[1] =   2.3963;
  a[2] =   0.22894;
  a[3] =   0.08498;
  a[4] =   3.8608;
  a[5] =  -7.4143;
  a[6] =   3.4342;

  b[0] =   0.11411;
  b[1] =  -2.2356;
  b[2] =   0.03115;
  b[3] =   0.02135;

  c[0] =  -1.4517;
  c[1] =   8.4745;
  c[2] = -34.379;
  c[3] =  45.888;

  au[0] = -0.24810;
  au[1] =  2.3632;
  au[2] =  0.23643;
  au[3] = -0.03241;
  au[4] =  4.2268;
  au[5] = -7.8120;
  au[6] =  3.5822;

  bu[0] =  0.09734;
  bu[1] = -2.2254;
  bu[2] =  0.03239;
  bu[3] =  0.02233;

  cu[0] = -1.4361;
  cu[1] =  8.1084;
  cu[2] =-33.306;
  cu[3] = 44.717;

  ad[0] = -0.25196;
  ad[1] =  2.4297;
  ad[2] =  0.21913;
  ad[3] =  0.21630;
  ad[4] =  3.4645;
  ad[5] = -6.9887;
  ad[6] =  3.2771;

  bd[0] =  0.13074;
  bd[1] = -2.2465;
  bd[2] =  0.02995;
  bd[3] =  0.02039;

  cd[0] = -1.4715;
  cd[1] =  8.9108;
  cd[2] =-35.714;
  cd[3] = 47.338;

}

F2pSMC::~F2pSMC() {
}

double F2pSMC::eval( const double& x ){
  return 
    A(x) 
    * pow( log( q2_ / lm2 ) / log( q02 / lm2 ), B(x) )
    * ( 1.0 + C(x) / q2_ ) ;
}

double F2pSMC::A( const double& x ){
  double x1 = 1.0 - x;
  return
    pow( x, a[0] ) * pow( x1, a[1] ) * 
    ( a[2] 
      + a[3] * x1 
      + a[4] * x1 * x1 +
      + a[5] * pow( x1, 3 ) 
      + a[6] * pow( x1, 4 ) );
}

double F2pSMC::B( const double& x ){
  return b[0] + b[1] * x + b[2] / ( x + b[3] );
}

double F2pSMC::C( const double& x ){
  return 
    c[0]   * x 
    + c[1] * x * x
    + c[2] * pow( x, 3 )
    + c[3] * pow( x, 4 );
}

