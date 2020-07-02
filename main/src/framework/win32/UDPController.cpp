#include"framework/UDPController.h"
#include<iostream>
#include<fstream>
#include<iostream>
#include<assert.h>
using namespace gfw;

UDPController::UDPController() : push_z(false), push_x(false), push_c(false)
, push_a(false), push_s(false)
, push_up(false), push_right(false)
, push_down(false), push_left(false), push_space(false)
, cursor(0), thread("udpcontroller"){}

bool UDPController::initializeSocket(int rport, int sport, const char* ip)
{
	session.initialize(rport, sport, ip);
	return true;
}

bool UDPController::try_accept()
{
	//session = SESSION(port, 0, "127.0.0.1");
	return session.accept_nonwait();
	//return session.accept();

	//return true;
}
bool UDPController::try_connect(std::string dst_ip, int port)
{
	//session = SESSION(port, 20000, dst_ip.c_str());
	return session.connect_nonwait();
	//return true;
}
bool UDPController::endConnection()
{
	session.disconnect();
	return true;
}

std::vector<ButtonData> UDPController::test_data()
{
	std::vector<ButtonData> rtn(300);
	for (int i = 0; i < rtn.size(); i++)
	{
		if (i % 2 == 0)
		{
			//bool push_z, push_x, push_c, push_a, push_s, push_up, push_right, push_down, push_left;
			ButtonData ar{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
			rtn[i] = ar;
		}
		else
		{
			ButtonData ar{ 0, 0, 0, 0, 0, 1, 0, 0, 0 ,0 };
			rtn[i] = ar;
		}
	}
	//{
	//	{ 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	//};
	return rtn;
}

static void incrementAck(int & acknum)
{

}

bool UDPController::poll()
{
	auto& data = data_recieved;
	while (cursor >= data.size())
	{
		int num = -1, acknum = -1;
		char buf[SESSION::MAX_LEN_MESSAGE];
		bool recieved = session.waitRecv(num, acknum, buf, nullptr);

		if (buf[0] == 'b' && buf[1] == 't' && buf[2] == 'n')
		{
			ButtonData recieved;
			char* buf2 = buf + 3;
			for (int i = 0; i < 10; i++)
			{
				recieved[i] = (bool)buf2[i];
			}

			data_recieved_map[num] = recieved;
			if (num == myack + 1)
				myack++;
			if (acknum > remoteack)
				remoteack = acknum;
		}
		Sleep(1);
	}

	z.setValue(data[cursor][0]);
	x.setValue(data[cursor][1]);
	c.setValue(data[cursor][2]);
	a.setValue(data[cursor][3]);
	s.setValue(data[cursor][4]);

	up.setValue(data[cursor][5]);
	right.setValue(data[cursor][6]);
	down.setValue(data[cursor][7]);
	left.setValue(data[cursor][8]);
	space.setValue(data[cursor][9]);

	cursor++;

	return true;
}