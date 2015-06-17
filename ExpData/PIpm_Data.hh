#ifndef _PIpm_Data_hh_
#define _PIpm_Data_hh_

#include "DataSet.hh"

using namespace std;

namespace Exp{
  
  namespace PIpm_Data {
    
    //***********************TASSO_PIpm_44_1989*******************************
    DataSet TASSO_PIpm_44_1989( const Exp::HADRON& h = Exp::PIpm ) throw( int );

    //***********************TASSO_PIpm_34_1989*******************************
    DataSet TASSO_PIpm_34_1989( const Exp::HADRON& h = Exp::PIpm ) throw( int );

    //***********************TPC_PIpm_29_1988*******************************
    DataSet TPC_PIpm_29_1988( const Exp::HADRON& h = Exp::PIpm ) throw( int );

    //***********************SLD_PIpm_inclusive_91.2_1999*********************
    DataSet SLD_PIpm_inclusive_91_2_1999( const Exp::HADRON& h = Exp::PIpm ) throw( int );

    //***********************SLD_PIpm_c_91.2_1999*****************************
    DataSet SLD_PIpm_c_91_2_1999( const Exp::HADRON& h = Exp::PIpm ) throw( int );

    //***********************SLD_PIpm_b_91.2_1999*******************************
    DataSet SLD_PIpm_b_91_2_1999( const Exp::HADRON& h = Exp::PIpm ) throw( int );

    //**************************ALEPH_PIpm_91.2_1995***************************
    DataSet ALEPH_PIpm_91_2_1995( const Exp::HADRON& h = Exp::PIpm ) throw( int );

    //***********************OPAL_PIpm_91.2_1994*******************************
    DataSet OPAL_PIpm_91_2_1994( const Exp::HADRON& h = Exp::PIpm ) throw( int );

    //***********************DELPHI_PIpm_inclusive_91.2_1998******************
    DataSet DELPHI_PIpm_inclusive_91_2_1998( const Exp::HADRON& h = Exp::PIpm ) throw( int );

    //***********************DELPHI_PIpm_b_91.2_1998**************************
    DataSet DELPHI_PIpm_b_91_2_1998( const Exp::HADRON& h = Exp::PIpm ) throw( int );
  }
}

#endif //  _PIpm_Data_hh_
