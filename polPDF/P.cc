#include "P.hh"
#include <Evolution/Kernel.hh>

#include <iostream>
#include <iomanip>

using namespace std;
using namespace Utility;
using namespace QCD;
using namespace Evolution;
using namespace polPDF;

P::P( Kernel *k ) : AnomalousDimension( k )
{
  // 
  name_ = "polPDF";
  
  // Anomalous Dimension
  p_[ 0 ][ Pqq ]  = new P::QQLO( this );
  p_[ 0 ][ Pgq ]  = new P::GQLO( this );
  p_[ 0 ][ Pqg ]  = new P::QGLO( this );
  p_[ 0 ][ Pgg ]  = new P::GGLO( this );
  
  p_[ 1 ][ Pqq ] = new P::QQNLO( this );
  p_[ 1 ][ Pgq ] = new P::GQNLO( this );
  p_[ 1 ][ Pqg ] = new P::QGNLO( this );
  p_[ 1 ][ Pgg ] = new P::GGNLO( this );
  
  // for NS evolution, + and - must be swapped for pol DIS case
  p_[ 0 ][ Pnsp ] = new P::QQLO( this );
  p_[ 0 ][ Pnsm ] = new P::QQLO( this );
  
  p_[ 1 ][ Pnsp ] = new P::NSMNLO( this );
  p_[ 1 ][ Pnsm ] = new P::NSPNLO( this );
  
  qqps_  = new P::QQPS( this );
  nsnlo_ = new P::NSNLO( this );
  
}

P::~P() {
  for( int i = 0; i < p_.size(); i++ )
    for( int j = 0; j < p_[ i ].size(); j++ )
      if( p_[ i ][ j ] != NULL ) delete p_[ i ][ j ];

  delete qqps_;
  delete nsnlo_;
}

// ------------------------ LO ------------------------------ //
complex< double > P::QQLO::operator() ( const complex< double >& n ) {
  return Kernel::CF2 * ( 4.0 * s_( 1, n ) -3.0 -2.0 / n / ( n + 1.0 ) );
}

complex< double > P::QGLO::operator() ( const complex< double >& n ) {
  return - 8.0 * p_->TF * ( n - 1.0 ) / n / ( n + 1.0 );
}

complex< double > P::GQLO::operator() ( const complex< double >& n ) {
  return - Kernel::CF4 * ( n + 2.0 ) / n / ( n + 1.0 );
}

complex< double > P::GGLO::operator() ( const complex< double >& n ) {
  static double c11_3 = 11.0 / 3.0;
  static double c8_3  =  8.0 / 3.0;
  return 
    Kernel::CA2 * ( 4.0 * s_( 1, n ) - c11_3 - 8.0 / n / ( n + 1.0 ) ) 
    + c8_3 * p_->TF;
}

// ------------------------ NLO ------------------------------ //
complex< double > P::QQPS::operator() ( const complex< double >& n ) {
  complex< double > n1 = n + 1.0;

  
  return 
    // PRD53,4775
    Kernel::CF16 * p_->TF 
    * ( pow( n, 4 ) 
	+ 2.0 * pow( n, 3 ) 
	+ 2.0 * pow( n, 2 ) 
	+ 5.0 * n + 2.0 ) 
    / pow( n, 3 ) 
    / pow( n + 1.0, 3 );
  
// NPB465,267
  /* 
     Kernel::CF16 * p_->TF * 
     ( 2.0 / pow( n * n1, 3 ) + 3.0 / pow( n1, -2 ) + 1.0 / n1 
     + 2.0 / pow( n1, 3 ) - pow( n + 1.0, -2 ) - 1.0 / n );
  */
}

complex< double > P::QQNLO::operator() ( const complex< double >& n ) {
  // this is actully NS- + PS
  return (*(p_->nsnlo_))( n, -1.0 ) + (*(p_->qqps_))( n );
}

