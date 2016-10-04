// $Id: ALLM91.cc,v 1.3 2008/05/20 00:44:19 miyachi Exp $
/*!
  \file    ALLM91.cc
  \version $Revision: 1.3 $
  \date    $Date: 2008/05/20 00:44:19 $
  \author  $Author: miyachi $
  \brief   Implementation of ALLM91 photon absorption cross section parameterization
*/
#include "ALLM91.hh"
#include <cmath>

using namespace std;
using namespace StrFncParam;

ALLM91::C1::C1() : vector< double >( 3, 0.0 ) 
{
}

ALLM91::C1::~C1(){}

double ALLM91::C1::operator() ( const double& t ) { 
  vector< double >& v = (*this);
  return v[0] + v[1] * pow( t, v[2] );
} 

ALLM91::C2::C2() : vector< double >( 3, 0.0 ) 
{
}

ALLM91::C2::~C2(){}

double ALLM91::C2::operator() ( const double& t ) { 
  vector< double >& v = (*this);
  return v[0] + ( v[0] - v[1] ) * ( 1.0 / ( 1.0 + pow( t, v[2] ) ) - 1.0 );
} 


ALLM91::ALLM91() : 
  m02( 0.30508 ), mp2( 10.676 ), mr2( 0.20623 ), 
  q02( 0.27799 ), lm2( 0.06527 ),
  cr(), br(), ar(), 
  cp(), bp(), ap()
{
  
  cp[0] =  0.26550;
  cp[1] =  0.04856;
  cp[2] =  1.04682;
  
  ap[0] = -0.04503;
  ap[1] = -0.36407;
  ap[2] =  8.17091;
  
  bp[0] =  0.49222;
  bp[1] =  0.52116;
  bp[2] =  3.5515;
  
  cr[0] =  0.67639;
  cr[1] =  0.49027;
  cr[2] =  2.66275;
  
  ar[0] =  0.60408;
  ar[1] =  0.17353;
  ar[2] =  1.61812;
  
  br[0] =  1.26066;
  br[1] =  1.83624;
  br[2] =  0.81141;
  
}

ALLM91::~ALLM91() {
}

double ALLM91::t(){
  double v = log( ( q2_ + q02 ) / lm2 );
  v /= log( q02 / lm2 );
  return log( v );
}

double ALLM91::operator() ( const double& W2, const double& Q2 ){
  if( q2_ != Q2 ){
    q2_ = Q2;
    t_  = this->t();
  }
  return (*this)( W2 );
}

double ALLM91::operator() ( const double& W2 ) {
  
  double x  = this->x( W2 );
  double xp = this->x( W2, mp2 );
  double xr = this->x( W2, mr2 );
  
  double f2p = cp( t_ ) * pow( xp, ap( t_ ) ) * pow( 1.0 - x, bp( t_ ) );
  double f2r = cr( t_ ) * pow( xr, ar( t_ ) ) * pow( 1.0 - x, br( t_ ) );
  
  double v = FineConstant / ( q2_ + m02 );
  v /= 1.0 - x ;
  //  v *= 1.0 + 4.0 * Mp2 * q2_ / pow( q2_ + W2 - Mp2, 2 );
  v *= ( q2_ + 4.0 * Mp2 * x * x ) / q2_;
  v *= ( f2p + f2r );
  
  return v;
}
