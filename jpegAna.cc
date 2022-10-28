#include <iostream>
#include <jpeglib.h>

#include "TFile.h"
#include "TH1D.h"

using namespace std;

// ----------------------------------------------------------------------
int loadJpg(const char* Name) {
  unsigned char a, r, g, b, l;
  int width, height;
  struct jpeg_decompress_struct cinfo;
  struct jpeg_error_mgr jerr;

  TFile *f1 = TFile::Open("test.root", "RECREATE");
  TH1D *h1 = new TH1D("h1", "h1", 4000, 0., 4000.);
  
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

  unsigned char * pDummy = new unsigned char [width*height*4];
  unsigned char * pTest = pDummy;
  if (!pDummy) {
    printf("NO MEM FOR JPEG CONVERT!\n");
    return 0;
  }
  row_stride = width * cinfo.output_components;
  pJpegBuffer = (*cinfo.mem->alloc_sarray)((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);

  int iline(0);
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
      if (iline == 2000) {
        h1->Fill(x, l);
      }
      
      *(pDummy++) = b;
      *(pDummy++) = g;
      *(pDummy++) = r;
      *(pDummy++) = a;
    }
    ++iline;
  }
  fclose(infile);
  (void) jpeg_finish_decompress(&cinfo);
  jpeg_destroy_decompress(&cinfo);
  cout << " width =  " << width << endl;
  cout << " height = " << height << endl;

  h1->Write();
  f1->Close();

  return 0;
}

int main() {

  cout << "hallo" << endl;
  loadJpg("/Users/ursl/glass-20220401-5.jpg");
          
  return 0; 
}
