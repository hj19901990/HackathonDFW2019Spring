#pragma once

#include "TCPClient.h"
#include "TCPServer.h"
#include "TCPSSLServer.h"
#include "TCPSSLClient.h"

#include "Protocols.h"
#define SSL_CERT_FILE "cert.pem"
#define SSL_KEY_FILE "key.pem"

class CloudServer
{
private:

	std::string local_port;

	ASocket::LogFnCallback LogPrinter;

	ASecureSocket::SSLSocket ConnectedClient; // socket of the connected client, we can have a vector of them for example.
											  /* blocking call, should return true if the accept is OK, ConnectedClient should also be a valid socket
											  number */
	CTCPSSLServer *m_pSSLTCPServer;

	std::thread recv_thread;

	void(*callback)(int timestamp, Pic *pic);

	void run();

public:
	
	void send_info(int timestampt, Individual info);

	CloudServer(void(*set_callback)(int timestamp, Pic *pic));
	~CloudServer();
};

