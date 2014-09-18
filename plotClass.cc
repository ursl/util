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
plotClass::plotClass(string dir,  string files, string setup) {

  fDBX = true; 
  fVerbose = true;

  fDirectory = dir; 

  delete gRandom;
  gRandom = (TRandom*) new TRandom3;

  fEpsilon = 0.00001; 
  fLumi = 20.; 

  legg = 0;
  c0 = c1 = c2 = c3 = c4 = c5 =0;
  tl = new TLatex();
  box = new TBox();
  pa = new TArrow();
  pl = new TLine(); 
  legge = 0;

  c0 = (TCanvas*)gROOT->FindObject("c0"); 
  if (!c0) c0 = new TCanvas("c0","--c0--",0,0,656,700);

  fHistFile = 0; // this must be opened in a derived class!
}

// ----------------------------------------------------------------------
plotClass::~plotClass() {
}

// ----------------------------------------------------------------------
// see http://root.cern.ch/phpBB3/viewtopic.php?f=3&t=15054
void plotClass::closeHistFile() {
  fHistFile->cd();
  fHistFile->Write(); 
  fHistFile->Close(); 

}

// ----------------------------------------------------------------------
void plotClass::bookHist(string name) {
  cout << "==> plotClass: bookHist " << name << endl;
}

// ----------------------------------------------------------------------
void plotClass::makeAll(int bitmask) {
  cout << "==> plotClass: makeAll " << bitmask << endl;
}

// ----------------------------------------------------------------------
void plotClass::treeAnalysis() {
  cout << "==> plotClass: treeAnalysis " << endl;
}

// ----------------------------------------------------------------------
void plotClass::normHist(TH1 *h, string ds, int method) {
  double scale(1.); 
  // -- normalize to 1
  if (method == UNITY) {
    scale = (h->Integral() > 0 ? 1./h->Integral() : 1.); 
    setTitles(h, h->GetXaxis()->GetTitle(), "normalized to 1");
  } else if (method == SOMETHING) {
    scale = fNorm * (h->Integral() > 0 ? fNorm/h->Integral() : 1.); 
    setTitles(h, h->GetXaxis()->GetTitle(), "weighted events");
  } else if (method == XSECTION) {
    // -- normalize to xsec*bf
    //    n = xsec * L
    //    "integral" over histogram should be xsec
    scale = (h->Integral() > 0 ? fDS[ds]->fXsec*fDS[ds]->fBf/h->Integral() : 1.); 
    setTitles(h, h->GetXaxis()->GetTitle(), "pb");
  } else if (method == LUMI) {
    // -- normalize to xsec*bf
    //    n = xsec * L
    //    "integral" over histogram should be events expected in fLumi
    scale = (h->Integral() > 0 ? fLumi/fDS[ds]->fLumi : 1.); 
    setTitles(h, h->GetXaxis()->GetTitle(), Form("events in %4.0f/fb", fLumi));
  } else if (method == NONORM) {
    scale = 1.;
  } else {
    scale = 1.;
  }

  cout << "==>plotClass:  normHist scaling by " << scale << ", based on method " << method << endl;

  double c(0.), e(0.); 
  for (int i = 0; i <= h->GetNbinsX(); ++i) {
    c = h->GetBinContent(i); 
    e = h->GetBinError(i); 
    h->SetBinContent(i, c*scale);
    h->SetBinError(i, e*scale);
  }
}


// ----------------------------------------------------------------------
void plotClass::overlayAll() {
}

