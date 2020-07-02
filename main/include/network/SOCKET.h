#pragma once
#pragma comment(lib, "ws2_32.lib")
#include<stdio.h>
#include<stdlib.h>
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include<winsock2.h>
const int MAX_SIZE_DATA = 10;
static char CONNECT_SIGNAL[] = "makeconnect";

class WINSOCK
{
public:
	WINSOCK();
	~WINSOCK();

	bool initialize();
	bool createSocket(int port);
	void destroySocket();
	int recieveString(char *buf, int size, char* IP, struct sockaddr_in *addr_out);
	int sendString(char* buf, char* IP, int port, int len);

private:
	WSADATA wsaData;
	int err;
	int port;
	SOCKET sock;
	struct sockaddr_in addr;
	struct sockaddr_in youraddr;
	char myIP[16];

	static const int BUFFERSIZE = 100;

	char buf[BUFFERSIZE];
};