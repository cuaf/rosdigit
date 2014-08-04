#include "DigitRecogniser.h"

DigitRecogniser::DigitRecogniser()
{
    for(int i = 0; i < 10; ++i) {
        char buff[50];
    	sprintf(buff, "src/rosdigit/digits/%d.PNG", i);
    	thDigit[i] = cv::imread(buff, CV_LOAD_IMAGE_GRAYSCALE);
        cv::blur(thDigit[i], thDigit[i], cv::Size(4,4));
        cv::dilate(thDigit[i], thDigit[i], cv::Mat(), cv::Point(-1, -1), 6);
        cv::threshold(thDigit[i], thDigit[i], 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
    }
}

#include <iostream>

int DigitRecogniser::recognise(cv::Mat image)
{
    cv::resize(image, image, cv::Size(768, 432));
    cv::Mat hsv, bin[2], finbin;

    cv::cvtColor(image, hsv, CV_BGR2HSV);

    // Extract low and high hue values - they correspond to shades of red
    // Adjust saturation and value to allow darker shades and whiter tints appear
    cv::inRange(hsv, cv::Scalar(0, 50, 100), cv::Scalar(17, 255, 255), bin[0]);
    cv::inRange(hsv, cv::Scalar(175, 50, 100), cv::Scalar(180, 255, 255), bin[1]);
    cv::bitwise_or(bin[0], bin[1], finbin);

    // Trick - blur & dilate & threshold binary image again
    // Small details will grey out and vanish
    // Dilate a lot - crucial for contour finding as we want to get rid of 
    // separation between segments
    cv::blur(finbin, finbin, cv::Size(4,4));
    cv::dilate(finbin, finbin, cv::Mat(), cv::Point(-1, -1), 5); // Dilate 5 times
    cv::threshold(finbin, finbin, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);

    // Now we can extract digit contours - should be one big contour
    // (dilate more if not)
    std::vector<std::vector<cv::Point> > contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(finbin.clone(), contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));
    if(contours.empty())
        return -1;

    /// Find the rotated rectangles for each contour
    std::vector<cv::RotatedRect> minRect( contours.size() );

    int finalRect = 0;
    for(int i = 0; i < contours.size(); i++ ) {
        minRect[i] = cv::minAreaRect(cv::Mat(contours[i]));
        if (minRect[i].angle < -45.0) {
            minRect[i].angle += 90.0;
            swap(minRect[i].size.width, minRect[i].size.height);
        }
        float hwRatio = (float) minRect[i].size.height / minRect[i].size.width;
        if (minRect[i].size.area() > minRect[finalRect].size.area() &&
            (hwRatio > 1.2 && hwRatio < 3.2 || hwRatio > 4.0)) {
            finalRect = i;
        }
    }


    // Got rectangle with the digit. Now, extract image data (de-rotate)
    cv::RotatedRect rect = minRect[finalRect];
    cv::Mat rotMtrx, rotated, final;
    rotMtrx = cv::getRotationMatrix2D(rect.center, rect.angle, 1.0);
    cv::warpAffine(finbin, rotated, rotMtrx, finbin.size(), cv::INTER_CUBIC);
    cv::getRectSubPix(rotated, rect.size, rect.center, final);
    cv::resize(final, final, cv::Size(61, 117)); // Samples' size

    // Determine digit by pattern matching, 1 being special case
    double min = 1e40;
    int digit = -1;
    for(int dig = 0; dig < 10; ++dig) {
        cv::Mat match;
        cv::matchTemplate(final, thDigit[dig], match, CV_TM_SQDIFF_NORMED);
        if(match.at<double>(0, 0) < min) {
            min = match.at<double>(0, 0);
            digit = dig;
        }
    }

    float hwRatio = (float) rect.size.height / rect.size.width;
    if(digit == 8 && hwRatio > 4.0) digit = 1;

    return digit;
}