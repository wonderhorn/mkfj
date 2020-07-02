#include"android/AndroidMedia.h"
using namespace gfw;

Musics::Musics() {}

bool Musics::initialize()
{
	return false;
}
bool Musics::startBGM(const std::string& name, double intro_time)
{
	return false;
}
void Musics::stopBGM()
{}
void Musics::resetBGM() {}
void Musics::finalize() {}
void Musics::checkLoop(long evCode, long param1, long param2) {}
void Musics::setVolume(int v) {}
void Musics::setAutoRepeat(bool b){}