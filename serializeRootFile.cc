#include <iostream>
#include <fstream>


#include "TROOT.h"
#include "TKey.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TTree.h"
#include "TVectorT.h"
#include "TVectorDfwd.h"

using namespace std;

string serialize();



// ----------------------------------------------------------------------
string serialize() {
  TKey *key(0);
  TIter next(gDirectory->GetListOfKeys());
  string result(""), prevName("");
  while ((key = (TKey*)next())) {
    if (gROOT->GetClass(key->GetClassName())->InheritsFrom("TDirectory")) {
      TDirectory *pD2 = (TDirectory*)key;
      cout << "dir " << pD2->GetName() << endl; 
      gDirectory->cd(pD2->GetName());
      cout << "gDirectory->GetName() = " << gDirectory->GetName() << endl;
      result += serialize(); 
    } 
    if (key->GetName() == prevName) continue;
    cout << "key " << key->GetName() << " cycle: " << key->GetCycle()
         << " class: " << key->GetClassName()
         << endl;
    if (gROOT->GetClass(key->GetClassName())->InheritsFrom("TH1F")) {
      TH1F *h1 = (TH1F*)key->ReadObj();
      cout << "h1 " << h1->GetName() << " nbinsx = " << h1->GetNbinsX() << endl;
      for (int i = 0; i <= h1->GetNbinsX(); ++i){
        cout << h1->GetBinContent(i) << ";";
      }
      cout << endl;
    } else if (gROOT->GetClass(key->GetClassName())->InheritsFrom("TH2F")) {
      TH2F *h1 = (TH2F*)key->ReadObj();
      cout << "h1 " << h1->GetName() << " nbinsx = " << h1->GetNbinsX()
           << " nbinsy = " << h1->GetNbinsY()
           << endl;
      for (int i = 0; i <= h1->GetNbinsX(); ++i){
        for (int j = 0; j <= h1->GetNbinsY(); ++j){
          cout << h1->GetBinContent(i, j) << ";";
        }
      }
      cout << endl;
    } else if (gROOT->GetClass(key->GetClassName())->InheritsFrom("TVectorT<double>")) {
      TVectorT<double> *h1 = (TVectorT<double> *)key->ReadObj();
      cout << "TVectorT " << h1->GetName() << " nrows = " << h1->GetNoElements()
           << endl;
      cout << h1[0][0] << endl;
    } else if (gROOT->GetClass(key->GetClassName())->InheritsFrom("TTree")) {
      TTree *h1 = (TTree*)key->ReadObj();
      cout << "TTree " << h1->GetName() << " nentries = " << h1->GetEntries()
           << endl;

      int nbr = h1->GetListOfBranches()->GetEntries();
      h1->SetScanField(0);
      for (int i = 0; i != nbr; ++i) {
        const char* name = h1->GetListOfBranches()->At(i)->GetName();
        cout << name << " start scan" << endl;
        //        h1->Scan(name, "", "precision=8");
        h1->Scan(name);
        cout << name << " end scan" << endl;
      }
    }
    prevName = key->GetName();
  }
  string dname = gDirectory->GetName();
  gDirectory->cd("..");
  return result;
}


// ----------------------------------------------------------------------
int main(int argc, char* argv[]) {
  string filename;
  for (int i = 0; i < argc; i++){
    if (!strcmp(argv[i], "-f"))   {filename = argv[++i];}
  }
  TFile *f = TFile::Open(filename.c_str());
  string contents = serialize();
 
  return 0;
}
