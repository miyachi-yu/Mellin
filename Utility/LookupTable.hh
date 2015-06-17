// $Id: LookupTable.hh,v 1.2 2008/12/09 07:49:10 miyachi Exp $
/*!
  \file    LookupTable.hh
  \version $Revision: 1.2 $
  \date    $Date: 2008/12/09 07:49:10 $
  \author  $Author: miyachi $
  \brief   Template class for LookupTable implementation
 */
#ifndef _LookupTable_hh_
#define _LookupTable_hh_

#include "config.h"

#include <exception>
#include <string>
#include <iostream>
#include <map>
#include <cstdlib>

#ifdef HAVE__SW_INCLUDE_DB4_DB_H
#include <db4/db_cxx.h>
#elif defined(HAVE__SW_INCLUDE_DB3_DB_H)
#include <db3/db_cxx.h>
#else
#include <db_cxx.h>
#endif

extern "C" {
#include <limits.h>
}

#include "LookupTableCtl.hh"

namespace Utility {
  
  /*!
    \class LookupTable  LookupTable.hh "Tanform/LookupTable.hh"
    \brief  Template class for LookupTable implementation
    
    LookupTable clas is implmented using Berkerey DB, libdb_cxx.so.
    In oerder to keep flexibility for stored data and key type,
    this LookupTable class is implemented as a template class.
    
    For the template class TKEY, 
    bool TKEY::operator=( const TKEY& key ), and 
    bool TKEY::operator<( const TKEY& key )  are required.
    
  */
  template< class TKEY, class TDAT >
  class LookupTable {
  public:
    
    /*!
      \param[in]  key is a reference of TKEY object which used as buffer
      \param[out] data is a reference of TDAT object which used as buffer
      \param[in]  path is a name of DB file
      \brief  a  constructor
      
      This is a constructor of LookupTable template class.
      The given key and data should be created in advance to this constructor,
      and they must exist until LookupTable closed.
      
      The given path is a name of the data base file, 
      which will be created if not exist. 
      In the present implementation, the data base file is located as
      ${HOME}/.mellin/cache/path.
    */
    LookupTable( TKEY& key, TDAT& data, const std::string& path ) 
      throw( std::exception& ) ;
    
    virtual ~LookupTable();             //!< a destructor
    
    void close();                       //!< close DB file
    
    /*!
      \return   true if the data is written successfully, false if not.
      \brief    put the data assosiated with key in to the DB file.
      
      It writes the data value assosiate with key value, which are store in
      the given buffer object to the constructor. 
      
    */
    bool put() ;
    
    /*!
      \param[in]  key is a buffer for key value
      \param[out] data buffer where the obtained data will be written.
      \return true if the data exist in the buffer, false otherwise
      \brief  get data assosiated with the given key
      
      It try to get data stored in LookupTable associated with the given key.
      On contrast to put method, one must give the key for which you want to
      extract data. If the corresponding data is found, the value will be
      copied to the given object. 
      
      If this falis to find the given key in LookupTable, it returns false.
      
    */
    bool get( TKEY& key, TDAT& data ) ; 
    
  private:
    
    //! open a DB file specified by the given path
    void open( const std::string& path ) throw( std::exception& ) ;
    
    Db     db_;  //!< Berkerey DB object 
    Dbt   key_;  //!< key object for DB
    Dbt  data_;  //!< data object for DB

    // ------------------- for memory wise cache ---------------------
    std::map< TKEY, TDAT > mcache_;
    
    bool isOpen_;
    std::string path_;

  };
  
  // ---------- global function and template class implementation -------- //
  
  template < class TKEY >
  int compare_keys( Db *dbp, const Dbt *a, const Dbt *b ){
    TKEY& keyA = *((TKEY*)( a->get_data() ));
    TKEY& keyB = *((TKEY*)( b->get_data() ));
    return ( keyA == keyB ? 0 : ( keyA < keyB ? -1 : 1 ) );
  }
  
  template< class TKEY, class TDAT >
  LookupTable< TKEY, TDAT >::LookupTable( TKEY& key, TDAT& data,
					  const std::string& path ) 
    throw( std::exception& ) :
    db_( NULL, 0 ), 
    key_( &key, sizeof( TKEY ) ), data_( &data, sizeof( TDAT ) ),
    mcache_(), isOpen_( false ), path_( path )
  {
    db_.set_bt_compare( compare_keys< TKEY > );
  }
  
