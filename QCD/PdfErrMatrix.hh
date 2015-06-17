// $Id: PdfErrMatrix.hh,v 1.15 2010/02/23 07:39:54 miyachi Exp $
/*!
  \brief  Error Matrix Class Definition file
 */
#ifndef  _QCD_PDFERRMATRIX_HH_
#define  _QCD_PDFERRMATRIX_HH_

#include <vector>
#include <iostream>

#include <ExpData/ExpData.hh>
#include <ExpData/DataSet.hh>

namespace QCD {
  
  class PdfParametrization;

  /*!
    \brief Error Matrix class
  */
  class PdfErrMatrix : public std::vector< std::vector< double > > {
  public:
    
    PdfErrMatrix();                          //!< a default constructor
    PdfErrMatrix( const int& id );           //!< a constructor with ID
    PdfErrMatrix( PdfParametrization* pdf ); //!< a constructor with PDF
    PdfErrMatrix( Exp::DataSet& data );      //!< a constructor with DataSet
    PdfErrMatrix( Exp::DataPoint& data );    //!< a constructor with DataPoint
    
    //! a constructor with DataSet container
    PdfErrMatrix( std::vector< Exp::DataSet >& data );
    
    //! a constructor with DataSet container
    PdfErrMatrix( Exp::ExpData* edata );
    
    //! a construtor with PdfParametrization
    PdfErrMatrix( std::vector< PdfParametrization* >& pdfs );
    
    PdfErrMatrix( const PdfErrMatrix& pdf );  //!< a constructor with PDF
    
    virtual ~PdfErrMatrix();                 //!< a destructor
    
    //! assignment operator
    virtual PdfErrMatrix& operator=( const PdfErrMatrix& pdf );

    //! add operator
    virtual PdfErrMatrix& operator+=( const PdfErrMatrix& pdf );

    
    virtual bool operator==( const PdfErrMatrix& em ); //!< equal operator
    virtual bool operator<( const PdfErrMatrix& em );  //!< less than operator
    virtual bool operator>( const PdfErrMatrix& em );  //!< larger than operator
    
    virtual int& ID() { return ID_; }       //!< get error matrix ID
    virtual int ID() const { return ID_; }  //!< get error matrix ID
    
    virtual void load( PdfParametrization* pdf ); //!< load from PDF
    virtual void load( Exp::DataSet& dset );       //!< load from DataSet
    virtual void load( std::vector< Exp::DataSet >& data ); //!< load from DataSet
    virtual void load( Exp::ExpData* edata );     //!< load from DataSet
    virtual void load( Exp::DataPoint& data );   //!< load from DataPoint
    
    virtual std::vector< std::string >& info() { return info_; } //!< get info
    virtual std::vector< std::string > info() const { return info_; }//!< get info

    //! information dunping operator
    friend std::ostream& operator<<( std::ostream& os, const PdfErrMatrix& em );
    
    virtual void resetMatrixSize( const int& n );
    
    void setnormflag( const bool& nflag ){ normflag_ = nflag; }

    bool& getnflag(){ return normflag_; }
    bool getnflag() const { return normflag_; }

    int& getprec(){ return prec_; }
    int getprec() const { return prec_; }

    void setprec( const int& prec ){ prec_ = prec; }
    
    //! get normalized errror matrix ( correlation matrix )
    virtual std::vector< std::vector< double > > normalized();

  private:
    
    int ID_; //!< error matrix ID
    std::vector< std::string > info_;
    
    bool normflag_;
    int prec_;
  };
  
};

#endif	    /* !PDFERRMATRIX_HH_ */
