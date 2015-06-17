//
// Thread.cc
//  
// Made by Yoshiyuki Miyachi
// Login   <miyachi@yoshiyuki-desktop>
// 
// Started on  Tue Sep 30 10:27:47 2008 Yoshiyuki Miyachi
// Started on  Tue Sep 30 10:27:47 2008 Yoshiyuki Miyachi
//

#include <iostream>
#include "Thread.hh"

using namespace std;
using namespace Utility;

void* Thread::start_routine( void *arg ){
  if( arg == NULL ) return NULL;
  Thread *thread = reinterpret_cast< Thread* >( arg );
  if( thread == NULL ) return NULL;
  thread->runInThread();
  return arg;
}

Thread::Thread() {
}

Thread::~Thread() {
}

pthread_t Thread::newThread() throw( int ) {
  pthread_t id;
  int err = pthread_create( &id, 0, Thread::start_routine, this );
  if( err != 0 ){
    cerr << "Error on creating new thread ( error = " << err << " )"<< endl;
    throw( 1 );
  }
  return id;
}

int Thread::joinThread( const pthread_t& id ){
  return pthread_join( id, 0 );
}
