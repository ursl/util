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

// ----------------------------------------------------------------------
vector<double> numpy::linspace(int nbins, double xlo, double xhi) {
  vector<double> result; 
  result.resize(nbins);
  for (unsigned int ibin = 0; ibin < nbins; ++ibin) {
    result[ibin] = xlo + ibin*(xhi-xlo)/nbins;
  }
  return result;
}
