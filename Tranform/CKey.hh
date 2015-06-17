// $Id: CKey.hh,v 1.5 2008/07/10 04:01:32 miyachi Exp $
/*!
  \file     CKey.hh
  \version  $Revision: 1.5 $
  \date     $Date: 2008/07/10 04:01:32 $
  \brief    Definition of extension of complex< double >
 */
#ifndef _CKey_hh_
#define _CKey_hh_

#include <iostream>
#include <complex>

namespace Transform {

/*!
  \class CKey  CKey.hh  "Tranform/CKey.hh"
  \brief extended class of std::complex< double > 

  This is a derived class from std::complex< double > to realize complex variable
  as key for lookup table. 
  Lookup table implementation using map in STL or Berkerey DB interface 
  (BTREE base DB), requires that Key should be orderd and some operators.

  STL complex does not provide operator< so that this class implements
  missing operators and methods requried by map class, originally.
 */
  class CKey : public std::complex< double > {
  public:

    CKey();                              //!< a default constructor
    CKey( double r, double i );          //!< constructor like complex< double >
    
    //! constructor from complex< double >
    CKey( const std::complex< double >& c );  
    
    CKey( const CKey& c );               //!< copy constructor
    
    virtual ~CKey();                     //!< a destructor
    
    CKey& operator=( const CKey& c );          //!< assignment operator
    bool  operator<( const CKey& c )  const ;  //!< less than operator
    bool  operator>( const CKey& c )  const ;  //!< grater than operator
    bool  operator==( const CKey& c ) const ;  //!< equal operator
    
  };
};
#endif //  _CKey_hh_
