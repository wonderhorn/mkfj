#include"framework/Media.h"

using namespace gfw;

Sounds::Sounds(): dsound(nullptr){}
Sounds::~Sounds()
{
	if (dsound)
	{
		finalize();
	}
}

void Sounds::play(SoundHandler snd)
{
	if(snd >= 0)
		dsound->Play(snd, volumes[snd] * (double) volume / 10);
}

SoundHandler Sounds::loadSound(const std::string& filename, double base_volume)
{
	char tmp[1024];
	strcpy_s(tmp, filename.c_str());
	if (dsound->openWave(tmp, num_sound++))
	{
		int idx = num_sound - 1;
		if (volumes.size() <= idx)
			volumes.resize(num_sound);
		volumes[idx] = base_volume;
		return idx;
	}
	else return -1;

}

bool Sounds::initialize(HWND hWnd)
{
	dsound = new DSOUND(hWnd);
	num_sound = 0;
	return true;
}
void Sounds::finalize()
{
	if (dsound)
	{
		delete dsound;
		dsound = nullptr;
	}
}