#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <unistd.h>

#include "util.hh"
#include "TMath.h"
#include "TColor.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TLatex.h"
#include "TString.h"
#include "TH1.h"
#include "TH2.h"
#include <TRegexp.h>
#include <TList.h>
#include <TROOT.h>
#include "TKey.h"
#include "TSystem.h"

#include "dataset.hh"

using namespace std;

// ----------------------------------------------------------------------
void setMaximum(double scale, TH1 *h1, TH1 *h2) {
  double m(-99.), m1(-99.), m2(-99.);
  if (0 != h1) m1 = h1->GetMaximum();
  if (0 != h2) m2 = h2->GetMaximum();

  m = (m1 > m2? m1: m2);
  if (0 != h1) h1->SetMaximum(scale*m);
  if (0 != h2) h2->SetMaximum(scale*m);
}

// ----------------------------------------------------------------------
void setTitles(TH1 *h, const char *sx, const char *sy, float size,
	       float xoff, float yoff, float lsize, int font) {
  if (h == 0) {
    cout << " Histogram not defined" << endl;
  } else {
    h->SetXTitle(sx);                  h->SetYTitle(sy);
    h->SetTitleOffset(xoff, "x");      h->SetTitleOffset(yoff, "y");
    h->SetTitleSize(size, "x");        h->SetTitleSize(size, "y");
    h->SetLabelSize(lsize, "x");       h->SetLabelSize(lsize, "y");
    h->SetLabelFont(font, "x");        h->SetLabelFont(font, "y");
    h->GetXaxis()->SetTitleFont(font); h->GetYaxis()->SetTitleFont(font);
    h->SetNdivisions(508, "X");
  }
}

// ----------------------------------------------------------------------
void setHist(TH1 *h, Int_t color, Int_t symbol, Double_t size, Double_t width) {
  h->SetLineColor(color);   h->SetLineWidth(width);
  h->SetMarkerColor(color); h->SetMarkerStyle(symbol);  h->SetMarkerSize(size);
  h->SetStats(kFALSE);
  h->SetFillStyle(0); h->SetFillColor(color);
}

// ----------------------------------------------------------------------
void setHist(TH1 *h, dataset *ds) {
  if (ds->fColor > -1) setHist(h, ds->fColor, ds->fSymbol, ds->fSize, ds->fWidth);
  if (ds->fFillStyle > -1) setFilledHist(h, ds->fColor, ds->fFcolor, ds->fFillStyle, ds->fWidth);
}


// ----------------------------------------------------------------------
void setHistTitles(TH1 *h, dataset *ds, const char *sx, const char *sy,
		   float size, float xoff, float yoff, float lsize, int font) {
  setHist(h, ds);
  setTitles(h, sx, sy, size, xoff, yoff, lsize, font);

}

// ----------------------------------------------------------------------
void setGraph(TGraph *h, Int_t color, Int_t symbol, Double_t size, Double_t width, Int_t lstyle) {
  h->SetLineColor(color);   h->SetLineWidth(width);  h->SetLineStyle(lstyle);
  h->SetMarkerColor(color); h->SetMarkerStyle(symbol);  h->SetMarkerSize(size);
}


// ----------------------------------------------------------------------
void colors(int i) {
  if (i == 0) {
    new TColor(600, 0.66, 0.66, 0.66, "gray");
    new TColor(601, 0.90, 0.90, 0.90, "gray");
    new TColor(602, 0.26, 0.26, 0.26, "gray");
    new TColor(610, 0.00, 0.66, 0.00, "green");
    new TColor(612, 0.00, 0.33, 0.00, "darkgreen");
    new TColor(613, 0.00, 0.90, 0.00, "lightgreen");
    new TColor(624, 0.00, 0.00, 0.60, "blue");
    new TColor(625, 0.00, 0.00, 0.85, "lightblue");
    new TColor(626, 0.00, 0.00, 0.35, "darkblue");
    new TColor(637, 0.50, 0.00, 0.00, "red");
    new TColor(638, 0.99, 0.00, 0.00, "lightred");
    new TColor(639, 0.99, 0.00, 0.00, "darkred");
  }
}


// ----------------------------------------------------------------------
void setFilledHist(TH1 *h, Int_t color, Int_t fillcolor, Int_t fillstyle, Int_t width) {
  // Note: 3004, 3005 are crosshatches
  // ----- 1000       is solid
  //       kYellow    comes out gray on bw printers
  h->SetLineColor(color);     h->SetLineWidth(width);
  h->SetFillStyle(fillstyle); h->SetFillColor(fillcolor);
}


// ----------------------------------------------------------------------
void printNonZero(TH1 *h) {
  double con(0.), min(0.), max(0.);
  for (Int_t i = 0; i <= h->GetNbinsX()+1; ++i) {
    con = h->GetBinContent(i);
    if (con > 0.) {
      min = h->GetBinLowEdge(i);
      max = min + h->GetBinWidth(i);
      cout << Form("%3d ", i) << Form(" %7.3f ", min) << " .. " << Form(" %7.3f ", max) << ":"
	   << Form(" %12.3f", con) << " +/- " << Form("%12.3f", h->GetBinError(i))
           << endl;
    }
  }
}


// ----------------------------------------------------------------------
void printAxesLabels(TH1 *h) {
  if (0 == h) {
    cout << "histogram does not exist" << endl;
    return;
  }
  double min(0.), max(0.);
  TString la;
  for (Int_t i = 0; i <= h->GetNbinsX()+1; ++i) {
    la = h->GetXaxis()->GetBinLabel(i);
    min = h->GetBinLowEdge(i);
    max = min + h->GetBinWidth(i);
    if (la != "") {
      cout << Form("%3d ", i) << Form(" %7.3f ", min) << " .. " << Form(" %7.3f ", max) << ":"
           << h->GetXaxis()->GetBinLabel(i)
           << ", bin content: " << h->GetBinContent(i)
           << endl;
    }
  }
}


// ----------------------------------------------------------------------
double getValueByLabel(TH1 *h, string label) {
  string axislabel;
  if (h) {
    for (int i = 1; i <= h->GetNbinsX(); ++i) {
      axislabel = h->GetXaxis()->GetBinLabel(i);
      if (string::npos != axislabel.find(label)) return h->GetBinContent(i);
    }
  }
  return -999.;
}


// ----------------------------------------------------------------------
void addOverflow(TH1D *h) {
  int nbins = h->GetNbinsX();
  double err0 = h->GetBinError(nbins);
  double err1 = h->GetBinError(nbins+1);
  double errTot = TMath::Sqrt(err0*err0 + err1*err1);
  h->SetBinContent(nbins, h->GetBinContent(nbins) + h->GetBinContent(nbins+1));
  h->SetBinError(nbins, errTot);
  h->SetBinContent(nbins+1, 0.);
  h->SetBinError(nbins+1, 0.);
}


