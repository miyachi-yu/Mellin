/*!
  \brief  Implementation of Fitting Data Base class
 */
#include <iostream>
#include <iomanip>
#include <sstream>

#include "FitData.hh"

#include <ExpData/ExpData.hh>
#include <ExpData/DataSet.hh>
#include <ExpData/DataPoint.hh>

using namespace std;
using namespace Fit;

FitData::FitData() : 
  it_( 0 ), ih_( 0 ),
  sig_( 0.0 ), dsig_( 0.0 ), q2_( 0.0 ), fit_( 0.0 ),
  c2_( 0.0 ), c2weight_( 1.0 ), sigUsed_( false ), expInfo_( "" ),
  dset_( NULL ), point_( NULL )
{
}

FitData::FitData( const FitData& data ): 
  it_( data.it_ ), ih_( data.ih_ ),
  sig_( data.sig_ ), dsig_( data.dsig_ ), q2_( data.q2_ ), fit_( data.fit_ ),
  c2_( data.c2_ ), c2weight_( data.c2weight_ ),
  sigUsed_( data.sigUsed_ ), expInfo_( data.expInfo_ ),
  dset_( data.dset_ ), point_( data.point_ )
{
}

FitData::~FitData(){
}

FitData& FitData::operator=( const FitData& data ){
  if( &data != this ){
    it_      = data.it_ ;
    ih_      = data.ih_ ;
    sig_     = data.sig_ ; 
    dsig_    = data.dsig_ ; 
    q2_      = data.q2_ ; 
    fit_     = data.fit_ ;
    c2_      = data.c2_ ; 
    c2weight_ = data.c2weight_ ; 
    sigUsed_ = data.sigUsed_ ; 
    expInfo_ = data.expInfo_ ;
    dset_    = data.dset_ ; 
    point_   = data.point_ ;
  }
  return *this;
}

string FitData::header(){
  ostringstream os;
  
  os << setw(35) << "NAME"    << flush;
  os << setw(2)  << "t"       << flush;
  os << setw(2)  << "h"       << flush;
  os << setw(10) << "Q^2"     << flush;
  os << setw(12) << "data"    << flush;
  os << setw(12) << "error"   << flush;
  os << setw(12) << "fit"     << flush;
  os << setw(12) << "chi^2"   << flush;
  os << setw(12) << "weight"  << flush;

  return os.str();
}

void FitData::dataSet( Exp::DataSet* dset ){
  if( dset ) {
    dset_     = dset;
    c2weight_ = ( dset_->expdata() ? dset_->expdata()->lambda() : 1.0 );
    expInfo_  = dset->info();
  }
}


namespace Fit {
  ostream& operator<<( ostream& os, const FitData& data ){
    os << setw(35) << data.expInfo_ << flush;
    os << setw(2)  << data.it_      << flush;
    os << setw(2)  << data.ih_      << flush;
    os << setw(10) << data.q2_      << flush;
    os << setw(12) << data.sig_     << flush;
    os << setw(12) << data.dsig_    << flush;
    os << setw(12) << data.fit_     << flush;
    os << setw(12) << data.c2_      << flush;
    os << setw(12) << data.c2weight_  << flush;
    return os;
  }
}

