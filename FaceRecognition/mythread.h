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


class CFaceRecognition :public QThread
{
	Q_OBJECT
public:
	void run();
	
private:

signals:

public slots:

};

