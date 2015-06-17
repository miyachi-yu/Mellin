// $Id: DataCache2.hh,v 1.5 2009/05/12 05:37:58 miyachi Exp $
/*!
  \file    DataCache2.hh
  \version $Revision: 1.5 $
  \date    $Date: 2009/05/12 05:37:58 $
  \author  $Author: miyachi $
  \brief   Data cache class definition
*/
#ifndef _DataCache2_hh_
#define _DataCache2_hh_

#include <map>
#include <vector>
#include <complex>
#include <iostream>


#include "CKey.hh"

namespace Transform {
  
  /*!
    \class  DataCache2  DataCache2.hh  "Tranform/DataCache2.hh"
    \brief  class for data cache 
    
    This class stores calculated values at a grid point ( N, M, Q^2 ) 
    to get the computation time shorter. 
    
  */
  class DataCache2 {
  public:
    
    /*!
      \class  Grid  Grid.hh  DataCache2.hh
      \brief  Grid class for DataCache2
      
      This class will be used as key for data caching based on 
      STL map class.
    */
    class Grid {
    public:
      //! default constructor
      Grid( const std::complex< double >& n = std::complex< double >( 0.0, 0.0 ),
	    const std::complex< double >& m = std::complex< double >( 0.0, 0.0 ),
	    const double& q2 = 1.0 );
      Grid( const Grid& grid );                   //!< copy constructor
      virtual ~Grid();                            //!< a destructor
      Grid& operator=( const Grid& grid );        //!< assignment operator
      virtual bool operator==( const Grid& grid ) const ; //!< comparison operator
      virtual bool operator< ( const Grid& grid ) const ; //!< less operator
      virtual bool operator> ( const Grid& grid ) const ; //!< garter operator
      
      //! dump grid information to std::ostream
      friend std::ostream& operator<<( std::ostream& os, const Grid& grid );
      
      double Q2() const { return Q2_; }
      CKey n() const { return n_; }
      CKey m() const { return m_; }
      
    protected:
      CKey n_;                                    //!< n moment value
      CKey m_;                                    //!< m moment value
      double Q2_;                                 //!< Q2 value
    };
    
    DataCache2();                           //!< a default constructor
    virtual ~DataCache2();                  //!< a destructor
    
    /*!
      \param[in] n is a value of moment 
      \param[in] m is a value of moment 
      \param[in] q2 is a value of scale Q2
      \return true if the data at ( n, q2 ) was cached.
      \brief  this checkes if the data at the given kinetic point was cached.
      
      Since the map alorythm automatically generate the object for the given 
      key, one must chech the existence of data for the key at first.
      It is just a wrapper method to provide such test.
    
      User must check the existence of data with this class, then try to 
      access the data with cache( n, q2 ) method.
    */
    bool cached( const std::complex< double >& n, 
		 const std::complex< double >& m, 
		 const double& q2 );
    
    /*!
      \brief  get the cached data at the given ( n, q2 ).
    */
    std::vector< std::complex< double > >& cache( const std::complex< double >& n,
						  const std::complex< double >& m,
						  const double& q2 );
    
    /*!
      \param[in] n is a value of moment
      \param[in] m is a value of moment
      \param[in] q2 is a value of scale Q2
      \param[in] data is a data to be cached
      \brief  This try to insert the give data to the cache buffer.
      
      User can store the data with this method. 
      If there is no data cached at the given ( n, q2 ), 
      data will be stored.
  */
    void cache( const std::complex< double >& n, 
		const std::complex< double >& m, 
		const double& q2, 
		std::vector< std::complex< double > >& data );
    
    void clear();                              //!< make the buffer empty
  
    //! dump cached information
    friend std::ostream& operator<<( std::ostream& os, DataCache2& cache );
    
    static int maxsize_;

    int size() { return cache_.size(); }       //!< get stored data size
    
  private:
    
    std::map< Grid, std::vector< std::complex< double > > > cache_; //!< buffer
    
    //! return if the data exist at the given grid
    bool cached( const Grid& grid ); 
    
    //! tryal version
    std::map< double, std::map< CKey, std::map< CKey, 
						std::vector< std::complex< double > > > > > cnew_;

    bool newmeth_;

    int ndata_;
    
  };

};
#endif // _DataCache2_hh_
