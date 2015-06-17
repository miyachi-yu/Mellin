// $Id: Fitting.hh,v 1.80 2013/06/13 01:52:07 miyachi Exp $
/*!
  \file    Fitting.hh
  \version $Revision: 1.80 $
  \author  $Author: miyachi $
  \date    $Date: 2013/06/13 01:52:07 $
  \brief   class definition of Fitting class
 */
#ifndef _Fitting_Fitting_hh_
#define _Fitting_Fitting_hh_

#include <TMinuit.h>
#include <TGraph.h>
#include <TH1.h>
#include <TH2.h>

#include <vector>
#include <string>
#include <iostream>
#include <map>

#include <Utility/Arguments.hh>
#include <Utility/Timer.hh>
#include <Utility/Thread.hh>

#include <Tranform/Pvalue.hh>

#include <QCD/Parameter.hh>
#include <QCD/PdfErrMatrix.hh>

#include <ExpData/ExpData.hh>

#include <Evolution/Kernel.hh>
#include <Evolution/KernelSum.hh>
#include <Evolution/KernelDiff.hh>

#include <Fragmentation/Evolution.hh>
#include <unpolPDF/Evolution.hh>
#include <polPDF/Evolution.hh>

#include <Xsec/Xsec.hh>
#include <Xsec/xXsec.hh>
#include <Xsec/xzXsec.hh>

#include <unpolDIS/StrFunc.hh>
#include <unpolSIDIS/StrFunc.hh>
#include <unpolDISXsec/StrFunc.hh>
#include <unpolSIDISXsec/StrFunc.hh>
#include <unpolSIDISXsec/StrFuncSub.hh>
#include <polDIS/StrFunc.hh>
#include <polSIDIS/StrFunc.hh>

#include <FragXsec_T/FragXsec.hh>

#include <ErrorCalc/ErrCalc.hh>

#include "FitData.hh"

namespace QCD {
  class PdfBase;
}

namespace Fit {
  
  /*!
    \brief    Fitting class based on TMinuit
    
    How to use this Fitting class.

    \par Basic description
    -  inherit Fit::Fitting class 
     => overwrite chi2 function which return the value to be minimize
    -  tell the inherited class object which Xsection::Xsec model to be used,
       with setModel method.
     => this method try to define fitting parameters used in initial distributions.
    -  call Minuit routine.  like Migrad()

    \par --Minuit option
    One can also execute Minuit commands directly. The commands shold be 
    given with --Minuit option. The commands must be given separated with
    "|", like  --Minuit="MINIMIZE|MIGRAD|MINOS|HESSE".
    One can also give path to text file with --Minuit option. The given
    file should contain list of Minuit command. It sould be equivalent
    to a Minuit control file used with Fortran original library.
    One can use #, * for comment lines.
    
    \par  --contour option
    Contour plots in two parameter space can be obtained with 
    Fitting::contour( int par1, int par2, int n = 40 ) method.
    It create 2-D histogram and fill with chi^2 varing parameters,
    par1 and par2, keeping other parameters same.
    2-D histogram object is realized with TH2F class in ROOT package,
    so that .....

    \par Basic principle
    The fitting is performed on comparison of Xsection::Xsec level;
    i.e., cross seciton, structure functions. However, parameterization
    is usually in Evolution::Kernel level.
    This Fitting class handles "Parameter set" with Evolution::Kerenl.
    User must tell which Kernel should be handlled by Fitting object 
    with 
    Fitting::set( Evolution::Kernel *k )
    One can also use
    Fitting::set( Xsection::Xsec *xsec )
    instead. In this case, all Evelution::Kernel in the given Xsection::
    Xsec, will be registered.

    \par \f$\chi^{2}\f$ definition
    \f$\chi^{2}\f$ calculated in Fitting class is
    \f[
    \chi^{2} = 
    \sum_{ i } \left( \right)
    \f]

    \par parameter update, cache clear
    Each fitting routine, parameter will be updated by TMinuit.
    Since Xsection::Xsec and Evolution::Kernel have own data cache,
    it must be cleared. (Because these data depend on paramters.)
    Kernel cache will be automatically cleared, on update. 
    However, since Fitting class do now know what Xsection::Xsec
    model  is now in used. Instead to clear directly, Fitting will 
    cann Fitting::postUpdate() function, after modifing parameters.
    So inherited class must implement postUpdate and clear cache 
    in the method.

    \par paramters
    Paramters are stored as double variable in PdfBase object. 
    So one only need to give the address to TMinuit with 
    TMinuit::DefineParameter method. In order to keep futhre
    accessibility, the parameter pointers are stored locally.
    With the local stored information, the paramter will be 
    registered. Some PdfBase inherited class must be called after
    re-definition of paramter values. So that, the pointers to 
    the PdfBase are also stored. This helps to call only updated
    PdfBase class.

    \par error matrix
    One can obtain error matrix from Minuit and attach the 
    matrix to the initial distribution. At the same time,
    the fitted error on the experiment data will be stored
    also in ExpData object. 

    In order to invoke error matrix handling routine,
    one must give --errorMatrix. Required error matrix estimation
    leven can be modified with --emlevel=[level], for which the details 
    can be found in MINUIT manual, ( from 0 to 3 ).
    With --oexpdata=[filename], one can output ExpData with
    fitted value and error if requreid.

    \par statistical correlation among data points

    HERMES provides statistical correlation among data points
    as covariance matrices, \f$ \rho_{i,j} \f$. 
    

  */
  class Fitting : public TMinuit, public Utility::Thread {
    
