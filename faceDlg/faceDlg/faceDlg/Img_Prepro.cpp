#include "Img_Prepro.h"



Img_Prepro::Img_Prepro()
{
	
	
	// Load classifiers from "opencv/data/haarcascades" directory  
	//nestedCascade.load("../../haarcascade_eye_tree_eyeglasses.xml");

	// Change path before execution  
	cascade.load("Config\\haarcascade_frontalface_default.xml");
}
std::vector<rectangle> Img_Prepro::FaceDetectionDlib(cv::Mat src) {
	cv::Mat src_img = src.clone();
	//src = cv::imread("filtered.bmp");
	cv::cvtColor(src_img, src_img, CV_BGR2RGB);
	matrix<dlib::rgb_pixel> img;
	assign_image(img, cv_image<rgb_pixel>(src_img));


	frontal_face_detector detector = get_frontal_face_detector();
	


	//load_image(img, "D:\\HackDFW\\test.bmp");
	//pyramid_up(img);
	std::vector<rectangle> max_rect;
	std::vector<rectangle> vec_rst = detector(img);
	if (!vec_rst.size()) return max_rect;
	int max_area = 0;
	rectangle max_rec;
	for (rectangle rect : vec_rst) {
		if (max_area < rect.height()*rect.width()) {
			max_area = rect.height()*rect.width();
			max_rec = rect;
		}
			
	}
	max_rect.push_back(max_rec);
	return max_rect;
	//win.clear_overlay();
	//win.set_image(img);
	//win.add_overlay(dets, rgb_pixel(255, 0, 0));

	
}
//std::vector<cv::Rect> Img_Prepro::FaceDetection(cv::Mat& img, double scale) {
//	
//	std::vector<cv::Rect> faces;
//	cv::Mat gray;
//
//	cvtColor(img, gray, cv::COLOR_BGR2GRAY); // Convert to Gray Scale 
//	double fx = 1 / scale;
//
//	// Resize the Grayscale Image  
//	//resize(gray, smallImg, cv::Size(), fx, fx, cv::INTER_LINEAR);
//	equalizeHist(gray, gray);
//
//	// Detect faces of different sizes using cascade classifier  
//	cascade.detectMultiScale(gray, faces, 1.1,
//		2, 0 | cv::CASCADE_SCALE_IMAGE, cv::Size(30, 30));
//
//	// Draw circles around the faces 
//	
//	std::vector<cv::Rect> face_list;
//	if (!faces.size()) return face_list;
//
//	cv::Rect max_face = faces[0];
//	int max_area = faces[0].width*faces[0].height;
//
//	for (size_t i = 1; i < faces.size(); i++)
//	{
//		cv::Rect r = faces[i];
//		if (faces[i].width*faces[i].height > max_area)
//			max_face = r;
//		//cv::Mat smallImgROI;
//		//std::vector<cv::Rect> nestedObjects;
//		//cv::Point center;
//		cv::Scalar color = cv::Scalar(255, 0, 0); // Color for Drawing tool 
//		/*int radius;
//
//		double aspect_ratio = (double)r.width / r.height;
//		if (0.75 < aspect_ratio && aspect_ratio < 1.3)
//		{
//			center.x = cvRound((r.x + r.width*0.5)*scale);
//			center.y = cvRound((r.y + r.height*0.5)*scale);
//			radius = cvRound((r.width + r.height)*0.25*scale);
//			circle(img, center, radius, color, 3, 8, 0);
//		}
//		else*/
//			rectangle(img, cvPoint(cvRound(r.x*scale), cvRound(r.y*scale)),
//				cvPoint(cvRound((r.x + r.width - 1)*scale),
//					cvRound((r.y + r.height - 1)*scale)), color, 3, 8, 0);
//		
//	}
//	face_list.push_back(max_face);
//	imshow("Face Detection", img);
//	cv::waitKey(1);
//	//cv::imwrite("filtered.bmp", img);
//	return face_list;
//	// Show Processed Image with detected faces 
//	//if(faces.size())
//	
//		
//}
