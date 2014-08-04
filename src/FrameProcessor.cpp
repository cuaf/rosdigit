#include "FrameProcessor.h"

FrameProcessor::FrameProcessor(int argc, char **argv, DigitRecogniser *rec):
	frameNo(0), final(0), skipper(0)
{
    ros::init(argc, argv, "rosdigit");
    ros::NodeHandle n;
    subImage = n.subscribe("ardrone/image_raw", 1, &FrameProcessor::imageCallback, this);

    this->recogniser = rec;
}

void FrameProcessor::spin()
{
    while(ros::ok()) {
        cv::waitKey(1); //Allows GUI events to be proccessed
        ros::spinOnce();
    }

    ros::spin();
}


void FrameProcessor::imageCallback(const sensor_msgs::ImageConstPtr& msg)
{   
    ++skipper;
    //if(skipper < 2) return;
    skipper = 0;

    //Convert to OpenCV image
    cv_bridge::CvImagePtr cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
    cv::Mat image = cv_ptr->image;
    //Create a seperate image for dipslay purposes.
    cv::Mat displayImage;
    image.copyTo(displayImage);
    //cv::cvtColor(displayImage, displayImage, CV_BGR2GRAY);
    printf("%d\n", recogniser->recognise(displayImage));
    
}