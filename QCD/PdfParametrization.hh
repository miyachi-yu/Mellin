// $Id: PdfParametrization.hh,v 1.32 2009/07/29 09:00:59 miyachi Exp $
/*!
  \file    PdfParametrization.hh
  \version $Revision: 1.32 $
  \author  $Author: miyachi $
  \brief   Definition of interface of PdfParametrization class
 */

#ifndef _PdfParametrization_hh_
#define _PdfParametrization_hh_

#include <iostream>
#include <complex>
#include <vector>
#include <string>

#include "Flavor.hh"
#include "ConfigPdfBase.hh"
#include "ConfigFFbase.hh"
#include "PdfErrMatrix.hh"
#include "Parameter.hh"

namespace QCD {
  
  class PdfBase;
  
  /*!
    \class  PdfParametrization
    \brief  abstruct class for PDF parametrizations
    
    This is a base class for PDF parametrization class.
    
    \par
    This object can be configred with an external configuration
    file which can be managed with ConfigPdfBase, ConfigFFbase,
    or inherited classes. The configuration file must contains 
    parameterization information for the light quarks either
    directly specifying with quark flavor information or in the
    combination of SU(3).
    
    \par Light quarks ( flavor information )
    It requires u, u-bar, d, d-bar, s, and s-bar parameterization.
    
    \par Light quarks ( SU(3) combination )
    At least, 
    \f[
    a_0 = ( u + \bar{u} ) + ( d + \bar{d} ) + ( s + \bar{s} )
    \f]
    
    \f[
    a_3 = ( u + \bar{u} ) - ( d + \bar{d} ) 
    \f]
    
    \f[
    a_8 = ( u + \bar{u} ) + ( d + \bar{d} ) -2 ( s + \bar{s} )
    \f]
    
    are requiored. If there are only these three combination given,
    SU(3) flavor symmetric sea distribution 
    \$[ \bar{u} = \bar{d} = s = \bar{s} \$] is asuumed.
    
    One can additionally specify combinations for
    \f[
    a_{3}^{-} = ( u - \bar{u} ) - ( d - \bar{d} ) 
    \f]
    
    \f[
    a_{8}^{-} = ( u - \bar{u} ) + ( d - \bar{d} ) -2 ( s - \bar{s} )
    \f]
    
    In this case, symmetry strange quark
    \$[ s = \bar{s} \$] will be assumed in the calculation.

    \par Heavy quarks
    
    In additon to the light quarks, the heavy quarks can be 
    specified in the configuration file for c, c-bar, d, and d-bar
    quarks.
    
  */
  class PdfParametrization {
  public:
    
    PdfParametrization();                             //!< a default constructor
    PdfParametrization( PdfParametrization& config ); //!< a copy constructor
    PdfParametrization( ConfigPdfBase& config );      //!< a constructor
    PdfParametrization( ConfigFFbase&  config );      //!< a constructor
    
    virtual ~PdfParametrization();                    //!< destructor
    
    /*!
      \par [in] q is a PARTON type
      \return value of PDF for the given q in the mellin space at n
      \brief  calculate the q(n) ( in the Mellin space )
      
      This calculte the PDF value for parton q at complex number n 
      in the mellin space. n shuld be pre-defined with setN method.
    */
    virtual std::complex< double > q( const Flavor::PARTON& q );
    
    //!< see q( const Flavor::TYPE&   q );
    virtual std::complex< double > q( const Flavor::TYPE&   q );
    
    virtual double& initialScale() { return scale_; } //!< get initial scale
    virtual void setN( const std::complex< double >& n );  //!< set complex n
    
    friend std::ostream& operator<<( std::ostream& os, 
				     const PdfParametrization& param );
    
    virtual double scale() const { return scale_; }      //!< get scale value
    virtual std::string name() const { return name_; }   //!< get name
    
    //!< get PdfBase* vector
    virtual std::vector< PdfBase* > pdfs() const { return pdfs_; } 
    
    virtual void scale( const double& v ) { scale_ = v; } //!< set scale value
    
    virtual void name( const std::string& s ) { name_ = s; }  //!< set name
    
    //< set PdfBase* std::vector
    virtual void pdfs( std::vector< PdfBase* > v );
    
    //! get reference of PdfBase* std::vector
    virtual std::vector< PdfBase* >& pdfs() { return pdfs_; }
    
    //! get xmlHeader (DTD) for PdfParameterization
    static std::string xmlHeader();
    
    //! check if the information in config is enough or not
    virtual bool check();

    //! set configration file path
    void path( std::string p ) { path_ = p; }
    
    //! get path
    virtual std::string path() { return path_; }
    
    //! get path
    std::string baseName();
    
    //! initialization with Configuration file
    virtual PdfParametrization& operator<<( ConfigPdfBase& conf );
    
    //! get error matrix
    PdfErrMatrix errorMatrix();
    
