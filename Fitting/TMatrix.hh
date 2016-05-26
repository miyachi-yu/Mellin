#ifndef  _Fitting_TMatrix_hh_
#define  _Fitting_TMatrix_hh_

#include <QCD/PdfErrMatrix.hh>

#include <TMatrixD.h>

namespace Fit {

  class TMatrix : public TMatrixD {
  public:
    
    TMatrix();                          //!< a default constructor
    TMatrix( const QCD::PdfErrMatrix& pem ) throw( int ); 
    //!< with PdfErrMatrix
    TMatrix( const TMatrixD& tmxd ); 
    //!< with TMatrixD
    TMatrix( const int& row, const int& column );
    //!< with TMatrixD

    virtual ~TMatrix(){};                 //!< a destructor
    
    bool setMX( const QCD::PdfErrMatrix& pem );

    friend std::ostream& operator<<( std::ostream& os, const TMatrix& mx );  

    //! Text format
    enum STYLE {
      TEXT = 1,
      TEX  = 2,
    };
  
    void print( std::ostream& os, const std::vector< std::string >& hrow, 
		const std::vector< std::string >& hcol );

    void style( const STYLE& style ) { style_ = style; }
    void precision( const int& p ){ precision_ = p ;}

    std::string suffix();

  private:
    TMatrix::STYLE style_;
    int precision_;

    void header( std::ostream& os, const std::vector< std::string >& head );
    void footer( std::ostream& os );
  };
  
}

#endif	    /* !TMatrix_HH_ */
