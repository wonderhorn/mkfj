#include"android/AndroidMedia.h"
using namespace gfw;

Sounds::Sounds() {}
Sounds::~Sounds() {}
bool Sounds::initialize() {}
void Sounds::finalize() {}
void Sounds::play(SoundHandler snd) {}
SoundHandler Sounds::loadSound(const std::string& filename, double base_volume)
{
	return -1;
}