/*!
  \brief  Multiplicity data fitting class definition
 */
#ifndef _Fitting_MultiFragFit_hh_
#define _Fitting_MultiFragFit_hh_

#include <Utility/Arguments.hh>
#include <Tranform/LaguerreQuadrature.hh>
#include <ExpData/ExpData.hh>
#include <Fragmentation/Evolution.hh>
#include <unpolPDF/Evolution.hh>
#include <Xsec/Xsec.hh>
#include <Xsec/xXsec.hh>
#include <FragXsec_T/FragXsec.hh>
#include <unpolSIDISXsec/StrFunc.hh>
#include <unpolSIDISXsec/StrFuncSub.hh>
#include <unpolDISXsec/StrFunc.hh>

#include "Fitting.hh"

#include <vector>
#include <map>

namespace Fit{
  
  /*!
    \brief Multiplicity and Hadron inclusive cross section fitting class
  */
  class MultiFragFit : public Fit::Fitting  {
  public:
    
    //!< a constructor
    //!< second argument for config process operation
    MultiFragFit( Utility::Arguments& args, 
		  const bool& judge = true ) throw( int ) ;
    //!< a destructor
    //!< del process of private objs is always excuted in this class
    virtual ~MultiFragFit();
    
    //!< calculate for i-th data ( fill fit_[ i ] )
    virtual bool calc( const int& i );
    
    virtual bool calc( FitData* fd );

    //!< call after chi^2 calulation loop
    virtual void postLoop();
    
    //! dump data comparison
    virtual void data_summary( std::ostream& os );
    
    //! load experiment data
    virtual void loadData( Exp::DataSet& dset );
    
    //! method to be called after fitting
    virtual bool postFit( Exp::DataSet& ds );
    
    /*!
      method to be called after fitting, when error matrix is
      determined acculately.
    */
    virtual bool postErrorMatrix( Exp::DataSet& ds );

    //! sum_H D^H_q < 1 condtion 
    virtual bool condition();

    bool getpost_conf_judge(){ return post_conf_judge_; }

    //! config process in inherited (1st generation) class
    //! necessity of conf process operation in the base class beforehand
    //! objects created through this function are always deleted
    //! in the deconstructor of this class
    //! MOVED FROM PROTECTED FOR (possible) future XXX_MultiFragFitLM 
    //! (distribution manipulation (exchange before StrFunc constructions) )
    void post_config_process() throw( int );

    //! clear cache
    //! MOVED FROM PROTECTED 
    //! (ORIGINALLY PUBLIC FUNC IN Fitting.cc. ANY MEANING FOR PROTECTED DEFF.??)
    virtual void postUpdate();

  protected:
    
    //!  get output file name 
    virtual std::string ofilename( const int& index );
    
  private:
    
    //! xsection model
    std::vector< std::vector< FragXsec_T::FragXsec* > > xsec_;

    //! xsection in z space
    std::vector< std::vector< Xsection::xXsec* > >      f_;
    
    //! multiplicity numerator model
    std::vector< std::vector< unpolSIDISXsec::StrFunc* > > xmnsec_;  
    std::vector< std::vector< unpolSIDISXsec::StrFuncSub* > > xmnsec0_;  
    std::vector< std::vector< unpolSIDISXsec::StrFuncSub* > > xmnsec1_;  
    std::vector< std::vector< unpolSIDISXsec::StrFuncSub* > > xmnsec2_;  
    
    //! multiplicity numerator in z space
    std::vector< std::vector< Xsection::xzXsec* > >        fmn_;
    std::vector< std::vector< Xsection::xzXsec* > >        fmn0_;
    std::vector< std::vector< Xsection::xzXsec* > >        fmn1_;
    std::vector< std::vector< Xsection::xzXsec* > >        fmn2_;
    
    //! multiplicity numerator in z space
    std::vector< std::vector< Xsection::xXsec* > >        xfmn_;
    std::vector< std::vector< Xsection::xXsec* > >        xfmn0_;
    std::vector< std::vector< Xsection::xXsec* > >        xfmn1_;
    std::vector< std::vector< Xsection::xXsec* > >        xfmn2_;
    
