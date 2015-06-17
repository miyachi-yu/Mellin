// $Id: COFbase.hh,v 1.5 2008/08/01 09:44:03 miyachi Exp $
/*!
  \file COFBase.hh
  \brief  class definition of Coefficient Function for FragXsec_T
 */
#ifndef _FragXsec_T_COFbase_hh_
#define _FragXsec_T_COFbase_hh_

#include <Xsec/Coefficient.hh>
#include <Xsec/XsecComp.hh>

namespace FragXsec_T {
  
  /*
    \brief   COefficient Function class for FragXsec_T
  */
  class COFbase : public Xsection::Coefficient {
  public:
    COFbase();              //!< a default constructor
    virtual ~COFbase();     //!< a destructor
    
    //! ComplexFunction Implementation
    virtual std::complex< double >
    operator()( const std::complex< double >& n ) = 0;
    
    //! for 2-D case
    virtual std::complex< double >
    operator()( const std::complex< double >& m,
		const std::complex< double >& n ) {
      return (*this)( m );
    };
    
  protected:
    
  };
  
};


#endif //  _FragXsec_T_COFbase_hh_
