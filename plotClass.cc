#include "plotClass.hh"

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

ClassImp(plotClass)

using namespace std;


// ----------------------------------------------------------------------
plotClass::plotClass(string dir, string files, string cuts, string setup) {

  gStyle->SetHatchesSpacing(2);

  fDirectory = dir;
  fSetup = setup;
  fMode = UNSET;
  fSuffix = "";

  delete gRandom;
  gRandom = (TRandom*) new TRandom3;

  legg = 0;
  c0 = c1 = c2 = c3 = c4 = c5 =0;
  tl = new TLatex();
  box = new TBox();
  pa = new TArrow();
  pl = new TLine();
  legge = 0;

  c0 = (TCanvas*)gROOT->FindObject("c0");
  if (!c0) c0 = new TCanvas("c0","--c0--",0,0,1200,700);


  fHistFile = 0; // this must be opened in a derived class!

}


// ----------------------------------------------------------------------
plotClass::~plotClass() {
  cout << "plotClass destructor" << endl;
  for (map<string, dataset*>::iterator imap = fDS.begin(); imap != fDS.end(); ++imap) {
    if (fDS[imap->first]->fF) {
      cout << "    => closing " << imap->first;
      cout << ": " << fDS[imap->first]->fF->GetName() << endl;
      imap->second->fF->Close();
    }
  }
}


// ----------------------------------------------------------------------
// see http://root.cern.ch/phpBB3/viewtopic.php?f=3&t=15054
void plotClass::closeHistFile() {
  fHistFile->Write();
}


// ----------------------------------------------------------------------
void plotClass::makeAll(int bitmask) {
  cout << "wrong class function" << endl;
}


// ----------------------------------------------------------------------
void plotClass::setup(string ds) {
  fSample = ds;
  string dir = "";
  fTreeDir = dir;
}


// ----------------------------------------------------------------------
void plotClass::changeSetup(string dir, string name, string setup) {
  fHistFileName = Form("%s/%s.%s.root", dir.c_str(), name.c_str(), fSetup.c_str());
  fNumbersFileName = fDirectory + Form("/%s.%s.txt", name.c_str(), fSetup.c_str());


  fTexFileName = fNumbersFileName;
  replaceAll(fTexFileName, ".txt", ".tex");

  cout << "plotClass::changeSetup: " << endl
       << "  name             = " << name << endl
       << "  setup            = " << setup << endl
       << "  fSetup           = " << fSetup << endl
       << "  fSuffix          = " << fSuffix << endl
       << "  fHistFileName    = " << fHistFileName << endl
       << "  fNumbersFileName = " << fNumbersFileName << endl
       << "  fTexFileName     = " << fTexFileName << endl
       << endl;
}


// ----------------------------------------------------------------------
void plotClass::init() {
  cout << "***************************************************************" << endl;
  fTEX.close();
  cout << Form("/bin/rm -f %s", fTexFileName.c_str()) << endl;
  system(Form("/bin/rm -f %s", fTexFileName.c_str()));
  cout << Form("open for TeX output: %s", fTexFileName.c_str()) << endl;
  fTEX.open(fTexFileName.c_str(), ios::app);
}


// ----------------------------------------------------------------------
void plotClass::setupTree(TTree *t, string mode) {
  cout << "wrong class function" << endl;

  // t->SetBranchAddress("run",     &fb.run);
  // t->SetBranchAddress("evt",     &fb.evt);

}


// ----------------------------------------------------------------------
void plotClass::loopOverTree(TTree *t, int ifunc, int nevts, int nstart) {
  cout << "wrong class function" << endl;
}


// ----------------------------------------------------------------------
void plotClass::loopFunction1() {
  cout << "wrong class function" << endl;
}


// ----------------------------------------------------------------------
void plotClass::candAnalysis() {
}

// ----------------------------------------------------------------------
TTree* plotClass::getTree(string ds, string dir, string tree) {
  if (!fDS[ds]) {
    cout << "xx> plotClass::getTree: dataset ->" << ds << "<- not found" << endl;
    return 0;
  }
  TTree *t(0);
  if (!dir.compare("")) {
    t = (TTree*)fDS[ds]->fF->Get(tree.c_str());
  } else {
    t = (TTree*)fDS[ds]->fF->Get(Form("%s/%s", dir.c_str(), tree.c_str()));
  }
  cout << "plotClass::getTree(" << ds << ", " << dir << ", " << tree << "): " << t << endl;
  return t;
}

// ----------------------------------------------------------------------
TFile* plotClass::loadFile(string file) {
  TFile *f = TFile::Open(file.c_str());
  return f;
}


