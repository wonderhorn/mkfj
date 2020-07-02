#pragma once
#include<map>

#include"network/SESSION.h"
#include"framework/VirtualController.h"
#include"utils/MultiThread.h"

namespace gfw
{

	class UDPController : public gfw::VirtualController
	{
	public:
		UDPController();
		bool initializeSocket(int rport, int sport, const char* ip);
		bool try_accept();
		bool try_connect(std::string dst_ip, int port);
		bool endConnection();
		bool poll();
		void push_send_data(const ButtonData& data);
		bool push_z, push_x, push_c, push_a, push_s, push_up
			, push_right, push_down, push_left, push_space;

		int cursor;
		int myack, remoteack;
		std::vector<ButtonData> data_send;
		std::vector<ButtonData> data_recieved;
		std::map<int, ButtonData> data_recieved_map;

		SESSION session;
		myutil::Thread thread;

		static std::vector<ButtonData> test_data();
	};
};