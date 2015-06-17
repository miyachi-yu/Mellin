#ifndef _P_Data_hh_
#define _P_Data_hh_

#include "DataSet.hh"

using namespace std;

namespace Exp{
  
  namespace P_Data {
    
    //***********************TASSO_P_44_1989*******************************
    DataSet TASSO_P_44_1989( const Exp::HADRON& h = Exp::PRpm ) throw( int );

    //***********************TASSO_P_34_1989*******************************
    DataSet TASSO_P_34_1989( const Exp::HADRON& h = Exp::PRpm ) throw( int );

    //***********************TPC_P_29_1988*******************************
    DataSet TPC_P_29_1988( const Exp::HADRON& h = Exp::PRpm ) throw( int );

    //***********************SLD_P_inclusive_91.2_1999*********************
    DataSet SLD_P_inclusive_91_2_1999( const Exp::HADRON& h = Exp::PRpm ) throw( int );

    //***********************SLD_P_c_91.2_1999*****************************
    DataSet SLD_P_c_91_2_1999( const Exp::HADRON& h = Exp::PRpm ) throw( int );

    //***********************SLD_P_b_91.2_1999*******************************
    DataSet SLD_P_b_91_2_1999( const Exp::HADRON& h = Exp::PRpm ) throw( int );

    //**************************ALEPH_P_91.2_1995***************************
    DataSet ALEPH_P_91_2_1995( const Exp::HADRON& h = Exp::PRpm ) throw( int );

    //***********************OPAL_P_91.2_1994*******************************
    DataSet OPAL_P_91_2_1994( const Exp::HADRON& h = Exp::PRpm ) throw( int );

    //***********************DELPHI_P_inclusive_91.2_1998******************
    DataSet DELPHI_P_inclusive_91_2_1998( const Exp::HADRON& h = Exp::PRpm ) throw( int );

    //***********************DELPHI_P_b_91.2_1998**************************
    DataSet DELPHI_P_b_91_2_1998( const Exp::HADRON& h = Exp::PRpm ) throw( int );
  }
  
}

#endif //  _P_Data_hh_
