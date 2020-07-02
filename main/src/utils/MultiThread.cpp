#include"utils/MultiThread.h"
#include<iostream>
#include<thread>
//#include<Windows.h>
using namespace myutil;

Thread::Thread(const std::string& name)
	: name(name), param(nullptr), finished(false)
{}
void Thread::begin(unsigned int(*func)(void*))
{
  handle = std::thread([=]{ func(param); });
	//handle = _beginthreadex(NULL, 0, func, param, 0, NULL);
}

void Thread::end()
{
	finished = true;
  handle.join();
	//_endthreadex(handle);
	//WaitForSingleObject((HANDLE)handle, 1500);
}

void Thread::kill()
{
	finished = true;
  handle.join();
	//_endthreadex(handle);
	//WaitForSingleObject((HANDLE)handle, 1500);
}


unsigned int myutil::thread_test(void*)
{
	while (1)
	{
		std::cout << "hey" << std::endl;
		//Sleep(1000);
    std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}


IOThread::IOThread() : Thread("io"), func_after_read(nullptr)
{
	//InitializeCriticalSection(&cs);
}
std::string IOThread::readNonBlocking()
{
	//EnterCriticalSection(&cs);
  std::lock_guard<std::mutex> lock(cs);
	std::string rtn = q.front();
	q.pop_front();
	//LeaveCriticalSection(&cs);
	return rtn;
}
void IOThread::begin()
{
	Param(this);
	Thread::begin(thread_func);
	std::cout << ">>";
}
void IOThread::end()
{
	Thread::end();
	//DeleteCriticalSection(&cs);
}

unsigned int IOThread::thread_func(void* ptr)
{
	while (1)
	{
		//std::cout << "please input" << std::endl;
		std::string input;
		std::cin >> input;
		auto* iot = static_cast<IOThread*>(ptr);
		//EnterCriticalSection(&iot->cs);
    std::lock_guard<std::mutex> lock(iot->cs);
		iot->q.push_back(input);
		//std::cout << input << std::endl;
		if (iot->func_after_read)
			iot->func_after_read(iot->object, iot->q);
		//LeaveCriticalSection(&iot->cs);
		if (iot->finished)
			break;
	}
	return 0;
}
