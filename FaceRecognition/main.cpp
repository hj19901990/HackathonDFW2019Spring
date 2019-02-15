#include <QtCore/QCoreApplication>
#include "facedetection.h"
#include "faceanalysis.h"


int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);
	emotion_detection();
	//face_detection();
	//face_recognition();
	return a.exec();
}
