// $Id: MCInt.hh,v 1.1 2009/07/29 03:23:14 miyachi Exp $
/*!
  \file     MCInt.hh
  \date     $Date: 2009/07/29 03:23:14 $
  \version  $Revision: 1.1 $
  \author   $Author: miyachi $
  \brief    Definition of Trapezoid approximation integration class
*/
#ifndef _Transform_MCInt_hh_
#define _Transform_MCInt_hh_

#include "Integration.hh"

namespace Transform {
  
  /*!
    \class   MCInt    MCInt.hh    "Tranform/MCInt.hh"
    \brief   Integration class based on MonteCarlo method
  */
  class MCInt : public Integration {
  public:
    
    /*!
      \param [in] number of data point to be calculated
      \brief      a default constructor for MCInt class.
    */
    MCInt( const int& n = 100 );
    
    //! destructor
    virtual ~MCInt();
    
    //! Integrate the given region with Trapezoid approxymeation
    virtual double integral( const double& min, const double& max, 
			     RealFunction& f ) ;
    
    //! Integrate the given region with Trapezoid approxymeation
    virtual std::complex< double > 
    cintegral( const double& min, const double& max, ComplexFunction& f );
    
    //! Integrate the given region with Trapezoid approxymeation
    virtual double integral( const double& xmin, const double& xmax, 
			     const double& zmin, const double& zmax, 
			     RealFunction& f );
    
    //! Integrate the given region with Trapezoid approxymeation
    virtual std::complex< double > 
    cintegral( const double& xmin, const double& xmax, 
	       const double& zmin, const double& zmax, 
	       ComplexFunction& f );
    
    int& n() { return n_; }

  private:
    int n_;
    
  };
}
#endif
