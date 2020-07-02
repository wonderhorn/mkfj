//#include"SOCKET.h"
#include"network/SESSION.h"
#include<iostream>
using namespace std;

bool SESSION::initialize(int recvport, int sendport, const char *IP)
{
	this->recvport = recvport;
	this->sendport = sendport;
	if(IP)
		strcpy_s(this->IP, IP);
	//else this->IP = NULL;

	sock.initialize();
	if (!sock.createSocket(recvport))
	{
		cout << ("ソケットの作成に失敗しました") << endl;
		return false;
	}
	return true;
}
SESSION::~SESSION()
{
	sock.destroySocket();
}
int SESSION::send(int num, int acknum, const char *str)
{
	MESSAGE m;
	m.num = num;
	m.acknum = acknum;
	memcpy(m.str, str, MAX_LEN_MESSAGE);
	//if(rand() % 10 == 0)
	//	return 1;
	return sock.sendString((char*) &m, IP, sendport, sizeof(MESSAGE));
}
bool SESSION::waitRecv(int &num, int &acknum, char *str, char* IP)
{

	struct sockaddr_in addr_from;
	char buf[sizeof(MESSAGE)] = {};
	int n = 0;
	int count = 0;
	while((n += sock.recieveString(buf, sizeof(MESSAGE), IP, &addr_from)) < MAX_LEN_MESSAGE)
	{
		count++;
		if(count > 1000)
			return 0;
	}

	if(IP)
	{
		strcpy_s(IP, 15, inet_ntoa(addr_from.sin_addr));
	}
	else if(addr_from.sin_addr.S_un.S_addr != inet_addr(this->IP))
	{
		return 0;
	}

	MESSAGE m;
	memcpy(&m, buf, sizeof(MESSAGE));
	num = m.num;
	acknum = m.acknum;
	memcpy(str, m.str, MAX_LEN_MESSAGE);
	if(rand() % 10 == 0)
		return 0;
	return 1;
}
bool SESSION::accept()
{
	int n = 0, acknum = 0;
	char buf[MAX_LEN_MESSAGE] = {};
	bool connected = false;
	while(!connected)
	{
		while(!waitRecv(n, acknum, buf, IP));
		if(n == -1 && strncmp("letsconnect****", buf, 16) == 0)
		{
			sendport = acknum;
			send(-1, -1, "letsconnect****");
		}

		while (waitRecv(n, acknum, buf, NULL))
		//if(waitRecv(n, acknum, buf, NULL))
		{
			if (n == -1 && strncmp("connected******", buf, 16) == 0)//これが返ってこなかったときはもう一度 letsconnect****
			{
				connected = true;
				break;
			}
		}
	}
	//sendport = acknum;
	printf("acceptしました\n");
	return 1;
}

bool SESSION::connect()
{
	int acknum;
	if(!IP)
	{
		printf("connectにはIP指定が必要です\n");
		return 0;
	}

	while(1)
	{
		send(-1, recvport, "letsconnect****");
		int n = 0;
		char buf[MAX_LEN_MESSAGE] = {};
		if(waitRecv(n, acknum, buf, IP))
		{
			if(n == -1 && strncmp("letsconnect****", buf, 16) == 0)
			{
				for(int i = 0; i < 5; i++)
				{
					send(-1, -1, "connected******");
					Sleep(10);
				}
			}
			break;
		}
	}
	printf("connectしました\n");
	return 1;
}

void SESSION::disconnect()
{
	for (int i = 0; i < 10; i++)
	{
		send(-1, -1, "disconnected***");
		Sleep(10);
	}
	sock.destroySocket();
}

bool SESSION::accept_nonwait()
{
	bool recved = 0;
	int counter = 0;
	int n = 0, acknum = 0;
	char buf[MAX_LEN_MESSAGE] = {};
	while(1)
	{
		while(!waitRecv(n, acknum, buf, IP))
		{
			if(counter >= 10)
				return 0;
			counter++;
			Sleep(1);
		}
		if(counter < 10)recved = 1;
		while(recved)
		{
			if(n == -1 && strncmp("letsconnect****", buf, 16) == 0)
			{
				sendport = acknum;
				send(-1, -1, "letsconnect****");
			}

			if(waitRecv(n, acknum, buf, NULL))
			{
				if(n == -1 && strncmp("connected******", buf, 16) == 0)
					return 1;
			}
			Sleep(1);
		}
		if(counter >= 10)
			return 0;
		counter++;
		Sleep(1);
	}
	//sendport = acknum;
	printf("acceptしました\n");
	return 1;
}

bool SESSION::connect_nonwait()
{
	int acknum, counter = 0;
	if(!IP)
	{
		printf("connectにはIP指定が必要です\n");
		return 0;
	}

	while(1)
	{
		send(-1, recvport, "letsconnect****");
		int n = 0;
		char buf[MAX_LEN_MESSAGE] = {};
		if(waitRecv(n, acknum, buf, IP))
		{
			if(n == -1 && strncmp("letsconnect****", buf, 16) == 0)
			{
				for(int i = 0; i < 5; i++)
				{
					send(-1, -1, "connected******");
					Sleep(10);
				}
			}
			break;
		}
		if(counter >= 10)
			return 0;
		counter++;
	}
	printf("connectしました\n");
	return 1;
}