    //!< multiplicity denominator model
    std::vector< unpolDISXsec::StrFunc* > xmdsec_;   

    //!< multiplicity denominator in z space
    std::vector< Xsection::xXsec* >       fmd_;      
    
    std::vector< std::vector< double > > vz_; 
    std::vector< double > z_;                //!< mid-point of z
    std::vector< double > dz_;               //!< z-bin width
    std::vector< int >    type_;             //!< data type
    std::vector< int > dsettype_;            //!< dataset type
    std::vector< bool > frag_;               //!< fragXsec or multiplicity

    std::vector< double > zrange_;   //!< z range used for fitting
    
    std::map< int, std::vector< double > > vzran_;  //!< z range used for fitting
    
    std::vector< std::vector< double > > vx_; //!< x range
    std::vector< double > xave_;              //!< from Achim
    std::vector< double > zave_;              //!< from Achim

    std::vector< double > s_; //!< s for multiplicity
    
    bool useAverage_; //!< calculate at average kinematic
    
    std::vector< double > denom_;  //!< denominator for Multiplicity

    bool useXXsec_;                         //!< use xXsec instead of xzXsec

    std::vector< std::vector< bool > > fragData_; //!< Exp data status
    std::vector< std::vector< bool > > multData_; //!< Exp data status

    std::vector< double > xXsecLength_; //!< path length for xXsec
    
    bool useXsecSub_; //!< use unpolSIDISXsec::StrFuncSub instead.
 
    std::vector< double > xx_;
    std::vector< double > zz_;

    std::vector< double > fragc2w_; //!< chi2 weight for each hadron type
    std::vector< double > multc2w_; //!< chi2 weight for each hadron type

    //!< post config process excution flag (criteria for duplicate checking 
    //!< and executiong delete process)
    bool post_conf_judge_;
    
    // ------------------------------------------------------ //
    //             Private member method                     //
    // ------------------------------------------------------ //
    
    //! calculate only denominator for multiplicity
    void calc_denominator(); //!< calculate denominator
    
    /*!
      get appropriate xbin range ( xmin, xmax ) for multiplicity calc
      which can vary Q2 value.
      this correspondance should be confirmed by Achim.
    */
    std::vector< double > getx( const double& q2 );
    
    /*!
      \par [in] zmin is a lower limit
      \par [in] zmax is a upper limit
      \brief set z range to be fitted
    */
    void zrange( const double& zmin, const double& zmax );    
    
    //! setup FragXsec_T::FragXsec object 
    void setup_FragXsec( const QCD::Flavor::HADRON& h ) throw( int );

    //! setup FragXsec_T::FragXsec object 
    void setup_FragXsec() throw( int );

    //! setup Mellin inversion of FragXsec_T::FragXsec object 
    void setup_FragXsec_inverse() throw( int );

    //! setup Xsection object for Multiplicity calculation
    void setup_Multiplicity( const Xsection::Charge::TARGET& n,
			     const QCD::Flavor::HADRON& h ) throw( int ); 

    //! setup Xsection object for Multiplicity calculation
    void setup_Multiplicity( const Xsection::Charge::TARGET& n ) throw( int );

    //! setup Xsection object for Multiplicity calculation
    void setup_Multiplicity() throw( int ); 

    //! setup Mellin inversion of Xsection object for Multiplicity calculation
    void setup_Multiplicity_inverse() throw( int ); 
    
    // set integration contour for unpolSIDISXsec
    void set_contour( const double& x, Xsection::xXsec* xsec );
    
    double sigma_h( const int& it, const int& ih,
		    const double& q2, 
		    const double& s, 
		    const std::vector< double >& x,
		    const std::vector< double >& z );
    
    double dsigma_h( const int& it, const int& ih,
		     const double& q2, 
		     const double& s,
		     const std::vector< double >& x,
		     const std::vector< double >& z,
		     Exp::DataSet* ds = NULL );


    //!< configure zragne for different hadrons
    void config_vzrange();

    void config_c2weight();

    //! delete process of the objects created in post config process
    void post_delete_process();

  };
  
};
#endif // _Fitting_MultiFragFit_hh_
