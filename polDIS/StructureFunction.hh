// $Id: StructureFunction.hh,v 1.10 2008/06/09 08:19:00 miyachi Exp $
/*!
  \file     StructureFunction.hh
  \version  $Revision: 1.10 $
  \author   $Author: miyachi $
  \date     $Date: 2008/06/09 08:19:00 $
  \brief    class definition of structure functions in polarized DIS
 */
#ifndef _polDIS_StructureFunction_hh_
#define _polDIS_StructureFunction_hh_

#include <Utility/Arguments.hh>
#include <Xsec/Charge.hh>
#include <Xsec/Xsec.hh>
#include <polPDF/Evolution.hh>

namespace polDIS {
  
  /*!
    \class StructureFunction
    \brief Class for structure functions in polarized DIS
    
    Polarized structure functions are implemented in Xsec framework.
    At the moment, one can calculate only inclusive structure functions,
    g1p, g1n, and g1d.
    
    To do:   Include Fragmentation library for structure function for
    polarized SIDIS.
    
  */
  class StructureFunction : public Xsection::Xsec {
  public:
    
    //! a constructor
    StructureFunction( Utility::Arguments& args, 
		       const Xsection::Charge::TARGET& n 
		       = Xsection::Charge::p ) throw( int ) ;
    
    //! a constructor with a polPDF::Evo object
    StructureFunction( polPDF::Evo *evo,
		       const Xsection::Charge::TARGET& n 
		       = Xsection::Charge::p ) throw( int ) ;
    
    virtual ~StructureFunction(); //!< a destructor
    
  private:
  };
  
};

#endif //  _polDIS_StructureFunction_hh_

// --------------------- old implementation -----------------------------
/*  
namespace Evolution {
  class Kernel;
}

  class G1_x;
  
  class G1_n : public Transform::ComplexFunction {
  public:
    
    enum NUCLEON {
      proton = 0, neutron = 1, deuteron = 2
    };
    
    enum HADRON {
      all, pi, pi_p, pi_m, ka, ka_p, ka_m, p, p_p, p_m
    };
    
    G1_n( Evolution::Kernel* evo, 
	  const NUCLEON& nucl = proton,
	  const HADRON&  hadron = all ) throw( int );

    virtual ~G1_n();
    
    std::complex< double > operator()( const std::complex< double >& n );
    std::complex< double > operator()( const std::complex< double >& n, 
				  const double& Q2  );    
  private:
    Kernel*    evo_;
    NUCLEON    nucleon_;
    HADRON     hadron_;
    
    vector< vector< std::complex< double > > > cv_;
    vector< vector< ComplexFunction* > > cf_;

    vector< vector < DataCache::Grid > >   key_;
    vector< vector < LookupTable< DataCache::Grid, std::complex< double > >* > > db_;
    
    
    PDF a0_;
    PDF a3_;
    PDF a8_;
    PDF a15_;
    PDF a24_;
    PDF a35_;
    PDF g_;
    
    PDF u_;
    PDF d_;
    PDF s_;
    PDF c_;
    PDF b_;
    PDF t_;
    PDF ubar_;
    PDF dbar_;
    PDF sbar_;
    PDF cbar_;
    PDF bbar_;
    PDF tbar_;
    
    
    void wilsonCoefficient( const std::complex< double >& n );
    void updateC( const int& i, const int& j, 
		  const std::complex< double >& n );
    std::complex< double > Cq();
    std::complex< double > Cg();
    
    vector< std::complex< double > > data_;
    DataCache cache_;

    std::string name_;
    
    std::string db_name(  const int& i, const int& j  );
    
  };
  
  class G1_x : public InverseMellin {
  public:
    G1_x( Kernel *kernel, 
	  const G1_n::NUCLEON& nucl = G1_n::proton,
	  const G1_n::HADRON&  hadron = G1_n::all,
	  const int& low = 4, const int& high = 6, 
	  const double& precision = 1.0E-3 );
    virtual ~G1_x();
    
  private:
    G1_n g1n_;
    void initialization( const int& low, const int& high, 
			 const double& precision ) ;
  };
    
};

*/

