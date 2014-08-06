#include "FrameProcessor.h"
#include "DigitRecogniser.h"


int main(int argc, char **argv)
{   
    DigitRecogniser *rec = new DigitRecogniser();

    FrameProcessor *fproc = new FrameProcessor(argc, argv, rec);
    fproc->spin();
}
