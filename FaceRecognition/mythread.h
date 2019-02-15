#pragma once
#include "qthread.h"
#include <opencv2/opencv.hpp>  

class CComunicationThread :public QThread 
{
	Q_OBJECT
public:
	void run();
private:

signals:

public slots:

};


class CFaceRecognition :public QThread
{

};