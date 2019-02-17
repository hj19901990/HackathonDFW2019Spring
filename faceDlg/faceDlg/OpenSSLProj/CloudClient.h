#pragma once

#include <string>
#include <map>
#include <functional>
#include "Protocols.h"

typedef std::function<void(Individual*)> CallBack;

class CloudClient
{
private:

	std::string server_ip;

	std::string server_port;

	void *client;

	//CTCPSSLClient *pt_sslclient;

	std::map<int, Individual> individuals;

	void *p_recv_thread;

	void cmd_process();

	void reconnect();

	void Send(const char* pData, const size_t uSize);

	CallBack callback;

public:
	CloudClient(std::string, std::string, CallBack);

	int send_pic(int w, int h, char *buf);

	Individual get_info(int timestamp);

	~CloudClient();
};

