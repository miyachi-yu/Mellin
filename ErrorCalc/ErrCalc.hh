// $Id: ErrCalc.hh,v 1.17 2013/06/13 01:52:07 miyachi Exp $
/*!
  \brief class definition of Error Calculation 
 */
#ifndef _ErrorCalc_ErrCalc_hh_
#define _ErrorCalc_ErrCalc_hh_

#include <Tranform/ComplexFunction.hh>
#include <Tranform/RealFunction.hh>
#include <Tranform/Matrix.hh>
#include <QCD/PdfErrMatrix.hh>
#include <Evolution/KernelBase.hh>
#include <Evolution/PDFx.hh>
#include <ExpData/DataSet.hh>

#include <map>

namespace Xsection {
  class Xsec;
  class xXsec;
  class xzXsec;
}

namespace IntegXsec {
  class xXsec;
  class xzXsec;
}

namespace ErrorCalc {
  
  /*!
    
    \brief class for error calculation
    
    This will calculate correlation matrix on quantity F and G,
    which are depend on the same parameters, pi ( i = 0, ... )
    \f[
    cov = \sum_{i,j} M_{i}^{j} 
    { \partial F \over \partial p_i } \cdot { \partial G \over \partial p_j }
    \f]
    with ErrCalc::calcerr( F, G );
    
  */
  class ErrCalc{
  public:
    
    ErrCalc( Evolution::KernelBase* evo ); //! a constructor with KernelBase
    ErrCalc( Xsection::Xsec* xsec );       //! a constructor with Xsec
    ErrCalc( Xsection::xXsec* xxsec );     //! a constructor with xXsec
    ErrCalc( Xsection::xzXsec* xzxsec );   //! a constructor with xzXsec
    ErrCalc( IntegXsec::xXsec* ixxsec );     //! a constructor with xXsec
    ErrCalc( IntegXsec::xzXsec* ixzxsec );   //! a constructor with xzXsec

    virtual ~ErrCalc();                                //!< a destructor 
    
    //! set error matrix
    void setErrMatrix( const QCD::PdfErrMatrix& ematrix );

    //! add error matrix
    void addErrMatrix( const QCD::PdfErrMatrix& ematrix );
    
    //! add error matrix
    void addErrMatrix( Exp::ExpData* edata );
    
    //! set dataset if necessary
    void setDataSet( Exp::DataSet* dataset );

    //! get error matrix
    //    QCD::PdfErrMatrix getErrMatrix(){ return errMatrix_; }
    
    //! calculate error of complex function f( N )
    virtual 
    std::complex< double > operator()( const std::complex< double >& n );
    
    //! calculate error of complex function f( N, M )
    virtual 
    std::complex< double > operator()( const std::complex< double >& n, 
				       const std::complex< double >& np );
    
    //! calculate error of real function f( x )
    virtual double operator()( const double& x );

    
    //! calculate error of real function f( x, z )
    virtual double operator()( const double& x, const double& z );
    
    //! calculate error of real function f( x )
    virtual double operator()( const std::vector< double >& x );
    
    //! calculate error of real function f( x, z )
    virtual double operator()( const std::vector< double >& x,
			       const std::vector< double >& xp );
    
    /*!
      \param [in] p is a parton type 
      \brief set ErrCalc to calculate error of PDF, or FF
    */
    void setPID( const QCD::Flavor::PARTON& p ) { 
      pdfcalc_ = true; this->createpdfs();
      parton_ = p;
    }
    
    //! select which evolution kernel should be used
    void selectEvo( const int& i ){ ivevo_ = i; }
    
    //! need comment 
    void setPDFdrct( const bool& pdfd ){ pdfdirect_ = pdfd; }
    
    //! for comprehensive pdf error calc
    virtual std::vector< double > operator()();
    
    //! need comment
    void setvX( const std::vector< double >& vx ){
      vvX_ = vx; 
      vcalc_ = true; 
      pdfcalc_ = true; this->createpdfs();
    }
    
    //! get reference to the PdfErrMatrix container
    std::map< int, QCD::PdfErrMatrix >& ematrices() { return ematrices_; }
    
    int getDataID(){ return emxID_; }

    void setRealFunc( Transform::RealFunction* rf ){ rfunc_ = rf; }
    void setCompFunc( Transform::ComplexFunction* cf ){ cfunc_ = cf; }

    void rfunccalc( const bool& rfc ){ rfcalc_ = rfc; }
    void cfunccalc( const bool& cfc ){ cfcalc_ = cfc; }
    void dfunccalc( const bool& dfc ){ dcalc_ = dfc; }
    void vectcalc( const bool& vecc ){ vectvar_ = vecc; }

    void setPDFcalc( const bool& pdfc ){ 
      pdfcalc_ = pdfc; if( pdfcalc_ ) this->createpdfs(); }

    void setDchi2( const double& dchi2 ){ dchi2_ = dchi2; }

    Evolution::PDF* getPDF(){ return pdf_; }
    Evolution::PDFx* getPDFx(){ return pdfx_; }

