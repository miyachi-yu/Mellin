#ifndef _hpm_Data_hh_
#define _hpm_Data_hh_

#include "DataSet.hh"

namespace Exp{
  
  namespace hpm_Data {
    
    //***********************TASSO_hpm_44_1990*******************************
    DataSet TASSO_hpm_44_1990( const Exp::HADRON& h = Exp::Hpm ) throw( int );

    //***********************TASSO_hpm_35_1990*******************************
    DataSet TASSO_hpm_35_1990( const Exp::HADRON& h = Exp::Hpm ) throw( int );

    //***********************TPC_hpm_29_1988*******************************
    DataSet TPC_hpm_29_1988( const Exp::HADRON& h = Exp::Hpm ) throw( int );

    //***********************SLD_hpm__91.2_1999*********************
    DataSet SLD_hpm_91_2_1999( const Exp::HADRON& h = Exp::Hpm ) throw( int );

    //**************************ALEPH_hpm_91.2_1998***************************
    DataSet ALEPH_hpm_91_2_1998( const Exp::HADRON& h = Exp::Hpm ) throw( int );

    //***********************OPAL_hpm_transverse_91.2_1995*******************************
    DataSet OPAL_hpm_transverse_91_2_1995( const Exp::HADRON& h = Exp::Hpm ) throw( int );

    //***********************OPAL_hpm_91.2_1995*******************************
    DataSet OPAL_hpm_longitudinal_91_2_1995( const Exp::HADRON& h = Exp::Hpm ) throw( int );

   //***********************OPAL_hpm_91.2_1999*******************************
    DataSet OPAL_hpm_b_91_2_1999( const Exp::HADRON& h = Exp::Hpm ) throw( int );

    //***********************OPAL_hpm_91.2_1999*******************************
    DataSet OPAL_hpm_inclusive_91_2_1999( const Exp::HADRON& h = Exp::Hpm ) throw( int );

    //***********************DELPHI_hpm_91.2_1995******************************
    DataSet DELPHI_hpm_91_2_1996( const Exp::HADRON& h = Exp::Hpm ) throw( int );

    //***********************OPAL_hpm_91.2_1999*******************************
    DataSet OPAL_hpm_c_91_2_1999( const Exp::HADRON& h = Exp::Hpm ) throw( int );

  }
  
}

#endif //  _hpm_Data_hh_
