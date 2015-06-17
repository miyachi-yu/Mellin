// $Id: FFDisFavor.hh,v 1.3 2008/12/12 09:04:50 miyachi Exp $
/*!
  \file  FFDisFavor.hh
  \brief Class definition for a dis-favored fragmentation function
 */
#ifndef  __QCD_FFDisFavor_hh__
#define  __QCD_FFDisFavor_hh__

#include "PdfBase.hh"

namespace QCD {
  
  /*!
    \brief a class for dis-favored fragmentation function

    The relation between favored and dis-favored faragmentation functions
    is usually given as
    \f[
    D_{\rm dis}( z ) = ( 1 - z ) D_{\rm fav}.
    \f]
    The mellen transformed version becomes then,
    \f[
    D_{\rm dis}( m ) = D_{\rm fav}( m ) - D_{\rm fav}( n + 1 ).
    \f]
    Thun, principly from a fragmentation function with any functional form,
    one can build dis-favroed fragmentation function.

   */
  class FFDisFavor : public PdfBase {
  public:
    
    /*!
      \param[in] favored is a favoured fragmentation function object
      \brief     a constructor with the given favored fragmentation function
    */
    FFDisFavor(); 
    virtual ~FFDisFavor();                       //!< a destructor
    
    //! return FF(m)
    virtual std::complex< double > 
    operator() ( const std::complex< double >& z );
    
    //! return FF(x)
    virtual double x( const double& x );
    
    //!< update local stored variables using the present parameters
    virtual void update();
    
    //! set favored fragmentation function
    void set( PdfBase* favored ) { this->reference( favored ); }
    
    //! get a pointer to favored FF object
    PdfBase* favored() { return this->reference(); }
    
    /*!
      \brief  get value for the given i-th parameter
      
      Dis-favored fragmentation fucntion does not have any parameters,
      so that it just return a parameter stored in the "favored"
      fragmentation function.
      
      In order to return correct parameters, 
      this rule must be fullfilled in any FF parameterization.
      0:   2nd-moment
      1:   power of z
      2:   power of 1 - z
      especially for 0th and 2nd parametes should not be different...

     */
    virtual double getValue( const int& i ); 

    //! dump stored information to the give ostream object
    friend std::ostream& operator<<( std::ostream& os, FFDisFavor& ff );

  private:
    
  };
  
};

#endif // __QCD_FFDisFavor_hh__