// ----------------------------------------------------------------------
void addUnderflow(TH1D *h) {
  double err0 = h->GetBinError(0);
  double err1 = h->GetBinError(1);
  double errTot = TMath::Sqrt(err0*err0 + err1*err1);
  h->SetBinContent(1, h->GetBinContent(1) + h->GetBinContent(0));
  h->SetBinError(1, errTot);
  h->SetBinContent(0, 0.);
  h->SetBinError(0, 0.);
}

// ----------------------------------------------------------------------
void zeroNegativeEntries(TH1D *h) {
  for (int i = 0; i <= h->GetNbinsX()+1; ++i) {
    if ((h->GetBinContent(i) < 0.) || (TMath::IsNaN(h->GetBinContent(i)))) {
      h->SetBinContent(i, 0.);
      h->SetBinError(i, 0.);
    }
  }
}

// ----------------------------------------------------------------------
void showOverflow(TH1 *h) {
  int nbins = h->GetNbinsX();
  double err0 = h->GetBinError(nbins);
  double err1 = h->GetBinError(nbins+1);
  double errTot = TMath::Sqrt(err0*err0 + err1*err1);
  h->SetBinContent(nbins, h->GetBinContent(nbins) + h->GetBinContent(nbins+1));
  h->SetBinError(nbins, errTot);
  h->SetBinContent(nbins+1, 0.);
  h->SetBinError(nbins+1, 0.);


  err0 = h->GetBinError(0);
  err1 = h->GetBinError(1);
  errTot = TMath::Sqrt(err0*err0 + err1*err1);
  h->SetBinContent(1, h->GetBinContent(1) + h->GetBinContent(0));
  h->SetBinError(1, errTot);
  h->SetBinContent(0, 0.);
  h->SetBinError(0, 0.);
}


// ----------------------------------------------------------------------
TLegend* newLegend(string title, double x1, double y1, double x2, double y2,
                   vector<TH1*> hists, vector<string> names, vector<string> options) {
  if (hists.size() != names.size()) {
    cout << "hists and names vectors do not match size" << endl;
    return 0;
  }
  if (hists.size() != options.size()) {
    cout << "hists and options vectors do not match size" << endl;
    return 0;
  }
  TLegend *legg = new TLegend(x1, y1, x2, y2, title.c_str());
  legg->SetFillStyle(0);
  legg->SetBorderSize(0);
  legg->SetTextSize(0.04);
  legg->SetFillColor(0);
  legg->SetTextFont(52);
  for (unsigned int i = 0; i < hists.size(); ++i) {
    legg->AddEntry(hists[i], names[i].c_str(), options[i].c_str());
  }
  return legg;
}

// ----------------------------------------------------------------------
TLegend* newLegend(double x1, double y1, double x2, double y2) {
  TLegend *legg = new TLegend(x1, y1, x2, y2);
  legg->SetFillStyle(0);
  legg->SetBorderSize(0);
  legg->SetTextSize(0.04);
  legg->SetFillColor(0);
  legg->SetTextFont(52);
  return legg;
}


// ----------------------------------------------------------------------
void stampAndSave(TCanvas *fC, const char *s) {
  fC->cd();
  TLatex tl;
  TString filename(gFile->GetName()); Int_t index = filename.Index(".root"); filename.Remove(index);
  TString psname = filename + TString(s);
  double oldA = tl.GetTextAngle();
  double oldS= tl.GetTextSize();
  tl.SetTextAngle(90); tl.SetTextSize(0.02);
  tl.DrawTextNDC(0.001, 0.05, psname.Data());
  fC->SaveAs(psname.Data());
  tl.SetTextAngle(oldA);
  tl.SetTextSize(oldS);
}

// ----------------------------------------------------------------------
void shrinkPad(double b, double l, double r, double t) {
  gPad->SetBottomMargin(b);
  gPad->SetLeftMargin(l);
  gPad->SetRightMargin(r);
  gPad->SetTopMargin(t);
}

// ----------------------------------------------------------------------
void zone(int x, int y, TCanvas *c) {
  if (c == 0) {
    c = (TCanvas*)gROOT->FindObject("c0");
    if (c == 0) {
      cout << "TCanvas c0 not found. Creating my own version." << endl;
      c = new TCanvas("c0","--c0--",356,0,656,700);
    }
  }
  c->Clear();
  c->Divide(x, y);
  c->cd(1);
}


// ----------------------------------------------------------------------
int wait() {
  cout << " Continue [<RET>|q]?  ";
  char x;
  x = getchar();
  if ((x == 'q') || (x == 'Q')) return 1;
  return 0;
}


// ----------------------------------------------------------------------
vector<int> defVector(int n, ...) {
  va_list vl;
  va_start(vl, n);
  vector<int> vect;
  int a(0);
  for (int i = 0; i < n; ++i) {
    a = va_arg(vl, int);
    vect.push_back(a);
  }
  va_end(vl);
  return vect;
}



// ----------------------------------------------------------------------
double dEff(int in, int iN) {
  double n = (double)in;
  double N = (double)iN;
  return sqrt(((n+1)*(N-n+1))/((N+3)*(N+2)*(N+2)));
}

// ----------------------------------------------------------------------
double dEff(int iN, double eff) {
  double N = (double)iN;
  double n = N*eff;
  return sqrt(((n+1)*(N-n+1))/((N+3)*(N+2)*(N+2)));
}

// ----------------------------------------------------------------------
double dEff(double eff, int in) {
  double n = (double)in;
  double N = in/eff;
  return sqrt(((n+1)*(N-n+1))/((N+3)*(N+2)*(N+2)));
}

// ----------------------------------------------------------------------
double dEff(double n, double nE, double N, double NE) {

  if (n < N) {
    return sqrt((N*N - 2*n*N)*nE*nE + n*n*NE*NE)/(N*N);
  } else {
    return 0.;
  }
}

// ----------------------------------------------------------------------
double dBinomial(double n, double N) {
  double w = n/N;
  return TMath::Sqrt(TMath::Abs(w*(1-w)/N));
}

// ----------------------------------------------------------------------
double dRatio(double a, double b) {
  return TMath::Sqrt((a/(b*b)) + ((a*a)/(b*b*b)));
}

// ----------------------------------------------------------------------
double dRatio(double a, double ae, double b, double be) {
  return TMath::Sqrt(((ae*ae)/(b*b)) + ((a*a*be*be)/(b*b*b*b)));
}


