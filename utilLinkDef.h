#include <string.h>

#include "TH1.h"
#include "TGraph.h"
#include "TMinuit.h"
#include "TCanvas.h"
#include "TLorentzVector.h"

#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ function replaceAll(std::string &, const std::string &, const std::string &);
#pragma link C++ function cleanupString(std::string &);
#pragma link C++ function bothAreSpaces(char, char);
#pragma link C++ function rmSubString(std::string &, const std::string &);
#pragma link C++ function rmPath(std::string &);
#pragma link C++ function glob(std::string, std::string);
#pragma link C++ function split(const std::string &, char, std::vector<std::string> &);
#pragma link C++ function split(const std::string &, char);

#pragma link C++ function formatTex(double, std::string, std::string);
#pragma link C++ function formatTex(double, std::string, int, int);
#pragma link C++ function formatTexErrSci(double, double, std::string, int, int);
#pragma link C++ function formatTLV(const TLorentzVector &, int);
#pragma link C++ function formatTVector3(const TVector3 &, int);


#pragma link C++ function printNonZero(TH1*);
#pragma link C++ function printAxesLabels(TH1*);
#pragma link C++ function getValueByLabel(TH1 *, std::string);
#pragma link C++ function addOverflow(TH1D *);
#pragma link C++ function addUnderflow(TH1D *);
#pragma link C++ function zeroNegativeEntries(TH1D *);
#pragma link C++ function showOverflow(TH1 *);
#pragma link C++ function newLegend(std::string, double, double, double, double, std::vector<TH1*>, std::vector<std::string>, std::vector<std::string>);
#pragma link C++ function newLegend(double, double, double, double);

#pragma link C++ function stampAndSave(TCanvas *, const char*);
#pragma link C++ function shrinkPad(double, double, double, double);
#pragma link C++ function babar(double, double, double);
#pragma link C++ function zone(int, int, TCanvas *);
#pragma link C++ function wait();
#pragma link C++ function colors();

#pragma link C++ function setTitles(TH1*,const char*,const char*,float,float,float,float,int);
#pragma link C++ function setHist(TH1*,int,int,double,double);
#pragma link C++ function setGraph(TGraph*,int,int,double,double);
#pragma link C++ function setFilledHist(TH1 *,int,int,int,int);
#pragma link C++ function setMaximum(double, TH1 *, TH1 *);

#pragma link C++ function defVector(int, ...);

#pragma link C++ function chi2Prob(double, double);
#pragma link C++ function chi2Test(TH1*, TH1*, double&, double&, int);
#pragma link C++ function chi2TestErr(TH1*, TH1*, double&, double&, int);
#pragma link C++ function poissonError(int , double &, double &);
#pragma link C++ function poissonErrorExact(int , double &, double &);

#pragma link C++ function dEff(int, int);
#pragma link C++ function dEff(int, double);
#pragma link C++ function dEff(double, int);
#pragma link C++ function dEff(double, double, double, double);

#pragma link C++ function median(TH1*);
#pragma link C++ function average(double&, double&, int, double*, double*);
#pragma link C++ function average(double&, double&, std::vector<double> &, std::vector<double> &, double&);

#pragma link C++ function dBinomial(double, double);
#pragma link C++ function dRatio(double, double);
#pragma link C++ function dRatio(double, double, double, double);
#pragma link C++ function dBF(double, double, double, double, double, double);

#pragma link C++ function getError(TH1*);
#pragma link C++ function getErrorRange(TH1*, int, int);

#pragma link C++ function mk4Vector(TLorentzVector &, const Double_t, const Double_t, const Double_t, const Double_t);
#pragma link C++ function mk4Vector(const Double_t, const Double_t, const Double_t, const Double_t);

#pragma link C++ function hpl(TH1*, const char *);
#pragma link C++ function hpl(const char *, const char *);
#pragma link C++ function hplAll(const char *, const char *, int);


#pragma link C++ function isLepton(int);
#pragma link C++ function isQuark(int);

#pragma link C++ function quarkFlavor(int);

#pragma link C++ function isBeautyMeson(int);
#pragma link C++ function isBeautyMesonWeak(int);
#pragma link C++ function isBeautyBaryon(int);
#pragma link C++ function isBeautyBaryonWeak(int);

#pragma link C++ function isCharmMeson(int);
#pragma link C++ function isCharmMesonWeak(int);
#pragma link C++ function isCharmBaryon(int);
#pragma link C++ function isCharmBaryonWeak(int);

#pragma link C++ function isLightMeson(int);
#pragma link C++ function isStableCharged(int);

#endif
