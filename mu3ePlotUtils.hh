#ifndef MU3EPLOTUTILS_H
#define MU3EPLOTUTILS_H

#include "util.hh"
#include "TCanvas.h"
#include "TH1.h"
#include "TObject.h"

#include <vector>
#include <map>
#include <string>

// ----------------------------------------------------------------------
class mu3ePlotUtils: public TObject {
public:
  mu3ePlotUtils();
  ~mu3ePlotUtils();

  void plotVertexL1L2(std::map<std::string, TH1*> &hists, std::string pattern);

  // -- predefined setups
  void setOptionsForHitmaps(bool rebin = false);
  void setOptionsForTotDistributions(bool rebin = false);

  std::string fPDFName{"out/plot.pdf"};



  // -- gStyle options
  int         fOptStat{11111};
  bool        fSetLogx{false};
  bool        fSetLogy{false};
  bool        fSetLogz{false};

  // -- histogram plotting options
  std::string fOpt{"hist"};
  double      fSetTitleSize{0.2};
  bool        fRebin{false};
  int         fRebinX{1};
  int         fRebinY{1};

  // -- TPad options
  int         fSetPadBorderMode{0};
  int         fSetPadBorderSize{0};

  double      fSetPadTopMargin{0};
  double      fSetPadBottomMargin{0};
  double      fSetPadLeftMargin{0};
  double      fSetPadRightMargin{0};

  // -- Canvas options
  int         fSetCanvasWidth{800};
  int         fSetCanvasHeight{1000};

private:
  std::vector<int> fLayer1;
  std::vector<int> fLayer2;
  std::vector<int> fAllChips;

  int fCanvasCounter{0};
};

#endif
