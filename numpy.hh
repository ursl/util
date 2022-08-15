#ifndef NUMPY_HH
#define NUMPY_HH

#include <utility>
#include <vector>
#include <string>
#include <map>

#include "TString.h"
#include "TObject.h"


// ----------------------------------------------------------------------
// -- numpy
// --------
//
// My version of numpy
//
// Usage:

//
// ----------------------------------------------------------------------

class numpy {
public:
  numpy(int verbose = 1);
  ~numpy();


private:

  int fVerbose; 
  // ----------------------------------------------------------------------
  ClassDef(numpy, 1)

};


#endif
