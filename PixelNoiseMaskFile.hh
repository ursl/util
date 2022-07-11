#ifndef PIXELNOISEMASKFILE
#define PIXELNOISEMASKFILE
 
#include <iostream>
#include <vector>

#include "TObject.h"


// ----------------------------------------------------------------------
// Representation of pixel noise mask files
// ----------------------------------------
//
// pixel chip has      256 x 250 pixels (col x row)
// noise mask file has 256 x 256 entries!
// Entries with col > 249 are ignored, but must be present
//
// Noisy pixels have word = 0x00
// Other pixels (presumably) have a tune DAC value or so instead.
// 
// The following format is used: 
// row  value
// ----------
//   0  0xYY first row
// ...  0xYY
// 249  0xYY last row
// 250  0xDA
// 251  0xDA  
// 252  0xDA
// 253  0xcn  cn = column number (in hex)
// 254  0xDA
// 255  0xzz  zz = 1 (if LVDS errors occurred on chip in run)
// ----------------------------------------------------------------------


class PixelNoiseMaskFile: public TObject {
  
public:
  // -- create nmf with no noise (all physical values are 0xff)
  PixelNoiseMaskFile(uint8_t val = 0xff);
  // -- create nmf from a vector
  PixelNoiseMaskFile(std::vector<uint8_t> &v);
  // -- create nmf reading from file
  PixelNoiseMaskFile(const std::string filename);
  // -- d'tor
  ~PixelNoiseMaskFile();

  // -- fill all words with value
  void fill(uint8_t value);

  // -- print a short summary
  void summarize();

  // -- print all (ncol = 32, 64, 256)
  void print(int ncol = 256);

  // -- given an index, return in a pair the column and row
  std::pair<int, int> colrowFromIdx(int idx);
  // -- given column and row, return the index
  int idxFromColRow(int col, int row);
  
private:
  std::vector<uint8_t> fWords;

  int VERBOSE;
  
  ClassDef(PixelNoiseMaskFile,1)

};

#endif
