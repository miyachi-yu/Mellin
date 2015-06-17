// $Id: Timer.cc,v 1.5 2008/05/27 06:53:25 miyachi Exp $
/*!
  \file    Timer.cc
  \version $Revision: 1.5 $
  \date    $Date: 2008/05/27 06:53:25 $
  \author  $Author: miyachi $
  \brief   Implementation of Stop watch class
 */
#include "Timer.hh"

using namespace std;
using namespace Utility;

Timer::Timer() : 
  start_( -1 ), stop_( -1 )
{
  this->start();
}

Timer::~Timer(){
}

time_t Timer::start() {
  stop_ = -1;
  gettimeofday( &begin_, NULL );
  return time( &start_ );
}

time_t Timer::stop() {
  stop_ = time( NULL );
  gettimeofday( &end_, NULL );
  return ( stop_  - start_ );
}

double Timer::check( TYPE type ){
  
  if( stop_ == -1 ) gettimeofday( &end_, NULL );
  
  double s  = end_.tv_sec  - begin_.tv_sec;
  double us = s * 1.0E6 + end_.tv_usec - begin_.tv_usec;
  
  if( type == Day  ) return s / 86400.0;
  if( type == Hour ) return s / 3600.0;
  if( type == Min  ) return s / 60.0;
  if( type == Sec  ) return us / 1.0E6;
  if( type == Msec ) return us / 1.0E3;
  
  return us;
}

string Timer::local() {
  time_t buffer = time( NULL );
  return asctime( localtime( &buffer ) );
}
