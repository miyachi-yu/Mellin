// $Id: PdfSimpleXml.hh,v 1.5 2008/09/24 05:45:28 miyachi Exp $
/*!
  \file     ConfigPdf.hh
  \version  $Revision: 1.5 $
  \author   $Author: miyachi $
  \brief    Definition of PDF configuration file handling class
 */
#ifndef _PdfSimpleXml_hh_
#define _PdfSimpleXml_hh_

#include <list>
#include <vector>
#include <string>

#include "PdfBase.hh"

namespace xmlpp {
  class Node;
  class Element;
}

namespace QCD {

  /*!
    \class  PdfSimpleXml   PdfSimpleXml.hh   "QCD/PdfSimpleXml.hh"
    \brief  Class for PDF configuration file handling
    
    This derived class from Config reads PDF configuration 
    given in XML format ( see sample pdfset.xml ).
    
    This sample implementation, PdfSimple objects will be created
    and stored in PdfParameterization object.
    When different type of functional form is implemented as a
    PdfBase derived class, it should be also done that 
    the correspoinding PdfSimpleXml derive class is generated,
    where PdfBase* pdf() method should be implemented with 
    newly created PdfBase derived class.
    
  */
  class PdfSimpleXml {
  public:
    
    PdfSimpleXml() throw( int ); //!< a constructor
    virtual ~PdfSimpleXml();         //!< a destructor
    
    virtual double scale() const { return scale_; }  //!< get initial scale
    virtual double& scale() { return scale_; }       //!< get initial scale
    
    virtual std::string name() const { return name_; } //!< get name of PDF set
    virtual std::string& name() { return name_; }      //!< get name of PDF set
    
    //! create vector of pointers of PdfBase and return
    virtual std::vector< PdfBase* > pdfs( std::list< xmlpp::Node* >& pdfs ); 
    
  protected:  
    
    virtual bool load( xmlpp::Element* pdfset ); 
    
    //! create PdfBase object from the stored parameters in configuration file
    virtual PdfBase* pdf( xmlpp::Element* elm );
    
    //! set the given pdf object with the given xml elemet
    virtual void set( PdfBase *pdf, xmlpp::Element* elm );
    
  private:
    double scale_; //!< inital scale given in the configuraiton
    std::string name_;  //!< name of pdf given in the configuration
    
  };
};
#endif // _PdfSimpleXml_hh_
