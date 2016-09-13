/*!
// $Id: PDFx.cc,v 1.18 2008/08/27 18:30:34 imazu Exp $
  \file     PDFx.cc
  \Version  $Revision: 1.18 $
  \Author   $Author: imazu $
  \Date     $Date: 2008/08/27 18:30:34 $
  \brief    Class implementation of x-space PDF
*/
#include "PDFx.hh"

#include <iostream>
#include <iomanip>

#include <Tranform/GridIntegration.hh>
#include <Tranform/LegQuadrature.hh>

using namespace std;
using namespace Transform;
using namespace QCD;
using namespace Evolution;

PDFx::PDFx( KernelBase *kernel, const Flavor::PARTON& q,
	    const int& low, const int& high, const double& precision,
	    const double& length, const double& offset, 
	    const double& rangle ) : 
  InverseMellin( NULL, NULL ), pdf_( kernel, q )
{
  this->initialization( low, high, precision, length, offset, rangle );
}

PDFx::PDFx( KernelBase *kernel, const Flavor::TYPE& q,
	    const int& low, const int& high, const double& precision,
	    const double& length, const double& offset, 
	    const double& rangle ) : 
  InverseMellin( NULL, NULL ), pdf_( kernel, q )
{
  this->initialization( low, high, precision, length, offset, rangle );
}

PDFx::~PDFx() {
  delete dynamic_cast< GridIntegration* >( this->integration() )->method1();
  delete dynamic_cast< GridIntegration* >( this->integration() )->method2();
  delete  this->integration();
}

void PDFx::initialization( const int& low, const int& high, 
			   const double& precision,
			   const double& length, const double& offset, 
			   const double& rangle ){
  
  this->integration( new GridIntegration( new LegQuadrature( low ),
					  new LegQuadrature( high ),
					  4, precision ) );
  
  //  this->integrand().set( &pdf_ );
  this->transform( &pdf_ );

  this->setParameter( offset, rangle );
  this->upper( length );
  
}

namespace Evolution {
  ostream& operator<<( ostream& os, PDFx& pdfx ){
    os << "PDFx: " << setw(10) << pdfx.pdf().name() << endl;
    return os;
  }
}
