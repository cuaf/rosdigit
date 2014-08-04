#include "FrameProcessor.h"
#include "DigitRecogniser.h"


int main(int argc, char **argv)
{   
    DigitRecogniser *rec = new DigitRecogniser();

    /*cv::Mat image = cv::imread("8.jpg", CV_LOAD_IMAGE_COLOR);
    imshow("Original!", image);
    std::cout << rec->recognise(image) << std::endl;
    cv::waitKey(0);*/

    FrameProcessor *fproc = new FrameProcessor(argc, argv, rec);
    fproc->spin();
}