complex< double > P::QGNLO::operator() ( const complex< double >& n ) {
  
  complex< double > n2  = n  * n;
  complex< double > n3  = n2 * n;
  complex< double > n4  = n3 * n;
  complex< double > n5  = n4 * n;
  
  complex< double > np1 = n + 1.0;
  complex< double > np13 = pow( np1, 3.0 );
  complex< double > n3np13 = n3 * np13;
  
  complex< double > nm1 = n - 1.0;
  complex< double > s1  = s_( 1, n );
  complex< double > s12 = s1 * s1;
  complex< double > s2  = s_( 2, n );
  complex< double > s2p = sp_( n / 2.0 ); 
  complex< double > A   = nm1 / n / np1;
  
  return 
    // PRD53,4775
    Kernel::CA16 * p_->TF
    * ( A * ( - s2 + sp_( 2, n / 2.0, -1 ) + s12 )
	- 4.0 / n / pow( np1, 2 ) * s1
	- ( n5 + n4 - 4.0 * n3 + 3.0 * n2 - 7.0 * n - 2.0 ) / n3np13 )
    + Kernel::CF8 * p_->TF 
    * ( 2.0 * A * ( s2 - s12 )
	+ 4.0 * A / n * s1
	- ( 5.0 * n5 + 5.0 * n4 - 10.0 * n3 - n2 + 3.0 * n - 2.0 ) / n3np13 )
    ;
  
    // NPB465,267
    /*
    Kernel::CA16 * p_->TF * 
    ( ( S1p2[1] + S1[2] + 2.0 * Stildem1[2] ) * ( 2.0 * N1INV - NINV ) 
      + S1[1] * ( 4.0 * N1INV2 - 2.0 * NINV2 ) 
      - 4.0 * NINV  + 3.0 * N1INV  - 3.0 * NINV2 + 8.0 * N1INV2 + 2.0 * NINV3
      + 12.0 * N1INV3
      ) 
    +
    Kernel::CF8 * p_->TF * 
    (  ( S1p2[1] - S1[2] ) * ( 2.0 * NINV - 4.0 * N1INV )
       + 14.0 * NINV - 19.0 * N1INV 
       - NINV2 - 8.0 * N1INV2 
       - 2.0 *NINV3 + 4.0 * N1INV3
       );
    */
}

complex< double > P::GQNLO::operator() ( const complex< double >& n ) {
  
  // PRD53,4775
  complex< double > n2  = n  * n;
  complex< double > n3  = n2 * n;
  complex< double > n4  = n3 * n;
  complex< double > n5  = n4 * n;
  
  complex< double > np1 = n + 1.0;
  complex< double > np12 = np1 * np1;
  complex< double > nnp12 = n * np12;

  complex< double > nhalf = n / 2.0;

  complex< double > A = ( n + 2.0 ) / n / np1;

  complex< double > s1 = s_( 1, n );
  complex< double > s12 = s1 * s1;
  complex< double > s2 = s_( 2, n );

  complex< double > sp2 = sp_( 2, nhalf, -1 );
  
  return 
    Kernel::CF32 * p_->TF *
    ( - A / 3.0 * s_( 1, n )
      + ( 5.0 * n2 + 12.0 * n + 4.0 ) / 9.0 / nnp12
      )
    + Kernel::CF2_2 * 
    ( 2.0 * A * ( s2 + pow( s1, 2 ) )
      -2.0 * ( 3.0 * n2 + 7.0 * n + 2.0 ) / nnp12  * s1
      + ( 9.0 * n5 + 30.0 * n4 + 24.0 * n3 - 7.0 * n2 - 16.0 * n - 4.0 )
      / n3 / pow( np1, 3 )
      )
    + Kernel::CACF8 *
    ( A * ( -s2 + sp2 - s12 )
      + ( 11.0 * n2 + 22.0 * n + 12.0 ) / 3.0 / n2 / np1 * s1
      - ( 76.0 * n5 + 241.0 * n4 + 254.0 * n3 + 41.0 * n2 + 72.0 * n + 36.0 )
      / 9.0 / n3 / pow( np1, 3 )
      )
    ;
  

  // NPB465,267
  /*
  static double c56_9 = 56.0 / 9.0;
  static double c20_9 = 20.0 / 9.0;
  static double c7_9  =  7.0 / 9.0;
  static double c2_9  =  2.0 / 9.0;
  
  static double c38_3 = 38.0 / 3.0;
  static double c28_3 = 28.0 / 3.0;
  static double c16_3 = 16.0 / 3.0;
  static double c5_3  =  5.0 / 3.0;
  static double c2_3  =  2.0 / 3.0;
  static double c1_3  =  1.0 / 3.0;
  
  return 
    Kernel::CACF8 *
    ( ( S1p2[1] - S1[2] - 2.0 * Stildem1[2] ) * ( N1INV - 2.0 * NINV )
      + S1[1] * ( c16_3 * NINV - c5_3 * N1INV )
      - c56_9 * NINV  - c20_9 * N1INV  
      + c28_3 * NINV2 - c38_3 * N1INV2
      - 4.0 * NINV3 - 6.0 * N1INV3 )
    
    + Kernel::CF2_2 * 
    ( ( S1p2[1] + S1[2] ) * ( 4.0 * NINV - 2.0 * N1INV )    
      + S1[1] * ( 8.0 * NINV2 - 4.0 * N1INV2 + 4.0 * NINV - 2.0 * N1INV )
      + 15.0 * NINV - 6.0 * N1INV 
      -12.0 * NINV2 + 3.0 * N1INV2 
      + 4.0 * NINV3 - 2.0 * N1INV3 )

    + Kernel::CF32 * p_->TF *
    ( S1[1] * ( c1_3 * N1INV - c2_3 * NINV ) 
      + c7_9 * NINV - c2_9 * N1INV 
      - c2_3 * NINV2 + c1_3 * N1INV2 );
  */
}

