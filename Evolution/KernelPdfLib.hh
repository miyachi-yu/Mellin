// $Id: KernelPdfLib.hh,v 1.6 2013/04/05 09:02:12 miyachi Exp $
/*!
  \file  KernelPdfLib.hh
  \brief class definiton for Evolution Kernel with PDFLIB
 */
#ifndef __KernelPdfLib_hh_
#define __KernelPdfLib_hh_

#include "KernelBase.hh"

#include <Utility/LookupTable.hh>

namespace Transform {
  class Mellin;
}

namespace PDFLIB {
  class Package;
}

namespace Evolution {
  
  /*!
    \brief Evolution Kernel class with PDFLIB for Q2 evolution 
  */
  class KernelPdfLib : public KernelBase {
  public:
    
    KernelPdfLib( const std::string& name ); //!< a constructor with name
    virtual ~KernelPdfLib();                 //!< a destructor
    
  public: // ------------- KernelBase Virtual methods ---------------------- //
    
    virtual void setN( const std::complex< double >& n );   //!< set N moment    
    virtual void setQ2( const double& q2 );                 //!< set scale Q2
    
    virtual std::complex< double > q( const QCD::Flavor::PARTON& q ); //!< get PARTON q
    virtual std::complex< double > q( const QCD::Flavor::TYPE&   q ); //!< get TYPE q
    
    virtual void pdflib( PDFLIB::Package *p ); //!< set PDFLIB
    virtual double Q2();                       //!< get current value of Q2

    virtual QCD::PdfParametrization* initialDistribution(){ return NULL; };

    //!< get Mellin transformer
    Transform::Mellin* getIntegration(){ return m_; }

    std::string getname(){ return name_; }
    std::string setname( const std::string& name ){ return name_ = name ; }
    
private:
    
    std::string name_;          //!< name of this evolution class
    PDFLIB::Package *pdflib_;   //!< a pointer of PDFLIB::Package object
    Transform::Mellin *m_;                 //!< Mellin transformer
    
    // for file base cache 
    std::vector< Transform::DataCache::Grid > fkey_;
    std::vector< std::complex< double > > fdata_ ;
    std::vector< Utility::LookupTable< Transform::DataCache::Grid, 
					 std::complex< double > >* > fcache_; 
    
    void openDB();              //!< initialize cache DB
    void closeDB();             //!< initialize cache DB
    
  };
};

#endif //__KernelPdfLib_hh_
