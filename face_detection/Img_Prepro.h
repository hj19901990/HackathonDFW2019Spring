#pragma once
//INCLUDES OPENCV
#include <opencv/cxcore.h>
#include <opencv/highgui.h>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>


//using namespace cv;

class Img_Prepro
{
public:
	Img_Prepro();
	std::vector<cv::Rect>  FaceDetection(cv::Mat& img, double scale=1);
private:
	cv::CascadeClassifier cascade;
};

