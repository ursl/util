#include <iostream>
#include <fstream>
#include <iomanip>

#include "util.hh"
#include "TMath.h"
#include "TColor.h"
#include "TCanvas.h"
#include "TLatex.h"
#include "TString.h"
#include "TH1.h"
#include "TH2.h"

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
void setGraph(TGraph *h, Int_t color, Int_t symbol, Double_t size, Double_t width) {
  h->SetLineColor(color);   h->SetLineWidth(width);
  h->SetMarkerColor(color); h->SetMarkerStyle(symbol);  h->SetMarkerSize(size); 
}


// ----------------------------------------------------------------------
void colors(int i) {
  TColor *a;

  if (i == 0) {
    a = new TColor(600, 0.66, 0.66, 0.66, "gray");
    a = new TColor(601, 0.90, 0.90, 0.90, "gray");
    a = new TColor(602, 0.26, 0.26, 0.26, "gray");
    a = new TColor(610, 0.00, 0.66, 0.00, "green");
    a = new TColor(612, 0.00, 0.33, 0.00, "darkgreen");
    a = new TColor(613, 0.00, 0.90, 0.00, "lightgreen");
    a = new TColor(624, 0.00, 0.00, 0.60, "blue");
    a = new TColor(625, 0.00, 0.00, 0.85, "lightblue");
    a = new TColor(626, 0.00, 0.00, 0.35, "darkblue");
    a = new TColor(637, 0.50, 0.00, 0.00, "red");
    a = new TColor(638, 0.99, 0.00, 0.00, "lightred");
    a = new TColor(639, 0.99, 0.00, 0.00, "darkred");
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
void average(double &av, double &error, int n, double *val, double *verr) {
  double e(0.), w8(0.), sumW8(0.), sumAve(0.); 
  for (int i = 0; i < n; ++i) {
    //    cout << i << " " << val[i] << " +/- " << verr[i] << endl;

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
  if ( isnan(n) ) {
    sprintf(line, "\\vdef{%s}   {\\ensuremath{{\\mathrm{NaN} } } }", name.c_str());
  } else if (0 == digits ) {
    sprintf(line, "\\vdef{%s}   {\\ensuremath{{%i } } }", name.c_str(), static_cast<int>(n));
  } else if (1 == digits ) {
    sprintf(line, "\\vdef{%s}   {\\ensuremath{{%5.1f } } }", name.c_str(), n);
    if (sgn) sprintf(line, "\\vdef{%s}   {\\ensuremath{{%+5.1f } } }", name.c_str(), n);
  } else if (2 == digits ) {
    sprintf(line, "\\vdef{%s}   {\\ensuremath{{%5.2f } } }", name.c_str(), n);
    if (sgn) sprintf(line, "\\vdef{%s}   {\\ensuremath{{%+5.2f } } }", name.c_str(), n);
  } else if (3 == digits ) {
    sprintf(line, "\\vdef{%s}   {\\ensuremath{{%5.3f } } }", name.c_str(), n);
    if (sgn) sprintf(line, "\\vdef{%s}   {\\ensuremath{{%+5.3f } } }", name.c_str(), n);
  } else if (4 == digits ) {
    sprintf(line, "\\vdef{%s}   {\\ensuremath{{%5.4f } } }", name.c_str(), n);
    if (sgn) sprintf(line, "\\vdef{%s}   {\\ensuremath{{%+5.4f } } }", name.c_str(), n);
  } else if (5 == digits ) {
    sprintf(line, "\\vdef{%s}   {\\ensuremath{{%6.5f } } }", name.c_str(), n);
    if (sgn) sprintf(line, "\\vdef{%s}   {\\ensuremath{{%+6.5f } } }", name.c_str(), n);
  } else if (6 == digits ) {
    sprintf(line, "\\vdef{%s}   {\\ensuremath{{%7.6f } } }", name.c_str(), n);
    if (sgn) sprintf(line, "\\vdef{%s}   {\\ensuremath{{%+7.6f } } }", name.c_str(), n);
  } else {
    sprintf(line, "\\vdef{%s}   {\\ensuremath{{%f } } }", name.c_str(), n);
    if (sgn) sprintf(line, "\\vdef{%s}   {\\ensuremath{{%+f } } }", name.c_str(), n);
  }
  return string(line); 
}

// ----------------------------------------------------------------------
double median1(TH1 *h1) { 
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
