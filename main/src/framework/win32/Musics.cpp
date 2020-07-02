#include"framework/Media.h"
using namespace gfw;

Musics::Musics(): bgm(nullptr){}

bool Musics::initialize(HWND hWnd, WNDCLASSEX wc)
{
	bgm = new BGM(hWnd);
	return true;
}


void Musics::finalize()
{
	if (bgm)
	{
		bgm->cleanup();
		delete bgm;
	}
}

bool Musics::startBGM(const std::string& name, double intro_time)
{
	std::string str = name;
	BSTR bstr = SysAllocStringLen(NULL, 1024);
	MultiByteToWideChar(CP_ACP, 0, name.c_str(), name.size() + 1, bstr, 1024);
	HRESULT hr = bgm->openFile(bstr, intro_time);
	if (SUCCEEDED(hr))
	{
		bgm->play();
		SysFreeString(bstr);
		return true;
	}
	else
	{
		SysFreeString(bstr);
		return false;
	}

}

void Musics::stopBGM()
{
	bgm->stop();
}

void Musics::resetBGM()
{
	bgm->reset();
}

void Musics::checkLoop(long evCode, LONG param1, LONG param2)
{
	bgm->checkFinished(evCode, param1, param2);
}

void Musics::setVolume(int v)
{
	long vv = -9000 + 1000 * v;
	bgm->setVolume(vv);
}

void Musics::setAutoRepeat(bool b)
{
	bgm->autoRepeat(b);
}
