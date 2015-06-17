#ifndef _RExpData_hh_
#define _RExpData_hh_

#include <string>
#include <TGraphErrors.h>
#include <Rtypes.h>

#include "ExpData/DataSet.hh"

class RExpData : public TGraphErrors {
public:
  
  RExpData( Exp::DataSet& dset, const std::string& key );
  RExpData( Exp::DataSet& dset, 
	    const std::string& min, const std::string& max);
  
  virtual ~RExpData();
  
  void style( const Style_t& type, Width_t& size, const Color_t& color );
  TGraphErrors *model_;
  TGraphErrors *fit_;
  
  void rescale( const double& factor );

  void offset( const double& value );
  void draw( const bool& fFit = true, const bool& fModel = false );

  
private:
  

  ClassDef( RExpData, 1 )
};

#endif // _RExpData_hh_
