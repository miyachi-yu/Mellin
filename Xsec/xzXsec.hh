//$Id: xzXsec.hh,v 1.5 2008/10/23 04:50:51 miyachi Exp $
/*!
  \file  xzXsec.hh
  \brief class definition for cross section calculation in x space
 */
#ifndef _Xsec_xzXsec_hh_
#define _Xsec_xzXsec_hh_

#include <Tranform/DInverseMellin.hh>

namespace Xsection {
  
  class Xsec;
  
  /*!
    \class xzXsec
    \brief class for cross section calculation in x space
   */
  class xzXsec : public Transform::DInverseMellin {
  public:

    /*!
      \param[in] xc is a pointer to Xsec object
      \param[in] low is a lower number of nodes for gauss integration
      \param[in] high is a upper number of nodes for gauss integration
      \param[in] precision is a required precision for the integration
      \param[in] length is a integration path length
      \param[in] offset is a offset value for the path integral
      \param[in] angle is a angle between integration path and real axis
      \brief a constructor of cross section calculation class

      In order to construct xzXec object, one need at least a pointer
      to the Xsec object which calculate moment of cross section.

     */
    xzXsec( Xsec *xc, 
	   const int& low = 4, const int& high = 6, 
	   const double& precision = 1.0E-4,
	   const double& length = 10.0,
	   const double& offset = 1.8,
	   const double& angle = 0.6 );

    xzXsec( Xsec* xc, Transform::Integration* integ );
    
    virtual ~xzXsec();      //!< a destructor
    
    //! get a pointer to Xsec object
    Xsec* moment() { return xsec_; }
    
    void setGngrid( const int& ngrid );

    void setloginteg( const bool& logint );

  private:
    
    Xsec* xsec_;  //!< a pointer to Xsec object
    
    //! initalize Mellin Inversion intergration
    void initialization( const int& low = 4, const int& high = 6, 
			 const double& precision = 1.0E-4,
			 const double& length = 10.0,
			 const double& offset = 1.8,
			 const double& rangle = 0.6 );
    
  };
  
};
#endif // _Xsec_xzXsec_hh_
