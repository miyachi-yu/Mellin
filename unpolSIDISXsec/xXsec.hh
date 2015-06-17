//
// xXsec.hh
// 
// Made by Yoshiyuki Miyachi
// Login   <miyachi@yoshiyuki-desktop>
// 
// Started on  Thu Oct 23 11:10:55 2008 Yoshiyuki Miyachi
// Last update Thu Oct 23 12:48:35 2008 Yoshiyuki Miyachi
//

#ifndef  _unpolSIDISXsec_XXSEC_HH_
#define  _unpolSIDISXsec_XXSEC_HH_

#include <Xsec/xXsec.hh>

namespace unpolSIDISXsec {
  
  class xXsec : public Xsection::xXsec {
  public:
    
    xXsec( Xsection::Xsec* xc, 
	   const int& low = 4, const int& high = 6, 
	   const double& precision = 1.0E-4,
	   const double& length = 10.0,
	   const double& offset = 1.8,
	   const double& angle = 0.6 );
    
    xXsec( Xsection::Xsec* xc, Transform::Integration* integ );
    
    virtual ~xXsec();      //!< a destructor
    
    //! calculate value of at x
    virtual double operator()( const double& x );
    
    //! calculate value of at ( x, z )
    virtual double operator()( const double& x, const double& z );
    
    //! calcualte value of at ( x )
    virtual double operator()( const std::vector< double >& x );
    
    //! calcualte value of at ( x, z )
    virtual double operator()( const std::vector< double >& x,
			       const std::vector< double >& z );
    

    virtual void setxparams( const std::vector< double >& x );
    virtual void setxparams( const double& x1, const double& x2 = 0.0 ); 
    
    virtual void setzparams( const std::vector< double >& z ){};
    virtual void setzparams( const double& z1, const double& z2 = 0.0 ){}; 
    
  };
  
};

#endif	    /* !XXSEC_HH_ */
