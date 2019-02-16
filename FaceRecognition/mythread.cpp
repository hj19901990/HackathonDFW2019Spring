#include "mythread.h"

void CComunicationThread::run()
{

}






CFaceRecognitionThread::CFaceRecognitionThread()
{
	
}

void CFaceRecognitionThread::run()
{
	CFaceAnalysis *face_to_analysis = new CFaceAnalysis();
	face_to_analysis->face_detection(this->img_to_proc);
}