//$Id: xXsec.hh,v 1.10 2009/07/28 05:44:48 miyachi Exp $
/*!
  \file  xXsec.hh
  \brief class definition for cross section calculation in x space
 */
#ifndef _Xsec_xXsec_hh_
#define _Xsec_xXsec_hh_

#include <vector>
#include <Tranform/InverseMellin.hh>
#include <ExpData/ExpData.hh>
#include <ExpData/DataSet.hh>
#include <ExpData/DataPoint.hh>

namespace Xsection {
  
  class Xsec;
  
  /*!
    \class xXsec
    \brief class for cross section calculation in x space
  */
  class xXsec : public Transform::InverseMellin {
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
      
      In order to construct xXec object, one need at least a pointer
      to the Xsec object which calculate moment of cross section.
      
    */
    xXsec( Xsec *xc, 
	   const int& low = 4, const int& high = 6, 
	   const double& precision = 1.0E-4,
	   const double& length = 10.0,
	   const double& offset = 1.8,
	   const double& angle  = 0.6 );

    /*!
      \param[in] xc is a Xsection::Xsec to be inverted
      \param[in] integ is a pointer of Integration object
      \brief  a constructor specifying Integration method
      
      The given integ Integration object will be used for 
      the Mellin inversion.

    */
    xXsec( Xsec *xc, Transform::Integration *integ );
    
    virtual ~xXsec();      //!< a destructor
    
    //! get a pointer to Xsec object
    Xsec* moment() { return xsec_; }

    //! set a Xsec object pointer
    void moment( Xsec* xsec ) { xsec_ = xsec; }
    
    void setGngrid( const int& ngrid );

    void setloginteg( const bool& logint );

    double chiSquare( Exp::DataSet&   ds );
    double chiSquare( Exp::DataPoint& dp );
    
  protected:

    virtual double eval( Exp::DataPoint& dp ) { return 0.0; }
    
  private:
    
    Xsec* xsec_;  //!< a pointer to Xsec object
    
    //! initalize Mellin Inversion intergration
    void initialization( const int& low = 4, const int& high = 6, 
			 const double& precision = 1.0E-4,
			 const double& length = 10.0,
			 const double& offset = 1.8,
			 const double& rangle = 0.6 );
    
    std::vector< Transform::Integration* > objList_;

  };
  
};
#endif // _Xsec_xXsec_hh_
