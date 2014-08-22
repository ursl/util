#ifndef SELPOINT_H
#define SELPOINT_H

#include <vector>

class selpoint {
public:
  selpoint();
  ~selpoint();

  void eval(bool sg = true); 

  int fSgCnt, fBgCnt; 

  std::vector<std::pair<double *, double> > fSmallerThan;  // <variable pointer, cut value>
  std::vector<std::pair<double *, double> > fLargerThan;  // <variable pointer, cut value>


};

#endif
