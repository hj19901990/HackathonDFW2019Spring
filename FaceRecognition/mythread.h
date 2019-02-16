#pragma once
#include "qthread.h"
#include <QtCore/QCoreApplication>
#include <opencv2/opencv.hpp>  
#include "faceanalysis.h"
#include "facedetection.h"
#include <QMetaType>

using namespace std;

class CComunicationThread :public QThread 
{
	Q_OBJECT
public:
	void run();
	bool face_thread_free;
	cv::Mat recieved_img;
	CComunicationThread();
private:
	
signals:
	void send_img(cv::Mat input_img);
public slots:
	void recieve_thread_stat(bool is_free);
};


class CFaceRecognitionThread :public QThread
{
	Q_OBJECT
public:
	CFaceRecognitionThread();
	void run();
private:
	cv::Mat cv_img;
	matrix<dlib::rgb_pixel> dlib_img;
	frontal_face_detector detector;
	shape_predictor sp;
	anet_type net;
	cv::Ptr<SVM> svm;
	std::vector<rectangle> faces;
signals:
	void send_thread_stat(bool is_free);
public slots:
	void recieve_img(cv::Mat recieved_img);
};

