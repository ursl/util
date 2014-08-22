#include "selpoint.hh"

using namespace std;

// ----------------------------------------------------------------------
selpoint::selpoint() {
  fSmallerThan = vector<pair<double *, double> >();
  fLargerThan = vector<pair<double *, double> >();
  
  fSgCnt = 0; 
  fBgCnt = 0; 

}

// ----------------------------------------------------------------------
selpoint::~selpoint() {
  fSmallerThan.clear();
  fLargerThan.clear();
 
}

// ----------------------------------------------------------------------
void selpoint::eval(bool sg) {
  
  bool OK(true);
  for (unsigned int i = 0; i < fSmallerThan.size(); ++i) {
    if ((*(fSmallerThan[i].first)) > fSmallerThan[i].second) {
      OK = false;
      return;
    }
  }

  for (unsigned int i = 0; i < fLargerThan.size(); ++i) {
    if ((*(fLargerThan[i].first)) < fLargerThan[i].second) {
      OK = false;
      return;
    }
  }
  
  if (sg) {
    fSgCnt += 1; 
  } else {
    fBgCnt += 1; 
  }
}