// ----------------------------------------------------------------------
double dBF(double n, double nE, double N, double NE, double e, double eE) {
  double n2 = n*n;
  double e2 = e*e;
  double N2 = N*N;

  double dbdn2 = 1./(e2*N2);
  double dbde2 = (n2)/(e2*e2*N2);
  double dbdN2 = (n2)/(e2*N2*N2);

  return TMath::Sqrt(nE*nE*dbdn2 + NE*NE*dbdN2 + eE*eE*dbde2);
}

// ----------------------------------------------------------------------
double  getError(TH1* h) {
  double e(0.), t(0.);
  if (h->InheritsFrom(TH2::Class())) {
    for (int ix = 1; ix <= h->GetNbinsX(); ix++) {
      for (int iy = 1; iy <= h->GetNbinsY(); iy++) {
	t = h->GetCellError(ix, iy);
	e += t*t;
      }
    }
  } else {
    for (int i = 1; i <= h->GetNbinsX(); ++i) {
      t = h->GetBinError(i);
      e += t*t;
    }
  }
  return TMath::Sqrt(e);
}

// ----------------------------------------------------------------------
double  getErrorRange(TH1* h, int min, int max) {
  double e(0.), t(0.);
  if (min < 0) min = 1;
  if (max < 0) max = h->GetNbinsX();
  for (int i = min; i <= max; ++i) {
    t = h->GetBinError(i);
    e += t*t;
  }
  return TMath::Sqrt(e);
}

// -------------------------------------------------------------------------------
void babar(double xpos, double ypos, double scale, int prel) {
  TText *Babar = new TText();
  Babar->SetNDC(kTRUE);
  Babar->SetTextFont(32);
  Babar->SetTextSize(0.10*scale);
  Babar->DrawText(xpos,ypos,"B");
  Babar->SetTextSize(0.075*scale);
  Babar->DrawText(xpos+0.042*scale,ypos,"A");
  Babar->SetTextSize(0.10*scale);
  Babar->DrawText(xpos+0.078*scale,ypos,"B");
  Babar->SetTextSize(0.075*scale);
  Babar->DrawText(xpos+0.120*scale,ypos,"AR");

  if (prel) {
    Babar->SetTextSize(0.10*0.8*scale);
    Babar->DrawText(xpos, ypos - 0.1*0.8*scale, "Preliminary");
  }
  delete Babar;
}


// ----------------------------------------------------------------------
double chi2Prob(double chi2, double ndof) {
  return (1. - TMath::Gamma(0.5*ndof, 0.5*chi2));
}

// ----------------------------------------------------------------------
double chi2Test(TH1 *h1, TH1 *h2, double& chi2, double& ndof, int constrain) {
  int nbins = h1->GetNbinsX();
  if (nbins != h2->GetNbinsX()) {
    cout << "chi2Test: Number of bins not the same" << endl;
    return -99.;
  }
  double df = nbins - 1 - constrain;
  double chsq(0.), a1(0.), a2(0.);
  for (int i = 1; i <= nbins; ++i) {
    a1 = h1->GetBinContent(i);
    a2 = h2->GetBinContent(i);
    if ((TMath::Abs(a1) < 1.e-8) && (TMath::Abs(a2) < 1.e-8)) {
      df -= 1.;
    } else if ((a1 < 0.) || (a2 < 0.)) {
      df -= 1.;
    } else {
      cout << "Adding " << ((a1-a2)*(a1-a2))/(a1+a2)  << " from " << a1 << "  " << a2 << " for xmin =  " << h1->GetBinLowEdge(i) << endl;
      chsq += ((a1-a2)*(a1-a2))/(a1+a2);
    }
  }
  double gamma = 1. - TMath::Gamma(0.5*df, 0.5*chsq);
  chi2 = chsq;
  ndof = df;
  return gamma;
}

// ----------------------------------------------------------------------
double chi2TestErr(TH1 *h1, TH1 *h2, double& chi2, double& ndof, int constrain) {
  int nbins = h1->GetNbinsX();
  if (nbins != h2->GetNbinsX()) {
    cout << "chi2Test: Number of bins not the same" << endl;
    return -99.;
  }
  double df = nbins - 1 - constrain;
  double chsq(0.), a1(0.), a2(0.), e1(0.), e2(0.);
  for (int i = 1; i <= nbins; ++i) {
    a1 = h1->GetBinContent(i);
    e1 = h1->GetBinError(i) * h1->GetBinError(i);
    a2 = h2->GetBinContent(i);
    e2 = h2->GetBinError(i) *  h2->GetBinError(i);
    if ((TMath::Abs(a1) < 1.e-8) && (TMath::Abs(a2) < 1.e-8)) {
      df -= 1.;
    } else if ((a1 < 0.) || (a2 < 0.)) {
      df -= 1.;
    } else {
      chsq += ((a1 - a2) * (a1 - a2)) / (e1 + e2);
    }
  }
  double gamma = 1. - TMath::Gamma(0.5*df, 0.5*chsq);
  chi2 = chsq;
  ndof = df;
  return gamma;
}

// ----------------------------------------------------------------------
void mk4Vector(TLorentzVector &p4, const Double_t p, const Double_t t, const Double_t f, const Double_t m) {
  p4.SetXYZM(p*TMath::Sin(t)*TMath::Cos(f), p*TMath::Sin(t)*TMath::Sin(f), p*TMath::Cos(t), m);
}

// ----------------------------------------------------------------------
TLorentzVector mk4Vector(const Double_t p, const Double_t t, const Double_t f, const Double_t m) {
  TLorentzVector p4;
  p4.SetXYZM(p*TMath::Sin(t)*TMath::Cos(f), p*TMath::Sin(t)*TMath::Sin(f), p*TMath::Cos(t), m);
  return p4;
}

// ----------------------------------------------------------------------
void average(double &av, double &error, int n, double *val, double *verr) {
  double e(0.), w8(0.), sumW8(0.), sumAve(0.);
  for (int i = 0; i < n; ++i) {
    // -- calculate mean and error
    e = verr[i];
    if (e > 0.) {
      w8 = 1./(e*e);
      sumW8  += w8;
      sumAve += w8*val[i];
    } else {
      cout << "average: Error = 0 for " << val[i] << endl;
      continue;
    }
  }
  if (sumW8 > 0.) {
    av = sumAve/sumW8;
    error = 1./TMath::Sqrt(sumW8);
  } else {
    av = -99.;
    error = -99.;
  }

}


