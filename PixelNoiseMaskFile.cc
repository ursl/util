#include "PixelNoiseMaskFile.hh"

#include <TString.h>

using namespace std;

ClassImp(PixelNoiseMaskFile)

// ----------------------------------------------------------------------
PixelNoiseMaskFile::PixelNoiseMaskFile(uint8_t val) {
  fWords.reserve(256*256);
  for (int i = 0; i < 256*256; ++i) {
    fWords[i] = 0xff;
  }
  fill(val);
}

// ----------------------------------------------------------------------
PixelNoiseMaskFile::PixelNoiseMaskFile(std::vector<uint8_t> &v) {

}

// ----------------------------------------------------------------------
PixelNoiseMaskFile::PixelNoiseMaskFile(const string filename) {

}


// ----------------------------------------------------------------------
PixelNoiseMaskFile::~PixelNoiseMaskFile() {

}

// ----------------------------------------------------------------------
void PixelNoiseMaskFile::fill(uint8_t val) {
  for (int i = 0; i < 256*256; ++i) {
    if (i%256 < 250) {
      fWords.push_back(0xff);
    } else if (250 == i%256) {
      fWords.push_back(0xda);
    } else if (251 == i%256) {
      fWords.push_back(0xda);
    } else if (252 == i%256) {
      fWords.push_back(0xda);
    } else if (253 == i%256) {
      int col = i / 256; 
      fWords.push_back(col);
    } else if (254 == i%256) {
      fWords.push_back(0xda);
    } else if (255 == i%256) {
      fWords.push_back(0x0);
    }
  }
}


// ----------------------------------------------------------------------
void PixelNoiseMaskFile::summarize() {
  int cntNoisy(0);
  int errCode(0);
  for (unsigned int i = 0; i < fWords.size(); ++i){
    if ((0xda == fWords[i]) && (0xda == fWords[i+1])) {
      errCode = fWords[i+5];
      i += 5; //??
      continue;
    }
    pair<int, int> a = colrowFromIdx(i);
    if (0xff != fWords[i]) {
      ++cntNoisy;
      if (VERBOSE > 0) cout << Form("pix: %d/%d ", a.first, a.second);
    }
  }
  if (VERBOSE > 0) cout  << endl;
  //  gTotal += cntNoisy;
  cout << " Number of noisy pixels = " << cntNoisy
       << (errCode>0? Form(", errCode = %d", errCode): "")
       << endl;

}


// ----------------------------------------------------------------------
void PixelNoiseMaskFile::print(int ncol) {
  for (unsigned int i = 0; i < fWords.size(); ++i){
    cout << Form("%2x ", fWords[i]);
    if (ncol-1 == i%ncol) cout << endl;
  }
  cout << endl;
}


// ----------------------------------------------------------------------
pair<int, int> PixelNoiseMaskFile::colrowFromIdx(int idx) {
  int col = idx/256;
  int row = idx%256;
  return make_pair(col, row);
}

// ----------------------------------------------------------------------
int PixelNoiseMaskFile::idxFromColRow(int col, int row) {
  int idx = col*256 + row;
  return idx; 
}
