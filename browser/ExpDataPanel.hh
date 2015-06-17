//
// ExpDataPanel.hh
// 
// Made by Yoshiyuki Miyachi
// Login   <miyachi@yoshiyuki-desktop>
// 
// Started on  Fri Sep 26 15:50:12 2008 Yoshiyuki Miyachi
// Last update Tue Apr 28 18:35:58 2009 Yoshiyuki Miyachi
//

#ifndef   	EXPDATAPANEL_HH_
# define   	EXPDATAPANEL_HH_

#include <string>
#include <vector>

#include <TCanvas.h>

class TGraph;
class TH1;

#include <ExpData/ExpData.hh>

class ExpDataPanel : public TCanvas {
public:
  
  ExpDataPanel( const Exp::ExpData& ed, 
		const std::string& name,
		const std::string& xkey,
		const std::string& xkey2 = "" );
  
  virtual ~ExpDataPanel();
  
  virtual void draw( const std::string& filename = "" );

  std::vector< TGraph* >& getTGraph(){ return graphs_; }

  void addTGraphs( std::vector< TGraph* > ag ){ addgraphs_ = ag; }
  void addEgraphs( const Exp::ExpData& ed, const std::string& xkey = "", 
		   const std::string& xkey2 = "" );
  
  void setXmin( const double& xmin );
  void setXmax( const double& xmax );
  void setYmin( const double& ymin );
  void setYmax( const double& ymax );

private:
  
  void load( const Exp::ExpData& ed );
  void load( const Exp::DataSet& ed );
  
  std::string xkey_;
  std::string xkey2_;
  std::vector< TGraph* > graphs_;
  std::vector< TGraph* > addgraphs_;
  
  double xmin_;
  double xmax_;
  double ymin_;
  double ymax_;

  TH1* frame_;

};

#endif	    /* !EXPDATAPANEL_HH_ */
