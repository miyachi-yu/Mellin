// $Id: PdfBase.hh,v 1.22 2008/12/12 09:04:51 miyachi Exp $
/*!

  \brief    Definition of PDF parameterization base class
  
  $Revision: 1.22 $
  $Author: miyachi $
  $Date: 2008/12/12 09:04:51 $
  
 */
#ifndef _PdfBase_hh_
#define _PdfBase_hh_

#include <iostream>
#include <vector>
#include <map>
#include <complex>

#include <Tranform/RealFunction.hh>
#include <Tranform/ComplexFunction.hh>
#include <Tranform/CBeta.hh>

#include "Parameter.hh"

namespace Transform {
  class IntegFunc;
  class Mellin;
}

namespace QCD {

  class PdfParametrization;

  /*!
    \class  PdfBase    Pdfbase.hh    "QCD/PdfBase.hh"
    \brief  PDF parameterization base class (definition of functional form)
    
    This clas provide PDF basic class for DGLAP equation in the moment space.
    In generall, Mellin transform is a simple integraion equation. 
    For PDF, it reads
    \f[
    \int_{0}^{1} x^{N-1} {\rm d}x
    \f].
    PdfBase class provides numellical Mellin transform framework. 
    The inherited class must implement 
    virtual double x( const double& x )
    method to get value of PDF at the given x. 
    Using virtual complex< double > PdfBase::operator()( const complex< double >& n )
    method, this calculate n-th moment of PDF with a numerical Mellin transformer.
    
    By overloading PdfBase::operator()( const complex< double >& n ),
    the inherited class can return directly N-th moment with analitically transformed
    function.
    
    In order to add some functional form for PDF parameterization, 
    user has to inherit this basic function. 
    The creation of the derived class can be done in ConfigPdf derived class.
    See the details in ConfigPdf.cc.
    
  */
  class PdfBase : public Transform::ComplexFunction, 
		  public std::map< int, Parameter > {
  public:
    
    /*!
      \enum   TYPE
      \brief  Type of PDF
      
      PDF can be defined for each parton type, quark flavors and gluon.
      On the other hand, it can be also defined for the flavor singlet,
      non-singlet, valence quark and so on. 
      
    */
    enum TYPE { parton, combo };
    
    /*!
      \param[in]  type  is PDF type
      \param[in]  id    is an identification number for the given type
      \param[in]  n     is a number of parameters required for functional form 
      \brief  constructor for this basic PDF class
    */
    PdfBase( const TYPE& type = parton, const int& id = 0, const int& n = 4 );
    PdfBase( const PdfBase& pdf );             //!< a copy constructor
    virtual ~PdfBase();                        //!< a destructor
    
    PdfBase& operator=( const PdfBase& pdf );  //!< assignment operator
    
    /*!
      \param[in] z is a complex value where the mellin pdf should be calculated
      \return z-moment of the pdf
      \brief  calculate z-th moment of this PDF
      
      This function calculate z-th moment of the implemented PDF.
      In the derived class, user must implement own function.
    */
    virtual std::complex< double > 
    operator() ( const std::complex< double >& z );
    
    /*!
      \param[in] x is a real function where the PDF should be calculated.
      \return  the value of PDF at the ginven x
      \brief  return PDF( x ).
      
      User should implement value of PDF( x ) using analytic functional form.
    */
    virtual double x( const double& x ) = 0 ;
    
    TYPE type() const { return type_; } //!< get a copy of type_
    int  id()   const { return id_; }   //!< get a copy of id_
    
    TYPE& type() { return type_; }      //!< get a reference of type_
    int&  id() { return id_;}           //!< get a reference of id_
    
    std::string name() const ;           //!< get a name of the PDF
    
    std::string& pid() { return pid_; }  //!< get a reference of pid
    std::string& src() { return src_; }  //!< get a reference of src
    
    virtual PdfBase* reference() { return reference_; } //!< get ref
    virtual void reference( PdfBase* ref ) { reference_ = ref; } //!< set ref
    
    virtual double& value( const int& i ); //!< get value of parameter i

    virtual double getValue( const int& i ); //!< get copy of value 
    
    /*!
      \brief  update private member variables using the present stored parameters
    
      If the derived class possess own private variable to speed up the calculation,
      one can update these variable inside this function. 
      
    */
    virtual void update() = 0;
    
    virtual void resize( const int& n ); //!< resize container objects
    
    static const double upperlimit; //!< upper limit of parameter
    static const double lowerlimit; //!< lower limit of parameter
    
    //! dump stored information to the give ostream object
    friend std::ostream& operator<<( std::ostream& os, PdfBase& pdf );
    
    //! real function wrapper
    class RealFunctionWrapper : public Transform::RealFunction {
    public:
      RealFunctionWrapper( PdfBase *pdf );                         //!< a constructor
      RealFunctionWrapper( const RealFunctionWrapper& wrap );      //!< a constructor
      virtual ~RealFunctionWrapper();                              //!< a destructor
      RealFunctionWrapper& operator=( const RealFunctionWrapper& wrap ); //!< assignment operator
      double operator()( const double& x );                        //!< wrapeer
    private:
      PdfBase* pdf_;                                               //!< a PdfBase pointer 
    };
    
    //! set favored ff ID
    void set( const std::string& id ) { dfav_ = id; }
    
    //! get favored FF ID
    void dfav( const std::string& id ) { dfav_ = id; }
    std::string dfav() { return dfav_; }
    
    void fav( const std::string& id ) { fav_ = id; }
    std::string fav() { return fav_; }
    
    void parametrization( PdfParametrization* param ) { 
      parametrization_ = param;
    }
    
    PdfParametrization* parametrization() { return parametrization_; }
    
  protected:
    // information on parameters
    std::vector< std::vector< double > > corr_; //!< correlation matrix
    std::string dfav_; //!< Favored FF ID in XML file
    std::string fav_;
    
    PdfParametrization *parametrization_;
    
  private:
    
    TYPE    type_;                //!< type of PDF
    int     id_;                  //!< identifycation value of PDF
   
    std::string  pid_;            //!< identification of element
    std::string  src_;            //!< id of element to be refered
    
    RealFunctionWrapper wrapper_; //!< real function wrapper for Mellin integration
    Transform::Mellin    *mellin_;             //!< numelical Mellin Transformer
    Transform::IntegFunc *integ_;
    bool     toBeDel_;            //!< object to be deleted
    
    PdfBase *reference_;  //!< pointer to PdfBase specified with src
    
  };
};

#endif // _PdfBase_hh_

