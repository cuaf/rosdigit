#ifndef FRAMEPROCESSOR
#define FRAMEPROCESSOR

#include <ros/ros.h>

#include <sensor_msgs/Image.h>
#include <sensor_msgs/CameraInfo.h>
#include <sensor_msgs/image_encodings.h>

#include <cv_bridge/cv_bridge.h>

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include "DigitRecogniser.h"

class FrameProcessor
{
public:
	FrameProcessor(int argc, char **argv, DigitRecogniser *rec);
	void spin();

private:
	void imageCallback(const sensor_msgs::ImageConstPtr& msg);

	ros::Subscriber subImage;
	DigitRecogniser *recogniser;
	int frameNo, final, skipper;
};

#endif