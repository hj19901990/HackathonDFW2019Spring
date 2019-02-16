#include "CloudClient.h"

#include <thread>


CloudClient::CloudClient() : 
	LogPrinter([](const std::string& strLogMsg) { std::cout << strLogMsg << std::endl;})
{
	server_ip = "127.0.0.1";
	server_port = "4211";

	pt_sslclient = new CTCPSSLClient(LogPrinter);

	reconnect();

	this->recv_thread = std::thread(&CloudClient::cmd_process, this);
}

void CloudClient::reconnect() {

	while (true != pt_sslclient->Connect(server_ip, server_port)) {

		std::cout << "Reconnecting" << std::endl;

	}

	std::cout << "Connected!" << std::endl;
}



void CloudClient::cmd_process() {

	char sslRecvBuffer[128] = {};
	
	while (1) {

		while (false == pt_sslclient->Receive(sslRecvBuffer, 128, false)) {
			
			reconnect();
		}

		Common_Header *pt_header = (Common_Header *)sslRecvBuffer;

		if (pt_header->cmdType == PACKET_TYPE_INDIVIDUAL_INFO) {

			Individual individual((Individual_Info_Header*)pt_header);

			individuals[pt_header->timestamp] = individual;
		}
		
	}
}

Individual CloudClient::get_info(int timestamp) {

	if (individuals.find(timestamp) == individuals.end()) {

		//Not Found
		return Individual();
	}

	return individuals[timestamp];
}

bool CloudClient::send_pic(int w, int h, char *buf) {

	int pic_size = w * h * 3;

	int total_size = pic_size + sizeof(Pic_RGB_Header) - 1;

	char *pic_buffer = new char[total_size];

	Pic_RGB_Header *new_pt_header = (Pic_RGB_Header *)pic_buffer;

	new_pt_header->header.cmdType = PACKET_TYPE_PIC;

	new_pt_header->header.timestamp = 1;

	new_pt_header->width = w;

	new_pt_header->height = h;

	std::strncpy((char *)&(new_pt_header->buf), buf, pic_size);

	if (false == pt_sslclient->Send((char *)new_pt_header, sizeof(Pic_RGB_Header))) {
		return false;
	}

	int i = 100000;
	while (i--);

	if (false == pt_sslclient->Send((char *)new_pt_header, total_size)) {
		return false;
	}

	delete pic_buffer;

	return true;
}

CloudClient::~CloudClient()
{
	pt_sslclient->Disconnect();

	delete pt_sslclient;
}
