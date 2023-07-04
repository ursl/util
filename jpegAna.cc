#include <iostream>
#include <jpeglib.h>

#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"

using namespace std;

// ----------------------------------------------------------------------
int loadJpg(const char* Name) {
  unsigned char a, r, g, b, l;
  int width, height;
  struct jpeg_decompress_struct cinfo;
  struct jpeg_error_mgr jerr;

  TFile *f1 = TFile::Open("test.root", "RECREATE");
  
  FILE * infile;        /* source file */
  JSAMPARRAY pJpegBuffer;       /* Output row buffer */
  int row_stride;       /* physical row width in output buffer */
  if ((infile = fopen(Name, "rb")) == NULL) {
    fprintf(stderr, "can't open %s\n", Name);
    return 0;
  }
  cinfo.err = jpeg_std_error(&jerr);
  jpeg_create_decompress(&cinfo);
  jpeg_stdio_src(&cinfo, infile);
  (void) jpeg_read_header(&cinfo, TRUE);
  (void) jpeg_start_decompress(&cinfo);
  width = cinfo.output_width;
  height = cinfo.output_height;
  TH1D *hx = new TH1D("hx", "hx", height, 0., height);
  TH1D *hy = new TH1D("hy", "hy", width, 0., width);
  TH2D *h2  = new TH2D("h2", "h2", height, 0., height, width, 0., width);

  unsigned char * pDummy = new unsigned char[width*height*4];
  unsigned char * pTest = pDummy;
  if (!pDummy) {
    printf("NO MEM FOR JPEG CONVERT!\n");
    return 0;
  }
  row_stride = width * cinfo.output_components;
  pJpegBuffer = (*cinfo.mem->alloc_sarray)((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);

  int iline(0), y(0);
  while (cinfo.output_scanline < cinfo.output_height) {
    (void) jpeg_read_scanlines(&cinfo, pJpegBuffer, 1);
    for (int x = 0; x < width; x++) {
      a = 0; // alpha value is not supported on jpg
      r = pJpegBuffer[0][cinfo.output_components * x];
      if (cinfo.output_components > 2) {
        g = pJpegBuffer[0][cinfo.output_components * x + 1];
        b = pJpegBuffer[0][cinfo.output_components * x + 2];
      } else {
        g = r;
        b = r;
      }
      l = (0.2126*r + 0.7152*g + 0.0722*b);
      h2->Fill(y, x, l);
      if (iline == width/2) {
        hy->Fill(x, l);
        h2->Fill(y, x, 500.);
      }
      if (x == height/2) {
        hx->Fill(y, l);
        h2->Fill(y, x, 200.);
      }
      
      *(pDummy++) = b;
      *(pDummy++) = g;
      *(pDummy++) = r;
      *(pDummy++) = a;
    }
    ++iline;
    ++y;
  }
  fclose(infile);
  (void) jpeg_finish_decompress(&cinfo);
  jpeg_destroy_decompress(&cinfo);
  cout << " width =  " << width << endl;
  cout << " height = " << height << endl;

  hx->Write();
  hy->Write();
  h2->Write();
  f1->Close();

  return 0;
}

// ----------------------------------------------------------------------
int main(int argc, char* argv[]) {
  // -- command line arguments
  string file("fixme");
  for (int i = 0; i < argc; i++){
    if (!strcmp(argv[i], "-f"))   {file = argv[++i];}
  }
  cout << "jpegAna load file ->" << file << "<-" << endl;
  loadJpg(file.c_str());
          
  return 0; 
}
