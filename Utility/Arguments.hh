// $Id: Arguments.hh,v 1.26 2010/04/30 08:10:05 miyachi Exp $
/*!
  \file     Arguments.hh
  \version  "$Revision: 1.26 $"
  \author   "$Author: miyachi $"
  \brief    Defintion of command line arguments paser
 */

#ifndef _Arguments_hh_
#define _Arguments_hh_

#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <iomanip>
#include <complex>
#include <fstream>

namespace Utility {

  /*!
    \class  Arguments   Arguments.hh   "Utility/Arguments.hh"
    \brief  class for command line arguments paser
    
    This is a parser for the command line arguments.
    
  */
  class Arguments {
  public:
    
    //! verbose level
    enum V_LEVEL {
      FAITAL   = 4,
      ERROR    = 3,
      WARNING  = 2,
      LOG      = 1,
      DETAIL   = 0
    };

    Arguments( const Arguments& args ); //!< copy constructor
    virtual ~Arguments();               //! destructor
    
    /*!
      \param[in] name   name is a option name interested
      \return string object which contains option value for the given name
      \brief  get the option value for the given name
    */
    std::string getOpt( const std::string& name ) ;
    
    //! check if the given option exists or not
    bool hasOpt( const std::string& name ) ;
    
    /*!
      \param[in] name is a list of option name to be checked
      \return iterator of the given container object
      
      It will check if the given option is defined in the argument
      list or not. This routine try to find from the first, 
      then if the argument is find, just return the iterator.

     */
    std::vector< std::string >::iterator 
    hasOpt( std::vector< std::string >& name ) ;
    
    //! check argument list
    std::vector< std::string >::iterator 
    hasOpt( const std::vector< std::string >::iterator& itr1,
	    const std::vector< std::string >::iterator& itr2 ) ;
    
    //! dump stored command line argument information
    friend std::ostream& operator<<( std::ostream& os, const Arguments& args );
    
    std::string command() const { return command_; } //!< get command name

    //!< get stored option information
    std::map< std::string, std::string > options() const { return opts_; }

    //!< get stored arguments information
    std::vector< std::string > arguments() const { return args_; }
    
    std::ostream& log( const V_LEVEL& level = LOG ) ;
    std::ostream& err( ) ;
    
    void usage( const std::string& name, const std::string& defval, 
		const std::string& remark ); 
    
    void usage( const std::string& name, const int& defval, 
		const std::string& remark ); 
    
    void usage( const std::string& name, const double& defval, 
		const std::string& remark ); 
    
    bool verbose() const { return verbose_; } //!< get verbose mode
    
    
    static Arguments* instance();
    static Arguments* instance( int argc, char* argv[] );
    static void callAtExit();
    
    static Arguments& ref();
    static Arguments& ref( int argc, char* argv[] );
    
    /*!
      \param[in] option is a name of argument
      \param[in] def    is a default value
      \return the option value for the given argument name
      \brief get the given option value for the argument 
    */
    //  virtual bool get( std::string option, const bool& def = false );
    
    /*!
      \param[in] option is a name of argument
      \param[in] def    is a default value
      \return the option value for the given argument name
      \brief get the given option value for the argument 
    */
    virtual int get( const std::string& option,
		     const int& def = 0 );
    
    /*!
      \param[in] option is a name of argument
      \param[in] def    is a default value
      \return the option value for the given argument name
      \brief get the given option value for the argument 
    */
    virtual double get( const std::string& option,
			const double& def = 0.0 );
    
    /*!
      \param[in] option is a name of argument
      \param[in] def    is a default value
      \return the option value for the given argument name
      \brief get the given option value for the argument 
    */
    virtual std::string get( const std::string& option,
			     const std::string& def = "" );
    
    /*! 
      \brief get complex value from option
      
      example:  --n="(4.1,2.0)" or --n=4.0  (following c++ standard format)
      
    */
    virtual std::complex< double > get( const std::string& option, 
					const std::complex< double >& def =
					std::complex< double >( 0.0, 0.0 ) );
    /*!
      \param[in] option is a name of the argument to be check
      \param[in] separator is a separator character in the option
      value
      \param[in] a default values
      \return a vector of string given with argument "option"
    */
    virtual std::vector< std::string > 
    get( const std::string& option,
	 const std::string& separaotr,
	 const std::vector< std::string >& def );
    
    //!
    virtual std::vector< int > 
    get( const std::string& option,
	 const std::string& separaotr,
	 const std::vector< int >& def );
    
    //!
    virtual std::vector< double > 
    get( const std::string& option,
	 const std::string& separaotr,
	 const std::vector< double >& def );
    
    //!
    virtual std::vector< std::complex< double > > 
    get( const std::string& option,
	 const std::string& separaotr,
	 const std::vector< std::complex< double > >& def );
    
    //! set the given option with the given value
    virtual void set( const std::string& option, const std::string& value );

    //! set the given option with the given value
    virtual void set( const std::string& option, const int& value );

    //! set the given option with the given value
    virtual void set( const std::string& option, const double& value );

    //! set the given option with the given value
    virtual void set( const std::string& option, 
		      const std::complex< double >& value );
    
    //! get number of arguments
    virtual int argc() const { return argc_; }

    //! get a pointer to number of arguments ( just for ROOT TApplication )
    virtual int* argcPtr() { return &argc_; }
    
    //! get a pointer to argument list
    virtual char** argv() const { return argv_; }
    
  public:

    static std::string NO_OPTION;
    static std::string NO_VALUE;
    
  private:

    Arguments( int argc, char* argv[] );          //! constructor
    
  protected:
    
    static Arguments* instance_; // a pointer to singleton object
    
  private:
    
    std::string command_;                       //!< command name ( = argv[0] )
    std::map< std::string, std::string > opts_; //!< option list
    std::vector< std::string > args_;    //!< argument list ( non-option type )
    
    /*!
      \param[in]  name is a name of option
      \param[in]  value is a value given for the option name
      \brief     register new option with the given value
      
      The arguments given non-option type arguments should be stored
      with 
      option( "", value )
    */
    void option( const std::string& name, const std::string& value );
    
    //! for long option
    void option( const std::string& data );
    
    std::ostream *out_;
    
    V_LEVEL verbose_;
    
    std::ofstream null_;
    
    std::map< V_LEVEL, std::string > vlabel_;

    int argc_;   //!< number of arguments
    char** argv_; //!< a poiter of arguments list
    
  };
  
};

#define ARG_REF ( Utility::Arguments::ref()  )
#define ARG_REF2( argc, argv ) ( Utility::Arguments::ref( argc, argv ) )

#define ARG_DETAIL ( ARG_REF.log( Utility::Arguments::DETAIL ) )
#define ARG_LOG    ( ARG_REF.log( Utility::Arguments::LOG ) )
#define ARG_WARN   ( ARG_REF.log( Utility::Arguments::WARNING ) )
#define ARG_ERR    ( ARG_REF.log( Utility::Arguments::ERROR )		\
		     << " [" << std::setw(24) << __FILE__		\
		     << ':'  << std::setw(5)  << __LINE__		\
		     << ':'  << std::setw(24) << __FUNCTION__ << "] " )

#define ARG_FAITAL ( ARG_REF.log( Utility::Arguments::FAITAL )		\
		     << " [" << std::setw(24) << __FILE__		\
		     << ':'  << std::setw(5)  << __LINE__		\
		     << ':'  << std::setw(24) << __FUNCTION__ << "] " )

#endif // _Arguments_hh_