  public:
    
    /*!
      a default constructor
      second argument for config process operation
    */
    Fitting( Utility::Arguments& args, 
	     const bool& judge = true ) throw( int );        

    /*!                        
      a destructor
      delete process of the objects, associated with several creations 
      in config process, is always executed always in this base class...
      (not proper actually, but just for coding convenience in inherited 
      classes (unnecessity of special care of its deconstructor) with 
      different config processes)
    */
    virtual ~Fitting(); 

    // ---------------------------------------------------------------- //
    //                  functions to be over-loaded....
    // ---------------------------------------------------------------- //
    
    /*!
      \return \f$\chi^2\f$ value
      \brief  calculate \f$\chi^2\f$ between model and data.
      
      This will calculate total \f$\chi^2\f$ between model and data
      using Fitting::calc( const int& i ) method,
      in which model value should be calculated for the given i-th
      data point and the result should be stored to fit_[ i ] value.
      
     */
    virtual double chi2();
    
    /*!
      \brief calculate chi2 from i-th to j-th data
      
      This function will call Fitting::calc( const int& i ) 
      over the given signal range from start end end as,
      for( int i = start; i < end; i++ ){ this->calc( i ); }
    */
    virtual void chi2( const int& start, const int& end );
    
    /*!
      \param[in] i is a number of data to be calculated
      \return true if the model value was calculated.
      \brief calculate model value for i-th data.
      
      Model calculation must be done for the given i-th data point.
      The result should be stored to the private member variable,
      fit_[i].
      
      If the model calculation is not carried out due to some 
      reason, the function return false. 

     */
    virtual bool calc( const int& i ) = 0 ;

    /*!
     */
    virtual bool calc( FitData* fd ) = 0 ;
    
    /*!
      \brief  check if persent fitting condition is fine or not.
      
      This method will be called at the begining of \f$ \chi^2 \f$
      calculation. If present fitting parameters are fine,
      this must return true. 
      
      If this method return false, large value Fitting::CHI2HUGE
      will be set to 
      present \f$ \chi^2 \f$.
     */
    virtual bool condition() ;
    
    /*!
      \brief  the method to be called after chi^2 calculation loop
     */
    virtual void postLoop() = 0;
    
    /*!
      \param[in] os is an output stream where data comparison will be dump
      \brief dump information on data comparison
    */
    virtual void data_summary( std::ostream& os ) = 0;
    
    /*!
      \brief to be called after parameter update
      
      this is an abstract method, to be called when the parameters are
      updated. The cache for the evolution kernel will be automatically
      cleared. However, cache of Xsection::Xsec objects won't.
      Inherited class must be clear these cache by themselve.
      This is a function to do so.
      
    */
    virtual void postUpdate() = 0;
    
    /*!
      \brief  load experimental data using ExpData object created.
      
      After creating ExpData object, this function will be called.
      User must implement this method of inherited classes, to prepare
      data set used in the chi2 calculation.
      
    */
    virtual void loadData();
    virtual void loadData( Exp::ExpData* edata );
    
    /*!
      \brief  this will be called from Fitting::loadData
    */
    virtual void loadData( Exp::DataSet& dset ) = 0;

    void loadErrorMatrix( Exp::DataSet& dset );

    // ----------------------------------------------------------------- //
    //! set a pointer to Xsection::Xsec object
    virtual void setModel( Xsection::Xsec *m );
    
