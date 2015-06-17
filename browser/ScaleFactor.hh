#ifndef _ScaleFactor_hh_
#define _ScaleFactor_hh_

#include <vector>

class ScaleFactor : public std::vector< double > {
public:
  
  ScaleFactor();
  virtual ~ScaleFactor();
  
  double offset( const double& x );
  double q2min( const double& x );
  double q2max( const double& x );

  int index( const double& x );

private:
  

  std::vector< double > min_;
  std::vector< double > max_;

  
};

#endif // _ScaleFactor_hh_
