#include "mu3ePlotUtils.hh"

#include <vector>
#include <map>
#include <string>
#include <TH1.h>
#include <TH2.h>
#include <TCanvas.h>
#include <TStyle.h>

using namespace std;

// ---------------------------------------------------------------------
mu3ePlotUtils::mu3ePlotUtils() {
  fLayer1 = {1,2,3,4,5,6,
            33, 34, 35, 36, 37, 38,
            65, 66, 67, 68, 69, 70,
            97, 98, 99, 100, 101, 102,
            129, 130, 131, 132, 133, 134,
            161, 162, 163, 164, 165, 166,
            193, 194, 195, 196, 197, 198,
            225, 226, 227, 228, 229, 230};
  fLayer2 = {1025, 1026, 1027, 1028, 1029, 1030,
            1057, 1058, 1059, 1060, 1061, 1062,
            1089, 1090, 1091, 1092, 1093, 1094,
            1121, 1122, 1123, 1124, 1125, 1126,
            1153, 1154, 1155, 1156, 1157, 1158,
            1185, 1186, 1187, 1188, 1189, 1190,
            1217, 1218, 1219, 1220, 1221, 1222,
            1249, 1250, 1251, 1252, 1253, 1254,
            1281, 1282, 1283, 1284, 1285, 1286,
            1313, 1314, 1315, 1316, 1317, 1318};
  fAllChips = fLayer1;
  fAllChips.insert(fAllChips.end(), fLayer2.begin(), fLayer2.end());
}

// ---------------------------------------------------------------------
mu3ePlotUtils::~mu3ePlotUtils() {
}

// ---------------------------------------------------------------------
void mu3ePlotUtils::setOptionsForHitmaps(bool rebin) {
  fOpt = "colz";
  fSetLogx = false; 
  fSetLogy = false;
  fSetLogz = true;
  fOptStat = 0;
  fSetPadRightMargin = 0.15;
  fSetPadLeftMargin = 0.0;
  fSetCanvasWidth = 3200;
  fSetCanvasHeight = 4000;

  if (rebin) {
    fRebin = true;
    fRebinX = 2;
    fRebinY = 5;
  } else {
    fRebin = false;
    fRebinX = 1;
    fRebinY = 1;
  }
}

// ---------------------------------------------------------------------
void mu3ePlotUtils::setOptionsForTotDistributions(bool rebin) {
  fOpt = "hist";
  fSetLogx = false;
  fSetLogy = true;
  fSetLogz = false;
  fOptStat = 0;
  fSetPadRightMargin = 0.0;
  fSetPadLeftMargin = 0.15;
  fSetCanvasWidth = 800;
  fSetCanvasHeight = 1000;

  if (rebin) {
    fRebin = true;
    fRebinX = 2;
    fRebinY = 5;
  } else {
    fRebin = false;
    fRebinX = 1;
    fRebinY = 1;
  }
}

// ---------------------------------------------------------------------
void mu3ePlotUtils::plotVertexL1L2(map<string, TH1*> &hists, string pattern) {
  TCanvas *c = new TCanvas(Form("cmpu%d", fCanvasCounter), Form("cmpu%d", fCanvasCounter), fSetCanvasWidth, fSetCanvasHeight);
  fCanvasCounter++; 
  cout << "plotVertexL1L2() canvas created with name " << c->GetName() << endl;
  gStyle->SetOptStat(fOptStat);
  gStyle->SetPadBorderMode(fSetPadBorderMode);
  gStyle->SetPadBorderSize(fSetPadBorderSize);
  gStyle->SetPadTopMargin(fSetPadTopMargin);
  gStyle->SetPadBottomMargin(fSetPadBottomMargin);
  gStyle->SetPadLeftMargin(fSetPadLeftMargin);
  gStyle->SetTitleSize(fSetTitleSize);

  c->Divide(2, 1);
  c->cd(1);
  gPad->SetBottomMargin(0.0);
  gPad->SetLeftMargin(0.0);
  gPad->SetRightMargin(0.0);
  gPad->SetTopMargin(0.0);
  TPad *p1 = (TPad*)c->GetPad(1);
  p1->Divide(6,8);

  // -- Layer 1
  for (int i = 0; i < fLayer1.size(); i++) {
    p1->cd(i+1);
    gPad->SetLogx(fSetLogx);
    gPad->SetLogy(fSetLogy);
    gPad->SetLogz(fSetLogz);
    gPad->SetRightMargin(fSetPadRightMargin);
    gPad->SetTopMargin(fSetPadTopMargin);
    gPad->SetBottomMargin(fSetPadBottomMargin);
    gPad->SetLeftMargin(fSetPadLeftMargin);
    string hname = Form(pattern.c_str(), fLayer1[i]);
    TH2 *h = (TH2D*)hists[hname];
    if (fRebin) h->RebinX(fRebinX);
    if (fRebin) h->RebinY(fRebinY);
    if (h == nullptr) {
      cout << "  hname " << hname << " not found" << endl;
      continue;
    } else {
      //cout << "  hname " << hname << " added to canvas" << endl;
    }
    h->Draw(fOpt.c_str());
  }

  // -- Layer 2
  c->cd(2);
  gPad->SetBottomMargin(0.0);
  gPad->SetLeftMargin(0.0);
  gPad->SetRightMargin(0.0);
  gPad->SetTopMargin(0.0);
  TPad *p2 = (TPad*)c->GetPad(2);
  p2->Divide(6,10);
  for (int i = 0; i < fLayer2.size(); i++) {
    p2->cd(i+1);
    gPad->SetLogx(fSetLogx);
    gPad->SetLogy(fSetLogy);
    gPad->SetLogz(fSetLogz);
    gPad->SetRightMargin(fSetPadRightMargin);
    gPad->SetTopMargin(fSetPadTopMargin);
    gPad->SetBottomMargin(fSetPadBottomMargin);
    gPad->SetLeftMargin(fSetPadLeftMargin);
    string hname = Form(pattern.c_str(), fLayer2[i]);
    TH2 *h = (TH2D*)hists[hname];
    if (fRebin) h->RebinX(fRebinX);
    if (fRebin) h->RebinY(fRebinY);
    if (h == nullptr) {
      cout << "  hname " << hname << " not found" << endl;
      continue;
    } else {
      //cout << "  hname " << hname << " added to canvas" << endl;
    }
    h->Draw(fOpt.c_str());
  }
  
  c->SaveAs(fPDFName.c_str());
  delete p1;
  delete p2;
  delete c;
}