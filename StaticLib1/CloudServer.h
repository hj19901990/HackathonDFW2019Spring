#pragma once


#include <thread>
#include "Protocols.h"
#include <functional>

#define SSL_CERT_FILE "cert.pem"
#define SSL_KEY_FILE "key.pem"

typedef std::function<void(Pic *)> CallBack;

class CloudServer
{
private:

	std::string local_port;

	std::thread recv_thread;

	CallBack callback;

	void run();

	void *connectedClient;

	void *ssl_server;

public:
	
	void send_info(int timestampt, Individual info);

	CloudServer(CallBack);
	~CloudServer();
};

