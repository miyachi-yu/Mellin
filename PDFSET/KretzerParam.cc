// $Id: KretzerParam.cc,v 1.1 2009/07/28 04:07:31 miyachi Exp $
/*!
  \file  PdfParameterization.cc
  \brief class implementation of PDF parameterization
 */
#include "KretzerParam.hh"
#include "KretzerFF.hh"

#include <sstream>
#include <cstdlib>
#include <iomanip>

extern "C" {
#include <libgen.h>
}

using namespace std;
using namespace PDFSET;

KretzerParam::KretzerParam() : 
  QCD::PdfParametrization()
{
}

KretzerParam::KretzerParam( KretzerParam& conf ) : 
  QCD::PdfParametrization( conf )
{
}

KretzerParam::KretzerParam( ConfigKretzerFF& conf ) : 
  QCD::PdfParametrization()
{
  this->path(  conf.path()  );
  this->name(  conf.name()  );
  this->scale( conf.scale() );
  this->pdfs(  conf.pdfs()  );
}

KretzerParam::~KretzerParam() {
}

KretzerParam& KretzerParam::operator<<( ConfigKretzerFF& conf ){
  this->path(  conf.path()  );
  this->name(  conf.name()  );
  this->scale( conf.scale() );
  this->pdfs(  conf.pdfs()  );
  return *this;
}
