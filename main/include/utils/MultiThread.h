#pragma once
//#include<process.h>
//#include<windows.h>
#include <future>
#include<string>
#include<iostream>
#include<deque>
namespace myutil
{
	unsigned int thread_test(void*);
	class Thread
	{
	public:
		Thread(const std::string& name);
		void begin(unsigned int(*func)(void*));
		void end();
		void kill();
		void Param(void* ptr){ param = ptr; }
	private:
		std::string name;
		std::thread handle;
		void* param;
	protected:
		bool finished;
	};

	class IOThread: public Thread
	{
	public:
		IOThread();
		std::string readNonBlocking();
		bool empty(){ return q.empty(); }
		void begin();
		void end();
		void setEventArg(void* ptr){ std::lock_guard<std::mutex> lock(cs); this->object = ptr; }
		void setEventHander(void(*func_after_read)(void* param, std::deque<std::string>& q)){ this->func_after_read = func_after_read; }
		static unsigned int thread_func(void* ptr);
	private:
		std::deque<std::string> q;
		std::mutex cs;
		void(*func_after_read)(void* param, std::deque<std::string>& q);
		void *object;
	};
};
