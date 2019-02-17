#include "mythread.h"


void CComunicationThread::callback(Pic *pic) {
	timestamp = pic->timestamp;
	if (recieved_img.empty()) {
		recieved_img = cv::Mat(pic->height, pic->width, CV_8UC3);
		std::memcpy(recieved_img.data, pic->buffer, pic->height*pic->width);
		res_package.id = timestamp;
	}


	//res_package->id= pic->timestamp;
}

CComunicationThread::CComunicationThread() :
	cloudserver(std::bind(&CComunicationThread::callback, this, std::placeholders::_1))
{
	face_thread_free = true;

	
}

void CComunicationThread::run()
{
	//recieved_img = cv::imread("..\\TestResources\\woman.bmp");
	while (1)
	{
		if (face_thread_free == false) continue;
		else
		{
			if (!recieved_img.empty())
			{
				emit send_img(recieved_img, timestamp);
				recieved_img.release();
				
				face_thread_free = false;
			}
			
		}
		
	}
	
}

void CComunicationThread::recieve_thread_stat(bool is_free,Individual package)
{
	face_thread_free = is_free;
	res_package = package;
}





CFaceRecognitionThread::CFaceRecognitionThread()
{
	detector = get_frontal_face_detector();
	deserialize("..\\shape_predictor_68_face_landmarks.dat") >> sp;
	deserialize("..\\dlib_face_recognition_resnet_model_v1.dat") >> net;
	svm = StatModel::load<SVM>("..\\SVM_DATA.xml");
	qRegisterMetaType<cv::Mat>("cv::Mat");
	flag = false;
}

void CFaceRecognitionThread::run()
{
	while (1)
	{
		if (flag == false) continue;
		//Individual detect_res;
		if (face_detection(dlib_img, detector, faces) == XSUCESS)
		{

			face_recognition(dlib_img, sp, net, faces);
			face_emotion(dlib_img, cv_img, sp, svm, faces);
			send_thread_stat(true, detect_res);
			flag = false;
		}
		
	}
	

}

void CFaceRecognitionThread::recieve_img(cv::Mat recieved_img, int timestamp)
{
	cv_img = recieved_img.clone();
	//cv::cvtColor(cv_img, cv_img, CV_BGR2RGB);
	detect_res.id= timestamp;
	assign_image(dlib_img, cv_image<rgb_pixel>(cv_img));
	pyramid_up(dlib_img);
	flag = true;
	/*this->start();*/
}