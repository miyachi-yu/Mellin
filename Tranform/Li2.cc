// $Id: Li2.cc,v 1.9 2008/12/02 08:13:16 miyachi Exp $
/*!
  \file    Li2.cc
  \version $Revision: 1.9 $
  \date    $Date: 2008/12/02 08:13:16 $
  \brief   Implementation of Li2 special function class
 */
#include "Li2.hh"
#include "GridIntegration.hh"
#include <cstdlib>

using namespace std;
using namespace Transform;

double Li2::key( 0.0 );
double Li2::data( 0.0 );
Utility::LookupTable< double, double > Li2::cache( Li2::key, Li2::data, "li2.db" );

bool Li2::cacheReady = false;

/*
  A cache file was already open on the initiation of static variable above.
  During the initialization of cache file, the closing method will be
  registered to atexit function. So that, the DB file will be closed 
  properly at the end of the program.
 */
void Li2::initCache() {
  if( Li2::cacheReady ) return;
  atexit( Li2::closeCache );
  Li2::cacheReady = true;
}

void Li2::closeCache() {
  Li2::cache.close();
}

Li2::Li2() : 
  IntegFunc(), func_()
{
  if( ! Li2::cacheReady ) Li2::initCache();
  dynamic_cast< GridIntegration* >( this->integration() )->precision( 1.0E-6 );
}

double Li2::Integrand::operator()( const double& x ){
  if( x == 0 ) return 1.0;
  return -1.0 * log( 1 - x ) / x;
}

double Li2::operator()( const double& x ){
  if( ! Li2::cache.get( Li2::key = x, Li2::data ) ) { 
    Li2::data = this->integration()->integral( 0, x, func_ );
    Li2::cache.put();
  }
  return Li2::data ;
}

