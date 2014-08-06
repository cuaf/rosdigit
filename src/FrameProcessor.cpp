#include "FrameProcessor.h"

FrameProcessor::FrameProcessor(int argc, char **argv, DigitRecogniser *rec, int skipFrames, int averageOver):
	skipFrames(skipFrames), averageOver(averageOver), frameCounter(0), prevResult(0)
{
    ros::init(argc, argv, "rosdigit");
    ros::NodeHandle n;
    subImage = n.subscribe("ardrone/image_raw", 1, &FrameProcessor::imageCallback, this);

    this->recogniser = rec;
    std::fill(this->results, this->results + 10, 0);
}

void FrameProcessor::spin()
{
    while(ros::ok()) {
        cv::waitKey(1); //Allows GUI events to be proccessed
        ros::spinOnce();
    }

    ros::spin();
}

void FrameProcessor::outputResult()
{
    int result = -1, freq = 0;
    for(int i = 0; i < 10; ++i) {
        if(this->results[i] > freq) {
            freq = this->results[i];
            result = i;
        } 
    }

    //if(result == prevResult) return;
    prevResult = result;
    printf("%d\n", result);

    std::fill(this->results, this->results + 10, 0);
}


void FrameProcessor::imageCallback(const sensor_msgs::ImageConstPtr& msg)
{
    ++frameCounter;
    // Process every (skipFrames + 1)-th frame
    if(frameCounter % (skipFrames + 1) != 0) return;

    //Convert to OpenCV image
    cv_bridge::CvImagePtr cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
    cv::Mat image = cv_ptr->image;
    //Create a seperate image for dipslay purposes.
    cv::Mat displayImage;
    image.copyTo(displayImage);

    int result = recogniser->recognise(displayImage);
    if(result != -1) ++this->results[result];

    // Have we processed 'averageOver' frames? 
    if(frameCounter == (skipFrames + 1) * averageOver) {
        this->outputResult();
        frameCounter = 0;
    }
}