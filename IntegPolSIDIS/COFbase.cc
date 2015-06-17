#include "COFbase.hh"

using namespace IntegPolSIDIS;

COFbase::COFbase( IntegXsec::IntegXsecComp* xc ) :
  IntegXsec::ConvIntegrand( xc )
{
}

COFbase::~COFbase(){
}

