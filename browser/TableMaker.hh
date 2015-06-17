//
// TableMaker.hh
// 
// Made by Yoshiyuki Miyachi
// Login   <miyachi@yoshi-desktop>
// 
// Started on  Tue Nov 18 11:08:29 2008 Yoshiyuki Miyachi
// Last update Tue Nov 18 11:08:29 2008 Yoshiyuki Miyachi
//

#ifndef   	TABLEMAKER_HH_
# define   	TABLEMAKER_HH_

#include <iostream>
#include <vector>
#include <map>

namespace QCD {
  class PdfBase;
  class PdfParametrization;
  class PdfErrMatrix;
}

namespace Exp {
  class DataSet;
  class ExpData;
}

/*!
  \brief class for making table using several information
*/
class TableMaker {
public:
  
  //! Text format
  enum STYLE {
    TEXT = 1,
    TEX  = 2,
    HTML = 3
  };
  
  //! Verbose level
  enum LEVEL {
    SIMPLE = 1,
    DETAIL = 2
  };
  
  TableMaker();
  virtual ~TableMaker();
  
  void style( const STYLE& style ) { style_ = style; }

  friend std::ostream& operator<<( std::ostream& os, const TableMaker& maker );
  
  void print( std::ostream& os, QCD::PdfErrMatrix* em ) const;
  void print( std::ostream& os, QCD::PdfParametrization* param ) const ;
  void print( std::ostream& os, QCD::PdfBase* param, std::map< int, std::string >& head ) const ;
  void print( std::ostream& os, Exp::DataSet* dset ) const;
  void print( std::ostream& os, Exp::ExpData* data ) const;


  std::map< int, std::string > header( QCD::PdfParametrization* param ) const ;
  std::map< int, std::string > header( QCD::PdfErrMatrix* em ) const ;
  
  void header( std::ostream& os, std::map< int, std::string >& head ) const ;
  void footer( std::ostream& os ) const ;
  
  void precision( const int& p ){ precision_ = p ;}
  void withError( const bool& state ){ withError_ = state; }
  
  std::string suffix();
  
private:
  
  std::vector< QCD::PdfParametrization* > params_;
  
  STYLE style_;
  LEVEL level_;
  
  int precision_;
  
  bool withError_;
  
  void update_name( std::string& name ) const ;
  
};

#endif	    /* !TABLEMAKER_HH_ */
