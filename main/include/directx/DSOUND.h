#pragma once

#define DIRECTSOUND_VERSION 0x0800

#pragma comment ( lib, "dxguid.lib" )
#pragma comment ( lib, "dsound.lib" )

#include<tchar.h>
#include<dsound.h>
#include<mmsystem.h>

//ƒƒ‚ƒŠ‰ð•ú—pƒ}ƒNƒ
#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }


class DSOUND
{
public:
	DSOUND(HWND hWnd);
	~DSOUND();
	bool openWave(TCHAR *filepath, int i);
	bool openWaveFromMem(char* data, int i);

	HRESULT createDevise(int i);
	void Play(int i, double volume_rate = 1.0);
	void Volume(int v){ volume = v; }

private:
	static const int NUM_OF_BUFFER = 40;
	static bool SEOn;
	IDirectSound8 *pDS8;

	IDirectSoundBuffer* ptmpBuffer;
	IDirectSoundBuffer8 *pDSBuffer[NUM_OF_BUFFER];

	LPVOID lpvWrite;
	DWORD dwLength;

	WAVEFORMATEX wFmt;
	char *pWaveData;
	DWORD waveSize;
	int volume;
};