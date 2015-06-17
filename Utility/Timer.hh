// $Id: Timer.hh,v 1.7 2008/05/27 06:53:25 miyachi Exp $
/*!
  \file    Timer.hh
  \version $Revision: 1.7 $
  \date    $Date: 2008/05/27 06:53:25 $
  \author  $Author: miyachi $
  \brief   Definition of Stop watch class
 */
#ifndef _Timer_hh_
#define _Timer_hh_

#include <ctime>
extern "C" {
#include <sys/time.h>
}

#include <string>

namespace Utility {
  /*!
    \class   Timer   Timer.hh   "Utility/Timer.hh"
    \brief   Stop watch class
  */
  class Timer {
  public:
    
    //! type of unit of time
    enum TYPE { Day, Hour, Min, Sec, Msec, Usec };
    
    Timer();                   //!< a default constructor
    virtual ~Timer();          //!< a destructor
    
    time_t start();            //!< start timer
    time_t stop();             //!< stop  timer
    
    /*!
      \param[in]  type is a unit of time which you want to get
      \return    spent time since timer started
      \brief     get time spent since timer started.
      
      It returns the time length spent after the timer started 
      in the given unit. If the timer stoped before, it returns
      the length from start and stop in the given unit.
    */
    double check( TYPE type = Sec );

    //! get local time in string
    std::string local();
    
  private:
    time_t start_;  //!< start time
    time_t stop_;   //!< stop time
    
    struct timeval begin_;   //!< time structure for starting time
    struct timeval end_;     //!< time structure for the end
    
  };
};
#endif // _Timer_hh_
