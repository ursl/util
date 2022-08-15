#include "numpy.hh"

#include <fstream>
#include <iostream>
#include <sstream>

#include "TROOT.h"
#include "TStyle.h"
#include "TMath.h"
#include "TRandom3.h"
#include "TString.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TF1.h"

#include "util.hh"

ClassImp(numpy)

using namespace std;

// ----------------------------------------------------------------------
numpy::numpy(int verbose): fVerbose(verbose) {

}

// ----------------------------------------------------------------------
numpy::~numpy() {
}
