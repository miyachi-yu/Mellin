#include "P.hh"
#include "Evolution/Kernel.hh"

#include <iostream>
#include <iomanip>

using namespace std;
using namespace Evolution;
using namespace polFrag;

P::P( Kernel *k ) : 
  AnomalousDimension( k )
{
  // 
  name_ = "polFF";
  
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
  // additional 1/2/nf factor -> 1/4/TF factor  - timelike case
//  return - 8.0 * p_->TF * ( n - 1.0 ) / n / ( n + 1.0 );
  return - 1.0 / 4.0 / p_->TF * 8.0 * p_->TF * ( n - 1.0 ) / n / ( n + 1.0 );
}

complex< double > P::GQLO::operator() ( const complex< double >& n ) {
  // additional 2*nf factor -> 4*TF factor  - timelike case
//  return - Kernel::CF4 * ( n + 2.0 ) / n / ( n + 1.0 );
  return - 4.0 * p_->TF * Kernel::CF4 * ( n + 2.0 ) / n / ( n + 1.0 );
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
  complex< double > np1 = n + 1.0;
  
  return 
    // NPB496,41
    Kernel::CF16 * p_->TF 
       * ( pow( n, 4 ) 
	+ 2.0 * pow( n, 3 ) 
	+ 2.0 * pow( n, 2 ) 
	+ 5.0 * n + 2.0 ) 
       / pow( n, 3 ) / pow( np1, 3 )
       
    + Kernel::CF4 * p_->TF 
       * ( n + 2.0 ) * ( 3.0 * n + 1.0 ) 
       / pow( n, 3 ) / pow( np1, 3 )
       ;

}

complex< double > P::QQNLO::operator() ( const complex< double >& n ) {
  // this is actully NS- + PS
  return (*(p_->nsnlo_))( n, -1.0 ) + (*(p_->qqps_))( n );
}

complex< double > P::QGNLO::operator() ( const complex< double >& n ) {

  // additional 1/2/nf factor -> 1/4/TF factor  - timelike case
  static double c8_9 = 8.0 / 9.0;
  static double c2_9 = 2.0 / 9.0;
  static double PI2 = M_PI * M_PI;

  complex< double > n2  = n  * n;
  complex< double > n3  = n2 * n;
  complex< double > n4  = n3 * n;
  complex< double > n5  = n4 * n;
  
  complex< double > np1 = n + 1.0;
  complex< double > nnp1 = n * np1;
  complex< double > np12 = pow( np1, 2.0 );
  complex< double > n2np12 = n2 * np12;
  complex< double > np13 = pow( np1, 3.0 );
  complex< double > n3np13 = n3 * np13;
  
  complex< double > nm1 = n - 1.0;
  complex< double > s1  = s_( 1, n );
  complex< double > s12 = s1 * s1;
  complex< double > s2  = s_( 2, n );
  complex< double > s2p = sp_( n / 2.0 ); 

  complex< double > A   = nm1 / n / np1;
  complex< double > B   = 7.0 * n5 + 7.0 * n4 - 5.0 * n3 + 5.0 * n2 + 4.0 * n - 2.0;
  complex< double > C   = 11.0 * n3 - 12.0 * n2 + 37.0 * n + 12.0;
  complex< double > D   = 67.0 * n5 + 34.0 * n4 + 32.0 * n3 
                                     + 98.0 * n2 + 249.0 * n + 72.0;
  
  return
       1.0 / 4.0 / p_->TF * (
    // NPB496,41
	    Kernel::CA16 * p_->TF * ( 
		 A * ( - s2 + sp_( 2, n / 2.0, -1 ) + s12 )
		 - 4.0 / n / pow( np1, 2 ) * s1
		 - ( n5 + n4 - 4.0 * n3 + 3.0 * n2 - 7.0 * n - 2.0 ) / n3np13 
		 )
	    + Kernel::CF8 * p_->TF * ( 
		 2.0 * A * ( s2 - s12 )
		 + 4.0 * A / n * s1
		 - ( 5.0 * n5 + 5.0 * n4 - 10.0 * n3 - n2 + 3.0 * n - 2.0 ) / n3np13 
		 )
	    )

       + 1.0 / 4.0 / p_->TF * (
	    p_->TF * p_->TF * c8_9 * (
		 3.0 * nm1 / nnp1 * s1 
		 - ( 5.0 * n3 - 3.0 * n2 + 7.0 * n + 3.0) / n2np12
		 )
	    + Kernel::CF2 * p_->TF * (
		 2.0 * nm1 / nnp1 * ( - s12 + 3.0 * s2 )
		 + ( 3.0 * n2 + 5.0 ) / n / np12 * s1
		 - B / n3np13
	         )
	    + Kernel::CA * p_->TF * c2_9 * (
		 6.0 * nm1 / nnp1 * ( 3.0 * s12 - 3.0 * s2 - PI2 )
		 - 3.0 * C / n2np12 * s1
		 + D / n3np13
		 )
	    )
    ;

}

