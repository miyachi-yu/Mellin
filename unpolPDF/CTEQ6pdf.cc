//$Id: CTEQ6pdf.cc,v 1.5 2009/07/29 09:00:59 miyachi Exp $
/*!
  \file   CTEQ6pdf.cc
  \brief  class implementaiton for Evoplution Kernel based on CTEQ6 PDFLIB
 */

#include "CTEQ6pdf.hh"
#include <Utility/Arguments.hh>

using namespace std;
using namespace Utility;
using namespace unpolPDF;


CTEQ6pdf::CTEQ6pdf() : 
  KernelPdfLib( Arguments::instance()->hasOpt( "order" ) && 
		Arguments::instance()->getOpt( "order" ) == "LO" ? 
		"CTEQ6L" : "CTEQ6M" ),
  cteq6_( NULL )
{
  //! choose CTEQ6L for LO and CTEQ6M for NLO
  int i_ = ( this->getname() == "CTEQ6M" ? 1 : 3 );
  cteq6_ = new PDFLIB::CTEQ6( i_ );
  this->pdflib( cteq6_ );
}


CTEQ6pdf::~CTEQ6pdf(){
  if( cteq6_ ) delete cteq6_;
}
