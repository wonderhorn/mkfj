#include<dshow.h>
#pragma once

//ƒƒ‚ƒŠ‰ð•ú—pƒ}ƒNƒ
#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }

#define WM_GRAPH_NOTIFY (WM_APP + 1)

class BGM
{
public:
	BGM(HWND hWnd);
	~BGM();

	void cleanup();
	void clearFile();
	HRESULT openFile(BSTR filename);
	HRESULT openFile(BSTR filename, float introTime);
	HRESULT openFile(BSTR filename, float introTime, float endTime);

	void play();
	void stop();
	void reset();
	void cursor(double t);
	void autoRepeat(bool b){ auto_repeat = b; }
	void setVolume(long v);
	void checkFinished(long evCode, LONG param1, LONG param2);
	static void proc(long evCode, LONG param1, LONG param2);
	static bool isFinishied;
	static float introTime;
	static float endTime;
	long volume;
	static bool auto_repeat;

private:
	static IGraphBuilder *pGraphBuilder;
	static IMediaControl *pMediaControl;
	static IMediaEventEx *g_pMediaEventEx;
	static IMediaPosition *g_pMediaPosition;
	static IBasicAudio *pBasicAudio;
	static IFilterGraph *pGraph;
	static IMediaSeeking *pSeek;
};