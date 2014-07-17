#include <iostream>
#include <fstream>
#include <iomanip>

#include "dataset.hh"

using namespace std;

// ----------------------------------------------------------------------
dataset::dataset() {
  fF = 0; 
  fXsec = fFilterEff = fBf = fLumi = 0.; 
  fName = "";
  fColor = fLcolor = fFcolor = fSymbol = fFillstyle = fSize = fWidth = -1; 
}


// ----------------------------------------------------------------------
TH1D*  dataset::getHist(string name) {
  TH1D *h = (TH1D*)(fF->Get(Form("%s", name.c_str()))); 
  setHist(h); 
  if (fColor > -1) setHist(h, fColor, fSymbol, fSize, fWidth); 
  if (fFillstyle > -1) setFilledHist(h, fColor, fFcolor, fFillstyle, fWidth); 
  return h; 
}

