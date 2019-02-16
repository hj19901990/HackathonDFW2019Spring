#include "mythread.h"

CComunicationThread::CComunicationThread()
{
	face_thread_free = true;
	
}

void CComunicationThread::run()
{
	recieved_img = cv::imread("..\\TestResources\\woman.bmp");
	emit send_img(recieved_img);
}

void CComunicationThread::recieve_thread_stat(bool is_free)
{
	face_thread_free = is_free;
}





CFaceRecognitionThread::CFaceRecognitionThread()
{
	detector = get_frontal_face_detector();
	deserialize("..\\shape_predictor_68_face_landmarks.dat") >> sp;
	deserialize("..\\dlib_face_recognition_resnet_model_v1.dat") >> net;
	svm = StatModel::load<SVM>("..\\SVM_DATA.xml");
	qRegisterMetaType<cv::Mat>("cv::Mat");
	
}

void CFaceRecognitionThread::run()
{
	if (face_detection(dlib_img, detector, faces) == XSUCESS)
	{
		face_recognition(dlib_img, sp, net, faces);
	}
	else
	{
		send_thread_stat(true);
	}
}

void CFaceRecognitionThread::recieve_img(cv::Mat recieved_img)
{
	cv_img = recieved_img.clone();
	cv::cvtColor(cv_img, cv_img, CV_BGR2RGB);
	assign_image(dlib_img, cv_image<rgb_pixel>(cv_img));
	pyramid_up(dlib_img);
	this->start();
}