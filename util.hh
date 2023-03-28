#ifndef UTIL_H
#define UTIL_H

#include <string>

#include "TH1.h"
#include "TCanvas.h"
#include "TROOT.h"
#include "TGraph.h"
#include "TMinuit.h"
#include "TDirectory.h"
#include "TColor.h"
#include "TFile.h"
#include "TLegend.h"
#include "TLorentzVector.h"

#include <vector>
#include <cstdarg>


class dataset;


// ======================================================================
// string utilities
// ======================================================================
void replaceAll(std::string &s, const std::string &from, const std::string &to);
void cleanupString(std::string &);
bool bothAreSpaces(char lhs, char rhs);
void rmSubString(std::string &sinput, const std::string &remove);
void rmPath(std::string &sInput);

std::vector<std::string>  glob(std::string basedir, std::string basename);
std::vector<std::string>  split(const std::string &s, char delim);
void split(const std::string &s, char delim, std::vector<std::string> &elems); // helper function for above


// -- vanilla?
std::string formatTex(double n, std::string name, std::string tag);
// -- normal variant
std::string formatTex(double n, std::string name, int digits, int sgn = 0);
// -- value with error in scientific notation
std::string formatTexErrSci(double n, double nE, std::string name, int digits, int sgn = 0);

std::string formatTLV(const TLorentzVector &, int mode = 0);
std::string formatTVector3(const TVector3 &, int mode = 0);



// ======================================================================
// ROOT General utilities
// ======================================================================
void printNonZero(TH1*);
void printAxesLabels(TH1*);
double getValueByLabel(TH1 *h, std::string label);

void addOverflow(TH1D *);
void addUnderflow(TH1D *);
void zeroNegativeEntries(TH1D *);

TLegend* newLegend(std::string title, double x1, double y1, double x2, double y2,
                   std::vector<TH1*> hists, std::vector<std::string> names, std::vector<std::string> options);
TLegend* newLegend(double x1, double y1, double x2, double y2);

void stampAndSave(TCanvas *fC, const char *s = "bla.pdf");
void shrinkPad(double b = 0.1, double l = 0.1, double r = 0.1, double t = 0.1);
void babar(double x, double y, double scale = 1.0, int prel = 0);
void zone(int x = 1, int y = 1, TCanvas *c = 0);
int  wait();
void colors(int choice = 0);


std::vector<int> defVector(int n, ...);

// ======================================================================
// ROOT Utitilities for displays and typing shortcuts
// ======================================================================
// -- Histograms
void setTitles(TH1 *h, const char *sx, const char *sy,
	       float size = 0.05, float xoff = 1.1, float yoff = 1.1, float lsize = 0.05, int font = 42);
void setHist(TH1 *h, int color = kBlack, int symbol = 20, double size = 1., double width = 2.);
void setGraph(TGraph *g, int color = kBlack, int symbol = 20, double size = 1., double width = 2.);
void setFilledHist(TH1 *h, int lcol = kBlack, int fcol = kYellow, int fstyle = 1000, int width = 1);
void setMaximum(double scale = 1.2, TH1 *h1 = 0, TH1 *h2 = 0);

void setHist(TH1 *h, dataset *ds);
void setHistTitles(TH1 *h, dataset *ds, const char *sx, const char *sy,
		   float size = 0.05, float xoff = 1.1, float yoff = 1.1, float lsize = 0.05, int font = 42);

void showOverflow(TH1 *h);


// ======================================================================
// Utitilities for calculations
// ======================================================================

double chi2Prob(double chi2, double ndof);

// -- Chi^2 test for two histograms, where the errors are sqrt(n_bin)
//    If they are constrained to the same area, constrain = 0
//    If they are NOT constrained, set constrain = -1
double chi2Test(TH1*, TH1*, double& chi2, double& ndof, int constrain = 0);

// -- Chi^2 test for two histograms, where the errors are taken from the histogram
//    If they are constrained to the same area, constrain = 0
//    If they are NOT constrained, set constrain = -1
//    This function is equivalent to chi2Test() if the errors are sqrt(n_bin)
double chi2TestErr(TH1*, TH1*, double& chi2, double& ndof, int constrain = 0);

// -- from arxiv:physics/0403046 (R. Barlow)
double poissonError(int n, double &up, double &down);

void average(double &av, double &error, int n, double *val, double *err);
void average(double &av, double &error, std::vector<double> &vval, std::vector<double> &verr, double &chi2);

double median(TH1* h);

// TGraph* scanParameter(int parnum, int nsig, TMinuit &a, void (*func)(int &, double *, double &, double *, int));

// ======================================================================
// Utitilities for error calculations
// ======================================================================
// -- d'Agostini page 86
double dEff(int, int);
double dEff(int, double);
double dEff(double, int);
double dEff(double, double, double, double);
// -- from the ROOT source
double dBinomial(double, double);
// -- normal error propagation, assumes error = sqrt(..)
double dRatio(double, double);
// -- normal error propagation:  n, error, N, error
double dRatio(double, double, double, double);
// -- error on branching fraction n/(epsilon N)
double dBF(double, double, double, double, double, double);
// -- Add up in quadrature the errors of all bins
double getError(TH1*);
double getErrorRange(TH1*, int = -1, int = -1);

// ======================================================================
// Utilities for analysis
// ======================================================================
void mk4Vector(TLorentzVector &p4, const Double_t p, const Double_t t, const Double_t f, const Double_t m);
TLorentzVector mk4Vector(const Double_t p, const Double_t t, const Double_t f, const Double_t m);

bool isLepton(int id);
bool isQuark(int id);
int  quarkFlavor(int i);
bool isBeautyMeson(int i);
bool isBeautyMesonWeak(int i);
bool isBeautyBaryon(int i);
bool isBeautyBaryonWeak(int i);
bool isCharmMeson(int i);
bool isCharmMesonWeak(int i);
bool isCharmBaryon(int i);
bool isCharmBaryonWeak(int i);
bool isLightMeson(int i);
bool isStableCharged(int i);



// ======================================================================
// see whether they are visible when present here
// ======================================================================
void  hpl(TH1 *h, const char *options="");
void  hpl(const char *h, const char *options="");
void  hplAll(const char *h, const char *options="", int n = -1);
// ======================================================================


#endif