    //! set Kernel to be fitted
    virtual void set( Evolution::Kernel *kernel );
    
    //! get a pointer to ExpData object
    //    virtual Exp::ExpData* expr() { return expr_; }
    std::vector< Exp::ExpData* >& vexpr(){ return expr_; }

    //! get number 
    virtual int numberOfData() { return nData_; }
    
    //! print out fitting parameters into file
    virtual void dumpParameters();
    
    //! output basic fit info. summary to the given output stream
    virtual void baseinfo_summary( std::ostream& os );

    //! output summary to the given output stream
    virtual void summary( std::ostream& os );

    //! output error matrix
    virtual void em_assignment( std::ostream& os );

    virtual void fit_data_summary( std::ostream& os );
    
    //! get current error matrix
    QCD::PdfErrMatrix errorMatrix();
    
    //! get verbose state
    virtual bool verbose() { return verbose_; }
    
    //! set verbose state
    virtual void verbose( const bool& state ) { verbose_ = state; }
    
    //! set verbose state
    virtual void runCommands();
    
    //! get number of degree of freedom
    virtual double dof() {
      return this->numberOfData() -1.0 - this->GetNumFreePars();  
    }
    
    //! save contour plot to the external file
    virtual void contour( const int& n = 20 );
    
    /*!
      \param [in]  par1 is a index of parameter 1
      \param [in]  par2 is a index of parameter 2
      \param [in]  n is a number of bin
      \return a pinter of TH2 object which contains chi2 distribution
      \brief create chi2 distribution for par 1 and par 2
      
      2D histogram will be created. stored value in per_ and err_
      will be used to determine the axis range. 
      At the moment the 3 * error will be used both direction.
      
    */
    virtual TH2* contour( int par1, int par2, int n = 20 );
    
    /*!
      \return graph as a result of Lagrange Multiplier method
      \brief  execute Lagrange Multiplier method
      
      This performs 1D Lagrange Multiplier method.
      In order to use this method, 
      user must over-write protected method,
      Fitting::referenceFunction(),
      which returns value of interested observal.
      
      \par What is Lagrange Multiplier method?
      Lagrange Multiplier method minimize parameters \f$ \vec{a} \f$
      \f[
      \Phi( \lambda, \vec{a} ) = 
      \chi^2(\vec{a}) + \lambda * O( \vec{a} )
      \f],
      for various value of \f$ \lambda \f$. 
      
    */
    virtual TGraph* lagrangeMultiplier( const int& n = 20, 
					const double& min = -10.0,
					const double& max =  10.0 );
    
    virtual bool start() ;        //!< start fitting
    virtual bool startMinuit() ;  //!< start fitting

    double getchi2(){ return chi2_; }
    void setchi2( const double& chi2 ){ chi2_ = chi2; }
    double getreschi2(){ return res_chi2_; }
    void setreschi2( const double& reschi2 ){ res_chi2_ = reschi2; }
    double getlambda(){ return lambda_; }

    double getchi2scale(){ return chi2scale_; }
    double setchi2scale( const double& chi2s ){ return chi2scale_ = chi2s; }

    bool getconf_judge(){ return  conf_judge_; }
    
    //! for A1hFit obj in MultiFrag_A1hFitLM
    //! ...sorry for this contamination...(imazu)
    Fragmentation::Evo*&   pion(){ return pion_; }
    Fragmentation::Evo*&   kaon(){ return kaon_; }
    Fragmentation::Evo*&   rest(){ return rest_; }
    Fragmentation::Evo*&   proton(){ return proton_; }

    Evolution::KernelSum*& hadron(){ return hadron_; }

    unpolPDF::Evo*& updf(){ return updf_; }
    polPDF::Evo*&   ppdf(){ return ppdf_; }

    // ==========================================================
    //                    original pdf......
    // ==========================================================
    Fragmentation::Evo*&   o_pion(){ return o_pion_; }
    Fragmentation::Evo*&   o_kaon(){ return o_kaon_; }
    Fragmentation::Evo*&   o_rest(){ return o_rest_; }
    Fragmentation::Evo*&   o_proton(){ return o_proton_; }
    unpolPDF::Evo*&        o_updf(){ return o_updf_; }     
    polPDF::Evo*&          o_ppdf(){ return o_ppdf_; }
    
