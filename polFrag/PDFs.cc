//$Id: PDFs.cc,v 1.3 2008/05/23 02:08:04 miyachi Exp $
/*!

  \file PDFs.cc
  \version $Revision: 1.3 $
  \author  $Author: miyachi $
  \date    $Date: 2008/05/23 02:08:04 $
  \brief   Implementation of Initial fragmentation function parameterization.

  This is implementation of the initial fragmentation function, 
  polFrag::PDFs class.
  It may be rename sometime in future.....

 */

#include "PDFs.hh"
#include <QCD/PdfBase.hh>
#include <iostream>

using namespace std;
using namespace QCD;
using namespace polFrag;

PDFs::PDFs() : 
  PdfParametrization()
{
}

PDFs::~PDFs() {
}

PDFs& PDFs::operator<<( ConfigFF& ff ){
  
  this->name( ff.name() );
  this->scale( ff.scale() );
  this->pdfs( ff.pdfs() );
  
  return *this;
}
