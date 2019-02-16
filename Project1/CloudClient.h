#pragma once

#include "TCPClient.h"
#include "TCPServer.h"
#include "TCPSSLServer.h"
#include "TCPSSLClient.h"
#include <string>
#include <map>

#include "Protocols.h"

class CloudClient
{
private:

	std::string server_ip;

	std::string server_port;

	ASocket::LogFnCallback LogPrinter;

	CTCPSSLClient *pt_sslclient;

	std::map<int, Individual> individuals;

	std::thread recv_thread;

	void cmd_process();

	void reconnect();

	void Send(const char* pData, const size_t uSize);

public:
	CloudClient();

	bool send_pic(int w, int h, char *buf);

	Individual get_info(int timestamp);

	~CloudClient();
};

