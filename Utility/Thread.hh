/*!
  \brief  Theread class definition file
*/

#ifndef _Utility_THREAD_HH_
#define _Utility_THREAD_HH_

extern "C" {
#include <pthread.h>
}

namespace Utility {

  /*!
    \brief Thread handling class 
   */
  class Thread {
  public:

    Thread();             //!< a default constructor
    virtual ~Thread();    //!< a destructor
    
    /*!
      \brief start new thread
    */
    virtual pthread_t newThread() throw( int ) ; 
    
    /*!
      \brief wait the given thread termination
    */
    virtual int joinThread( const pthread_t& id );
    
  private:
    
    //! Thread wrapping method
    static void* start_routine( void *arg );
    
    /*!
      \brief abstract method to be called on new thread initiation
      
      This virtual method will be called when the new thread is 
      created with start method.
      
    */
    virtual int runInThread() = 0;
    
  };
  
}

#endif	    /* !THREAD_HH_ */