// ----------------------------------------------------------------------
void average(double &av, double &error, vector<double> &val, vector<double> &verr, double &chi2) {

  double e(0.), w8(0.), sumW8(0.), sumAve(0.);
  for (unsigned int i = 0; i < val.size(); ++i) {
    cout << i << " " << val[i] << " +/- " << verr[i];

    // -- calculate mean and error
    e = verr[i];
    if (e > 0.) {
      w8 = 1./(e*e);
      sumW8  += w8;
      sumAve += w8*val[i];
      cout << " w8 = " << w8 << " sumW8 = " << sumW8 << " sumAve = " << sumAve << endl;
    } else {
      cout << "average: Error = 0 for " << val[i] << endl;
      continue;
    }
  }
  if (sumW8 > 0.) {
    av = sumAve/sumW8;
    sumW8 = TMath::Sqrt(sumW8);
    error = 1./sumW8;
  } else {
    av = -99.;
    error = -99.;
  }
  cout << "sqrt(sumW8) = " << sumW8 << " av = " << av << " +/- " << error << endl;

  chi2 = 0;
  for (unsigned int i = 0; i < val.size(); ++i) {
    e = verr[i];
    if (e > 0.) {
      w8 = 1./(e*e);
    } else {
      w8 = 0.;
    }
    chi2 += w8*(av-val[i])*(av-val[i]);
  }
}


// ----------------------------------------------------------------------
double poissonError(int n, double &up, double &down) {
  if (n > 9) {
    up = down = TMath::Sqrt(n);
    return (0.5*(up+down));
  }

  if (n < 0) {
    up = down = -99.;
    return -99.;
  }
  if (n < 2) {
    down = 0.827;
    up   = 2.299;
    return (0.5*(up+down));
  }
  if (2 == n) {
    down = 1.292;
    up   = 2.637;
    return (0.5*(up+down));
  }
  if (3 == n) {
    down = 1.633;
    up   = 2.918;
    return (0.5*(up+down));
  }
  if (4 == n) {
    down = 1.914;
    up   = 3.162;
    return (0.5*(up+down));
  }
  if (5 == n) {
    down = 2.159;
    up   = 3.382;
    return (0.5*(up+down));
  }
  if (6 == n) {
    down = 2.380;
    up   = 2.581;
    return (0.5*(up+down));
  }
  if (7 == n) {
    down = 2.581;
    up   = 3.770;
    return (0.5*(up+down));
  }
  if (8 == n) {
    down = 2.768;
    up   = 3.944;
    return (0.5*(up+down));
  }
  if (9 == n) {
    down = 2.943;
    up   = 4.110;
    return (0.5*(up+down));
  }

  return -1.;
}


// ----------------------------------------------------------------------
double poissonErrorExact(int n, double &up, double &down) {
  double alpha = 0.158655;
  double vlo = 0.5*TMath::ChisquareQuantile(alpha, 2.*n);
  double vup = 0.5*TMath::ChisquareQuantile(1-alpha, 2.*(n+1));

  up   = vup - n;
  down = n - vlo;

  return (0.5*(up+down));

}


// ----------------------------------------------------------------------
int quarkFlavor(int i) {
  if (isBeautyMeson(i) || isBeautyBaryon(i)) {
    return 5;
  } else if (isCharmMeson(i) || isCharmBaryon(i)) {
    return 4;
  } else {
    return 3;
  }
}


// ----------------------------------------------------------------------
bool isQuark(int id) {
  unsigned int aid = TMath::Abs(id);
  if (1 == aid) return true;
  if (2 == aid) return true;
  if (3 == aid) return true;
  if (4 == aid) return true;
  if (5 == aid) return true;
  if (6 == aid) return true;
  return false;
}

// ----------------------------------------------------------------------
bool isLepton(int id) {
  unsigned int aid = TMath::Abs(id);
  if (13 == aid) return true;
  if (11 == aid) return true;
  if (15 == aid) return true;
  if (12 == aid) return true;
  if (14 == aid) return true;
  if (16 == aid) return true;
  return false;
}

// ----------------------------------------------------------------------
bool isBeautyMesonWeak(int i) {
  if (511 == TMath::Abs(i)) return true;
  if (521 == TMath::Abs(i)) return true;
  if (531 == TMath::Abs(i)) return true;
  return false;
}

// ----------------------------------------------------------------------
bool isBeautyBaryonWeak(int i) {
  if (5122 == TMath::Abs(i)) return true;
  if (5132 == TMath::Abs(i)) return true;
  if (5232 == TMath::Abs(i)) return true;
  if (5332 == TMath::Abs(i)) return true;
  return false;
}


// ----------------------------------------------------------------------
bool isCharmMesonWeak(int i) {
  if (411 == TMath::Abs(i)) return true;
  if (421 == TMath::Abs(i)) return true;
  if (431 == TMath::Abs(i)) return true;
  return false;
}

// ----------------------------------------------------------------------
bool isCharmBaryonWeak(int i) {
  if (4122 == TMath::Abs(i)) return true;
  if (4132 == TMath::Abs(i)) return true;
  if (4232 == TMath::Abs(i)) return true;
  if (4332 == TMath::Abs(i)) return true;
  return false;
}

// ----------------------------------------------------------------------
bool isBeautyMeson(int i) {
  int rest = (TMath::Abs(i)%1000);
  return (rest > 499 && rest < 600);
}

// ----------------------------------------------------------------------
bool isBeautyBaryon(int i) {
  int rest = (TMath::Abs(i)%1000);
  return (rest > 4999 && rest < 6000);
}

// ----------------------------------------------------------------------
bool isCharmMeson(int i) {
  int rest = (TMath::Abs(i)%1000);
  return (rest > 399 && rest < 500);
}

// ----------------------------------------------------------------------
bool isCharmBaryon(int i) {
  int rest = (TMath::Abs(i)%1000);
  return (rest > 3999 && rest < 5000);
}

// ----------------------------------------------------------------------
bool isLightMeson(int i) {
  int rest = (TMath::Abs(i)%1000);
  return (rest > 99 && rest < 400);
}

// ----------------------------------------------------------------------
bool isStableCharged(int i) {
  if (211 == TMath::Abs(i)) return true;
  if (321 == TMath::Abs(i)) return true;
  if (11 == TMath::Abs(i)) return true;
  if (13 == TMath::Abs(i)) return true;
  if (2122 == TMath::Abs(i)) return true;
  return false;
}




// ----------------------------------------------------------------------
void replaceAll(string &str, const string &from, const string &to) {
  if (from.empty()) return;
  size_t start_pos = 0;
  while((start_pos = str.find(from, start_pos)) != string::npos) {
    str.replace(start_pos, from.length(), to);
    start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
  }
}



