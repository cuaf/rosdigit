#ifndef DIGITRECOGNISER
#define DIGITRECOGNISER

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/calib3d/calib3d.hpp"

#include <cstdio>
#include <vector>
#include <algorithm> 

using namespace std;

class DigitRecogniser
{
public:
	DigitRecogniser();
	int recognise(cv::Mat);
private:
  	cv::Mat thDigit[10];
};

#endif 