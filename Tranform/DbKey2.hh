#ifndef _Tranform_DbKey2_hh_
#define _Tranform_DbKey2_hh_

#include "CKey.hh"

namespace Transform {
  
  /*!
    \brief  key of LookupTable 
  */
  class DbKey2 {
  public:
    
    DbKey2(); //!< a default constructor
    
    //! constructor with specific values
    DbKey2( const std::complex< double >& n1, const std::complex< double >& n2,
	   const double& x1, const double& x2 );
    
    DbKey2( const DbKey2& key ); //!< a copy constructor
    
    virtual ~DbKey2();          //!< a destructor
    
    DbKey2& operator=( const DbKey2& key );        //!< assignment operator
    bool operator<( const DbKey2& key )  const ;  //!< less than operator
    bool operator>( const DbKey2& key )  const ;  //!< grater than operator
    bool operator==( const DbKey2& key ) const ;  //!< equal operator
    
  private:
    Transform::CKey n1_;
    Transform::CKey n2_;
    double x1_;
    double x2_;
    
  };
}

#endif // _Tranform_DbKey2_hh_