// ----------------------------------------------------------------------
string formatTex(double n, string name, int digits, int sgn) {
  char line[200];
  if ( TMath::IsNaN(n) ) {
    snprintf(line, 199, "\\vdef{%s}   {\\ensuremath{{\\mathrm{NaN} } } }", name.c_str());
  } else if (0 == digits ) {
    snprintf(line, 199, "\\vdef{%s}   {\\ensuremath{{%i } } }", name.c_str(), static_cast<int>(n));
  } else if (1 == digits ) {
    snprintf(line, 199, "\\vdef{%s}   {\\ensuremath{{%5.1f } } }", name.c_str(), n);
    if (sgn) snprintf(line, 199, "\\vdef{%s}   {\\ensuremath{{%+5.1f } } }", name.c_str(), n);
  } else if (2 == digits ) {
    snprintf(line, 199, "\\vdef{%s}   {\\ensuremath{{%5.2f } } }", name.c_str(), n);
    if (sgn) snprintf(line, 199, "\\vdef{%s}   {\\ensuremath{{%+5.2f } } }", name.c_str(), n);
  } else if (3 == digits ) {
    snprintf(line, 199, "\\vdef{%s}   {\\ensuremath{{%5.3f } } }", name.c_str(), n);
    if (sgn) snprintf(line, 199, "\\vdef{%s}   {\\ensuremath{{%+5.3f } } }", name.c_str(), n);
  } else if (4 == digits ) {
    snprintf(line, 199, "\\vdef{%s}   {\\ensuremath{{%5.4f } } }", name.c_str(), n);
    if (sgn) snprintf(line, 199, "\\vdef{%s}   {\\ensuremath{{%+5.4f } } }", name.c_str(), n);
  } else if (5 == digits ) {
    snprintf(line, 199, "\\vdef{%s}   {\\ensuremath{{%6.5f } } }", name.c_str(), n);
    if (sgn) snprintf(line, 199, "\\vdef{%s}   {\\ensuremath{{%+6.5f } } }", name.c_str(), n);
  } else if (6 == digits ) {
    snprintf(line, 199, "\\vdef{%s}   {\\ensuremath{{%7.6f } } }", name.c_str(), n);
    if (sgn) snprintf(line, 199, "\\vdef{%s}   {\\ensuremath{{%+7.6f } } }", name.c_str(), n);
  } else if (7 == digits ) {
    snprintf(line, 199, "\\vdef{%s}   {\\ensuremath{{%8.7f } } }", name.c_str(), n);
    if (sgn) snprintf(line, 199, "\\vdef{%s}   {\\ensuremath{{%+8.7f } } }", name.c_str(), n);
  } else if (8 == digits ) {
    snprintf(line, 199, "\\vdef{%s}   {\\ensuremath{{%9.8f } } }", name.c_str(), n);
    if (sgn) snprintf(line, 199, "\\vdef{%s}   {\\ensuremath{{%+9.8f } } }", name.c_str(), n);
  } else if (9 == digits ) {
    snprintf(line, 199, "\\vdef{%s}   {\\ensuremath{{%10.9f } } }", name.c_str(), n);
    if (sgn) snprintf(line, 199, "\\vdef{%s}   {\\ensuremath{{%+10.9f } } }", name.c_str(), n);
  } else if (10 == digits ) {
    snprintf(line, 199, "\\vdef{%s}   {\\ensuremath{{%11.10f } } }", name.c_str(), n);
    if (sgn) snprintf(line, 199, "\\vdef{%s}   {\\ensuremath{{%+11.10f } } }", name.c_str(), n);
  } else if (11 == digits ) {
    snprintf(line, 199, "\\vdef{%s}   {\\ensuremath{{%12.11f } } }", name.c_str(), n);
    if (sgn) snprintf(line, 199, "\\vdef{%s}   {\\ensuremath{{%+12.11f } } }", name.c_str(), n);
  } else {
    snprintf(line, 199, "\\vdef{%s}   {\\ensuremath{{%f } } }", name.c_str(), n);
    if (sgn) snprintf(line, 199, "\\vdef{%s}   {\\ensuremath{{%+f } } }", name.c_str(), n);
  }

  return string(line);
}

