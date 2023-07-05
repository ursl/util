#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include <iostream>
#include <string>

using namespace cv;
using namespace std;

int main(int argc, char* argv[]) {
  string file("fixme");
  for (int i = 0; i < argc; i++){
    if (!strcmp(argv[i], "-f"))   {file = argv[++i];}
  }

  std::string image_path = samples::findFile(file);
  Mat img = imread(image_path, IMREAD_COLOR);
  if (img.empty()) {
    std::cout << "Could not read the image: " << image_path << std::endl;
    return 1;
  }
  imshow("Display window", img);
  int k = waitKey(0); // Wait for a keystroke in the window
  if(k == 's') {
    imwrite("starry_night.png", img);
  }
  return 0;
}