// ----------------------------------------------------------------------
void plotClass::overlay(TH1* h1, string f1, TH1* h2, string f2, int method, bool log, bool legend) {

  normHist(h1, f1, method); 
  normHist(h2, f2, method); 

  double hmax(1.2*h1->GetMaximum()); 
  if (h2->GetMaximum() > hmax) hmax = 1.2*h2->GetMaximum(); 
  if (log) {
    gPad->SetLogy(1); 
    hmax *= 2.;
    h1->SetMinimum(0.5); 
  } else {
    h1->SetMinimum(0.); 
  }
  h1->SetMaximum(hmax); 

  h1->DrawCopy("hist"); 
  h2->DrawCopy("histsame");
  cout << "==> plotClass: overlay(" << f1 << ", " << h1->GetName() << " integral= " << h1->Integral()
       << ", " << f2 << ", " << h2->GetName() << " integral= " << h2->Integral()
       << ") legend = " << legend << " log: " << log 
       << endl;
  
  if (legend) {
    newLegend(0.40, 0.75, 0.7, 0.85); 
    legg->AddEntry(h1, fDS[f1]->fName.c_str(), "p"); 
    legg->AddEntry(h2, fDS[f2]->fName.c_str(), "l"); 
    legg->Draw();
    if (fDBX) {
      tl->SetNDC(kTRUE);
      tl->SetTextSize(0.02);
      tl->SetTextColor(fDS[f1]->fColor); 
      tl->DrawLatex(0.90, 0.88, Form("%.1e", h1->Integral())); 
      tl->SetTextColor(fDS[f2]->fColor); 
      tl->DrawLatex(0.90, 0.82, Form("%.1e", h2->Integral())); 
    }
  }
}

// ----------------------------------------------------------------------
void plotClass::overlay(string h1name, string f1, string h2name, string f2, int method, bool log, bool legend) {
  TH1D *h1 = fDS[f1]->getHist(Form("%s", h1name.c_str())); 
  TH1D *h2 = fDS[f2]->getHist(Form("%s", h2name.c_str())); 
  overlay(h1, f1, h2, f2, method, log, legend); 
}


// ----------------------------------------------------------------------
void plotClass::loopFunction1() {
}


// ----------------------------------------------------------------------
void plotClass::loopFunction2() {
}

// ----------------------------------------------------------------------
void plotClass::loopOverTree(TTree *t, int ifunc, int nevts, int nstart) {
  int nentries = Int_t(t->GetEntries());
  int nbegin(0), nend(nentries); 
  if (nevts > 0 && nentries > nevts) {
    nentries = nevts;
    nbegin = 0; 
    nend = nevts;
  }
  if (nevts > 0 && nstart > 0) {
    nentries = nstart + nevts;
    nbegin = nstart; 
    if (nstart + nevts < t->GetEntries()) {
      nend = nstart + nevts; 
    } else {
      nend = t->GetEntries();
    }
  }
  
  nentries = nend - nstart; 
  
  int step(1000000); 
  if (nentries < 5000000)  step = 500000; 
  if (nentries < 1000000)  step = 100000; 
  if (nentries < 100000)   step = 10000; 
  if (nentries < 10000)    step = 1000; 
  if (nentries < 1000)     step = 100; 
  if (2 == ifunc)          step = 10000; 
  cout << "==> plotClass::loopOverTree> loop over dataset " << fCds << " in file " 
       << t->GetDirectory()->GetName() 
       << " with " << nentries << " entries"  << " looping from  " << nbegin << " .. " << nend
       << endl;

  // -- setup loopfunction through pointer to member functions
  void (plotClass::*pF)(void);
  if (ifunc == 1) pF = &plotClass::loopFunction1;
  if (ifunc == 2) pF = &plotClass::loopFunction2;

  cout << "pF: " << pF << endl;

  // -- the real loop starts here
  for (int jentry = nbegin; jentry < nend; jentry++) {
    t->GetEntry(jentry);
    if (jentry%step == 0) cout << Form(" .. evt = %d", jentry) << endl;
    (this->*pF)();
  }

}


// ----------------------------------------------------------------------
void plotClass::setupTree(TTree *t) {
}

// ----------------------------------------------------------------------
TTree* plotClass::getTree(string ds, string dir) {
  TTree *t(0);
  if (!dir.compare("")) {
    t = (TTree*)fDS[ds]->fF->Get("events"); 
  } else {
    t = (TTree*)fDS[ds]->fF->Get(Form("%s/events", dir.c_str())); 
  }
  return t; 
}

