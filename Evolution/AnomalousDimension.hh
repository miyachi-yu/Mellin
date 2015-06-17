// $Id: AnomalousDimension.hh,v 1.26 2008/12/02 08:13:15 miyachi Exp $
/*!
  \file    AnomalousDimension.hh
  \version $Revision: 1.26 $
  \author  $Author: miyachi $
  \brief   Definition of Anomalous Dimension Base class
 */
#ifndef _AnomalousDimension_hh_
#define _AnomalousDimension_hh_

#include <QCD/Scheme.hh>

#include <Utility/LookupTable.hh>

#include <Tranform/GeneralHarmonic.hh>
#include <Tranform/GeneralHarmonicPrime.hh>
#include <Tranform/GeneralHarmonicTilde.hh>
#include <Tranform/DataCache.hh>

#include <string>
#include <iostream>
#include <vector>
#include <map>

namespace Evolution {

  class Kernel;
  
  /*!
    \class AnomalousDimension AnomalousDimension.hh "Evolution/AnomalousDimension.hh"
    \brief  Anomalous Dimension Base class
    
    This is a base class for Anomalous Dimensiton Implementation.
    The common values used in the anomalous dimension such as n, 1/n, 1/(1+n),
    and so on will be prepared within this class. 
    In addition to these quantities, PolyGamma function and others, 
    relevant for the anomalous dimension also should be prepared here.
  */
  class AnomalousDimension {
  public:
    
    //!  type of anomalous dimension
    enum TYPE {
      Pqq = 0, Pqg = 1, Pgq = 2, Pgg = 3, Pnsp = 4, Pnsm = 5, Pnsv = 6
    };
    
    AnomalousDimension( Kernel* k = NULL );   //!< a default constructor
    virtual ~AnomalousDimension();            //!< destructor
    
    //! set Q2 value
    virtual void setQ2( const double& q2 ) { q2_ = q2; }
    virtual double Q2() { return q2_; }      //!< get Q2
    
    std::complex< double > r( const AnomalousDimension::TYPE& type, 
			      const QCD::Scheme::ORDER& order, 
			      const std::complex< double >& n );
    
    Kernel* kernel() { return kernel_; }       //!< get pointer to Kernel object

    //! set pointer to Kernel object
    Kernel* kernel( Kernel *k ) { return kernel_ = k; }  
    Kernel* kernel() const { return kernel_; } //!< get pointer to Kernel object
    
    //! dump Kernel information to output stream
    friend std::ostream& operator<<( std::ostream& os, AnomalousDimension& ad );
    
    std::string name( AnomalousDimension::TYPE type ); //!< get name of type
    
    void useDB( const bool& state ) { useDB_ = state; } //!< set DB flag
    bool useDB() { return useDB_; }                     //!< get DB flag
    
    void nf( const double& n );  //!< set Nf
    double nf() { return Nf_; }  //!< get present number of flavor
    
    virtual void openDB();       //!< open DataBase Files
    virtual void closeDB();      //!< close DataBase Files
    
  protected:
    
    double  TF;                  //!< TF value
    std::string  name_;
    Kernel *kernel_;             //!< a pointer to Kernel object
    double  q2_;                 //!< scale value
    double  Nf_;                 //!< number of flavor at present Q2
    
    //! pointers to anomalous dimension
    std::vector< std::vector < Transform::ComplexFunction* > > p_; 
    
    //! for data base file
    bool useDB_;
    std::vector< std::vector< Transform::DataCache::Grid > > key_;
    std::vector< std::vector< std::complex< double > > > data_ ;
    std::vector< 
      std::vector< 
	Utility::LookupTable< 
	  Transform::DataCache::Grid, 
	  std::complex< double > >* > > cache_; 
    
    static bool initialized;              //!< initialization flag for static 
    static bool initialization();         //!< initalization of static variables
    
    static Transform::GeneralHarmonic s_;        //!< complex function S_m( N )
    static Transform::GeneralHarmonicPrime sp_;  //!< S'_l(n)
    static Transform::GeneralHarmonicTilde st_;  //!< S~_l(n)
    
    //! get database data filename
    std::string db_name( AnomalousDimension::TYPE type, 
			 QCD::Scheme::ORDER order);         

  };
};
#endif //  _AnomalousDimension_hh_
