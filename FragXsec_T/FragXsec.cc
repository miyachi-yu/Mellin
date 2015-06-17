// $Id: FragXsec.cc,v 1.9 2008/10/28 09:12:26 miyachi Exp $
/*!
  \file    FragXsec.cc
  \version $Revision: 1.9 $
  \author  $Author: miyachi $
  \date    $Date: 2008/10/28 09:12:26 $
  \brief Class implementation for cross section calculation for hadron production
 */
#include "FragXsec.hh"
#include "XsecComp.hh"

using namespace std;
using namespace Utility;
using namespace FragXsec_T;

FragXsec::FragXsec( Arguments& args, 
		    const QCD::Flavor::HADRON& h ) throw( int ):
  Xsec()
{
  
  try{
    
    this->xsecComp( new XsecComp( args, h ) );
    if( this->xsecComp() == NULL ) throw 1 ;
    
  }
  
  catch( int error ) {
    cerr << __FILE__ << ":" << __LINE__ << "\tcatch error ("
	 << error << ") !!" << endl;
    throw error;
  }
}

FragXsec::FragXsec( Evolution::Kernel *kernel,
		    const QCD::Flavor::HADRON& h ) throw( int ):
  Xsec()
{
  
  try{
    
    this->xsecComp( new XsecComp( kernel, h ) );
    if( this->xsecComp() == NULL ) throw 1 ;
    
  }
  
  catch( int error ) {
    cerr << __FILE__ << ":" << __LINE__ << "\tcatch error ("
	 << error << ") !!" << endl;
    throw error;
  }
}

FragXsec::~FragXsec(){
  delete this->xsecComp() ;
}