    //! fill the given error matrix em
    void errorMatrix( PdfErrMatrix& em );
     
    PdfBase* g() { return g_; }          //!< get pointer to gluon pdf

    PdfBase* u() { return u_; }          //!< get pointer to u quark pdf
    PdfBase* d() { return d_; }          //!< get pointer to d quark pdf
    PdfBase* s() { return s_; }          //!< get pointer to s quark pdf
    PdfBase* c() { return c_; }          //!< get pointer to c quark pdf
    PdfBase* b() { return b_; }          //!< get pointer to b quark pdf

    PdfBase* ubar() { return ubar_; }    //!< get pointer to u-bar quark pdf
    PdfBase* dbar() { return dbar_; }    //!< get pointer to d-bar quark pdf
    PdfBase* sbar() { return sbar_; }    //!< get pointer to s-bar quark pdf
    PdfBase* cbar() { return cbar_; }    //!< get pointer to c-bar quark pdf
    PdfBase* bbar() { return bbar_; }    //!< get pointer to b-bar quark pdf

    PdfBase* uv() { return uv_; }
    PdfBase* dv() { return dv_; }

    PdfBase* a0() { return a0_; }
    PdfBase* a3() { return a3_; }
    PdfBase* a8() { return a8_; }

    PdfBase* U() { return U_; }
    PdfBase* D() { return D_; }
    PdfBase* S() { return S_; }
    PdfBase* C() { return C_; }
    PdfBase* B() { return B_; }
    
    QCD::Flavor::HADRON& hadron() { return hadron_; } //!< get hadron type
    int emNumber();                                   //!< get error matrix ID
    std::complex< double >& n() { return n_;      }   //!< get current n
    
    //!< get paramer (index) of pdf with name
    PdfBase*   find( const std::string& name );
    Parameter* find( const std::string& name, const int& index );
    
    void remove( const std::string& name );


  protected:
    
    static std::string tagName;                       //!< name for XML tag
        
    //! complex number where PDF values will be calculated
    std::complex< double > n_; 
    std::string name_;                  //!< name of this parameterization
    double scale_;                      //!< initial QCD scale
    std::vector< PdfBase* > pdfs_;      //!< PdfBase pointer container
    
    PdfBase *g_;    //!< gluon distribuiton
    
    PdfBase *u_;    //!< u quark
    PdfBase *d_;    //!< d quark
    PdfBase *s_;    //!< s quark

    PdfBase *ubar_; //!< u-bar quark
    PdfBase *dbar_; //!< d-bar quark
    PdfBase *sbar_; //!< s-bar quark
    
    PdfBase *uv_;   //!< u valence
    PdfBase *dv_;   //!< d valence
    PdfBase *sv_;   //!< d valence
    
    // ---------------- heavy quarks ------------------
    PdfBase *c_;    //!< c quark
    PdfBase *b_;    //!< b quark
    PdfBase *cbar_; //!< c-bar quark
    PdfBase *bbar_; //!< b-bar quark
    
    // for SU(3) symmetry version
    PdfBase *a0_;  //!< SU(3) flavor singlet  
    PdfBase *a3_;  //!< SU(3) flavor triplet (non-singlet)
    PdfBase *a8_;  //!< SU(3) flavor octet   (non-singlet)
    
    PdfBase *a3m_; //!< SU(3) flavor triplet (non-singlet)
    PdfBase *a8m_; //!< SU(3) flavor octet   (non-singlet)
    
    PdfBase *sea_;   //!< sea quark distribution
    PdfBase *delta_; //!< u-bar - d-bar (MRST)

    PdfBase *dusum_;   //!< d-bar + u-bar (CTEQ)
    PdfBase *duratio_; //!< d-bar over u-bar (CTEQ)
    
    std::string path_;             //!< path of the configuration file

    PdfBase *U_;  //!< u + ubar
    PdfBase *D_;  //!< d + dbar
    PdfBase *S_;  //!< s + sbar
    PdfBase *C_;  //!< c + cbar
    PdfBase *B_;  //!< b + bbar

    bool flavors_; //! status quark flavor combination
    
    /*!
      \param[in] pdf is a pointer to PdfBase
      \param[in] ref is a pointer to PdfBase
      \brief register the given PdfBase object ref to this parameterization set
      
      This register PdfBase object *ref to one of the PdfBase object pointer
      internal variable, g_, u_, d_, ...... The type of pdf will be checked 
      with the given *pdf object. If NULL is given to ref, pdf itself will be
      registered.
      
    */
    void assignPdf( PdfBase *pdf, PdfBase *ref = NULL ); 

    //! assign generated PdfBase to corresponding container
    virtual void assign();
    
    //! hadron information to be used in FF parameterization
    Flavor::HADRON hadron_;
    
    bool toBeDeleted_; //!< delete all PdfBase object in destructor
    
  };
};

#endif //  _PdfParametrization_hh_
