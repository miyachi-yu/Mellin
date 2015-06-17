#include "COFbase.hh"

using namespace IntegUnpolDIS;

COFbase::COFbase( IntegXsec::IntegXsecComp* xc ) :
  IntegXsec::ConvIntegrand( xc )
{
}

COFbase::~COFbase(){
}