complex< double > P::GQNLO::operator() ( const complex< double >& n ) {
  
  // additional 2*nf factor -> 4*TF factor  - timelike case  
  // NPB496,41
  static double c4_9 = 4.0 / 9.0;
  static double PI2 = M_PI * M_PI;

  complex< double > n2  = n  * n;
  complex< double > n3  = n2 * n;
  complex< double > n4  = n3 * n;
  complex< double > n5  = n4 * n;

  complex< double > np1 = n + 1.0;
  complex< double > nnp1 = n * np1;
  complex< double > np12 = pow( np1, 2.0 );
  complex< double > n2np12 = n2 * np12;
  complex< double > np13 = pow( np1, 3.0 );
  complex< double > n3np13 = n3 * np13;
  
  complex< double > nnp12 = n * np12;

  complex< double > A = ( n + 2.0 ) / n / np1;

  complex< double > s1 = s_( 1, n );
  complex< double > s12 = s1 * s1;
  complex< double > s2 = s_( 2, n );

  complex< double > B = 7.0 * n5 + 22.0 * n4 + 7.0 * n3 
                               - 24.0 * n2 - 22.0 * n - 4.0;
  complex< double > C = 67.0 * n4 + 101.0 * n3 
                               + 34.0 * n2 + 144.0 * n + 72.0;
  
  return 
       4.0 * p_->TF * (
	    Kernel::CF32 * p_->TF * (
		 - A / 3.0 * s_( 1, n )
		 + ( 5.0 * n2 + 12.0 * n + 4.0 ) / 9.0 / nnp12
		 )
	    + Kernel::CF2_2 * (
		 2.0 * A * ( s2 + pow( s1, 2 ) )
		 -2.0 * ( 3.0 * n2 + 7.0 * n + 2.0 ) / nnp12  * s1
		 + ( 9.0 * n5 + 30.0 * n4 + 24.0 * n3 - 7.0 * n2 - 16.0 * n - 4.0 )
		 / n3 / pow( np1, 3 )
		 )
	    + Kernel::CACF8 * (
		 A * ( -s2 + sp_( 2, n / 2.0, -1 ) -s12 )
		 + ( 11.0 * n2 + 22.0 * n + 12.0 ) / 3.0 / n2 / np1 * s1
		 - ( 76.0 * n5 + 241.0 * n4 + 254.0 * n3 + 41.0 * n2 
		     + 72.0 * n + 36.0 )
		 / 9.0 / n3 / pow( np1, 3 )
		 )
	    )

       + 4.0 * p_->TF * (
	    Kernel::CF_2 * (
		 - 2.0 * ( n + 2.0 ) / nnp1 * ( - s12 + s2 + PI2 / 3.0 )
		 - ( 3.0 * n3 + n2 - 18.0 * n - 8.0 ) / n2np12 * s1
		 + B / n3np13
		 )
	    + Kernel::CA * Kernel::CF * (
		 - 2.0 * ( n + 2.0 ) / nnp1 * ( s12 - 3.0 * s2 )
		 + ( n + 2.0 ) * ( 11.0 * n - 1.0 ) / 3.0 / n / np12 * s1
		 - C * ( n + 2.0 ) / 9.0 / n3np13
		 )
	    + Kernel::CF * p_->TF * c4_9 * (
		 - 3.0 * ( n + 2.0 ) / nnp1 * s1 
		 + ( n + 2.0 ) * ( 5.0 * n + 2.0 ) / n / np12
		 )
	    )
    ;

}

