#include "faceanalysis.h"


void emotion_detection()
{
	frontal_face_detector detector = get_frontal_face_detector();
	shape_predictor pose_model;
	deserialize("..\\shape_predictor_68_face_landmarks.dat") >> pose_model;


	cv::Ptr<SVM> svm = StatModel::load<SVM>("..\\SVM_DATA.xml");

	//cv::Mat rst= cv::imread("..\\TestResources\\kid.jpg");

	cv::Mat rst = cv::imread("..\\TestResources\\kid.jpg");
	//cv::imshow("test", cv_img);
	//cv::cvtColor(rst, rst, CV_BGR2RGB);
	matrix<dlib::rgb_pixel> img;
	assign_image(img, cv_image<rgb_pixel>(rst));



	pyramid_up(img);


	//cv_image<bgr_pixel> cimg(rst);
	// Detect faces   
	
	//std::vector<rectangle> faces = detector(cimg);
	std::vector<rectangle> faces = detector(img);
	//matrix<rgb_pixel> faces = detector(cimg);;
	// Find the pose of each face.  
	std::vector<full_object_detection> shapes;
	for (unsigned long i = 0; i < faces.size(); ++i)
		shapes.push_back(pose_model(img, faces[i]));

	if (!shapes.empty()) {
		float testData[1][136];
		float coefficient = -(faces[0].top() - faces[0].bottom()) / 300.0;
		for (int i = 0; i < 68; i++) {
			circle(rst, cvPoint(shapes[0].part(i).x(), shapes[0].part(i).y()), 2, cv::Scalar(255, 0, 0), -1);
			testData[0][i * 2] = (shapes[0].part(i).x() - faces[0].left()) / coefficient;
			testData[0][i * 2 + 1] = (shapes[0].part(i).y() - faces[0].top()) / coefficient;
			//  shapes[0].part(i).x();//68个  
		}
		cv::Mat result;

		cv::Mat query(1, 136, CV_32FC1, testData);

		if (svm->predict(query) == 250) {
			cv::putText(rst, "Happy", cv::Point(20, 60), 3, 2, cvScalar(0, 0, 255));
			cout << "happy" << endl;
		}

		if (svm->predict(query) == 170) {
			cv::putText(rst, "Calm", cv::Point(20, 60), 3, 2, cvScalar(0, 0, 255));
			cout << "calm" << endl;
		}
		if (svm->predict(query) == 300) {
			cv::putText(rst, "Upset", cv::Point(20, 60), 3, 2, cvScalar(0, 0, 255));
			cout << "upset" << endl;
		}

	}
	//Display it all on the screen  
	imshow("emotion detection", rst);
}


//CFaceAnalysis::CFaceAnalysis()              //initial different classify models
//{
//	detector = get_frontal_face_detector();    
//	deserialize("..\\shape_predictor_68_face_landmarks.dat") >> sp;
//	deserialize("..\\dlib_face_recognition_resnet_model_v1.dat") >> net;
//	svm = StatModel::load<SVM>("..\\SVM_DATA.xml");                          
//}
//
//int CFaceAnalysis::face_detection(cv::Mat src)
//{
//	matrix<dlib::rgb_pixel> img;
//	assign_image(img, cv_image<rgb_pixel>(src));   //convert cv format to dlib format
//	pyramid_up(img);                    //eliminate the size influences
//	std::vector<matrix<rgb_pixel>> faces;
//	for (auto face : detector(img))
//	{
//		auto shape = sp(img, face);
//		matrix<rgb_pixel> face_chip;
//		extract_image_chip(img, get_face_chip_details(shape, 150, 0.25), face_chip);
//		faces.push_back(move(face_chip));
//	}
//	if (faces.size() != 0)
//	{
//		
//		return XSUCESS;
//	}
//	else
//	{
//		return XFAIL;
//	}
//}
//
//
//int CFaceAnalysis::face_analysis()
//{
//	return 0;
//}