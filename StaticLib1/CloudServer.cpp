#include "CloudServer.h"

#include <iostream>

#include <string>
#include <fstream>
#include <thread>

#include "TCPClient.h"
#include "TCPServer.h"
#include "TCPSSLServer.h"
#include "TCPSSLClient.h"

using namespace std;

#include <thread>

ASocket::LogFnCallback LogPrinter = [](const std::string& strLogMsg) { std::cout << strLogMsg << std::endl; };

//ASecureSocket::SSLSocket ConnectedClient; // socket of the connected client, we can have a vector of them for example.
											  /* blocking call, should return true if the accept is OK, ConnectedClient should also be a valid socket
											  number */
//CTCPSSLServer *m_pSSLTCPServer;

CloudServer::CloudServer(CallBack cur_callback) :
	recv_thread(&CloudServer::run, this),
	callback(cur_callback)
{
	this->local_port = "4211";

	this->connectedClient = (void*)new ASecureSocket::SSLSocket();

}

void CloudServer::run() {
	
	this->ssl_server = (void *)new CTCPSSLServer(LogPrinter, this->local_port);

	auto m_pSSLTCPServer = (CTCPSSLServer *)ssl_server;

	m_pSSLTCPServer->SetSSLCertFile(SSL_CERT_FILE);
	m_pSSLTCPServer->SetSSLKeyFile(SSL_KEY_FILE);

	m_pSSLTCPServer->Listen(*((ASecureSocket::SSLSocket *)this->connectedClient));
	
	char *sslRecvBuffer = new char[3686400];


	while (1) {

		int recvCnt = m_pSSLTCPServer->Receive(*((ASecureSocket::SSLSocket *)this->connectedClient), sslRecvBuffer, 3686400, false);

		/* Keep listening */
		if (recvCnt == 0) {
			m_pSSLTCPServer->Listen(*((ASecureSocket::SSLSocket *)this->connectedClient));
			continue;
		}

		Common_Header *pt_header = (Common_Header *)sslRecvBuffer;

		if (pt_header->cmdType == PACKET_TYPE_PIC) {

			Pic_RGB_Header *pt_header = (Pic_RGB_Header *)sslRecvBuffer;

			int pic_size = pt_header->width * pt_header->height * 3;

			int total_size = pic_size + sizeof(Pic_RGB_Header);

			char *pic_buffer = new char[total_size];

			int recvCnt = m_pSSLTCPServer->Receive(*((ASecureSocket::SSLSocket *)this->connectedClient), pic_buffer, total_size, true);

			/* Keep listening */
			if (recvCnt == 0) {
				m_pSSLTCPServer->Listen(*((ASecureSocket::SSLSocket *)this->connectedClient));
				continue;
			}

			Pic_RGB_Header *new_pt_header = (Pic_RGB_Header *)pic_buffer;

			Pic cur_pic(new_pt_header);
	
			this->callback(&cur_pic);

			delete[] pic_buffer;

		}
	}
	
}

void CloudServer::send_info(int timestampt, Individual info) {

	Individual_Info_Header header;
	auto m_pSSLTCPServer = (CTCPSSLServer *)ssl_server;

	header.header.cmdType = PACKET_TYPE_INDIVIDUAL_INFO;
	header.header.timestamp = timestampt;

	header.individual_info.id = info.id;
	header.individual_info.age = info.age;
	header.individual_info.gender = info.gender;
	header.individual_info.emotion = info.emotion;

	header.individual_info.name_len = info.name.length();

	const char *pname = info.name.c_str();

	std::copy(pname, pname + info.name.length(), header.individual_info.name);
	
	m_pSSLTCPServer->Send(*((ASecureSocket::SSLSocket *)this->connectedClient), (char*)&header, sizeof(Individual_Info_Header));

	printf("a");
}

CloudServer::~CloudServer()
{
	auto m_pSSLTCPServer = (CTCPSSLServer *)ssl_server;
	
	m_pSSLTCPServer->Disconnect(*((ASecureSocket::SSLSocket *)this->connectedClient));
	
	delete m_pSSLTCPServer;
}
