#pragma once
#include"network/SOCKET.h"

class SESSION
{
public:
	SESSION(){}
	~SESSION();
	bool initialize(int recvport, int sendport, const char* IP);
	int send(int num, int acknum, const char* str);
	bool waitRecv(int& num, int& acknum, char* str, char* IP);
	bool accept();
	bool connect();
	bool accept_nonwait();
	bool connect_nonwait();
	void disconnect();
	static const int MAX_LEN_MESSAGE = 64;

private:
	struct MESSAGE
	{
		int num, acknum;
		char str[MAX_LEN_MESSAGE];
	};
	WINSOCK sock;
	int recvport, sendport;
	char IP[16];

	static const int MAX_NUM_MESSAGE = 3000;
	MESSAGE sent_mes[MAX_NUM_MESSAGE];
	int mes_sent;
};