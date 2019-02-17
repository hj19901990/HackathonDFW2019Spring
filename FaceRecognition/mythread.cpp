#include "mythread.h"


void CComunicationThread::callback(Pic *pic) {
	
	if (recieved_img.empty()) {
		recieved_img = cv::Mat(pic->height, pic->width, CV_8UC3);
		
		std::memcpy(recieved_img.data, pic->buffer, pic->height*pic->width*3);
		timestamp = pic->timestamp;
		//cv::imwrite("D:\\img.bmp", recieved_img);

		//res_package.id = timestamp;
	}


	//res_package->id= pic->timestamp;
}

CComunicationThread::CComunicationThread() :
	cloudserver(std::bind(&CComunicationThread::callback, this, std::placeholders::_1))
{
	face_thread_free = true;
	qRegisterMetaType<Individual_Info>("Individual_Info");
	
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

void CComunicationThread::recieve_thread_stat(bool is_free, Individual_Info package)
{
	face_thread_free = is_free;
	res_package = package;
	sendresult();
}

void CComunicationThread::sendresult()
{
	Individual_Info_Header package_header;
	package_header.individual_info = res_package;
	package_header.header.timestamp = timestamp;
	Individual send_pack(&package_header);
	cloudserver.send_info(timestamp, send_pack);
}



CFaceRecognitionThread::CFaceRecognitionThread()
{
	detector = get_frontal_face_detector();
	deserialize("..\\shape_predictor_68_face_landmarks.dat") >> sp;
	deserialize("..\\dlib_face_recognition_resnet_model_v1.dat") >> net;
	svm = StatModel::load<SVM>("..\\SVM_DATA.xml");
	qRegisterMetaType<cv::Mat>("cv::Mat");
	flag = false;   //prevent vacant
}

void CFaceRecognitionThread::run()
{
	while (1)
	{
		if (flag == false) continue;
		//Individual detect_res;
		if (face_detection(dlib_img, detector, faces) == XSUCESS)
		{

			detect_res.id=face_recognition(dlib_img, sp, net, faces,detect_res);
			detect_res.emotion=face_emotion(dlib_img, cv_img, sp, svm, faces);
			
			
		}
		flag = false;
		emit send_thread_stat(true, detect_res);
	}
	

}

void CFaceRecognitionThread::recieve_img(cv::Mat recieved_img, int timestamp)
{
	cv_img = recieved_img.clone();
	//cv::cvtColor(cv_img, cv_img, CV_BGR2RGB);
	cv::imwrite("D:\\jinfeng.bmp", cv_img);
	//detect_res.id= timestamp;
	assign_image(dlib_img, cv_image<rgb_pixel>(cv_img));
	pyramid_up(dlib_img);
	flag = true;
	/*this->start();*/
}