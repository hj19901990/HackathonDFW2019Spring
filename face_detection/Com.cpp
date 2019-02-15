#include "Com.h"

	CComm::CComm()
	{
		ListenSocket = INVALID_SOCKET;	// INVALID_SOCKET Initially

#ifdef _WIN32	
		WORD VersionRequested = MAKEWORD(1, 1);
		WSADATA wsaData;

		WSAStartup(VersionRequested, &wsaData);	// start winsock
		if (wsaData.wVersion != VersionRequested)
		{
			printf("Wrong version or WinSock not loaded\n");
			fflush(0);
		}
#endif
	}


	CComm::~CComm()
	{
		CleanSocket();

	}

	void CComm::CleanSocket() {
		if (ListenSocket != INVALID_SOCKET)
			fflush(0);
		closesocket(ListenSocket);	// close socket

#ifdef _WIN32	// WSACleanup
		WSACleanup();
#endif
	}

	bool CComm::SendMsg(char *Msg, int Len, const char *host, short port)
	{
		signed int Sent;
		hostent *hostdata;
		in_addr ip;
		ip.s_addr = inet_addr(host);


		//if (atoi(host))	// Standard IP address
		//{

		//	
		//	hostdata = gethostbyaddr((char *)&ip, sizeof(ip), PF_INET);
		//	int err = WSAGetLastError();
		//	if (err)
		//		int a = 0;
		//}
		//else	// Otherwise host name
		//{
		//	hostdata = gethostbyname(host);
		//}

		//if (!hostdata)
		//{
		//	//printf("Failure to connect\n");
		//	fflush(0);
		//	return false;
		//}

		sockaddr_in dest;	// Send to MCU
		dest.sin_family = PF_INET;
		dest.sin_addr = ip;
		dest.sin_port = htons(port);

		//Data sending
		Sent = sendto(ListenSocket, Msg, Len, 0, (sockaddr *)&dest, sizeof(sockaddr_in));

		if (Sent != Len)
		{
			printf("Wrong UDP Message\n");
			fflush(0);
			return false;
		}

		return true;
	}



	//Binding listen port
	bool CComm::Listen(int PortNum)
	{
		ListenSocket = socket(PF_INET, SOCK_DGRAM, 0);
		if (ListenSocket == INVALID_SOCKET)
		{
			//printf("Error: socket creatation failure\n");
			fflush(0);
			return false;
		}


		srv.sin_family = PF_INET;
		srv.sin_addr.s_addr = htonl(INADDR_ANY);	// Any address
		srv.sin_port = htons(PortNum);

		if (bind(ListenSocket, (struct sockaddr *)&srv, sizeof(srv)) != 0)
		{
			//printf("Error: Failure binding\n");
			fflush(0);
			closesocket(ListenSocket);
			return false;
		}

		return true;
	}

	int  CComm::RecvfromTimeOutUDP(SOCKET socket, long sec, long usec)
	{
		// Setup timeval variable
		struct timeval timeout;
		struct fd_set fds;

		timeout.tv_sec = sec;
		timeout.tv_usec = usec;
		// Setup fd_set structure
		FD_ZERO(&fds);
		FD_SET(socket, &fds);
		// Return value:
		// -1: error occurred
		// 0: timed out
		// > 0: data ready to be read
		return select(0, &fds, 0, 0, &timeout);
	}

