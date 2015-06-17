// $Id: G1Fit.hh,v 1.9 2008/11/26 10:11:10 miyachi Exp $
/*!
  \file    G1Fit.hh
  \version $Revision: 1.9 $
  \date    $Date: 2008/11/26 10:11:10 $
  \author  $Author: miyachi $
  \brief   class definition for e+-e- hadron production cross-section fitting
 */
#ifndef _Fitting_G1Fit_hh_
#define _Fitting_G1Fit_hh_

#include <polPDF/Evolution.hh>
#include <Xsec/Xsec.hh>
#include <Xsec/xXsec.hh>
#include <polDIS/StrFunc.hh>

#include "Fitting.hh"

#include <vector>

namespace Fit {
  
  /*!
    \brief cross section fitting class for hadron production in e+-e-
    
    This is an example implementation of Fitting superseeded class.
    
  */
  class G1Fit : public Fit::Fitting  {
  public:
    
    //< a constructor
    G1Fit() throw( int ) ;
    virtual ~G1Fit();                   //!< a destructor

    virtual bool calc( const int& i );
    
    virtual void postLoop() {}

    //! dump data comparison
    virtual void data_summary( std::ostream& os );
    
    //! load experiment data
    virtual void loadData( );
    
    /*!
      \par [in] xmin is a lower limit
      \par [in] xmax is a upper limit
      \brief set x range to be fitted
    */
    void xrange( const double& xmin, const double& xmax );
    
    //! method to be called after fitting
    virtual bool postFit();
    
    //! method to be called after fitting, when error matrix is
    //! determined acculately.
    virtual bool postErrorMatrix();
    
  protected:
    
    //!  get output file name 
    virtual std::string ofilename( const int& index );
    
    //! clear cache
    virtual void postUpdate();
    
    std::vector< Xsection::Xsec* >   g1_;   //!< cross section model
    std::vector< Xsection::xXsec* >  xg1_;  //!< cross section model
    
    std::vector< double >            x_;    //!< Bjorken x
    std::vector< int >               type_;
    std::vector< double >            xrange_;
    
  };
  
};
#endif // _Fitting_G1Fit_hh_