// ----------------------------------------------------------------------
string formatTexErrSci(double n, double nE, string name, int digits, int sgn) {

  char line[200];

  if (TMath::Abs(n) < 1.e-15) {
    snprintf(line, 200, "\\vdef{%s}   {\\ensuremath{0} }", name.c_str());
    return string(line);
  }

  bool negative(false);
  if (n < 0) {
    n *= -1.;
    negative = true;
  }
  double expoN  = TMath::Log10(n);
  //  cout << "original expoN = " << expoN << endl;
  //  if (negative) expoN -= 1;
  double expoNE = TMath::Log10(nE);
  //  cout << "original expoNE = " << expoNE << endl;
  if (expoNE < 0) expoNE -= 1;

  double ratio = expoNE - expoN;
  double mantN  = n / TMath::Power(10, static_cast<int>(expoN));
  if (negative) mantN *= -1.;
  double mantNE = nE / TMath::Power(10, static_cast<int>(expoNE-ratio));

  // cout << setw(2) << "expoN = " << expoN << " expoNE == " << expoNE
  //      << " mantN = " << mantN << " mantNE = " << mantNE << " ratio = " << ratio
  //      << endl;

  if (TMath::IsNaN(n) || TMath::IsNaN(nE)) {
    snprintf(line, 200, "\\vdef{%s}   {\\ensuremath{{\\mathrm{NaN} } } }",
	    name.c_str());
  } else if (1 == digits ) {
    snprintf(line, 200, "\\vdef{%s}   {\\ensuremath{{(%5.1f \\pm %5.1f)\\times 10^{%d}} } }",
	    name.c_str(), mantN, mantNE, static_cast<int>(expoN));
    if (sgn) snprintf(line, 200, "\\vdef{%s}   {\\ensuremath{{(%+5.1f \\pm %5.1f)\\times 10^{%d}} } }",
		     name.c_str(), mantN, mantNE, static_cast<int>(expoN));
  } else if (2 == digits ) {
    snprintf(line, 200, "\\vdef{%s}   {\\ensuremath{{(%5.2f \\pm %5.2f)\\times 10^{%d}} } }",
	    name.c_str(), mantN, mantNE, static_cast<int>(expoN));
    if (sgn) snprintf(line, 200, "\\vdef{%s}   {\\ensuremath{{(%+5.2f \\pm %5.2f)\\times 10^{%d}} } }",
		     name.c_str(), mantN, mantNE, static_cast<int>(expoN));
  } else if (3 == digits ) {
    snprintf(line, 200, "\\vdef{%s}   {\\ensuremath{{(%5.3f \\pm %5.3f)\\times 10^{%d}} } }",
	    name.c_str(), mantN, mantNE, static_cast<int>(expoN));
    if (sgn) snprintf(line, 200, "\\vdef{%s}   {\\ensuremath{{(%+5.3f \\pm %5.3f)\\times 10^{%d}} } }",
		     name.c_str(), mantN, mantNE, static_cast<int>(expoN));
  } else if (4 == digits ) {
    snprintf(line, 200, "\\vdef{%s}   {\\ensuremath{{(%5.4f \\pm %5.4f)\\times 10^{%d}} } }",
	    name.c_str(), mantN, mantNE, static_cast<int>(expoN));
    if (sgn) snprintf(line, 200, "\\vdef{%s}   {\\ensuremath{{(%+5.4f \\pm %5.4f)\\times 10^{%d}} } }",
		     name.c_str(), mantN, mantNE, static_cast<int>(expoN));
  } else if (5 == digits ) {
    snprintf(line, 200, "\\vdef{%s}   {\\ensuremath{{(%6.5f \\pm %6.5f)\\times 10^{%d}} } }",
	    name.c_str(), mantN, mantNE, static_cast<int>(expoN));
    if (sgn) snprintf(line, 200, "\\vdef{%s}   {\\ensuremath{{(%+6.5f \\pm %6.5f)\\times 10^{%d}} } }",
		     name.c_str(), mantN, mantNE, static_cast<int>(expoN));
  } else if (6 == digits ) {
    snprintf(line, 200, "\\vdef{%s}   {\\ensuremath{{(%7.6f \\pm %7.6f)\\times 10^{%d}} } }",
	    name.c_str(), mantN, mantNE, static_cast<int>(expoN));
    if (sgn) snprintf(line, 200, "\\vdef{%s}   {\\ensuremath{{(%+7.6f \\pm %7.6f)\\times 10^{%d}} } }",
		     name.c_str(), mantN, mantNE, static_cast<int>(expoN));
  } else if (7 == digits ) {
    snprintf(line, 200, "\\vdef{%s}   {\\ensuremath{{(%8.7f \\pm %8.7f)\\times 10^{%d}} } }",
	    name.c_str(), mantN, mantNE, static_cast<int>(expoN));
    if (sgn) snprintf(line, 200, "\\vdef{%s}   {\\ensuremath{{(%+8.7f \\pm %8.7f)\\times 10^{%d}} } }",
		     name.c_str(), mantN, mantNE, static_cast<int>(expoN));
  } else if (8 == digits ) {
    snprintf(line, 200, "\\vdef{%s}   {\\ensuremath{{(%9.8f \\pm %9.8f)\\times 10^{%d}} } }",
	    name.c_str(), mantN, mantNE, static_cast<int>(expoN));
    if (sgn) snprintf(line, 200, "\\vdef{%s}   {\\ensuremath{{(%+9.8f \\pm %9.8f)\\times 10^{%d}} } }",
		     name.c_str(), mantN, mantNE, static_cast<int>(expoN));
  } else if (9 == digits ) {
    snprintf(line, 200, "\\vdef{%s}   {\\ensuremath{{(%10.9f \\pm %10.9f)\\times 10^{%d}} } }",
	    name.c_str(), mantN, mantNE, static_cast<int>(expoN));
    if (sgn) snprintf(line, 200, "\\vdef{%s}   {\\ensuremath{{(%+10.9f \\pm %10.9f)\\times 10^{%d}} } }",
		     name.c_str(), mantN, mantNE, static_cast<int>(expoN));
  } else if (10 == digits ) {
    snprintf(line, 200, "\\vdef{%s}   {\\ensuremath{{(%11.10f \\pm %11.10f)\\times 10^{%d}} } }",
	    name.c_str(), mantN, mantNE, static_cast<int>(expoN));
    if (sgn) snprintf(line, 200, "\\vdef{%s}   {\\ensuremath{{(%+11.10f \\pm %11.10f)\\times 10^{%d}} } }",
		     name.c_str(), mantN, mantNE, static_cast<int>(expoN));
  } else if (11 == digits ) {
    snprintf(line, 200, "\\vdef{%s}   {\\ensuremath{{(%12.11f \\pm %12.11f)\\times 10^{%d}} } }",
	    name.c_str(), mantN, mantNE, static_cast<int>(expoN));
    if (sgn) snprintf(line, 200, "\\vdef{%s}   {\\ensuremath{{(%+12.11f \\pm %12.11f)\\times 10^{%d}} } }",
		     name.c_str(), mantN, mantNE, static_cast<int>(expoN));
  } else if (digits <0) {
    snprintf(line, 200, "\\vdef{%s}   {\\ensuremath{{%5.1f \\times 10^{%d}} } }",
	    name.c_str(), mantN, static_cast<int>(expoN));
    if (sgn) snprintf(line, 200, "\\vdef{%s}   {\\ensuremath{{%+5.1f \\times 10^{%d}} } }",
		     name.c_str(), mantN, static_cast<int>(expoN));
  } else {
    snprintf(line, 200, "\\vdef{%s}   {\\ensuremath{{%5.0f \\times 10^{%d}} } }",
	    name.c_str(), mantN, static_cast<int>(expoN));
  }

  return string(line);
}


// ----------------------------------------------------------------------
string formatTex(double n, string name, string tag) {

  char line[200];

  if (TMath::IsNaN(n) ) {
    snprintf(line, 200, "\\vdef{%s:%s}   {\\ensuremath{{\\mathrm{NaN} } } }", name.c_str(), tag.c_str());
  } else if ( n > 100. ) {
    snprintf(line, 200, "\\vdef{%s:%s}   {\\ensuremath{{%6.0f } } }", name.c_str(), tag.c_str(), n);
  } else if ( n > 1. ) {
    snprintf(line, 200, "\\vdef{%s:%s}   {\\ensuremath{{%4.2f } } }", name.c_str(), tag.c_str(), n);
  } else if ( n > 1.e-1) {
    snprintf(line, 200, "\\vdef{%s:%s}   {\\ensuremath{{%4.3f } } }", name.c_str(), tag.c_str(), n);
  } else if ( n > 1.e-3) {
    snprintf(line, 200, "\\vdef{%s:%s}   {\\ensuremath{{%4.3f } } }", name.c_str(), tag.c_str(), n);
  } else {
    snprintf(line, 200, "\\vdef{%s:%s}   {\\ensuremath{{0.0 } } }", name.c_str(), tag.c_str());
  }

  string result(line);
  return result;
}


