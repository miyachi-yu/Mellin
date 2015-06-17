#ifndef _Fitting_A1hFit_hh_
#define _Fitting_A1hFit_hh_

#include <Utility/Arguments.hh>
#include <ExpData/ExpData.hh>

#include <Fragmentation/Evolution.hh>
#include <unpolPDF/Evolution.hh>
#include <polPDF/Evolution.hh>

#include <unpolDIS/StrFunc.hh>
#include <polDIS/StrFunc.hh>
#include <unpolSIDIS/StrFunc.hh>
#include <polSIDIS/StrFunc.hh>

#include "Fitting.hh"

#include <vector>
#include <map>

namespace Fit{
  
  /*!
    \brief cross section fitting class for hadron production in e+-e-
    
    This is an example implementation of Fitting superseeded class.
   
   */
  class A1hFit : public Fit::Fitting  {
  public:
    
    //< a constructor
    A1hFit( Utility::Arguments& args, 
	    const bool& judge = true ) throw( int ) ;
    //!< a destructor
    //!< del process of private objs is always excuted in this class
    virtual ~A1hFit();

    virtual bool calc( FitData* fd ) ;
    virtual bool calc( const int& i ); //!< calc for i-th data
    
    //! dump data comparison
    virtual void data_summary( std::ostream& os );

    //!< load experiment data
    virtual void loadData( Exp::DataSet& dset );
    
    //  -------------  methods to be called after fitting  ---------------
    virtual void postLoop();        //!< after chi2 calculation
    virtual bool postFit( Exp::DataSet& ds );         //!< after fitting
    virtual bool postErrorMatrix( Exp::DataSet& ds ); //!< after fitting ( error )

    //! positivity condition : q( x, Q^2_0 ) > | \Delta q( x, Q^2_0 ) |
    virtual bool condition();

    bool getpost_conf_judge(){ return post_conf_judge_; }
    
    Evolution::PDFx*& xpolpdf(){ return xpolpdf_; }
    Evolution::PDFx*& xunpolpdf(){ return xunpolpdf_; }

    //! config process in inherited (1st generation) class
    //! necessity of conf process operation in the base class beforehand
    //! objects created through this function are always deleted
    //! in the deconstructor of this class
    //! MOVED FROM PROTECTED FOR MultiFrag_A1hFitLM 
    //! (distribution manipulation (exchange before StrFunc constructions) )
    void post_config_process() throw( int );

    //! clear cache
    //! MOVED FROM PROTECTED 
    //! (ORIGINALLY PUBLIC FUNC IN Fitting.cc. ANY MEANING FOR PROTECTED DEFF.??)
    virtual void postUpdate();

  protected:
    
    //!  get output file name 
    virtual std::string ofilename( const int& index );
    
    //! moved from private for A1hFitLM    
    Evolution::PDFx* xpolpdf_;
    Evolution::PDFx* xunpolpdf_;

  private:
    
    Fragmentation::Evo* FFevo_;          //!< Evolution Kernel
    
    std::vector< polDIS::StrFunc* >           g1_;
    std::vector< Xsection::xXsec* >          g1x_;

    std::vector< polDIS::StrFunc* >          dg1_;
    std::vector< Xsection::xXsec* >         dg1x_;

    std::vector< unpolDIS::StrFunc* >         f1_;
    std::vector< Xsection::xXsec* >          f1x_;
    
    std::vector< std::vector< polSIDIS::StrFunc* > >  g1h_;
    std::vector< std::vector< Xsection::xzXsec* > >  g1hx_;
    std::vector< std::vector< Xsection::xXsec* > >  g1hxx_;
    
    std::vector< std::vector< polSIDIS::StrFunc* > >  dg1h_;
    std::vector< std::vector< Xsection::xzXsec* > >  dg1hx_;
    std::vector< std::vector< Xsection::xXsec* > >  dg1hxx_;
    
    std::vector< std::vector< unpolSIDIS::StrFunc* > >  f1h_;
    std::vector< std::vector< Xsection::xzXsec* > >    f1hx_;
    std::vector< std::vector< Xsection::xXsec* > >    f1hxx_;
    
    std::vector< double > x_;
    std::vector< double > z_; // for SIDIS //
    
    QCD::Flavor::HADRON  h_;             //!< final state hadron type
    QCD::Flavor::HADRON  hanti_;         //!< final state anti hadron type
    
    //!  0:  p,   1:  n,  2:  d
    std::vector< int >    type_;         //!< data type
    
    /*!
      -1   DIS
      0    pi+
      1    ka+
      2    rest
      3    h+
      4    pi-
      5    ka-
      6    rest-
      7    h-
    */
    std::vector< int > dsettype_;        //!< dataset type
    
    std::vector< double > zrange_;       //!< zrange for A1h integration
    std::vector< double > vx_;           //!< vector x for A1h

    //!< post config process excution flag (criteria for duplicate checking 
    //!< and executiong delete process)
    bool post_conf_judge_;

    void zrange( const double& zmin, const double& zmax );    
    
    std::vector< double > f1cal_;  //!< calculated F1  value
    std::vector< double > f1hcal_; //!< calculated F1h value
    std::vector< double > original_; //!< original for diff mode
    
    double useXXsec_;
    
    void load_options();
    void setup_DIS() throw( int ); 
    void setup_SIDIS() throw( int ); 
    void setup_SIDIS( const Xsection::Charge::TARGET& n ) throw( int );
    void setup_SIDIS( const Xsection::Charge::TARGET& n,  
		      const QCD::Flavor::HADRON& h ) throw( int ); 
    
    
    // ----------------------------------------------------------------- //
    //                    integration contour parameters                 //
    // ----------------------------------------------------------------- //
    double ppdfOffset_;
    double ppdfAngle_;
    double ppdfLength_;
    
    double updfOffset_;
    double updfAngle_;
    double updfLength_;
    
    double ffOffset_;
    double ffAngle_;
    double ffLength_;
    
    void calcF1();
    
    void g1x_path(  const double& x, Xsection::xXsec* xsec );
    void g1hx_path( const double& x, Xsection::xXsec* xsec );
    
    //! delete process of the objects created in post config process
    void post_delete_process();

  };
  
};
#endif // _Fitting_A1hFit_hh_
