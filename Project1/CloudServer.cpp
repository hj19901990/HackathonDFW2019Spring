#include "CloudServer.h"

#include <iostream>

#include <string>
#include <fstream>
#include <thread>

using namespace std;

#include <thread>

CloudServer::CloudServer(void(*set_callback)(Pic *)) :
	LogPrinter([](const std::string& strLogMsg) { std::cout << strLogMsg << std::endl; }),
	recv_thread(&CloudServer::run, this),
	callback(set_callback)
{
	this->local_port = "4211";

}

void CloudServer::run() {
	
	this->m_pSSLTCPServer = new CTCPSSLServer(LogPrinter, this->local_port);

	this->m_pSSLTCPServer->SetSSLCertFile(SSL_CERT_FILE);
	this->m_pSSLTCPServer->SetSSLKeyFile(SSL_KEY_FILE);

	this->m_pSSLTCPServer->Listen(ConnectedClient);

	char *sslRecvBuffer = new char[3686400];


	while (1) {

		int recvCnt = this->m_pSSLTCPServer->Receive(ConnectedClient, sslRecvBuffer, 3686400, false);

		/* Keep listening */
		if (recvCnt == 0) {
			this->m_pSSLTCPServer->Listen(ConnectedClient);
			continue;
		}

		Common_Header *pt_header = (Common_Header *)sslRecvBuffer;

		if (pt_header->cmdType == PACKET_TYPE_PIC) {

			Pic_RGB_Header *pt_header = (Pic_RGB_Header *)sslRecvBuffer;

			int pic_size = pt_header->width * pt_header->height * 3;

			int total_size = pic_size + sizeof(Pic_RGB_Header) - 1;

			char *pic_buffer = new char[total_size];

			int recvCnt = this->m_pSSLTCPServer->Receive(ConnectedClient, pic_buffer, total_size, true);

			/* Keep listening */
			if (recvCnt == 0) {
				this->m_pSSLTCPServer->Listen(ConnectedClient);
				continue;
			}

			Pic_RGB_Header *new_pt_header = (Pic_RGB_Header *)pic_buffer;

			Pic cur_pic(new_pt_header);
	
			this->callback(&cur_pic);

		}
	}
	
}

CloudServer::~CloudServer()
{
	
	this->m_pSSLTCPServer->Disconnect(ConnectedClient);
	
	delete this->m_pSSLTCPServer;
}
