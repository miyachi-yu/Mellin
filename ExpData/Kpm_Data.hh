#ifndef _Kpm_Data_hh_
#define _Kpm_Data_hh_

#include "DataSet.hh"

namespace Exp {
  
  namespace Kpm_Data {
    
    //***********************TASSO_Kpm_44_1989*******************************
    DataSet TASSO_Kpm_44_1989( const Exp::HADRON& h = Exp::Kpm ) throw( int );
    
    //***********************TASSO_Kpm_34_1989*******************************
    DataSet TASSO_Kpm_34_1989( const Exp::HADRON& h = Exp::Kpm ) throw( int );

    //***********************TPC_Kpm_29_1988*******************************
    DataSet TPC_Kpm_29_1988( const Exp::HADRON& h = Exp::Kpm ) throw( int );

    //***********************SLD_Kpm_inclusive_91.2_1999*********************
    DataSet SLD_Kpm_inclusive_91_2_1999( const Exp::HADRON& h = Exp::Kpm ) throw( int );

    //***********************SLD_Kpm_c_91.2_1999*****************************
    DataSet SLD_Kpm_c_91_2_1999( const Exp::HADRON& h = Exp::Kpm ) throw( int );

    //***********************SLD_Kpm_b_91.2_1999*******************************
    DataSet SLD_Kpm_b_91_2_1999( const Exp::HADRON& h = Exp::Kpm ) throw( int );

    //**************************ALEPH_Kpm_91.2_1995***************************
    DataSet ALEPH_Kpm_91_2_1995( const Exp::HADRON& h = Exp::Kpm ) throw( int );

    //***********************OPAL_Kpm_91.2_1994*******************************
    DataSet OPAL_Kpm_91_2_1994( const Exp::HADRON& h = Exp::Kpm ) throw( int );

    //***********************DELPHI_Kpm_91.2_1995******************************
    DataSet DELPHI_Kpm_91_2_1995( const Exp::HADRON& h = Exp::Kpm ) throw( int );

    //***********************DELPHI_Kpm_inclusive_91.2_1998******************
    DataSet DELPHI_Kpm_inclusive_91_2_1998( const Exp::HADRON& h = Exp::Kpm ) throw( int );

    //***********************DELPHI_Kpm_b_91.2_1998**************************
    DataSet DELPHI_Kpm_b_91_2_1998( const Exp::HADRON& h = Exp::Kpm ) throw( int );
  }
}
#endif //  _Kpm_Data_hh_
