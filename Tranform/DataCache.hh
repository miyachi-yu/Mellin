// $Id: DataCache.hh,v 1.9 2008/11/26 10:11:10 miyachi Exp $
/*!
  \file    DataCache.hh
  \version $Revision: 1.9 $
  \date    $Date: 2008/11/26 10:11:10 $
  \author  $Author: miyachi $
  \brief   Data cache class definition
*/
#ifndef _DataCache_hh_
#define _DataCache_hh_

#include <map>
#include <vector>
#include <complex>
#include <iostream>

#include "CKey.hh"

namespace Transform {
  
  /*!
    \class  DataCache  DataCache.hh  "Tranform/DataCache.hh"
    \brief  class for data cache 
    
    This class stores calculated values at a grid point ( N, Q^2 ) 
    to get the computation time shorter. 
    
  */
  class DataCache {
  public:
    
    /*!
      \class  Grid  Grid.hh  DataCache.hh
      \brief  Grid class for DataCache
      
      This class will be used as key for data caching based on 
      STL map class.
    */
    class Grid {
    public:
      //! default constructor
      Grid( const std::complex< double >& n = std::complex< double >( 1.0, 0.0 ),
	    const double& q2 = 1.0 );
      Grid( const Grid& grid );                   //!< copy constructor
      virtual ~Grid();                            //!< a destructor
      Grid& operator=( const Grid& grid );        //!< assignment operator
      virtual bool operator==( const Grid& grid ) const ; //!< comparison operator
      virtual bool operator< ( const Grid& grid ) const ; //!< less operator
      virtual bool operator> ( const Grid& grid ) const ; //!< garter operator
      
      //! dump grid information to ostream
      friend std::ostream& operator<<( std::ostream& os, const Grid& grid );
      
    protected:
      CKey n_;                  //!< moment value
      double Q2_;               //!< Q2 value
    };
    
    DataCache();                           //!< a default constructor

    DataCache( const DataCache& cache );   //!< a copy constructor

    virtual ~DataCache();                  //!< a destructor

    DataCache& operator=( const DataCache& cache ); //!< assignment operator
    
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
    bool cached( const std::complex< double >& n, const double& q2 );
    
    /*!
      \brief  get the cached data at the given ( n, q2 ).
    */
    std::vector< std::complex< double > >& cache( const std::complex< double >& n,
						  const double& q2 );
    
    /*!
      \param[in] n is a value of moment
      \param[in] q2 is a value of scale Q2
      \param[in] data is a data to be cached
      \brief  This try to insert the give data to the cache buffer.
      
      User can store the data with this method. 
      If there is no data cached at the given ( n, q2 ), 
      data will be stored.
    */
    void cache( const std::complex< double >& n, const double& q2, 
		std::vector< std::complex< double > >& data );
    
    void clear();                              //!< make the buffer empty
    
    int size() { return cache_.size(); }       //!< get stored data size
    
    //! dump cached information
    friend std::ostream& operator<<( std::ostream& os, DataCache& cache );
    
    static int maxsize_;
      
  private:
    std::map< Grid, std::vector< std::complex< double > > > cache_; //!< buffer
    
    //!< return if the data exist at the given grid
    bool cached( const Grid& grid ); 

    
  };
};
#endif // _DataCache_hh_
