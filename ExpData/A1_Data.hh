#ifndef _A1_Data_hh_
#define _A1_Data_hh_

#include "DataSet.hh"

namespace Exp{
  namespace A1_Data {
    
    //***********************:JLAB-E-99-117,A1, 2004**************************
    DataSet A1( const Exp::TARGET& t ) throw( int );
    
    //************************E154,1997,A1************************************
    DataSet A1e154( const Exp::TARGET& t ) throw( int );
    
    //************************COMPASS,2006,A1*********************************
    DataSet A1compass( const Exp::TARGET& t ) throw( int );

    //************************E142,1996,A1************************************
    DataSet A1e142( const Exp::TARGET& t ) throw( int );
    
    //************************CLAS,2006,A1************************************
    DataSet A1CLAS( const Exp::TARGET& t ) throw( int );
    
  }
  
}

#endif //  _A1_Data_hh_
