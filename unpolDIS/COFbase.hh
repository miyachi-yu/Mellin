// $Id: COFbase.hh,v 1.5 2008/05/23 02:08:04 miyachi Exp $
/*!
  \file  COFBase.hh
 */
#ifndef _unpolDIS_COFbase_hh_
#define _unpolDIS_COFbase_hh_

#include <Xsec/Coefficient.hh>

namespace unpolDIS {
  
  /*
    \brief a base class for COefficient Function for F1 structure function
  */
  class COFbase : public Xsection::Coefficient {
  public:
    COFbase();                 //!< a default constructor
    virtual ~COFbase();        //!< a destructor
    
    //! for 1D case
    virtual std::complex< double >
    operator()( const std::complex< double >& n ) = 0;
    
    //! for 2D case
    virtual std::complex< double >
    operator()( const std::complex< double >& m,
		const std::complex< double >& n ) {
      return (*this)( m );
    };
    
  };
  
};


#endif //  _unpolDIS_COFbase_hh_
