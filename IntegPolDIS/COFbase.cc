#include "COFbase.hh"

using namespace IntegPolDIS;

COFbase::COFbase( IntegXsec::IntegXsecComp* xc ) :
  IntegXsec::ConvIntegrand( xc )
{
}

COFbase::~COFbase(){
}