// ----------------------------------------------------------------------
void plotClass::loadFiles(string afiles) {
  
  string files = fDirectory + "/" + afiles;
  cout << "==> Loading files listed in " << files << endl;

  char buffer[1000];
  ifstream is(files.c_str());
  string sname, sdecay; 
  double mass, lambda; 

  while (is.getline(buffer, 1000, '\n')) {
    if (buffer[0] == '#') {continue;}
    if (buffer[0] == '/') {continue;}
    if (buffer[0] == '\n') {continue;}
    
    string sbuffer = string(buffer); 
    replaceAll(sbuffer, "\t", " ");
    replaceAll(sbuffer, "  ", " ");

    string::size_type m1 = sbuffer.find("xsec="); 
    string stype = sbuffer.substr(5, m1-6); 
    //    splitType(stype, sname, mass, lambda); 
    
    cout << "sname: " << sname << endl;

    string::size_type m2 = sbuffer.find("file="); 
    string sxsec = sbuffer.substr(m1+5, m2-m1-6); 
    string sfile = sbuffer.substr(m2+5); 

    TFile *pF(0); 
    // -- MC
    pF = loadFile(sfile); 
    TTree *t = (TTree*)pF->Get("events"); 
    int nevt = t->GetEntries();
    if (string::npos != sname.find("dy")) {
      dataset *ds = new dataset(); 
      sdecay = "Drell-Yan";
      ds->fColor = kRed; 
      ds->fLcolor = kRed; 
      ds->fFcolor = kRed; 
      ds->fSymbol = 25; 

      ds->fF      = pF; 
      ds->fXsec   = atof(sxsec.c_str());          // [xsec] = pb
      ds->fBf     = 1.;
      ds->fMass   = -1.;
      ds->fLambda = -1.;
      ds->fLumi   = nevt/ds->fXsec/ds->fBf/1000.; // [lumi] = 1/fb
      //      ds->fName   = "MadGraph " + sdecay; 
      ds->fName   = sdecay; 
      ds->fFillStyle = 3365; 
      ds->fSize = 1; 
      ds->fWidth = 2; 
      fDS.insert(make_pair(sname, ds)); 
      cout << "  inserted into fDS" << endl;
    }


    if (string::npos != sname.find("lq")) {
      dataset *ds = new dataset(); 
      sdecay = "LQ";
      if (string::npos != sname.find("pair")) sdecay = "LQ #bar{LQ}";
      sdecay = Form("%s (%.0fGeV, #Lambda=%2.1f)", sdecay.c_str(), mass, lambda);
      ds->fColor = kBlue; 
      ds->fLcolor = kBlue; 
      ds->fFcolor = kBlue; 
      ds->fSymbol = 24; 

      ds->fF      = pF; 
      ds->fXsec   = atof(sxsec.c_str());          // [xsec] = pb
      ds->fBf     = 1.;
      ds->fMass   = mass;
      ds->fLambda = lambda;
      ds->fLumi   = nevt/ds->fXsec/ds->fBf/1000.; // [lumi] = 1/fb
      ds->fName   = sdecay; 
      ds->fFillStyle = 3356; 
      ds->fSize = 1; 
      ds->fWidth = 2; 
      fDS.insert(make_pair(sname, ds)); 
    }



    // mb ub nb pb fb 
    cout << "opened MC file "  << sfile  << " as " << sname << " (" << stype << ") with xsec = " << sxsec
	 << Form(" = %8.5f", fDS[sname]->fXsec)
	 << Form(", equivalent lumi = %5.1f/fb", fDS[sname]->fLumi)
	 << endl;

  }

}


// ----------------------------------------------------------------------
TFile* plotClass::loadFile(string file) {
  TFile *f = TFile::Open(file.c_str());
  return f; 
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
  if (legg) delete legg;
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
    c5 = new TCanvas("c5", "c5", 210,   0, 800, 1000);
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
