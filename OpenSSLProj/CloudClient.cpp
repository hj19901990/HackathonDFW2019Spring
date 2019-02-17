#include "CloudClient.h"

#include <thread>
#include "TCPClient.h"
#include "TCPServer.h"
#include "TCPSSLServer.h"
#include "TCPSSLClient.h"
#include <mutex>

ASocket::LogFnCallback LogPrinter = [](const std::string& strLogMsg) { std::cout << strLogMsg << std::endl; };

CloudClient::CloudClient(std::string ip, std::string port, CallBack cur_callback):
	callback(cur_callback)
{
	server_ip = ip;
	server_port = port;

	client = (void *)(new CTCPSSLClient(LogPrinter));

	reconnect();

	this->p_recv_thread = (void *)(new std::thread(&CloudClient::cmd_process, this));
}

void CloudClient::reconnect() {

	CTCPSSLClient *pt_sslclient = (CTCPSSLClient *)client;

	while (true != pt_sslclient->Connect(server_ip, server_port)) {

		std::cout << "Reconnecting" << std::endl;

	}

	std::cout << "Connected!" << std::endl;
}



void CloudClient::cmd_process() {

	char sslRecvBuffer[128000] = {};

	CTCPSSLClient *pt_sslclient = (CTCPSSLClient *)client;
	
	while (1) {

		while (false == pt_sslclient->Receive(sslRecvBuffer, 128000, false)) {
			
			reconnect();
		}

		Common_Header *pt_header = (Common_Header *)sslRecvBuffer;

		if (pt_header->cmdType == PACKET_TYPE_INDIVIDUAL_INFO) {

			Individual indi((Individual_Info_Header*)pt_header);

			this->callback(&indi);
		}
		
	}
}

Individual CloudClient::get_info(int timestamp) {

	if (individuals.find(timestamp) == individuals.end()) {

		//Not Found
		return Individual();
	}

	Individual returnVal = individuals[timestamp];

	return returnVal;
}

int CloudClient::send_pic(int w, int h, char *buf) {

	CTCPSSLClient *pt_sslclient = (CTCPSSLClient *)client;
	
	time_t cur_time = time(NULL);

	int time_stamp = (uintmax_t)cur_time/10000;

	int pic_size = w * h * 3;

	int total_size = pic_size + sizeof(Pic_RGB_Header);

	char *pic_buffer = new char[total_size];

	Pic_RGB_Header *new_pt_header = (Pic_RGB_Header *)pic_buffer;

	new_pt_header->header.cmdType = PACKET_TYPE_PIC;

	new_pt_header->header.timestamp = time_stamp;

	new_pt_header->width = w;

	new_pt_header->height = h;

	memcpy(((char *)new_pt_header + sizeof(Pic_RGB_Header)), buf, pic_size);

	if (false == pt_sslclient->Send((char *)new_pt_header, sizeof(Pic_RGB_Header))) {
		return false;
	}

	int i = 100000;
	while (i--);

	if (false == pt_sslclient->Send((char *)new_pt_header, total_size)) {
		return false;
	}

	delete[] pic_buffer;

	return time_stamp;
}

CloudClient::~CloudClient()
{
	CTCPSSLClient *pt_sslclient = (CTCPSSLClient *)client;

	pt_sslclient->Disconnect();

	delete pt_sslclient;

	delete p_recv_thread;
}