  template< class TKEY, class TDAT >
  LookupTable< TKEY, TDAT >::~LookupTable() {
  }
  
  template< class TKEY, class TDAT >
  void LookupTable< TKEY, TDAT >::close() {
    
    if( LookupTableCtl::actl == LookupTableCtl::disable ) return;
    
    if( LookupTableCtl::actl == LookupTableCtl::memory ) {
      mcache_.clear();
      return;
    }
    
    if( isOpen_ == false ) return;
    
    try{ 
      db_.close( 0 ); 
    }
    
    catch( DbException &e  ){ 
      std::cout 
	<< __FILE__ << ":" << __LINE__ << "\t"
	<< e.what()
	<< std::endl;
      throw( std::bad_exception() ); 
    }
    
    catch( std::exception &e ){
      std::cout 
	<< __FILE__ << ":" << __LINE__ << "\t"
	<< e.what()
	<< std::endl;
      throw( std::bad_exception() ); 
    }

    isOpen_ = false;
  }
  
  template< class TKEY, class TDAT >
  void LookupTable< TKEY, TDAT >::open( const std::string& fname ) 
    throw( std::exception& ) {
    
    if( LookupTableCtl::actl == LookupTableCtl::disable ||
	LookupTableCtl::actl == LookupTableCtl::memory )  return;
    
    char resolved_path[ PATH_MAX ];
    
    try {
      
      std::string dir = LookupTableCtl::cacheDirPath();
      
      if( realpath( ( dir + "/" + fname ).c_str(), resolved_path ) == NULL ){
	std::cerr 
	  << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << "\t"
	  << "Fail to covert to real path from "
	  << std::string( dir + "/" + fname )
	  << std::endl;
	throw( 1 );
      }
      
      db_.open( NULL, resolved_path, NULL, DB_BTREE, DB_CREATE, 0 );
    }
    
    catch( DbException &e  ){ 
      std::cout 
	<< __FILE__ << ":" << __LINE__ << "\t"
	<< e.what()
	<< std::endl;
      throw( std::bad_exception() ); 
    }
    
    catch( std::exception &e ){ 
      std::cout 
	<< __FILE__ << ":" << __LINE__ << "\t"
	<< e.what()
	<< std::endl;
      throw( std::bad_exception() ); 
      throw( e ); 
    }
    
  }
  
  template< class TKEY, class TDAT >
  bool LookupTable< TKEY, TDAT >::put() {
    
    // return true if read_only mode
    if( LookupTableCtl::actl == LookupTableCtl::disable || 
        LookupTableCtl::actl == LookupTableCtl::read_only ) return true;
    
    if( isOpen_ == false ){
      this->open( path_ );
      isOpen_ = true;
    }

    if( LookupTableCtl::actl == LookupTableCtl::memory ) {
      TKEY& key = *( (TKEY*)( key_.get_data() ) );
      if( mcache_.find( key ) != mcache_.end() ) return false;
      mcache_[ key ] = *( (TDAT*) data_.get_data() );
      return true;
    }
    
    return( db_.put( NULL, &key_, &data_, DB_NOOVERWRITE ) == 0 ?
	    true : false );
  }
  
  template< class TKEY, class TDAT >
  bool LookupTable< TKEY, TDAT >::get( TKEY& key, TDAT& data ) {
    
    if( LookupTableCtl::actl == LookupTableCtl::disable ) return false;
    
    if( LookupTableCtl::actl == LookupTableCtl::memory ) {
      if( mcache_.find( key ) == mcache_.end() ) return false;
      data = mcache_[ key ];
      return true;
    }
    
    if( isOpen_ == false ){
      this->open( path_ );
      isOpen_ = true;
    }
    
    key_.set_data( &key );
    key_.set_ulen( sizeof( TKEY ) );
    
    data_.set_data( &data  );
    data_.set_ulen( sizeof( TDAT ) );
    data_.set_flags( DB_DBT_USERMEM );
    
    return( db_.get( NULL, &key_, &data_, 0 ) == 0 ? true : false );
  }
  
};
#endif // _LookupTable_hh_
