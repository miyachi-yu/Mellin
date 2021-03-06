#include "P.hh"
#include <Evolution/Kernel.hh>

#include <iostream>
#include <iomanip>

using namespace std;
using namespace Transform;
using namespace QCD;
using namespace Evolution;
using namespace Fragmentation;

P::P( Kernel *k ) : 
  AnomalousDimension( k )
{
  // 
  name_ = "Fragmentation";
  
  // Anomalous Dimension
  // GQ->QG, QG->GQ in timelike case
  p_[ 0 ][ Pqq ]  = new P::QQLO( this );
  p_[ 0 ][ Pgq ]  = new P::QGLO( this );
  p_[ 0 ][ Pqg ]  = new P::GQLO( this );
  p_[ 0 ][ Pgg ]  = new P::GGLO( this );
  
  p_[ 1 ][ Pqq ] = new P::QQNLO( this );
  p_[ 1 ][ Pgq ] = new P::QGNLO( this );
  p_[ 1 ][ Pqg ] = new P::GQNLO( this );
  p_[ 1 ][ Pgg ] = new P::GGNLO( this );
  
  // for NS evolution, + and - must be swapped for pol case
  // for NS evolution, + and - must not be swapped for unpol case
  p_[ 0 ][ Pnsp ] = new P::QQLO( this );
  p_[ 0 ][ Pnsm ] = new P::QQLO( this );
  
  p_[ 1 ][ Pnsp ] = new P::NSPNLO( this );
  p_[ 1 ][ Pnsm ] = new P::NSMNLO( this );
  
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

complex< double > P::QQLO::operator() ( const complex< double >& n ) {
  return Kernel::CF2 * ( 4.0 * s_( 1, n ) - 3.0 - 2.0 / n / ( n + 1.0 ) );
}

complex< double > P::QGLO::operator() ( const complex< double >& n ) {
//     return - 1.0 / 2.0 / nf() * 8.0 * TF * 
     return - 1.0 / 4.0 / p_->TF * 8.0 * p_->TF * 
	  ( n * n + n + 2.0 ) / n / ( n + 1.0 ) / ( n + 2.0 );
}

complex< double > P::GQLO::operator() ( const complex< double >& n ) {
//     return - 2.0 * nf() * Kernel::CF4 * 
     return - 4.0 * p_->TF * Kernel::CF4 * 
	  ( n * n + n + 2.0 ) / ( n - 1.0 ) / n / ( n + 1.0 );
}

complex< double > P::GGLO::operator() ( const complex< double >& n ) {
  static double c11_3 = 11.0 / 3.0;
  static double c8_3  =  8.0 / 3.0;
  return 
       Kernel::CA2 * (  4.0 * s_( 1, n ) - c11_3 - 4.0 / n / ( n - 1.0 )
			- 4.0 / ( n + 1.0 ) / ( n + 2.0 ) ) + c8_3 * p_->TF;
}

// ------------------------ NLO ------------------------------ //
complex< double > P::QQPS::operator() ( const complex< double >& n ) {
       //PRD48, 116 ( -8 FACTOR ) + NPB192, 417
  static double c80_9 = 80.0 / 9.0;
  static double c32_3 = 32.0 / 3.0;
  static double c224_9 = 224.0 / 9.0;

  complex< double > n2  = n  * n;
  complex< double > n3  = n2 * n;
  complex< double > n4  = n3 * n;
  complex< double > n5  = n4 * n;

  complex< double > np1 = n + 1.0;
  complex< double > np2 = n + 2.0;
  complex< double > nm1 = n - 1.0;

  return 

       //PRD48, 116 ( -8 FACTOR ) PART  - tilelike part
       - Kernel::CF16 * p_->TF / 2.0 *
       ( - c80_9 / nm1 + 8.0 / n3 + 12.0 / n2 - 12.0 / n + 8.0 * pow( np1, -3 )
	 + 28.0 * pow( np1, -2 ) - 4.0 / np1 + c32_3 * pow( np2, -2 ) 
	 + c224_9 / np2 )

       // NPB192, 417 PART  - spacelike part
       - Kernel::CF16 * p_->TF * 
       ( 5.0 * n5 + 32.0 * n4 + 49.0 * n3 + 38.0 * n2 + 28.0 * n + 8.0 )
       / nm1 / n3 / pow( np1, 3) / pow( np2, 2)
       ;

}

complex< double > P::QQNLO::operator() ( const complex< double >& n ) {
  // this is actully NS+ + PS
  return (*(p_->nsnlo_))( n, +1.0 ) + (*(p_->qqps_))( n );
}

complex< double > P::QGNLO::operator() ( const complex< double >& n ) {

       //PRD48, 116 ( -8 FACTOR )  - timelike part
       //& additional 1/ 2 nf factor -> (- 2 / TF) factor  - timelike part
  static double c8_3 = 8.0 / 3.0;
  static double c5_3 = 5.0 / 3.0;
  static double c4_3 = 4.0 / 3.0;
  static double c10_3 = 10.0 / 3.0;
  static double c40_9 = 40.0 / 9.0;
  static double c26_9 = 26.0 / 9.0;
  static double c22_3 = 22.0 / 3.0;
  static double c68_3 = 68.0 / 3.0;
  static double c190_9 = 190.0 / 9.0;
  static double c356_9 = 356.0 / 9.0;

  static double PI2 = M_PI * M_PI;

  complex< double > n2  = n  * n;
  complex< double > n3  = n2 * n;
  complex< double > n4  = n3 * n;
  complex< double > n5  = n4 * n;
  
  complex< double > np1 = n + 1.0;
  complex< double > np2 = n + 2.0;
  complex< double > nm1 = n - 1.0;
  complex< double > A   = ( n2 + n + 2.0 ) / n / np1 / np2;

// note variable n -> n + 1 (for unpol timelike)
  complex< double > s1  = s_( 1, n + 1.0 );
  complex< double > s12 = s1 * s1;
  complex< double > s2  = s_( 2, n + 1.0 );

  complex< double > Gnp1 = ( s_( 2, ( n + 1.0 ) / 2.0 - 1.0 ) 
			     - s_( 2, ( n + 1.0 )/ 2.0 - 1.0 / 2.0 ) ) / 2.0;
  
  return
       - 2.0 / p_->TF  * p_->TF * p_->TF * c8_3 *
       ( s1 * A + 1.0 / n2 - c5_3 / n - pow( n * np1, -1 ) 
	 - 2.0 * pow( np1, -2 ) + c4_3 * pow( np1, -1 ) 
	 + 4.0 * pow( np2, -2 ) - c4_3 * pow( np2, -1 ) )

       - 2.0 / p_->TF * Kernel::CF * p_->TF *
       ( ( - 2.0 * s12 + 2.0 * s1 + 10.0 * s2 ) * A
	 + 4.0 * s1 * ( - 1.0 / n2 + 1.0 / n + pow( n * np1, -1 ) 
		        + 2.0 * pow( np1, -2 ) -4.0 * pow( np2, -2 ) )
	 - 2.0 / n3 + 5.0 / n2 - 12.0 / n + 4.0 * pow( n2 * np1, -1 )
	 - 12.0 * pow( n * np1 * np1, -1) - 6.0 * pow( n * np1, -1 )
	 + 4.0 * pow( np1, -3 ) - 4.0 * pow( np1, -2 ) + 23.0 / np1 
	 - 20.0 / np2 )

       - 2.0 / p_->TF * Kernel::CA * p_->TF *
       ( ( 2.0 * s12 - c10_3 * s1 - 6.0 * s2 + 2.0 * Gnp1 - PI2 ) * A
	 - 4.0 * s1 * ( - 2.0 / n2 + 1.0 / n + pow( n * np1, -1 ) 
			+ 4.0 * pow( np1, -2 ) - 6.0 * pow( np2, -2 ) )
	 - c40_9 / nm1 + 4.0 / n3 + c8_3 / n2 + c26_9 / n
	 - 8.0 * pow ( n * np1, -2 ) + c22_3 * pow( n * np1, -1 ) 
	 + 16.0 * pow( np1, -3) + c68_3* pow( np1, -2 ) - c190_9 / np1
	 + 8.0 * pow( np1 * np1 * np2, -1 ) - 4.0 * pow( np2, -2)
	 + c356_9 / np2
	    )
       ;
}

complex< double > P::GQNLO::operator() ( const complex< double >& n ) {
  
       //PRD48, 116 ( -8 FACTOR )  - timelike part
       //& additional 2 nf factor -> ( -32 * TF ) factor  - timelike part
  static double c2_3 = 2.0 / 3.0;
  static double c1_2 = 1.0 / 2.0;
  static double c5_2 = 5.0 / 2.0;
  static double c9_2 = 9.0 / 2.0;
  static double c17_9 = 17.0 / 9.0;
  static double c8_3 = 8.0 / 3.0;
  static double c44_9 = 44.0 / 9.0;

  static double PI2 = M_PI * M_PI;

  complex< double > n2  = n  * n;
  complex< double > n3  = n2 * n;
  complex< double > n4  = n3 * n;
  complex< double > n5  = n4 * n;
  
  complex< double > np1 = n + 1.0;
  complex< double > np2 = n + 2.0;
  complex< double > nm1 = n - 1.0;
  complex< double > np12 = np1 * np1;
  complex< double > nnp12 = n * np12;

  complex< double > A   = ( n2 + n + 2.0 ) / nm1 / n / np1;

  complex< double > s1 = s_( 1, n );
  complex< double > s12 = s1 * s1;
  complex< double > s2 = s_( 2, n );

  complex< double > Gn = ( s_( 2, n / 2.0 - 1.0 ) 
			     - s_( 2, n / 2.0 - 1.0 / 2.0 ) ) / 2.0;
  
  return 
       - 32.0 * p_->TF * Kernel::CF_2 *
       ( ( s12 - 3.0 * s2 - c2_3 * PI2 ) * A 
	 + 2.0 * s1 * ( 4.0 * pow ( nm1, -2 ) - 2.0 / nm1 / n - 4.0 / n2 
			+ 3.0 * pow( np1, -2 ) - 1.0 / np1 )
	 - 8.0 * pow( nm1, -2 ) / n + 8.0 / nm1 / n2 + 2.0 / n3 + 8.0 / n2
	 - c1_2 / n + pow( np1, -3 ) - c5_2 * pow(np1, -2 ) + c9_2 / np1 )

       - 32.0 * p_->TF * Kernel::CF * Kernel::CA *
       ( ( - s12 + 5.0* s2 - Gn + PI2 / 6.0 ) * A
	 + 2.0 * s1 * ( - 2.0 * pow( nm1, -2 ) + 2.0 / nm1 / n + 2.0 / n2
			- 2.0 * pow( np1, -2 ) + 1.0 / np1 )
	 - 8.0 * pow( nm1, -3 ) + 6.0 * pow( nm1, -2 ) + c17_9 / nm1
	 + 4.0 * pow( nm1, -2 ) / n - 12.0 / nm1 / n2 - 8.0 / n2 + 5.0 / n
	 - 2.0 / np1 / n2 - 2.0 * pow( np1, -3 ) - 7.0 * pow( np1, -2)
	 - 1.0 / np1 - c8_3 * pow( np2, -2 ) - c44_9 / np2 )
       ;

}

complex< double > P::GGNLO::operator() ( const complex< double >& n ) {
  
       //PRD48, 116 ( -8 FACTOR ) + NPB192, 417

  static double c160_9 = 160.0 / 9.0;
  static double c32_3 = 32.0 / 3.0;
  static double c16_9 = 16.0 / 9.0;
  static double c536_9 = 536.0 / 9.0;
  static double c64_3 = 64.0 / 3.0;
  static double c4_9 = 4.0 / 9.0;
  static double c8_3 = 8.0 / 3.0;

  static double c16_3 = 16.0 / 3.0;
  static double c80_9 = 80.0 / 9.0;
  static double c224_9 = 224.0 / 9.0;
  static double c4_3 = 4.0 / 3.0;
  static double c11_12 = 11.0 / 12.0;
  static double c22_3 = 22.0 / 3.0;
  static double c14_3 = 14.0 / 3.0;

  static double PI2 = M_PI * M_PI;

  complex< double > n2  = n  * n;
  complex< double > n3  = n2 * n;
  complex< double > n4  = n3 * n;
  complex< double > n5  = n4 * n;
  complex< double > n6  = n5 * n;
  complex< double > nhalf = n / 2.0;


  complex< double > np1 = n + 1.0;
  complex< double > np2 = n + 2.0;
  complex< double > nm1 = n - 1.0;
  complex< double > np12 = np1 * np1;
  complex< double > nnp1 = n * np1;
  complex< double > n2np12 = nnp1 * nnp1;
  complex< double > n3np13 = nnp1 * n2np12;
  complex< double > nm1np2 = nm1 * np2;
  complex< double > nnp1np2 = nnp1 * np2;

  complex< double > DEN = pow( n - 1.0, 1 ) * pow( n, 2 ) *
       pow( n + 1.0, 2 ) * pow( n + 2.0, 1 );

  complex< double > A = 
       38.0 * n4 + 76.0 * n3 + 94.0 * n2 + 56.0 * n + 12.0;
  complex< double > B = 
       2.0 * n6 + 4.0 * n5 + n4 - 10.0 * n3 - 5.0 * n2 - 4.0 * n - 4.0;
  complex< double > C = 
       2.0 * n5 + 5.0 * n4 + 8.0 * n3 + 7.0 * n2 - 2.0 * n - 2.0;
  complex< double > D =
       n5 * ( 457.0 * n4 + 2742.0 * n3 + 6040.0 * n2 + 6098.0 * n + 1567.0 )
       - 2344.0 * n4 - 1632.0 * n3 + 560.0 * n2 + 1488.0 * n + 576.0;
   
  complex< double > s1 = s_( 1, n );
  complex< double > s12 = s1 * s1;
  complex< double > s2 = s_( 2, n );
// dash  
  complex< double > sp2 = sp_( 2, nhalf, 1 );
  complex< double > sp3 = sp_( 3, nhalf, 1 );
//tilda
  complex< double > st  = st_( n, 1 );

  return
// NPB192, 417 part (unpol spacelike)
      Kernel::CA * p_->TF *
       ( - c160_9 * s1 + c32_3 + c16_9 * A / DEN )

    + Kernel::CF * p_->TF *
       ( 8.0 + 16.0 * B / DEN / nnp1 )

    + Kernel::CA * Kernel::CA *
       ( c536_9 * s1 + 64.0 * s1 * C / DEN / nm1np2 - c64_3
	 + 32.0 * sp2 * ( n2 + n + 1.0 ) / DEN * nnp1
	 - c4_9 * D / DEN / nm1np2 / nnp1np2
	 -16.0 * s1 * sp2 + 32.0 * st - 4.0 * sp3 )

//PRD48, 116 ( -8 FACTOR ) part
    + 8.0 * Kernel::CA * p_->TF * c8_3 *
       ( s2 - pow( nm1, -2 ) + 1.0 / n2 - pow( np1, -2 )
	 + pow( np2, -2 ) - PI2 / 6.0 )

    - 8.0 * Kernel::CF * p_->TF *
       ( - c16_3 * pow( nm1, -2 ) + c80_9 / nm1
	 + 8.0 / n3 - 16.0 / n2 + 12.0 / n 
	 + 8.0 * pow( np1, -3 ) - 24.0 * pow( np1, -2 )
	 + 4.0 / np1 - c16_3 * pow( np2, -2)
	 - c224_9 / np2 )

    - 8.0 * Kernel::CA * Kernel::CA *
       ( - 8.0 * s1 * s2 
	 + 8.0 * s1 * ( pow( nm1, -2 ) - 1.0 / n2
			+ pow( np1, -2 ) - pow( np2, -2 ) + PI2 / 6.0 )
	 + ( 8.0 * s2 - c4_3 * PI2 ) * ( 1.0 / nm1 - 1.0 / n
				 + 1.0 / np1 - 1.0 / np2 + c11_12 )
	 - 8.0 * pow( nm1, -3 ) + c22_3 * pow( nm1, -2) 
	 - 8.0 * pow( nm1, -2 ) / n - 8.0 / n2 / nm1
	 - 8.0 / n3 - c14_3 / n2 - 8.0 * pow( np1, -3 )
	 + c14_3 * pow( np1, -2 ) - 8.0 * pow( np1, -2 ) / np2
	 - 8.0 * pow( np2, -2 ) / np1
	 - 8.0 * pow( np2, -3 ) - c22_3 * pow( np2, -2 ) )
       ;
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

  static double PI2 = M_PI * M_PI;
  
  complex< double > n2  = n  * n;
  complex< double > n3  = n2 * n;
  complex< double > n4  = n3 * n;
  
  complex< double > np1   = n + 1.0;
  complex< double > nnp1  = n * np1;
  complex< double > nnp12 = nnp1 * nnp1;
  complex< double > nnp13 = nnp12 * nnp1;
  
  complex< double > s1  = s_( 1, n );
  complex< double > s2  = s_( 2, n );
  
//dash
  complex< double > sp2 = sp_( 2, n / 2.0, ( eta > 0 ? 1 : -1 ) );
  complex< double > sp3 = sp_( 3, n / 2.0, ( eta > 0 ? 1 : -1 ) );
//tilda
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

       //PRD48, 116 ( -8 FACTOR )
  complex< double > D = 
       - 8.0 * Kernel::CF_2 * ( - 4.0 * s1 + 3.0 + 2.0 / nnp1 ) *
       ( 2.0 * s2 - PI2 / 3.0 - c1 / nnp12 )
    ;

  return A + B + C + D;
  
}