// ----------------------------------------------------------------------
string formatTLV(const TLorentzVector &v, int mode) {
  if (1 == mode) {
    return string(Form("(%5.4f, %5.4f, %5.4f, E=%5.4f)", v.X(), v.Y(), v.Z(), v.E()));
  } else if (2 == mode) {
    return string(Form("(%5.4f, %5.4f, %5.4f, m=%5.4f)", v.X(), v.Y(), v.Z(), v.M()));
  }
  return string(Form("(%5.4f, %+3.2f, %+3.2f)", v.Perp(), v.Eta(), v.Phi()));
}


// ----------------------------------------------------------------------
string formatTVector3(const TVector3 &v, int mode) {
  if (1 == mode) {
    return string(Form("(%5.4f, %5.4f, %5.4f)", v.X(), v.Y(), v.Z()));
  }
  return string(Form("(%5.4f, %+3.2f, %+3.2f)", v.Perp(), v.Eta(), v.Phi()));
}


// ----------------------------------------------------------------------
double median(TH1 *h1) {
  // http://root.cern.ch/phpBB3/viewtopic.php?t=3620
  //compute the median for 1-d histogram h1
  Int_t nbins = h1->GetXaxis()->GetNbins();
  Double_t *x = new Double_t[nbins];
  Double_t *y = new Double_t[nbins];
  for (Int_t i=0;i<nbins;i++) {
    x[i] = h1->GetXaxis()->GetBinCenter(i+1);
    y[i] = h1->GetBinContent(i+1);
  }
  Double_t median = TMath::Median(nbins,x,y);
  delete [] x;
  delete [] y;
  return median;
}

// ----------------------------------------------------------------------
void cleanupString(string &s) {
  replaceAll(s, "\t", " ");
  string::size_type s1 = s.find("#");
  if (string::npos != s1) s.erase(s1);
  if (0 == s.length()) return;
  string::iterator new_end = unique(s.begin(), s.end(), bothAreSpaces);
  s.erase(new_end, s.end());
  if (s.substr(0, 1) == string(" ")) s.erase(0, 1);
  if (s.substr(s.length()-1, 1) == string(" ")) s.erase(s.length()-1, 1);
}


// ----------------------------------------------------------------------
bool bothAreSpaces(char lhs, char rhs) {
  return (lhs == rhs) && (lhs == ' ');
}


// ----------------------------------------------------------------------=
void rmSubString(string &sInput, const string &sub) {
  string::size_type foundpos = sInput.find(sub);
  if (foundpos != string::npos)  {
    sInput.erase(sInput.begin() + foundpos, sInput.begin() + foundpos + sub.length());
  }
}

// ----------------------------------------------------------------------=
void rmPath(string &sInput) {
  string::size_type foundpos = sInput.find("/");
  while(string::npos != foundpos) {
    sInput.erase(sInput.begin(), sInput.begin() + foundpos + 1);
    foundpos = sInput.find("/");
  }
}

// ----------------------------------------------------------------------
vector<string> glob(string basedir, string basename) {
  cout << "Looking in " << basedir << " for " << basename << endl;
  vector<string> lof;
  TString fname;
  const char *file;
  TSystem *lunix = gSystem; //new TUnixSystem();
  void *pDir = lunix->OpenDirectory(basedir.c_str());
  while ((file = lunix->GetDirEntry(pDir))) {
    fname = file;
    if (fname.Contains(basename.c_str())) {
      lof.push_back(string(fname));
    }
  }
  return lof;
}


// ----------------------------------------------------------------------
vector<string> split(const string &s, char delim) {
  vector<string> elems;
  split(s, delim, elems);
  return elems;
}


// ----------------------------------------------------------------------
void /*vector<string>&*/ split(const string &s, char delim, vector<string> &elems) {
  stringstream ss(s);
  string item;
  while (getline(ss, item, delim)) {
    elems.push_back(item);
  }
  //  return elems;
}


// ----------------------------------------------------------------------
void hplAll(const char *hpat, const char *options, int n) {

  TCanvas *cc = (TCanvas*)gROOT->FindObject("c0");
  cc->Clear();
  vector<TH1D *> h1;
  if (n > 0) h1.reserve(n);
  vector<TH2D *> h2;
  if (n > 0) h2.reserve(n);

  TIter next(gDirectory->GetListOfKeys());
  TKey *key(0);
  while ((key = (TKey*)next())) {
    if (gROOT->GetClass(key->GetClassName())->InheritsFrom("TDirectory")) continue;
    TH1 *sig = (TH1*)key->ReadObj();
    int version = key->GetCycle();
    TString hname(sig->GetName());
    //    hname += Form(";%d", version);
    //    cout << "hname = " << hname << endl;
    if (0 == sig->GetEntries()) continue;
    if (!hname.Contains(hpat)) continue;
    if (gROOT->GetClass(key->GetClassName())->InheritsFrom("TH1D")) {
      h1.push_back((TH1D*)gDirectory->Get(hname));
      h1.back()->SetName(Form("%s;%d", h1.back()->GetName(), version));
    }
    if (gROOT->GetClass(key->GetClassName())->InheritsFrom("TH2D")) {
      h2.push_back((TH2D*)gDirectory->Get(hname));
      h2.back()->SetName(Form("%s;%d", h2.back()->GetName(), version));
    }
  }

  if (h1.size() > 0) {
    cout << "size of h1: " << h1.size() << endl;
    cin.clear();
    for (unsigned int i = 0; i < h1.size(); ++i) {
      hpl(h1[i], options);
      cc->Modified();
      cc->Update();
      gSystem->ProcessEvents(); // macos peculiarity!
      cout << h1[i]->GetName() << " [RET|q|Q]" << endl;
      string x;
      std::getline(std::cin, x);
      if (x == EOF || x == "q" || x == "Q") {
	break;
      }
    }
  }

  if (h2.size() > 0) {
    cout << "size of h2: " << h2.size() << endl;
    cin.clear();
    for (unsigned int i = 0; i < h2.size(); ++i) {
      hpl(h2[i], options);
      cc->Modified();
      cc->Update();
      gSystem->ProcessEvents(); // macos peculiarity!
      cout << h2[i]->GetName() << " [RET|q|Q]" << endl;
      string x;
      std::getline(std::cin, x);
      if (x == EOF || x == "q" || x == "Q") {
	break;
      }
    }
  }

}


// ----------------------------------------------------------------------
void hpl(const char *h, const char *options) {

  TH1 *h1 = (TH1*)gDirectory->Get(h);
  if (!h1) {
    cout << h << " not found" << endl;
  } else {
    hpl(h1, options);
  }
}