  protected:
    
    std::complex< double >& getN(){ return N_; }    //!< get moment N
    std::complex< double >& getM(){ return M_; }    //!< get moment M
    std::complex< double >& getNp(){ return Np_; }  //!< get moment Np
    std::complex< double >& getMp(){ return Mp_; }  //!< get moment Mp

    double& getX(){ return X_; }                    //!< get value of x
    double& getZ(){ return Z_; }                    //!< get value of z
    double& getXp(){ return Xp_; }                  //!< get value of x
    double& getZp(){ return Zp_; }                  //!< get value of z

    std::vector< double >& getvX(){ return vX_; }   //!< get vector vX
    std::vector< double >& getvZ(){ return vZ_; }   //!< get vector vZ
    std::vector< double >& getvXp(){ return vXp_; } //!< get vector vXp
    std::vector< double >& getvZp(){ return vZp_; } //!< get vector vZp

    int getivevo(){ return ivevo_; } //!< get which evolution in use
    
    std::vector< Evolution::KernelBase* > getvevo(){ return vevo_; }
    
    std::complex< double > 
    calcerror( Transform::ComplexFunction* f, 
	       Transform::ComplexFunction* g ) throw( int );
    
    double calcerror( Transform::RealFunction* f, 
		      Transform::RealFunction* g ) throw( int );
    
    //! for comprehensive pdf error calc
    std::vector< double > vcalcerror( Transform::RealFunction* f, 
				      Transform::RealFunction* g ) throw( int );    

  private:

    void createpdfs();
    
    void setF_G( Transform::ComplexFunction* f, 
		 Transform::ComplexFunction* g ) throw( int ) ;
    
    void setF_G( Transform::RealFunction* f, 
		 Transform::RealFunction* g ) throw( int ) ;
    
    double dpar_;
    int ivevo_;
    
    bool pdfcalc_;
    bool pdfdirect_;
    QCD::Flavor::PARTON parton_;
    
    std::complex< double > N_;
    std::complex< double > M_;
    std::complex< double > Np_;
    std::complex< double > Mp_;
    
    double X_;
    double Z_;
    double Xp_;
    double Zp_;
    std::vector< double > vX_;
    std::vector< double > vZ_;
    std::vector< double > vXp_;
    std::vector< double > vZp_;

    bool valconst();
    
    Transform::ComplexFunction* cfuncF_; //!< pointer to complex function F
    Transform::ComplexFunction* cfuncG_; //!< pointer to complex function G

    Transform::RealFunction* rfuncF_; //!< pointer to real function F
    Transform::RealFunction* rfuncG_; //!< pointer to real function G
    
    bool isFequalG_; //!< is function F equal to G
    
    std::complex< double > pdfcalcC();
    double pdfcalcR();
    
    bool checkmatrix();
    void pdfupdate();
    
    void setparamvec( std::vector< Evolution::KernelBase* > vevo_ );
    
    void setparam( QCD::PdfParametrization *p );
    
    Xsection::Xsec* xsec_;
    Xsection::xXsec* xxsec_;
    Xsection::xzXsec* xzxsec_;
    IntegXsec::xXsec* ixxsec_;
    IntegXsec::xzXsec* ixzxsec_;
    Transform::ComplexFunction* cfunc_;
    Transform::RealFunction* rfunc_;

    Evolution::PDF* pdf_;
    Evolution::PDFx* pdfx_;
    
    std::vector< Evolution::KernelBase* > vevo_;
    std::vector< QCD::PdfBase* > vpdfbase_;

    std::vector< QCD::Parameter* > par_;
    
    std::map< int, QCD::PdfErrMatrix > ematrices_; //!< error matrix container
    
    /*!
      \brief calclulate value of function F
     */
    std::complex< double > getFvalue();

    /*!
      \brief calclulate value of function G
     */
    std::complex< double > getGvalue();

    std::vector< std::complex< double > > vgetFvalue();
    std::vector< std::complex< double > > vgetGvalue();
    
    
    //! error calculation manipulator? for real value
    void eManipul( double& err ) throw( int );
    
    //! error calculation manipulator? for complex value
    void eManipul( std::complex< double >& err ) throw( int );
    
    bool ccalc_;   // judge complex calc
    bool dcalc_;   // judge 2-variable calc
    bool vectvar_; // arguments for F and G are vector
    bool rfcalc_;  // judge real function calc
    bool cfcalc_;  // judge complex function calc
    
    //! for comprehensive pdf error calc
    void eManipul( std::vector< std::complex< double > >& err ) throw( int );

    std::vector< double > vpdfcalcR();
    std::vector< double > vvX_;
    std::vector< double > vvZ_;

    bool vcalc_; // for vector calculation

    //! for data normalization
    Exp::DataSet* dataset_;
    double datanorm_;
    double dataStEr_;
    int dataID_;
    int emxID_;

    //! for delta chi2 values different from 1
    double dchi2_;

  };
  
};

#endif // _ErrorCalc_ErrCalc_hh_
