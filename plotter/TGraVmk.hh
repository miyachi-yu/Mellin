#ifndef _TGraVmk_hh_
#define _TGraVmk_hh_

#include <vector>
#include <cmath>

/*
namespace forFit{
  Double_t forfit( const Double_t* x, const Double_t* par ); // for TF1
  std::vector< double > fparv_( 5, 0.0 ); // parameters of a polynominal
}
*/

class TGraph;

class TGraVmk : public std::vector< TGraph* > {
public:
  TGraVmk();
  virtual ~TGraVmk();

  bool setvec( const std::vector< double >& x,
	       const std::vector< double >& y );

  void setinfo( const std::string& info );
  void setinfo2( const std::string& info2 );

  void setxlimit( const double& xlim ){ xlimit_ = fabs( xlim ); }
  void setylimit( const double& ylim )
  { yfrag2_ = true; ylimit_ = fabs( ylim ); }

  void setxupperlimit( const double& xuplim ){ xupperlimit_ = xuplim; }
  void setxlowerlimit( const double& xlowlim ){ xlowerlimit_ = xlowlim; }

  void setyfrag( const bool& yfrag = true ){ yfrag_ = yfrag; }

  double getylimit()
  { return ( ! ylimit_ ? xlimit_ * xlimit_: ylimit_ ); }
  double getylimit() const
  { return ( ! ylimit_ ? xlimit_ * xlimit_: ylimit_ ); }
  double getxlimit()
  { return xlimit_; }
  double getxlimit() const
  { return xlimit_; }

  std::vector< std::vector< double > > getvminmax()
  { return vminmax_; }
  std::vector< std::vector< double > > getvminmax() const 
  { return vminmax_; }

  void setsampleTG(); // set sample TGraph with current funcparv_

  std::vector< double >& getfparv(){ return funcparv_; }
  std::vector< double > getfparv() const { return funcparv_; }

  std::vector< TGraph* >& getsampleTG(){ return sampleTG_; }
  std::vector< TGraph* > getsampleTG() const { return sampleTG_; }

  std::vector< std::string >& getvinfo(){ return vinfo_; }
  std::vector< std::string > getvinfo() const { return vinfo_; }  

  std::vector< std::string >& getvinfo2(){ return vinfo2_; }
  std::vector< std::string > getvinfo2() const { return vinfo2_; }  

  void setbaseinfo( const std::string& str ){ baseinfo_ = str; }
  std::string getbaseinfo(){ return baseinfo_; }
  std::string getbaseinfo() const { return baseinfo_; }

  // operate a fit to tgraph & set opt. params to funcparav_ 
  void polyfitToTG( const int& itg );

  virtual double operator()( const double& x );
  virtual double operator()( const double& x ) const;

private:
  bool yfrag_;  // frag for ylimit condition (for Tplot)
  bool yfrag2_; // frag for ylimit condition (for Dplot)

  double xlimit_; // limit in x value for array registration
  double ylimit_; // limit in y value for array registration

  double xupperlimit_; // additional limit in x value for array registration
  double xlowerlimit_; // additional limit in x value for array registration

  void set_min_max( const double& x, const double& y );

  std::vector< double > tminmax_;

  std::vector< std::vector< double > > vminmax_;

  std::vector< TGraph* > sampleTG_;

  std::vector< std::string > vinfo_;
  std::vector< std::string > vinfo2_;

  std::string baseinfo_;

  std::vector< double >& funcparv_;

};

#endif // _TGraVmk_hh_