// ----------------------------------------------------------------------
void hpl(TH1 *hin, const char *options) {

  TH1 *h = (TH1*)hin->Clone(Form("hpl_%s", hin->GetName()));

  TString parse(options);

  if (!h) {
    cout << "histogram not found" << endl;
  }

  // -- "same" shortcut
  if (parse.Contains(",s", TString::kIgnoreCase)) {
    parse += TString("same");
  }

  // -- limits
  float xmin(99.), xmax(98.),  ymin(99.), ymax(98.);
  TRegexp re1("(.*:.*)");
  TRegexp re2("(.*:.*,.*:.*)");
  TSubString bla2 = parse(re2);
  sscanf(bla2.Data(), "(%f:%f,%f:%f)", &xmin, &xmax, &ymin, &ymax);
  parse(re2) = "";
  if ((xmin < ymax) && (ymin < ymax)) {
    cout << "restricting x to " << xmin << " .. " << xmax << " and y to " << ymin << " .. " << ymax << endl;
    h->SetAxisRange(xmin, xmax);
    h->SetMinimum(ymin);
    h->SetMaximum(ymax);
  }
  TSubString  bla1 = parse(re1);
  sscanf(bla1.Data(), "(%f:%f)", &xmin, &xmax);
  parse(re1) = "";
  if (xmin < xmax) {
    cout << "restricting to " << xmin << " .. " << xmax << endl;
    h->SetAxisRange(xmin, xmax);
  }


  // -- normalize histogram to histogram already plotted
  if (parse.Contains("norm", TString::kIgnoreCase)) {
    parse.ReplaceAll(TString("norm"), "");
    TList *tl = gPad->GetListOfPrimitives();
    if (tl) {
      TListIter next(tl);
      TObject *to;
      double norma(-99.);
      TH1 *h1;
      while ((to=next())) {
	if (to->InheritsFrom(TH1::Class())) {
	  h1 = (TH1*)to;
	  norma = h1->GetSumOfWeights();
	  cout << h1->GetName() << "  " << h1->GetTitle() << " with " << norma << " events "  << endl;
	  break; // Take first one
	}
      }
      if (norma > -99.) {
	h->Scale(norma/h->GetSumOfWeights());
      }
    }
  }



  // -- grids, logx
  if (parse.Contains("logy", TString::kIgnoreCase)) {
    parse.ReplaceAll(TString("logy"), "");
    gPad->SetLogy(1);
  }
  if (parse.Contains("liny", TString::kIgnoreCase)) {
    parse.ReplaceAll(TString("liny"), "");
    gPad->SetLogy(0);
  }
  if (parse.Contains("logx", TString::kIgnoreCase)) {
    parse.ReplaceAll(TString("logx"), "");
    gPad->SetLogx(1);
  }
  if (parse.Contains("linx", TString::kIgnoreCase)) {
    parse.ReplaceAll(TString("linx"), "");
    gPad->SetLogx(0);
  }
  if (parse.Contains("logz", TString::kIgnoreCase)) {
    parse.ReplaceAll(TString("logz"), "");
    gPad->SetLogz(1);
  }
  if (parse.Contains("linz", TString::kIgnoreCase)) {
    parse.ReplaceAll(TString("linz"), "");
    gPad->SetLogz(0);
  }
  if (parse.Contains("gridx", TString::kIgnoreCase)) {
    parse.ReplaceAll(TString("gridx"), "");
    gPad->SetGridx(1);
  }
  if (parse.Contains("gridy", TString::kIgnoreCase)) {
    parse.ReplaceAll(TString("gridy"), "");
    gPad->SetGridy(1);
  }
  if (parse.Contains("ngridx", TString::kIgnoreCase)) {
    parse.ReplaceAll(TString("ngridx"), "");
    gPad->SetGridx(0);
  }
  if (parse.Contains("ngridy", TString::kIgnoreCase)) {
    parse.ReplaceAll(TString("ngridy"), "");
    gPad->SetGridy(0);
  }
  if (parse.Contains("optstat0", TString::kIgnoreCase)) {
    parse.ReplaceAll(TString("optstat0"), "");
    gStyle->SetOptStat(0);
  }

  // -- Colors, fillstyle
  if (parse.Contains("red", TString::kIgnoreCase)) {
    parse.ReplaceAll(TString("red"), "");
    h->SetMarkerColor(kRed);
    h->SetLineColor(kRed);
    h->SetFillColor(kRed);
  }
  else if (parse.Contains("green", TString::kIgnoreCase)) {
    parse.ReplaceAll(TString("green"), "");
    h->SetMarkerColor(kGreen);
    h->SetLineColor(kGreen);
    h->SetFillColor(kGreen);
  }
  else if (parse.Contains("yellow", TString::kIgnoreCase)) {
    parse.ReplaceAll(TString("yellow"), "");
    h->SetMarkerColor(kYellow);
    h->SetLineColor(kYellow);
    h->SetFillColor(kYellow);
  }
  else if (parse.Contains("blue", TString::kIgnoreCase)) {
    parse.ReplaceAll(TString("blue"), "");
    h->SetMarkerColor(kBlue);
    h->SetLineColor(kBlue);
    h->SetFillColor(kBlue);
  }
  else if (parse.Contains("cyan", TString::kIgnoreCase)) {
    parse.ReplaceAll(TString("cyan"), "");
    h->SetMarkerColor(kCyan);
    h->SetLineColor(kCyan);
    h->SetFillColor(kCyan);
  }
  else {
    h->SetMarkerColor(kBlack);
    h->SetLineColor(kBlack);
  }

  if (parse.Contains("dot", TString::kIgnoreCase)) {
    parse.ReplaceAll(TString("dot"), "");
    h->SetLineStyle(3);
  }
  else if (parse.Contains("dash", TString::kIgnoreCase)) {
    parse.ReplaceAll(TString("dash"), "");
    h->SetLineStyle(2);
  }

  if (parse.Contains("fill", TString::kIgnoreCase)) {
    parse.ReplaceAll(TString("fill"), "");
    h->SetLineColor(kBlack);
    h->SetFillStyle(1000);
  }
  else if (parse.Contains("upr", TString::kIgnoreCase)) {
    parse.ReplaceAll(TString("upr"), "");
    h->SetFillStyle(3004);
  }
  else if (parse.Contains("upl", TString::kIgnoreCase)) {
    parse.ReplaceAll(TString("upl"), "");
    h->SetFillStyle(3005);
  }
  else {
    h->SetFillStyle(0);
  }

  h->Draw(parse);
}