// ----------------------------------------------------------------------
void plotClass::loadFiles(string afiles) {

  string files = fDirectory + string("/") + afiles;
  cout << "==> plotClass::loadFiles loading files listed in " << files << endl;

  char buffer[1000];
  ifstream is(files.c_str());
  while (is.getline(buffer, 1000, '\n')) {
    if (buffer[0] == '#') {continue;}
    if (buffer[0] == '/') {continue;}

    string sbuffer = string(buffer);
    replaceAll(sbuffer, " ", "");
    replaceAll(sbuffer, "\t", "");
    if (sbuffer.size() < 1) continue;

    string::size_type m1 = sbuffer.find("lumi=");
    string::size_type m2 = sbuffer.find("eff=");
    string::size_type m3 = sbuffer.find("file=");
    string stype("nada");
    bool useBf(false);
    if (m1 > sbuffer.size()) {
      m1 = sbuffer.find("bf=");
      useBf = true;
    }
    double eff(0.), effE(0.);
    if (m2 > sbuffer.size()) {
      m2 = sbuffer.find("file=");
    } else {
      string seff = sbuffer.substr(m2+4, m3-m2-4);
      float val, err;
      int expo;
      sscanf(seff.c_str(), "(%f,%f)e%d", &val, &err, &expo);
      eff = val*TMath::Power(10., expo);
      effE = err*TMath::Power(10., expo);
    }
    stype = sbuffer.substr(5, m1-5);
    string slumi("nada"), sbf("nada");
    if (useBf) {
      sbf = sbuffer.substr(m1+3, m2-m1-3);
    } else {
      slumi = sbuffer.substr(m1+5, m3-m1-5);
    }
    string sfile = sbuffer.substr(m3+5);

    string sname("nada"), sdecay("nada"), ldecay("");
    double bf(0.), bfE(0.);
    if (useBf) {
      //cout << "sbf = " << sbf  << endl;
      float val, err;
      int expo;
      sscanf(sbf.c_str(), "(%f,%f)e%d", &val, &err, &expo);
      bf = val*TMath::Power(10., expo);
      bfE = err*TMath::Power(10., expo);
    } else {
      bf = 0.;
      bfE = 0.;
    }

    //    if (useBf) cout << " -> BF = " << bf << " +/- " << bfE << endl;

    TFile *pF(0);
    dataset *ds(0);

    if (string::npos != stype.find("data")) {
      cout << "reading sfile ->" << sfile << "<-" << " stype ->" << stype << "<-" << endl;
      // -- DATA
      pF = loadFile(sfile);

      ds = new dataset();
      ds->fSize = 1.2;
      ds->fWidth = 2;
      if (string::npos != stype.find("pixel,")) {
        sname = "pixelData";
        ds->fColor = kBlack;
        ds->fSymbol = 20;
        ds->fF      = pF;
        ds->fBf     = bf;
        ds->fBfE    = bfE;
        ds->fFilterEff = eff;
        ds->fFilterEffE = effE;
        ds->fMass   = 1.;
        ds->fFillStyle = 3365;
        ds->fLumi   = atof(slumi.c_str());
        cout << "adding ds " << sname << endl;
      }
    }
  }
}


// ----------------------------------------------------------------------
void plotClass::replaceAll(string &sInput, const string &oldString, const string &newString) {
  string::size_type foundpos = sInput.find(oldString);
  while (foundpos != string::npos)  {
    sInput.replace(sInput.begin() + foundpos, sInput.begin() + foundpos + oldString.length(), newString);
    foundpos = sInput.find(oldString);
  }
}

// ----------------------------------------------------------------------
void plotClass::newLegend(double x1, double y1, double x2, double y2, string title) {
  //  if (legg) delete legg;
  legg = new TLegend(x1, y1, x2, y2, title.c_str());
  legg->SetFillStyle(0);
  legg->SetBorderSize(0);
  legg->SetTextSize(0.04);
  legg->SetFillColor(0);
  legg->SetTextFont(42);
}

// ----------------------------------------------------------------------
void plotClass::makeCanvas(int i) {
  if (i & 16) {
    c5 = new TCanvas("c5", "c5", 210,   0, 800, 900);
    c5->ToggleEventStatus();
  }
  if (i & 8) {
    c4 = new TCanvas("c4", "c4", 210,   0, 800, 600);
    c4->ToggleEventStatus();
  }
  if (i & 4) {
    c3 = new TCanvas("c3", "c3", 200,  20, 800, 800);
    c3->ToggleEventStatus();
  }
  if (i & 1) {
    //    c1 = new TCanvas("c1", "c1", 20,  60, 1200, 400);
    c1 = new TCanvas("c1", "c1", 20,  60, 1000, 400);
    c1->ToggleEventStatus();
  }
  if (i & 2) {
    c2 = new TCanvas("c2", "c2", 300, 200, 400, 800);
    c2->ToggleEventStatus();
  }
}


// ----------------------------------------------------------------------
void plotClass::readCuts(string filename) {
  cout << "==> plotClass: Reading " << filename << " for cut settings" << endl;
  vector<string> cutLines;
  readFile(filename, cutLines);
  cout << "==> finished reading cut setting  " << endl;
}


// ----------------------------------------------------------------------
void plotClass::readFile(string filename, vector<string> &lines) {
  cout << "    readFile " << filename << endl;
  char  buffer[200];
  ifstream is(filename.c_str());
  if (!is) {
    cout << "file ->" << filename << "<- not found, exit(1)" << endl;
    exit(1);
  }
  char input[1000];
  while (is.getline(buffer, 200, '\n')) {
    if (buffer[0] != '+') {
      lines.push_back(string(buffer));
    } else {
      sscanf(buffer, "+input %s", input);
      readFile(input, lines);
    }
  }
}


// ----------------------------------------------------------------------
void plotClass::printCuts(ostream &OUT) {

  OUT << "----------------------------------------------------------------------" << endl;
  OUT << endl;

  OUT << "metaMin    ";
  OUT << endl;

}

// ----------------------------------------------------------------------
void plotClass::setItalic() {
  tl->SetTextFont(52);
}


// ----------------------------------------------------------------------
void plotClass::setRoman() {
  tl->SetTextFont(42);
}


// ----------------------------------------------------------------------
void plotClass::savePad(string name, TCanvas *c) {
  if (0 == c) {
    gPad->SaveAs(Form("%s/%s", fDirectory.c_str(), name.c_str()));
  } else {
    c->SaveAs(Form("%s/%s", fDirectory.c_str(), name.c_str()));
  }
}

// ----------------------------------------------------------------------
void plotClass::cd(std::string dataset, std::string dir) {
  if (0 == fDS.count(dataset)) {
    cout << "unknown dataset: " << dataset << endl;
  } else {
    fDS[dataset]->cd(dir.c_str());
  }
}
