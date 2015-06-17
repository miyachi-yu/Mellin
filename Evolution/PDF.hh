// $Id: PDF.hh,v 1.16 2009/02/19 04:26:51 miyachi Exp $
/*!
  \file    PDF.hh
  \version $Revision: 1.16 $
  \date    $Date: 2009/02/19 04:26:51 $
  \author  $Author: miyachi $
  \brief   Definition of PDF class ( for moment )
 */
#ifndef _PDF_hh_
#define _PDF_hh_


#include <QCD/Flavor.hh>
#include <QCD/PdfBase.hh>

#include <Tranform/ComplexFunction.hh>

#include <iostream>
#include <string>
#include <complex>

namespace Evolution {
  
  class KernelBase;
  
  /*!
    \class  PDF   PDF.hh   "Evolution/PDF.hh"
    \brief  Class for moment PDF which directly is evolved by DGLAP
    
    This is a class for moment PDF which is directly evolved with
    DGLAP QCD evolution equation defined in the moment space.
    Thus, PDF class is derived from ComplexFunction class.
    
  */
  class PDF : public Transform::ComplexFunction {
  public:
    
    //! constructor with a kernel and parton type q
    PDF( KernelBase *kernel, const QCD::Flavor::PARTON& q );
    //! constructor with a kernel and SU(N) combination of quark flavor q
    PDF( KernelBase *kernel, const QCD::Flavor::TYPE&   q );

    PDF( const PDF& pdf );             //!< copy constructor
    virtual ~PDF();                    //!< a destructor
    
    PDF& operator=( const PDF& pdf );  //!< assignment operator
    
    //! calculate complex function at the given complex z
    virtual std::complex< double > operator() ( const std::complex< double >& x );
    
    std::string name() const ;              //!< get paton name in string format
    QCD::PdfBase::TYPE type() const { return type_; } //!< get flavor type
    int id() const;                         //!< get flavor id
    
    //! get a pointer to KernelBase object
    KernelBase* kernel() { return kernel_; } 
    
    friend std::ostream& operator<<( std::ostream& os, PDF& pdf );

    //! get reference to parton or type object
    QCD::Flavor::PARTON& getPID(){ return parton_; }
    QCD::Flavor::TYPE&  getTID(){ return combo_; }

    QCD::Flavor::PARTON getPID() const { return parton_; }
    QCD::Flavor::TYPE  getTID() const { return combo_; }
    
    QCD::PdfBase* parameters();
    
    
  private:
    KernelBase     *kernel_;   //!< a pointer to Kernel object
    
    QCD::PdfBase::TYPE  type_;      //!< type of flavor 
    QCD::Flavor::PARTON parton_;    //!< quark flavor
    QCD::Flavor::TYPE   combo_;     //!< type of flavor SU(N) combination
    
  };
};

#endif // _PDF_hh_
  
  
