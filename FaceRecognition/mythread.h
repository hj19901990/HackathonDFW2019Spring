#pragma once
#include "qthread.h"
#include <opencv2/opencv.hpp>  
#include "faceanalysis.h"

using namespace std;

class CComunicationThread :public QThread 
{
	Q_OBJECT
public:
	void run();
	cv::Mat recieved_img;
private:

signals:
	
public slots:

};


class CFaceRecognitionThread :public QThread
{
	Q_OBJECT
public:
	CFaceRecognitionThread();
	void run();
	
private:
	cv::Mat img_to_proc;
signals:

public slots:

};