complex< double > P::GGNLO::operator() ( const complex< double >& n ) {
  
  // PRD53,4775
  complex< double > n2  = n  * n;
  complex< double > n3  = n2 * n;
  complex< double > n4  = n3 * n;
  complex< double > n5  = n4 * n;
  complex< double > n6  = n5 * n;
  
  complex< double > np1 = n + 1.0;
  complex< double > np12 = np1 * np1;
  complex< double > nnp1 = n * np1;
  complex< double > n2np12 = nnp1 * nnp1;
  complex< double > n3np13 = nnp1 * n2np12;
  
  complex< double > nhalf = n / 2.0;

  complex< double > A = ( n + 2.0 ) / n / np1;

  complex< double > s1 = s_( 1, n );
  complex< double > s12 = s1 * s1;
  complex< double > s2 = s_( 2, n );
  
  complex< double > sp2 = sp_( 2, nhalf, -1 );

  return
    
    + Kernel::CF8 * p_->TF *
    ( n6 + 3.0 * n5 + 5.0 * n4 + n3 -8.0 * n2 + 2.0 * n + 4.0 )
    / n3 / pow( np1, 3 ) 
    
    + Kernel::CA32 * p_->TF * 
    ( - 5.0 / 9.0 * s1 
      + ( 3.0 * n4 + 6.0 * n3 + 16.0 * n2 + 13.0 * n - 3.0 )
      / 9.0 / n2np12 )
    
    + Kernel::CA2_2 *
    ( - sp_( 3, nhalf, -1 ) 
      - 4.0 * s1 * sp2
      + 8.0 * st_( n, -1 ) 
      + 8.0 / nnp1 * sp2
      + 2.0 * ( 67.0 * n4 + 134.0 * n3 + 67.0 * n2 + 144.0 * n + 72.0 )
      / 9.0 / n2np12 * s1
      - ( 48.0 * n6 + 144.0 * n5 + 469.0 * n4 + 698.0 * n3 + 7.0 * n2
	  + 258.0 * n + 144.0 ) / 9.0 / n3np13 
      )
    ;

  // NPB465,267
  /*
  static double c241_9 = 241.0 / 9.0;
  static double c134_9 = 134.0 / 9.0;
  static double c107_9 = 107.0 / 9.0;
  static double c19_9  =  19.0 / 9.0;
  static double c14_9  =  14.0 / 9.0;
  static double c5_9   =   5.0 / 9.0;
  
  static double c86_3 = 86.0 / 3.0;
  static double c58_3 = 58.0 / 3.0;
  static double c16_3 = 16.0 / 3.0;
  
  return
    Kernel::CA2_2 * 
    ( S1[1] * ( c134_9 + 8.0 * NINV2 - 16.0 * N1INV2 )
      + S1[2] * ( 8.0 * NINV - 16.0 * N1INV ) 
      + 4.0 * S1[3]
      - 8.0 * S1_2p2_1
      + 8.0 * Stildem1[2] * ( NINV - 2.0 * N1INV )
      + 4.0 * Stildem1[3]
      - 8.0 * Stilde1_2m
      - c107_9 * NINV + c241_9 * N1INV 
      + c58_3 * NINV2 - c86_3 * N1INV2
      - 8.0 * NINV3 - 48.0 * N1INV3 
      - c16_3
      )
    
    + Kernel::CA32 * p_->TF *
    ( - c5_9 * S1[1] 
      + c14_9 * NINV - c19_9 * N1INV 
      + ( - NINV2 - N1INV2 + 1.0 ) / 3.0 )
    
    + Kernel::CF8  * p_->TF *
    ( -10.0 * N1INV + 2.0 * N1INV2 + 4.0 * N1INV3 
      + 1.0 + 
      10.0 *  NINV - 10.0 * NINV2  + 4.0 * NINV3 )
    ;
  */
}

