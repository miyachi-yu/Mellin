// $Id: IntegPath.cc,v 1.2 2008/07/17 10:09:21 miyachi Exp $
/*!
  \file  IntegPath.cc
  \brief class implementation of Mellin inversion contour
 */
#include "IntegPath.hh"

using namespace std;
using namespace Transform;

IntegPath::IntegPath() {
}

IntegPath::~IntegPath() {
}

complex< double > IntegPath::operator()( const complex< double >& z ) {
  return complex< double >( 0.0, 0.0 );
}

