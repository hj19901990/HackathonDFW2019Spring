#pragma once

#include <Windows.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <queue>
#include <winsock.h>
#include < vcclr.h > 

//#include "DrawDevice.h"

#define socklen_t	int


#pragma comment(lib, "wsock32.lib")
#pragma comment(lib, "user32.lib")
public class CComm
{

public:
	CComm();
	~CComm();
	int  RecvfromTimeOutUDP(SOCKET socket, long sec, long usec);
	bool SendMsg(char *Msg, int Len, const char *host, short port);
	bool Listen(int PortNum);
	void CleanSocket();
private:
	sockaddr_in srv;			// binding address

public:
	SOCKET ListenSocket;	// wait socket
	int					_UDP_port;
	const char*			_UDP_host;
};