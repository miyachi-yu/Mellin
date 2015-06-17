//
// Kinematic.hh
// 
// Made by Yoshiyuki Miyachi
// Login   <miyachi@yoshiyuki-desktop>
// 
// Started on  Fri Sep 26 16:18:52 2008 Yoshiyuki Miyachi
// Last update Fri Sep 26 17:15:03 2008 Yoshiyuki Miyachi
//

#ifndef   	_ExpData_KINEMATIC_HH_
# define   	_ExpData_KINEMATIC_HH_

#include <string>
#include <iostream>

namespace Exp {
  
  class Kinematic {
  public:
    
    Kinematic();

    Kinematic( const std::string& info,
	       const double& data,
	       const std::string& unit );
    
    Kinematic( const Kinematic& kine );
    
    virtual ~Kinematic();
    
    Kinematic& operator=( const Kinematic& kine );
    
    bool operator==( const Kinematic& kine ) ;
    bool operator>( const Kinematic& kine )  ;
    bool operator<( const Kinematic& kine )  ;

    std::string& info()       { return info_; }
    std::string  info() const { return info_; }
    
    double& data()       { return data_; }
    double  data() const { return data_; }

    std::string& unit()       { return unit_; }
    std::string  unit() const { return unit_; }
    
    friend std::ostream& operator<<( std::ostream& os, const Kinematic& dp );
    
  private:

    std::string info_;
    std::string unit_;
    double      data_;
    
  };

};

#endif	    /* !KINEMATIC_HH_ */