complex< double > P::NSPNLO::operator() ( const complex< double >& z ) {
  return (*(p_->nsnlo_))( z, 1.0 );
}

complex< double > P::NSMNLO::operator() ( const complex< double >& z ) {
  return (*(p_->nsnlo_))( z, -1.0 );
}

complex< double > P::NSNLO::operator() ( const complex< double >& n,
					 const double& eta ) {
  
  static double c536_9 = 536.0 / 9.0;
  static double c160_9 = 160.0 / 9.0;
  static double c16_9  =  16.0 / 9.0;
  static double c4_9   =   4.0 / 9.0;
  
  static double c88_3 = 88.0 / 3.0;
  static double c32_3 = 32.0 / 3.0;
  static double c17_3 = 17.0 / 3.0;
  static double c4_3  =  4.0 / 3.0;
  
  complex< double > n2  = n  * n;
  complex< double > n3  = n2 * n;
  complex< double > n4  = n3 * n;
  
  complex< double > np1   = n + 1.0;
  complex< double > nnp1  = n * np1;
  complex< double > nnp12 = nnp1 * nnp1;
  complex< double > nnp13 = nnp12 * nnp1;
  
  complex< double > s1  = s_( 1, n );
  complex< double > s2  = s_( 2, n );
  
  complex< double > sp2 = sp_( 2, n / 2.0, ( eta > 0 ? 1 : -1 ) );
  complex< double > sp3 = sp_( 3, n / 2.0, ( eta > 0 ? 1 : -1 ) );
  complex< double > st  = st_( n, ( eta > 0 ? 1 : -1 ) );
  
  complex< double > c1 = 2.0 * n + 1.0;
  complex< double > c2 = 2.0 * s1 - 1.0 / nnp1;
  complex< double > c3 = eta * ( 2.0 * n2 + c1 ) / nnp13;
  
  complex< double > A =    
    Kernel::CF_2 *
    ( 16.0 * s1 * c1 / nnp12
      + 16.0 * c2 * ( s2 - sp2 )
      + 24.0 * s2 + 64.0 * st - 8.0 * sp3 - 3.0
      - 8.0 * ( 3.0 * n3 + n2 - 1.0 ) / nnp13 
      -16.0 * c3 );
  
  complex< double > B =
    Kernel::CACF *
    ( c536_9 * s1 
      - 8.0 * c2 * ( 2.0 * s2 - sp2 )
      - c88_3 * s2 - 32.0 * st + 4.0 * sp3 - c17_3
      - c4_9 * ( 151.0 * n4 + 236.0 * n3 + 88.0 * n2 + 3.0 * n + 18.0 ) / nnp13
      + 8.0 * c3 );
  
  complex< double > C =
    Kernel::CF * p_->TF *
    ( - c160_9 * s1 + c32_3 * s2 + c4_3 + 
      c16_9 * ( 11.0 * n2 + 5.0 * n - 3.0 ) / nnp12 )
    ;
  
  return A + B + C;
  
}

