#ifndef _RungeKutta_hh_
#define _RungeKutta_hh_

namespace Transform {

/*!
  \class RungeKutta RungeKutta.hh "Tranform/RungeKutta.hh"
  \brief class for RungeKutta calculation 
 */
class RungeKutta  {
public:
  
  class Differential {
  public:
    virtual ~Differential();
    virtual double operator()( const double& t, const double& y ) const = 0 ;
  };
  
  RungeKutta( Differential *f );
  virtual ~RungeKutta();
  
  double operator()( const double& y, 
		     const double& t, const double& dt ) const ;
  
private:
  Differential *f_;   //!< a pointer to differential function
  
};
};
#endif // _RungeKutta_hh_