    // ==========================================================
    //                    difference pdf......
    // ==========================================================
    Evolution::KernelDiff*& d_pion(){ return d_pion_; }
    Evolution::KernelDiff*& d_kaon(){ return d_kaon_; }
    Evolution::KernelDiff*& d_rest(){ return d_rest_; }
    Evolution::KernelDiff*& d_proton(){ return d_proton_; }
    Evolution::KernelDiff*& d_updf(){ return d_updf_; }
    Evolution::KernelDiff*& d_ppdf(){ return d_ppdf_; }

    //! configuration process, which should be called only once
    //! objects created through this function are always deleted 
    //! in the deconstructor of this class
    //! MOVED FROM PROTECTED FOR MultiFrag_A1hFitLM 
    //! (distribution manipulation (exchange before StrFunc constructions) )
    void config_process() throw( int );

  protected:

    //! overwrite TMinuit member function to evaluate chi2
    virtual Int_t Eval( Int_t npar, Double_t* grad, 
			Double_t& fval, Double_t* par, Int_t flag );
    
    //! get a reference to timer object
    virtual Utility::Timer& timer() { return timer_; }
    
    //! get output file name 
    virtual std::string ofilename( const int& index ) = 0;
    
    virtual void defineParameters();               //!< define parameters

    //! dump basic information
    virtual void summary_basic( const int& i, std::ostream& os );
    
    std::vector< std::string >&  names()      { return nam_; } //!< get names
    
    std::vector< std::string > toFit_;
    
    Fragmentation::Evo   *pion_;     //!< Evolution Kernel for pion
    Fragmentation::Evo   *kaon_;     //!< Evolution Kernel for kaon
    Fragmentation::Evo   *rest_;     //!< Evolution Kernel for rest
    Fragmentation::Evo   *proton_;   //!< Evolution Kernel for proton

    Evolution::KernelSum *hadron_;   //!< Evolution Kernel for hadron

    bool ffFit_; //!< true if at least one ff is under control

    unpolPDF::Evo *updf_; //!< Evolution Kernel for unpolarized PDF
    polPDF::Evo   *ppdf_; //!< Evolution Kernel for   polarized PDF

    // ==========================================================
    //                    original pdf......
    // ==========================================================
    Fragmentation::Evo   *o_pion_;     //!< Evolution Kernel for pion
    Fragmentation::Evo   *o_kaon_;     //!< Evolution Kernel for kaon
    Fragmentation::Evo   *o_rest_;     //!< Evolution Kernel for rest
    Fragmentation::Evo   *o_proton_;   //!< Evolution Kernel for proton
    unpolPDF::Evo        *o_updf_;     //!< Evolution Kernel for unpolarized PDF
    polPDF::Evo          *o_ppdf_;     //!< Evolution Kernel for   polarized PDF
    
    // ==========================================================
    //                    difference pdf......
    // ==========================================================
    Evolution::KernelDiff *d_pion_;     //!< Evolution Kernel for pion
    Evolution::KernelDiff *d_kaon_;     //!< Evolution Kernel for kaon
    Evolution::KernelDiff *d_rest_;     //!< Evolution Kernel for rest
    Evolution::KernelDiff *d_proton_;   //!< Evolution Kernel for proton
    Evolution::KernelDiff *d_updf_;     //!< Evolution Kernel for unpolarized PDF
    Evolution::KernelDiff *d_ppdf_;     //!< Evolution Kernel for   polarized PDF
    
    // common data container
    bool useFData_;
    std::vector< FitData* > fData_;

    std::vector< double > sig_;          //!< data
    std::vector< double > dsig_;         //!< data error
    std::vector< double > q2_;           //!< Q^2
    std::vector< std::string > expInfo_; //!< data information
    std::vector< double > fit_;          //!< fit value
    std::vector< double > c2_;           //!< chi^2 for each data point
    std::vector< double > c2weight_;     //!< chi^2 for each data point
    std::vector< bool > sigUsed_; //!< data used in chi2 calculation
    
    //! pointer to DataSet object
    std::vector< Exp::DataSet* > dset_; 

    //! pointer to DataPoint object
    std::vector< Exp::DataPoint* > point_;
    
    bool dataNormalization_;             //!< data normalization handling flag
    std::vector< Exp::DataSet* > rescaledData_; //!< dataset under control

    // ---------------------------------------------------------------- //
    // parameters for fitting procedure.....
    std::vector< QCD::Parameter* >  params_;//!< paramters under Minuit control
    std::vector< std::string >      nam_;    //!< name for above parameters

    int nData_;                          //!< number of data used in chi2
    
    double q2limit_;                     //!< lower limit of hard scale Q^2
    
