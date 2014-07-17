#ifndef DATASET_H
#define DATASET_H

#include <TFile.h>
#include <TH1.h>

#include "util.hh"

class dataset {
public:
  dataset();
  void   cd(std::string dir) {fF->cd(dir.c_str()); }
  TFile* getFile() {return fF;}
  TH1D*  getHist(std::string name);

  TFile *fF; 
  double fXsec, fFilterEff, fBf, fLumi;
  std::string fName;
  int fColor, fLcolor, fFcolor, fSymbol, fFillstyle, fSize, fWidth; 
};

#endif
