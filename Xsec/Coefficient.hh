// $Id: Coefficient.hh,v 1.13 2009/04/24 09:33:34 imazu Exp $
/*!
  \file    Coefficient.hh
  \version $Revision: 1.13 $
  \author  $Author: imazu $
  \date    $Date: 2009/04/24 09:33:34 $
  \brief   class definition for Willson coefficient function
 */
#ifndef _Xsec_Coefficient_hh_
#define _Xsec_Coefficient_hh_

#include <Tranform/ComplexFunction.hh>
#include <Tranform/GeneralHarmonic.hh>
#include <Tranform/DataCache.hh>
#include <Tranform/DataCache2.hh>

namespace Xsection {
  
  class XsecCompBase;
  
  /*!
    \class Coefficient 
    
  */
  class Coefficient : public Transform::ComplexFunction {
  public:
    
    Coefficient();               //!< a default constructor
    virtual ~Coefficient();      //!< a destructor
    
    virtual std::complex< double >
    operator()( const std::complex< double >& z );
    
    virtual std::complex< double >
    operator()( const std::complex< double >& m,
		const std::complex< double >& n );
    
    virtual void set( Xsection::XsecCompBase* xc );
    
    virtual double rescale( const int& i = 0 );
    
    virtual std::complex< double > eval( const std::complex< double >& n );
    
    virtual std::complex< double > eval( const std::complex< double >& n,
					 const std::complex< double >& m );
    
  protected:
    
    XsecCompBase* xc_;
    static Transform::GeneralHarmonic s_;        //!< complex function S_m( N )
    
  private:
    Transform::DataCache cache_;
    Transform::DataCache2 cache2_;
    std::vector< std::complex< double > > data_;
    
  };

};



#endif //  _Xsec_Coefficient_hh_
