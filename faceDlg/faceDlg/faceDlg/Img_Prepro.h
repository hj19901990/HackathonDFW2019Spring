#pragma once
//INCLUDES OPENCV
#include <opencv/cxcore.h>
#include <opencv/highgui.h>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>


#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/gui_widgets.h>
#include <dlib/image_io.h>
#include <dlib/opencv.h>
#include <dlib/dnn.h>
#include <dlib/clustering.h>
#include <dlib/string.h>
#include <dlib/image_io.h>
#include <algorithm>

//using namespace cv;
using namespace dlib;
class Img_Prepro
{
public:
	Img_Prepro();
	//std::vector<cv::Rect>  FaceDetection(cv::Mat& img, double scale=1);
	std::vector<rectangle> FaceDetectionDlib(cv::Mat src);
private:
	cv::CascadeClassifier cascade;
};

