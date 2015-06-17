//
// testThread.cc
//  
// Made by Yoshiyuki Miyachi
// Login   <miyachi@yoshiyuki-desktop>
// 
// Started on  Tue Sep 30 10:47:53 2008 Yoshiyuki Miyachi
// Started on  Tue Sep 30 10:47:53 2008 Yoshiyuki Miyachi
//

#include <iostream>
#include <iomanip>
extern "C" {
#include <unistd.h>
}
#include "Thread.hh"

using namespace std;


class A {
public:
  double integ() {
    double sum(0);
    for( int i = 0; i < 100; i++ ){
      sleep(1);
      sum ++;
      cout << "sum = " << setw(4) << sum << endl;
    }
    return sum;
  }
  
};

class MyThread : public Utility::Thread {
public:

  MyThread() : Utility::Thread(), a() {}
  virtual ~MyThread() {}
  
  double Run(){
    
    try {
      min_ = 1.0; max_ = 2.0;
      pthread_t id1 = this->newThread();
      min_ = 2.0; max_ = 4.0;
      pthread_t id2 = this->newThread();
      half2_ = a.integ();
      this->joinThread( id1 );
    }
    
    catch( int err ){
      return 0.0;
    }
    return half1_ + half2_;
    
  }
  
private:
  
  virtual int runInThread() {
    cout << "min:" << setw(10) << min_ << endl;
    cout << "max:" << setw(10) << max_ << endl;
    half1_ = a.integ( );
    return 0;
  }
  
  A a;
  double half1_;
  double half2_;

  double min_;
  double max_;

  
};


int main( int argc, char* argv[] ){
  
  MyThread thread1;
  
  cout << thread1.Run() << endl;
  
  return 0;
}