    double offset() { return offset_; }  //!< get offset of integ path
    double angle()  { return angle_; }   //!< get angle of integ path
    double length() { return length_; }  //!< get length of integ path
    
    void offset( const double& v ) { offset_ = v; } //!< set offset
    void angle( const double& v )  { angle_  = v; } //!< get angle 
    void length( const double& v ) { length_ = v; } //!< get length
    
    /*!
      \param[in] state is a status flag for Lagrange Multiplier method
      \brief set to enable/disable Lagrange Multiplier method
    */
    void multiplier( const bool& state ) { multiplier_ = state; }
    
    //! set weight for a reference function ( for Lagrange Multiplier )
    void multiplier( const double& lambda ) { lambda_ = lambda; }
    
    /*!
      \return a value of reference function for Lagrange Multiplier
    */
    virtual double referenceFunction() { return 0.0; }
    
    //! method to be called after fitting
    virtual bool postFit( Exp::DataSet& dset ) = 0 ;
    
    /*!
      \brief method to be called after fitting for error matrix
      
      This function will be invoked:
      1.   --postErrorMatrix is given in the arguments
      2.   --errorMatrix is given in the arguments
      3.   Fitting terminate with istat which is equal or larget than 
           --emlevel=(defalut:1)
    */
    virtual bool postErrorMatrix( Exp::DataSet& dset ) = 0 ;

    //! get errorMatrix flag
    bool emflag() { return emflag_; }
    
    //! get kernels under control
    std::vector< Evolution::Kernel* >& kernels() { return kernel_; }

    //! for error calc including data nomarilation effect
    void setNormMp( ErrorCalc::ErrCalc* ecal, Exp::DataSet* ds );

    //! output full error matrix including data nomarilations
    void outMatrix( ErrorCalc::ErrCalc* ecal );

    //! calculate error without fitting
    bool errorDirect_;

    //! calculate scale without fitting
    bool scaleDirect_;

    //! skip fit procedure flag
    bool skipfit_;
    
    //! check parameter condition before chi2 calculation
    bool checkFitCondition_;
    
    bool difference_; //!< diffairence mode
    double diffPrec_; //!< precision for difference calculation

    //! update PDF and clear cached data
    virtual void updatePdf();

    //! analyze Minuit command option
    virtual void minuitCmds( std::string cmd );

    //! p-value calculator
    Transform::Pvalue pvalue_;

  private:
    
    //! create FF evolution kernel object
    Fragmentation::Evo* ff( const QCD::Flavor::HADRON& h,
			    const bool& original = false ) throw( int ) ;
    
    //! create unpolPDF evolution kernel object
    unpolPDF::Evo* unpolPDF() throw( int ) ;
    
    //! create unpolPDF evolution kernel object
    polPDF::Evo*     polPDF() throw( int ) ;
    
    //! mirror the Minuit parameter to the model
    virtual void mirror();

    virtual void mirror( const int& n, double* par );

    //! update PDF and clear cached data
    //    virtual void updatePdf();
    //  moved to protected

    //! register parameter set to be fit
    virtual void insert( QCD::PdfBase *pdf );
    
    // moved to protected (for MultiFragFitLM)
    //    //! analyze Minuit command option
    //    virtual void minuitCmds( std::string cmd );

    //! dump additional information after fit procedure
    virtual bool postFit_summary( std::ostream& os );

    //! delete process of the objects created in config process
    void delete_process();

    //! resize several vectors (fit_, c2_, etc.) before chi2 calc
    void resize_vects();

    //! for some manipulations in case of skipfit 
    virtual void skipmanip(){};
    
    std::vector< Exp::ExpData* >      expr_;   //!< an ExpData object
    std::vector< Evolution::Kernel* > kernel_; //!< kernels under control
    std::vector< QCD::PdfBase* >      pdf_;    //!< managed PdfBase object
    
    // ---------------------------------------------------------------- //
    // parameters for fitting procedure.....
    // params_ & nam_ moved to protected for LM procedure (MultiFragFitLM.cc)
    std::vector< int >              iflags_; //!< TMinuit status monitor
    Utility::Timer timer_;                   //!< local timer object
    bool verbose_;                           //!< verbose mode flag
    std::vector< std::string > commands_;    //!< Minuit command
    std::string contFileName_;               //!< file name for contour plot
    std::vector< int > contparam_;           //!< parameter list for contour

