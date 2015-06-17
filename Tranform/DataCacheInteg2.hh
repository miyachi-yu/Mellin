#ifndef _DataCacheInteg2_hh_
#define _DataCacheInteg2_hh_

#include <map>
#include <vector>
#include <complex>
#include <iostream>

#include "DbKey2.hh"

namespace Transform {
  
  class DataCacheInteg2 {
  public:
    
    class Grid {
    public:
      //! default constructor
      Grid( const DbKey2& n 
	    = DbKey2( std::complex< double >( 1.0, 0.0 ),
		      std::complex< double >( 1.0, 0.0 ), 1.0, 1.0 ), 
	    const double& q2 = 1.0 );
      Grid( const Grid& grid );                   //!< copy constructor
      virtual ~Grid();                            //!< a destructor
      Grid& operator=( const Grid& grid );        //!< assignment operator
      virtual bool operator==( const Grid& grid ) const ; 
      //!< comparison operator
      virtual bool operator< ( const Grid& grid ) const ; //!< less operator
      virtual bool operator> ( const Grid& grid ) const ; //!< garter operator
      
      //! dump grid information to ostream
      friend std::ostream& operator<<( std::ostream& os, const Grid& grid );
      
    protected:
      DbKey2 n_;                  //!< kinematic valiables
      double Q2_;               //!< Q2 value
    };
    
    DataCacheInteg2();                           //!< a default constructor

    DataCacheInteg2( const DataCacheInteg2& cache );   //!< a copy constructor

    virtual ~DataCacheInteg2();                  //!< a destructor

    DataCacheInteg2& operator=( const DataCacheInteg2& cache ); 
    //!< assignment operator
    
    /*!
      \param[in] n is a value of moment 
      \param[in] q2 is a value of scale Q2
      \return true if the data at ( n, q2 ) was cached.
      \brief  this checkes if the data at the given kinetic point was cached.
      
      Since the map alorythm automatically generate the object for the given 
      key, one must chech the existence of data for the key at first.
      It is just a wrapper method to provide such test.
      
      User must check the existence of data with this class, then try to 
      access the data with cache( n, q2 ) method.
    */
    bool cached( const DbKey2& n, const double& q2 );
    bool cached( const std::complex< double >& n1, 
		 const std::complex< double >& n2, 
		 const double& x1, const double& x2, 
		 const double& q2 );

    /*!
      \brief  get the cached data at the given ( n, q2 ).
    */
    std::vector< std::complex< double > >& cache( const DbKey2& n,
						  const double& q2 );
    std::vector< std::complex< double > >& 
    cache( const std::complex< double >& n1, const std::complex< double >& n2, 
	   const double& x1, const double& x2, const double& q2 );
    
    /*!
      \param[in] n is a kinematic valriables
      \param[in] q2 is a value of scale Q2
      \param[in] data is a data to be cached
      \brief  This try to insert the give data to the cache buffer.
      
      User can store the data with this method. 
      If there is no data cached at the given ( n, q2 ), 
      data will be stored.
    */
    void cache( const DbKey2& n, const double& q2, 
		std::vector< std::complex< double > >& data );
    void cache( const std::complex< double >& n1, 
		const std::complex< double >& n2, 
		const double& x1, const double& x2, 
		const double& q2, 
		std::vector< std::complex< double > >& data );
    
    void clear();                              //!< make the buffer empty
    
    int size() { return cache_.size(); }       //!< get stored data size
    
    //! dump cached information
    friend std::ostream& operator<<( std::ostream& os, DataCacheInteg2& cache );
    
    static int maxsize_;
      
  private:
    std::map< Grid, std::vector< std::complex< double > > > cache_; //!< buffer
    
    //!< return if the data exist at the given grid
    bool cached( const Grid& grid ); 

    
  };
};
#endif // _DataCacheInteg2_hh_
