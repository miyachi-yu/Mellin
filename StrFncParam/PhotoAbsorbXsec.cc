// $Id: PhotoAbsorbXsec.cc,v 1.3 2008/05/20 00:44:19 miyachi Exp $
/*!
  \file  PhotoAbsorbXsec.cc
  \brief Implementation of photo absorption cross section sigma_T class
 */
#include "PhotoAbsorbXsec.hh"

#include <cmath>

using namespace std;
using namespace StrFncParam;

double PhotoAbsorbXsec::FineConstant = 
  4.0 * M_PI * M_PI * 
  7.297352533E-3 *  // GeV^2 ubarn
  3.89379292E+2;    // GeV^2 ubarn

double PhotoAbsorbXsec::Mp2 = 0.880354342; // 0.938271998 * 0.938271998;

PhotoAbsorbXsec::PhotoAbsorbXsec() : 
  q2_( 1.0 ){
}

PhotoAbsorbXsec::~PhotoAbsorbXsec() {
}

double PhotoAbsorbXsec::W2( const double& x ){
  return Mp2 + q2_ * ( 1 - x ) / x;
}

double PhotoAbsorbXsec::x( const double& W2, const double& m2 ){
  double v = 1.0 + ( W2 - Mp2 ) / ( q2_ + m2 );
  return 1.0 / v;
}
