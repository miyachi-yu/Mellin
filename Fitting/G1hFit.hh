// $Id: G1hFit.hh,v 1.3 2008/11/26 10:11:10 miyachi Exp $
/*!
  \file   G1hFit.hh
 */
#ifndef _Fitting_G1hFit_hh_
#define _Fitting_G1hFit_hh_

#include "G1Fit.hh"

#include <Fragmentation/Evolution.hh>
#include <polSIDIS/StrFunc.hh>
#include <Xsec/xzXsec.hh>

namespace Fit {
  
  /*!
    \brief   a class for \f$g_1\f$ and \f$g_1^h\f$  fit
  */
  class G1hFit : public G1Fit {
  public:
    
    G1hFit() throw( int ) ;                        //!< a default constructor
    virtual ~G1hFit();                             //!< a destructor
    
    virtual bool calc( const int& i );
    virtual void postLoop() {}
    virtual void data_summary( std::ostream& os ); //!< data summary
    virtual void postUpdate();                     //!< post update
    virtual void loadData();                       //!< load data
    
    //! method to be called after fitting
    virtual bool postFit();
    
    //! method to be called after fitting, when error matrix is
    //! determined acculately.
    virtual bool postErrorMatrix();
    
  private:
    
    std::vector< Fragmentation::Evo* > ff_;                 //!< FF
    std::vector< std::vector< polSIDIS::StrFunc* > > g1h_;  //!< g1h( n, m )
    std::vector< std::vector< Xsection::xzXsec* > > xg1h_;  //!< g1h( x, z )
    
  };
  
};

#endif // _Fitting_G1hFit_hh_
