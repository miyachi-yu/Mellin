// $Id: UDratioFit.hh,v 1.1 2013/06/25 01:00:32 miyachi Exp $
/*!
  \file    UDratioFit.hh
  \version $Revision: 1.1 $
  \date    $Date: 2013/06/25 01:00:32 $
  \author  $Author: miyachi $
  \brief   class definition for e+-e- hadron production cross-section fitting
 */
#ifndef _Fitting_UDratioFit_hh_
#define _Fitting_UDratioFit_hh_

#include <Utility/Arguments.hh>
#include <Tranform/LaguerreQuadrature.hh>
#include <ExpData/ExpData.hh>
#include <Evolution/KernelSum.hh>
#include <Evolution/PDFx.hh>

#include <Fragmentation/Evolution.hh>
#include <Xsec/Xsec.hh>
#include <Xsec/xXsec.hh>
#include <FragXsec_T/FragXsec.hh>

#include "Fitting.hh"
#include "UDratio.hh"

#include <vector>
#include <map>

namespace Fit {
  
  /*!
    \brief cross section fitting class for hadron production in e+-e-
    
    This is an example implementation of Fitting superseeded class.
   
   */
  class UDratioFit : public Fit::Fitting  {
  public:
    
    //< a constructor
    UDratioFit( Utility::Arguments& args ) throw( int ) ;
    virtual ~UDratioFit();             //!< a destructor

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
    virtual void loadData( Exp::DataSet& dset );

    //! method to be called after fitting
    virtual bool postFit( Exp::DataSet& ds );
    
    //! method to be called after fitting, when error matrix is
    //! determined acculately.
    virtual bool postErrorMatrix( Exp::DataSet& ds );
    
    //! sum_H D^H_q < 1 condtion 
    virtual bool condition();

  protected:
    
    //!  get output file name 
    virtual std::string ofilename( const int& index );
    
    //! clear cache
    virtual void postUpdate();

  private:
    
    std::vector< double > x_;                     //!< mid-point of x
    std::string dKey_;

    UDratio* ratio_;
    
  };
  
};
#endif // _Fitting_UDratioFit_hh_
