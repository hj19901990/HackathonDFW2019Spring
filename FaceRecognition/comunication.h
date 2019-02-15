#pragma once
#include "qthread.h"

class CComunicationThread :public QThread 
{
	Q_OBJECT
public:
	void run();
private:

signals:

public slots:

};