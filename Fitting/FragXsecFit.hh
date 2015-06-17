// $Id: FragXsecFit.hh,v 1.20 2009/01/27 10:31:42 miyachi Exp $
/*!
  \file    FragXsecFit.hh
  \version $Revision: 1.20 $
  \date    $Date: 2009/01/27 10:31:42 $
  \author  $Author: miyachi $
  \brief   class definition for e+-e- hadron production cross-section fitting
 */
#ifndef _Fitting_FragXsecFit_hh_
#define _Fitting_FragXsecFit_hh_

#include <Utility/Arguments.hh>
#include <Tranform/LaguerreQuadrature.hh>
#include <ExpData/ExpData.hh>
#include <Evolution/KernelSum.hh>
#include <Fragmentation/Evolution.hh>
#include <Xsec/Xsec.hh>
#include <Xsec/xXsec.hh>
#include <FragXsec_T/FragXsec.hh>

#include "Fitting.hh"

#include <vector>
#include <map>

namespace Fit {
  
  /*!
    \brief cross section fitting class for hadron production in e+-e-
    
    This is an example implementation of Fitting superseeded class.
   
   */
  class FragXsecFit : public Fit::Fitting  {
  public:
    
    //< a constructor
    FragXsecFit( Utility::Arguments& args ) throw( int ) ;
    virtual ~FragXsecFit();             //!< a destructor

    virtual bool calc( const int& i );  
    
    virtual bool calc( FitData* fd );

    virtual void postLoop() {}

    /*!
      \par [in] zmin is a lower limit
      \par [in] zmax is a upper limit
      \brief set z range to be fitted
    */
    void zrange( const double& zmin, const double& zmax );
    
    //! dump data comparison
    virtual void data_summary( std::ostream& os );
    
    //! load experiment data
    virtual void loadData();

    //! load steepest contour
    void loadSteepestContour();

    //! method to be called after fitting
    virtual bool postFit();
    
    //! method to be called after fitting, when error matrix is
    //! determined acculately.
    virtual bool postErrorMatrix();
    
    //! sum_H D^H_q < 1 condtion 
    virtual bool condition();

  protected:
    
    //!  get output file name 
    virtual std::string ofilename( const int& index );
    
    //! clear cache
    virtual void postUpdate();

  private:
    
    std::vector< std::vector< FragXsec_T::FragXsec* > > xsec_;   //!< cross section model
    std::vector< std::vector< Xsection::xXsec* > >     f_;      //!< xsection in z space
    
    std::vector< double > z_;                     //!< mid-point of z
    std::vector< double > dz_;                    //!< z-bin width
    std::vector< std::vector< double > > vz_;     //!< zrange
    std::vector< int >    type_;                  //!< data type
    std::vector< int >    htype_;                 //!< hadron type

    std::vector< double > zrange_;                //!< z range used for fitting
    
    Transform::LaguerreQuadrature lag_;              //!< Laguerre Quadrature
    std::string dKey_;

    
    //! Steepest Contour
    std::vector< std::vector< std::map< double, 
					std::vector< double > > > > cmap_; 
    bool steepestContourStatus_;
    
    void loadSteepestContour( const std::string& fname );
    void saveSteepestContour( const std::string& fname );
    
    Xsection::xXsec* xf( Xsection::Xsec *xsec );
    
    int htype( const std::string& name );
    int dataType( const std::string& name );
    
    std::vector< std::vector< bool > > fragData_;
    
  };
  
};
#endif // _Fitting_FragXsecFit_hh_
