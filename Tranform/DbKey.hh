#ifndef _Tranform_DbKey_hh_
#define _Tranform_DbKey_hh_

#include "CKey.hh"

namespace Transform {
  
  /*!
    \brief  key of LookupTable 
  */
  class DbKey {
  public:
    
    DbKey(); //!< a default constructor
    
    //! constructor with specific values
    DbKey( const std::complex< double >& n1, const double& x1 );
    
    DbKey( const DbKey& key ); //!< a copy constructor
    
    virtual ~DbKey();          //!< a destructor
    
    DbKey& operator=( const DbKey& key );        //!< assignment operator
    bool operator<( const DbKey& key )  const ;  //!< less than operator
    bool operator>( const DbKey& key )  const ;  //!< grater than operator
    bool operator==( const DbKey& key ) const ;  //!< equal operator
    
  private:
    Transform::CKey n1_;
    double x1_;
    
  };
}

#endif // _Tranform_DbKey_hh_
