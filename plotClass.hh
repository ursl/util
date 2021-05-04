#ifndef PLOTCLASS_h
#define PLOTCLASS_h


#include "TLatex.h"
#include "TLegend.h"
#include "TLegendEntry.h"
#include "TLine.h"
#include "TArrow.h"
#include "TBox.h"

#include "TString.h"
#include "TObject.h"
#include "TFile.h"
#include "TDirectory.h"
#include "TProfile.h"
#include "TH1.h"
#include "TH2.h"
#include "TF1.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TROOT.h"

#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <map>

#include "dataset.hh"

struct redTreeData {
  int run, evt;
};


// ----------------------------------------------------------------------
class plotClass: public TObject {

public :
  plotClass(std::string dir = "results",
            std::string files = "plotClass.files",
            std::string cuts = "nada",
            std::string setup = "");
  virtual        ~plotClass();

  enum MODE {UNSET};

  // -- Stuff to run over the tree from any derived class
  virtual void   setup(std::string ds);
  virtual void   changeSetup(std::string dir, std::string name, std::string setup);

  void           init();
  TTree*         getTree(std::string ds, std::string dir = "", std::string tree = "events");
  void           candAnalysis();
  void           setupTree(TTree *t, std::string mode = "");
  virtual void   makeAll(int bitmask = 0);
  virtual void   loopOverTree(TTree *t, int ifunc, int nevts = -1, int nstart = 0);
  virtual void   loopFunction1();


  // -- Utilities
  virtual void   readCuts(std::string filename);
  void           readFile(std::string filename, std::vector<std::string> &lines);
  virtual void   printCuts(std::ostream &OUT);

  virtual void   loadFiles(std::string afiles);
  TFile*         loadFile(std::string afiles);

  void           closeHistFile();

  // -- display utilities
  void           cd(std::string dataset, std::string dir = "");
  void           replaceAll(std::string &sInput, const std::string &oldString, const std::string &newString);
  void           newLegend(double x1, double y1, double x2, double y2, std::string title = "");
  void           makeCanvas(int i = 3);
  void           setItalic();
  void           setRoman();
  void           savePad(std::string name, TCanvas *c = 0);


  // -- histograms
  std::map<std::string, TH1*> fHists;
  std::string fHistFileName;
  TFile      *fHistFile;

  struct redTreeData fb;
  dataset* fCds;

  // -- datasets (files and associated information)
  std::string fDirectory, fSetup, fSuffix, fSuffixSel, fSample, fNumbersFileName, fTreeDir, fCutFile, fFilesFile, fTexFileName;
  int   fSetupInt;
  std::map<std::string, dataset*> fDS;
  std::ofstream fTEX;

  // -- Display utilities
  std::map<std::string, std::string> fVarToTex, fVarToTexSymbol;
  std::string fStampString, fStampCms, fStampLumi;
  int fFont;
  double fSize;
  TCanvas *c0, *c1, *c2, *c3, *c4, *c5;
  TLatex *tl;
  TBox *box;
  TArrow *pa;
  TLine *pl;
  TLegend *legg;
  TLegendEntry *legge;

  enum MODE fMode;


  static const int MAXPS = 20;

  // ----------------------------------------------------------------------
  ClassDef(plotClass,1)

};

#endif
