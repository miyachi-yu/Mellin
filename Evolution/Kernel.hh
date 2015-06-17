// $Id: Kernel.hh,v 1.30 2009/02/12 07:59:35 miyachi Exp $
/*!
  \file    Kernel.hh
  \version "$Revision: 1.30 $"
  \date    "$Date: 2009/02/12 07:59:35 $"
  \brief   Definition of QCD Evolution Kernel in Mellin space
  
  This is a base class defintion file for the QCD evolution in Mellin space.
  
 */
#ifndef _Kernel_hh_
#define _Kernel_hh_

#include <vector>
#include <complex>

#include <Utility/Arguments.hh>

#include <Tranform/Matrix.hh>

#include <QCD/PdfParametrization.hh>

#include "KernelBase.hh"
#include "AnomalousDimension.hh"
#include "DGLAPMatrix.hh"
#include "DGLAPpdf.hh"

namespace Evolution {
  
  /*!
    \class  Kernel   Kernel.hh   "Evolution/Kernel.hh"
    \brief  base class for QCD evolution equation
    
    This is a base class for the QCD evolution in Mellin space.
    For the moment class design and implemnetation strongly depend on
    Nucl. Phys. B465 (1997) 267. However it is ideal to be more
    independent at least for the class definition.
    
    This is a base class, so that users must implement own UserKernel 
    class depending on their requirements. The class requires at least
    PdfParameterization, and AnomalousDimension class.
    
  */
  class Kernel : public KernelBase  {
  public:
    
    Kernel( Utility::Arguments& args );              //!< a default constructor
    virtual ~Kernel();                      //!< a destructor
    
  public: // ------------- KernelBase Virtual methods ---------------------- //
    
    virtual void setN( const std::complex< double >& n );  //!< set N moment    
    virtual void setQ2( const double& q2 );                 //!< set scale Q2
    
    virtual double Q2() { return q2_; }                     //!< get scale Q2
    
    virtual std::complex< double > q( const QCD::Flavor::PARTON& q ); //!< get PARTON q
    virtual std::complex< double > q( const QCD::Flavor::TYPE&   q ); //!< get TYPE q
    
  public: // --------------------------------------------------------------- //
    
    //! get a reference to initial scale variable
    virtual double& initialScale(); 
    
    //! get a pointer of initial distribution object
    virtual QCD::PdfParametrization* initialDistribution() { return init_; }
    
    //! get a pointer of AnomalousDimension object
    virtual AnomalousDimension* anomalousDimension() { return P_; }
    
    //! get a reference of DGLAPMatrix object
    virtual DGLAPMatrix&        dglapMatrix() { return m_; }
    
    //! get a reference of DFLAPpdf object
    virtual DGLAPpdf& dglapPdf() { return qout_; }
    
    //! get NF handling flag ( true: fixed NF, false: dynamic NF )
    bool constNf() const { return constNf_; }
    
    //! set NF handling flag ( true: fixed NF, false: dynamic NF )
    bool constNf( const bool& chNfstate ) ;
    
    //! get reference to Argument object
    Utility::Arguments& args() { return args_; }
    
  public: // ---------- static members and methods ---------------- //
  
    static double CF;
    static double CF2;
    static double CF4;
    static double CF8;
    static double CF16;
    static double CF32;
    
    static double CF_2;
    static double CF2_2;
    
    static double CA;
    static double CA2;
    static double CA2_2;
    static double CA16;
    static double CA32;
    
    static double CACF;
    static double CACF8;
    
    static double PI4;
    
    static Transform::Matrix< std::complex< double > > I;
    
    //!< output routine to the given output stream
    friend std::ostream& operator<<( std::ostream& os, Kernel& k );
    
  protected:
    
    static bool initialized;
    static bool initialization();
    
    //! setup initial distribution value for Evolution
    virtual void loadInitialDistributions();
    
    //! set initial distribution object
    virtual void initialDistribution( QCD::PdfParametrization *init );
    
    //! set AnomalousDimension object
    virtual void anomalousDimension( AnomalousDimension* ad );
    
  protected:
    
    QCD::PdfParametrization *init_;
    AnomalousDimension      *P_;
    double                   q2_;
    bool                     forwardEvolution_;
    int                      iThresh_; // 1st threshold index
  virtual void update();
    
  protected:
    
    std::vector< double > thresh_;   //!< mass threshold
    
    // ----------------  For DGLAP evolution equation --------------------- //
    DGLAPMatrix m_;   //!< DGLAP evolution kernel itself
    DGLAPpdf qin_;    //!< PDF set for initial distribution
    DGLAPpdf qout_;   //!< PDF set for final distribution
    
  private:
    
    bool externalDB_;  //!< true if external DB will be in used
    bool constNf_;     //!< true if Nf is not changed 
    
    Utility::Arguments& args_;  //!< reference to Arguments object
    
    
  // ------------------- memory cache related rouine --------------------- //
    
    virtual void insertToCache( const std::complex< double >& n, 
				const double& q2,
				Transform::Matrix< std::complex< double > >& qS,
				std::vector< std::complex< double > >& qNSp,
				std::vector< std::complex< double > >& qNSm  );
    
    virtual void insertToCache( const std::complex< double >& n, 
				const double& q2, 
				DGLAPpdf& pdf );
    
    
    virtual bool retriveFromCache( const std::complex< double >& n, 
				   const double& q2,
				   DGLAPpdf& pdf );
    
    virtual bool retriveFromCache( const std::complex< double >& n, 
				   const double& q2,
				   Transform::Matrix< std::complex< double > >& qS,
				   std::vector< std::complex< double > >& qNSp,
				   std::vector< std::complex< double > >& qNSm  );
    
    
    int evolveToThreshold( DGLAPpdf& qin );


  };
};

#endif // _Kernel_hh_