complex< double > P::GGNLO::operator() ( const complex< double >& n ) {
  
  // NPB496,41
  static double c4_3 = 4.0 / 3.0;
  static double PI2 = M_PI * M_PI;

  complex< double > n2  = n  * n;
  complex< double > n3  = n2 * n;
  complex< double > n4  = n3 * n;
  complex< double > n5  = n4 * n;
  complex< double > n6  = n5 * n;

  complex< double > np1 = n + 1.0;
  complex< double > nnp1 = n * np1;
  complex< double > np12 = pow( np1, 2.0 );
  complex< double > n2np12 = n2 * np12;
  complex< double > np13 = pow( np1, 3.0 );
  complex< double > n3np13 = n3 * np13;
  
  complex< double > nhalf = n / 2.0;

  complex< double > A = ( n + 2.0 ) / n / np1;

  complex< double > s1 = s_( 1, n );
  complex< double > s12 = s1 * s1;
  complex< double > s2 = s_( 2, n );
  
  complex< double > sp2 = sp_( 2, nhalf, -1 );

  return
       + Kernel::CF8 * p_->TF * (
	    n6 + 3.0 * n5 + 5.0 * n4 + n3 -8.0 * n2 + 2.0 * n + 4.0 
	    ) / n3 / pow( np1, 3 ) 
    
       + Kernel::CA32 * p_->TF * (
	    - 5.0 / 9.0 * s1 
	    + ( 3.0 * n4 + 6.0 * n3 + 16.0 * n2 + 13.0 * n - 3.0 )
	    / 9.0 / n2np12 
	    )
       
       + Kernel::CA2_2 * (
	    - sp_( 3, nhalf, -1 ) 
	    - 4.0 * s1 * sp2
	    + 8.0 * st_( n, -1 ) 
	    + 8.0 / nnp1 * sp2
	    + 2.0 * ( 67.0 * n4 + 134.0 * n3 + 67.0 * n2 + 144.0 * n + 72.0 )
	    / 9.0 / n2np12 * s1
	    - ( 48.0 * n6 + 144.0 * n5 + 469.0 * n4 + 698.0 * n3 + 7.0 * n2
		+ 258.0 * n + 144.0 ) / 9.0 / n3np13 
	    )


       + Kernel::CF8 * p_->TF * ( n3 + 3.0 * n - 1.0 )/ n3np13
       + Kernel::CA * p_->TF * c4_3 * (
	    - 2.0 * s2 + PI2 / 3.0 + 4.0 * ( 2.0 * n + 1.0 ) / n2np12
	    )
       + Kernel::CA * Kernel::CA * 
       ( 4.0 * s1 - ( 11.0 * n2 + 11.0 * n + 24.0 ) / 3.0 / np1 ) 
       * ( - 2.0 * s2 + PI2 / 3.0 + 4.0 * ( 2.0 * n + 1.0 ) / n2np12 )
	           
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
      c16_9 * ( 11.0 * n2 + 5.0 * n - 3.0 ) / nnp12 );
  
  complex< double > D =
    Kernel::CF_2 *
       ( 4.0 * s1 - ( 3.0 * n2 + 3.0 * n + 2.0 ) / nnp1 ) * 
       ( - 2.0 * s2 + PI2 / 3.0 + ( 2.0 * n + 1.0 ) / n2 / pow( np1, 2.0 ) ); 

  return A + B + C + D;
  
}

