#include "faceanalysis.h"


void emotion_detection()
{
	frontal_face_detector detector = get_frontal_face_detector();
	shape_predictor pose_model;
	deserialize("D:\\HackDFW\\FaceRecognition\\shape_predictor_68_face_landmarks.dat") >> pose_model;


	cv::Ptr<SVM> svm = StatModel::load<SVM>("D:\\HackDFW\\FaceRecognition\\SVM_DATA.xml");

	cv::Mat rst= cv::imread("D:\\HackDFW\\kid.jpg");

	//pyramid_up(rst);
	cv_image<bgr_pixel> cimg(rst);
	// Detect faces   
	
	std::vector<rectangle> faces = detector(cimg);
	// Find the pose of each face.  
	std::vector<full_object_detection> shapes;
	for (unsigned long i = 0; i < faces.size(); ++i)
		shapes.push_back(pose_model(cimg, faces[i]));

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
