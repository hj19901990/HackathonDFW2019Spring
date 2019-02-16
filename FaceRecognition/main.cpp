#include <QtCore/QCoreApplication>
#include "mythread.h"
#include <QObject>


int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);
	//emotion_detection();
	//face_detection();
	//face_recognition();

	CComunicationThread *comunication = new CComunicationThread();
	CFaceRecognitionThread *recognition = new CFaceRecognitionThread();
	QObject::connect(comunication, &CComunicationThread::send_img, recognition, &CFaceRecognitionThread::recieve_img);
	comunication->start();
	return a.exec();
}
