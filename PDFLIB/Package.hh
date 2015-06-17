#ifndef _Package_hh_
#define _Package_hh_

#include <vector>
#include <string>

#include <QCD/Flavor.hh>
#include <Tranform/RealFunction.hh>

namespace PDFLIB {
  
  class Package : public Transform::RealFunction {
  public:
    
    Package( const std::string& path = "." ); //!< a default constructor
    Package( const Package& pdf );     //!< a copy constructor
    virtual ~Package();                //!< a destructor
    
    //! get a hard scale Q ( not Q^{2} )
    double q() const { return q_; }
    
    //! get a hard scale Q^{2}
    double q2() const { return q_*q_;}
    
    //! set a hard scale Q^{2} to the given value of q2
    virtual void setQ2( const double& q2 );
    
    //! set parton type with the given PARTON type
    void setPID( const QCD::Flavor::PARTON&   p ) { parton_ = p; mode_ = "PARTON"; }
    void setPID( const QCD::Flavor::TYPE&     p ) { type_ = p; mode_ = "TYPE"; }
    void setPID( const QCD::Flavor::SFNCTYPE& p ) { sfunc_ = p; mode_ = "SFNCTYPE"; }
    
    std::string mode() { return mode_; }    //!< get mode
    void mode( const std::string& mode ) { mode_ = mode; } //!< set mode
    
    
    std::string path() const { return path_; }        //!< get a path
    void path( const std::string& p ) { path_ = p ; } //!< set a path
    
    void   xWeight( const double& n ) { n_ = n; }     //!< set x weight
    double xWeight() const { return n_; }             //!< get x weight
    
    double weight( const double& x ) const ;          //!< get x^{n-1}
    
    std::string& name() { return name_; }
    std::string name() const { return name_; }

    virtual double operator()( const double& x );
    virtual double parton() { return 0.0;}
    virtual double type() { return 0.0;}
    virtual double func() { return 0.0;}


  protected:
    
    void load();                                      //!< call loadGrid()
    virtual void loadGrid() = 0;                      //!< load grid information     
    std::string path_;
    double q_;
    double x_;
    
    QCD::Flavor::PARTON    parton_;
    QCD::Flavor::TYPE      type_;
    QCD::Flavor::SFNCTYPE  sfunc_;
    
    std::string mode_; 
    
    double n_;
    
    std::string name_;

  };
  
};

#endif // _Package_hh_
