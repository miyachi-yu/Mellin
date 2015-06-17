//
// RealFunctionWrapper.hh
// 
// Made by Yoshiyuki Miyachi
// Login   <miyachi@yoshiyuki-desktop>
// 
// Started on  Fri Oct 17 15:17:53 2008 Yoshiyuki Miyachi
// Last update Wed Nov 12 13:55:51 2008 Yoshiyuki Miyachi
//

#ifndef   	REALFUNCTIONWRAPPER_HH_
# define   	REALFUNCTIONWRAPPER_HH_

#include <Tranform/RealFunction.hh>
#include <Xsec/xXsec.hh>

class RealFunctionWrapper : public Transform::RealFunction {
public:
  
  enum MODE {
    LengthInX=1, LengthInZ=2,
    AngleInX=3,  AngleInZ=4,
    OffsetInX=5, OffsetInZ=6
  };
  
  RealFunctionWrapper();
  virtual ~RealFunctionWrapper();
  
  virtual double operator()( const double& x );
  
  void set( const std::vector< double >& x, 
	    const std::vector< double >& z ){
    x_ = x; z_ = z;
  }
  
  void set( Xsection::xXsec* xsec ) { xsec_ = xsec; }
  
  void set( const MODE& mode ) { mode_ = mode; }
  
  void Q2( const double& q2 );
  
  Xsection::xXsec* xsec() { return xsec_; }
  
private:
  
  Xsection::xXsec* xsec_;
  std::vector< double > x_;
  std::vector< double > z_;
  MODE mode_;
  
};

#endif	    /* !REALFUNCTIONWRAPPER_HH_ */