    double chi2_;
    double res_chi2_; // chi2 contribution from rescale param (ref. chi2())
    //!< IMPLICIT! chi2 scale parameter (default 1)
    //!< multiplied at the end of Eval function
    double chi2scale_;  

    double offset_;
    double angle_;
    double length_;
    
    bool multiplier_;
    double lambda_;
    double reffnc_;
    
    bool emflag_;          //!< error matrix output flag.....
    bool postErrMatrix_;   //!< flag to call post Error Matrix routine
    
    std::vector< QCD::PdfErrMatrix > ematrices_;     //!< error matrix
    
    bool with_stat_corr_;  //!< stat. cov. flag

    //!< config process excution flag (criteria for avoiding duplicate config 
    //!< process operation and executiong delete process)
    bool conf_judge_;

    /*!
      \brief  error matrix required level

      0:  Not calculated at all
      1:  Diagonal approximation only, not accurate ( default )
      2:  Full matrix, but forced positive-definite
      3:  Full accurate covariance matrix 
          (After MIGRAD, 
	  this is the indication of normal convergence.)

    */
    int  emlevel_; //!< error matrix requirement level

    
    // ----------- MINUIT status
    /*
      Returns concerning the current status of the minimization
      *-*      =========================================================
      *-*       User-called
      *-*          Namely, it returns:
      *-*        FMIN: the best function value found so far
      *-*        FEDM: the estimated vertical distance remaining to minimum
      *-*        ERRDEF: the value of UP defining parameter uncertainties
      *-*        NPARI: the number of currently variable parameters
      *-*        NPARX: the highest (external) parameter number defined by user
      *-*        ISTAT: a status integer indicating how good is the covariance
      *-*           matrix:  0= not calculated at all
      *-*                    1= approximation only, not accurate
      *-*                    2= full matrix, but forced positive-definite
      *-*                    3= full accurate covariance matrix
      *
     */
    double fmin_;
    double fedm_;
    double errdef_;
    int npari_;
    int nparx_;
    
  protected:

    // -------------------------------------------------------- //
    void del( std::vector< std::vector< Xsection::xzXsec* > >& f );
    void del( std::vector< Xsection::xzXsec* >& f );
    
    void del( std::vector< std::vector< Xsection::xXsec* > >& f );
    void del( std::vector< Xsection::xXsec* >& f );

    void del( std::vector< std::vector< unpolDISXsec::StrFunc* > >& f );
    void del( std::vector< unpolDISXsec::StrFunc* >& f );
    
    void del( std::vector< std::vector< unpolSIDISXsec::StrFunc* > >& f );
    void del( std::vector< unpolSIDISXsec::StrFunc* >& f );
    
    void del( std::vector< std::vector< unpolDIS::StrFunc* > >& f );
    void del( std::vector< unpolDIS::StrFunc* >& f );
    
    void del( std::vector< std::vector< unpolSIDIS::StrFunc* > >& f );
    void del( std::vector< unpolSIDIS::StrFunc* >& f );
    
    void del( std::vector< std::vector< unpolSIDISXsec::StrFuncSub* > >& f );
    void del( std::vector< unpolSIDISXsec::StrFuncSub* >& f );
    
    void del( std::vector< std::vector< polDIS::StrFunc* > >& f );
    void del( std::vector< polDIS::StrFunc* >& f );
    
    void del( std::vector< std::vector< polSIDIS::StrFunc* > >& f );
    void del( std::vector< polSIDIS::StrFunc* >& f );
    
    void del( std::vector< std::vector< FragXsec_T::FragXsec* > >& f );
    void del( std::vector< FragXsec_T::FragXsec* >& f );
    
    void config_minuit();
    void config_contour();
    void config_inversion();
    void config_errorMatrix();
    void config_expdata() throw( int );
    void config_param() throw( int );
    void config_local();
    
    void config_data_renormalization();
    
    void invertErrorMatrix();
    
    static std::string emLevelTag( const int& i );

    std::string prefix_;

    int istat_; //! error state flag
    
    int dumpParamLevel_; //!< parameter xml output control flag


  private:
    bool useThread_;
    virtual int runInThread();
    int sigHalfSize_;

    static double CHI2HUGE;

    //! for distribution interpolation in MultiFrag_A1hFitLM
    //! (trying to use the same distributions)
    //! to avoid double delete in A1hFit & MultiFragFit deconstructors
    template < class T > void null_delete( T* t ){
      if( t ){ delete t; t = NULL; }
    };

  };
  
};
#endif // _Fitting_Fitting_hh_

