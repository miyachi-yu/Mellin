#ifndef _Fitting_A1hFitLM_hh_
#define _Fitting_A1hFitLM_hh_

#include "A1hFit.hh"
#include "TMatrix.hh"

namespace Fit{
  
  class A1hFitLM : public A1hFit  {
  public:
    
    //< a constructor
    A1hFitLM( Utility::Arguments& args ) throw( int );
    virtual ~A1hFitLM();             //!< a destructor

    /*!
      check persent fitting condition (at the begining of chi2() )
      If false, Fitting::CHI2HUGE is set to present chi2.
    */
    virtual bool condition();

    //! print out fitting parameters into file
    virtual void dumpParameters();

  protected:

    virtual void defineParameters();               //!< define parameters

    /*!
      \return a value of reference function for Lagrange Multiplier
      \ref. Fitting::Eval()
    */
    virtual double referenceFunction();

    //! post post config for tolerance manipulation
    //! tolerance 0.1 -> 1.0 / param_error    
    void ppost_config_process();    

  private:

    //! indices of free parameters 
    //! fpindx_[ 0 ] : QCD::Parameter indices
    //! fpindx_[ 1 ] : Exp::DataSet indices
    std::vector< std::vector< int > > fpindx_;

    //! initial paramters under Minuit control 
    //! ( initial values of each eigen vector )
    std::vector< QCD::Parameter >  eigparams_;

    //! weights for direct calc
    std::vector< double >  parweights_;

    //! id of the eigen vector for LM
    int egvid_;

    //! Basic Error Matrix given from proceeded fit
    QCD::PdfErrMatrix errmx_;

    //! Matrix with eigen vectors ( |M| is normalized to 1 )
    TMatrix orthmx_;
    //! its inverse
    TMatrix orthmxinv_;
    //! Matrix with eigen values ( diagnal matrix )
    TMatrix eigvmx_;
    //! its inverse ( error^2 diagnal matrix )
    TMatrix eigvmxinv_;

    //! dump additional information after fit procedure
    virtual bool postFit_summary( std::ostream& os );

    //! mirror the Minuit parameter to the model
    /*
    virtual void mirror();
    */
    virtual void mirror( const int& n, double* par );

    //! primitive mirror for direct calc 
    //! (reflection of weight manipulation)
    void mirror( const std::vector< QCD::Parameter >& par );

    // manipulations for direct calc
    virtual void skipmanip();

    //! set reference values for comparison
    void setrefs();

    //! creation of several matrices
    void mxcreation();

    //! creation of basic err matrices
    //    QCD::PdfErrMatrix& createErrMx();
    QCD::PdfErrMatrix createErrMx();

    //! set eigparams
    void seteigparams();

    //! set weights for direct calc
    void setweights();

    //! output to files
    void file_out();

    //! output parameters
    void param_out( std::ostream& os );

    //! output error correlation between original and eig values
    //! M^j_i = err2^j_{eigv i} / sum_i err2^j_{eigv i} 
    //! M^j_i = sqrt( err2^j_{eigv i} ) / sqrt( sum_i err2^j_{eigv i} )
    void param_corr_out( std::ostream& os );

    //! calc err. of chi2s
    void chi2err_calc();
    //! output err. of chi2s
    void chi2err_out( std::ostream& os, 
		      const std::vector< double >& vchi2_org_,
		      const std::vector< double >& vchi2_err_ );

    //! parameter discreteness by the unit of its error
    double dissig_;
    //! parameter extension by the unit of its error
    double malsig_;

    //! get values of eigen vectors
    double geteigvvalue( const int& i );

    //! set weight for a reference function ( for Lagrange Multiplier )
    //! ref. Fitting::Eval()
    void setlambda( const double& lambda ) 
    { this->multiplier( true ); this->multiplier( lambda ); } 

    //! output several matrices to log
    void matrix_output();

    //! for comparison
    bool reffrag_;
    double reffvalue_;
    double refforgchi2_;
    double refforgvalue_;
    double refferr_;

    //! output filename
    std::string tfname_; // for chi2 vs normalized eigvec
    std::string dfname_; // for data chi2 vs normalized eigvec

  };
  
};

#endif // _Fitting_A1hFitLM_hh_
