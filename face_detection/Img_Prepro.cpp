#include "Img_Prepro.h"



Img_Prepro::Img_Prepro()
{
	
	
	// Load classifiers from "opencv/data/haarcascades" directory  
	//nestedCascade.load("../../haarcascade_eye_tree_eyeglasses.xml");

	// Change path before execution  
	cascade.load("Config\\haarcascade_frontalface_default.xml");
}

std::vector<cv::Rect> Img_Prepro::FaceDetection(cv::Mat& img, double scale) {
	
	std::vector<cv::Rect> faces;
	cv::Mat gray;

	cvtColor(img, gray, cv::COLOR_BGR2GRAY); // Convert to Gray Scale 
	double fx = 1 / scale;

	// Resize the Grayscale Image  
	//resize(gray, smallImg, cv::Size(), fx, fx, cv::INTER_LINEAR);
	equalizeHist(gray, gray);

	// Detect faces of different sizes using cascade classifier  
	cascade.detectMultiScale(gray, faces, 1.1,
		2, 0 | cv::CASCADE_SCALE_IMAGE, cv::Size(30, 30));

	// Draw circles around the faces 
	
	std::vector<cv::Rect> face_list;
	if (!faces.size()) return face_list;

	cv::Rect max_face = faces[0];
	int max_area = faces[0].width*faces[0].height;

	for (size_t i = 1; i < faces.size(); i++)
	{
		cv::Rect r = faces[i];
		if (faces[i].width*faces[i].height > max_area)
			max_face = r;
		//cv::Mat smallImgROI;
		//std::vector<cv::Rect> nestedObjects;
		//cv::Point center;
		//cv::Scalar color = cv::Scalar(255, 0, 0); // Color for Drawing tool 
		/*int radius;

		double aspect_ratio = (double)r.width / r.height;
		if (0.75 < aspect_ratio && aspect_ratio < 1.3)
		{
			center.x = cvRound((r.x + r.width*0.5)*scale);
			center.y = cvRound((r.y + r.height*0.5)*scale);
			radius = cvRound((r.width + r.height)*0.25*scale);
			circle(img, center, radius, color, 3, 8, 0);
		}
		else*/
			//rectangle(gray, cvPoint(cvRound(r.x*scale), cvRound(r.y*scale)),
			//	cvPoint(cvRound((r.x + r.width - 1)*scale),
			//		cvRound((r.y + r.height - 1)*scale)), color, 3, 8, 0);
		
	}
	face_list.push_back(max_face);
	return face_list;
	// Show Processed Image with detected faces 
	//if(faces.size())
	//	cv::imwrite("filtered.bmp", gray);
		//imshow("Face Detection", gray);
}
