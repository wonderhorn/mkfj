#include"directx/DSHOW.h"
#include"utils/MyUtil.h"
//#include"MyPackFile.h"
//#include "headers.h"
#include<stdlib.h>
#define WM_GRAPH_NOTIFY (WM_APP + 1)

bool BGM::isFinishied = 0;
float BGM::introTime = 0.0;
float BGM::endTime = 0.0;
IGraphBuilder *BGM::pGraphBuilder = NULL;
IMediaControl* BGM::pMediaControl = NULL;
IMediaEventEx *BGM::g_pMediaEventEx = NULL;
IMediaPosition *BGM::g_pMediaPosition = NULL;
IMediaSeeking *BGM::pSeek = NULL;
IBasicAudio *BGM::pBasicAudio = NULL;
IFilterGraph *BGM::pGraph = NULL;
bool BGM::auto_repeat = true;

BGM::BGM(HWND hWnd)
{
	auto_repeat = true;
	pGraphBuilder = NULL;
	pMediaControl = NULL;
	g_pMediaEventEx = NULL;
	g_pMediaPosition = NULL;
	pBasicAudio = NULL;
	pGraph = NULL;

	outputLog("DirectShowを初期化します。");
	// COMを初期化
	 CoInitialize(NULL);

	if(
		FAILED(CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC, 
			IID_IGraphBuilder, (LPVOID *)&pGraphBuilder))
		|| FAILED(pGraphBuilder->QueryInterface(IID_IMediaEventEx, (LPVOID*)&g_pMediaEventEx))
		|| FAILED(pGraphBuilder->QueryInterface(IID_IMediaControl, (LPVOID *)&pMediaControl))
		|| FAILED(pGraphBuilder->QueryInterface(IID_IMediaPosition, (LPVOID*)&g_pMediaPosition))
		|| FAILED(pGraphBuilder->QueryInterface(IID_IBasicAudio, (LPVOID *)&pBasicAudio))
		|| FAILED(pGraphBuilder->QueryInterface(IID_IMediaSeeking, (LPVOID *)&pSeek))
		)

	{
		outputLog("DirectShowの初期化に失敗しました。");
		cleanup();
		return;
	}

	pSeek->SetTimeFormat(&TIME_FORMAT_MEDIA_TIME);
	g_pMediaEventEx->SetNotifyWindow((OAHWND)hWnd, WM_GRAPH_NOTIFY, NULL);
	CoUninitialize();
	outputLog("DirectShowの初期化を終了しました。");;
}

BGM::~BGM()
{
	SAFE_RELEASE(pGraphBuilder);

	if(pMediaControl)
	{
		pMediaControl->Stop();
		//pMediaControl->Release();
		SAFE_RELEASE(pGraphBuilder);
	}
	SAFE_RELEASE(pSeek);
	SAFE_RELEASE(g_pMediaEventEx);
	SAFE_RELEASE(g_pMediaPosition);
	SAFE_RELEASE(pBasicAudio);
	SAFE_RELEASE(pGraph);

}
void BGM::cleanup()
{
	//if(pGraphBuilder)pGraphBuilder->Release();
	SAFE_RELEASE(pGraphBuilder);

	if(pMediaControl)
	{
		pMediaControl->Stop();
		SAFE_RELEASE(pGraphBuilder);
	}
	SAFE_RELEASE(pSeek);
	SAFE_RELEASE(g_pMediaEventEx);
	SAFE_RELEASE(g_pMediaPosition);
	SAFE_RELEASE(pBasicAudio);
	SAFE_RELEASE(pGraph);

	pSeek = NULL;
	pGraphBuilder = NULL;
	pMediaControl = NULL;
	g_pMediaEventEx = NULL;
	g_pMediaPosition = NULL;
	pBasicAudio = NULL;
	pGraph = NULL;
	//if(remove("temp0.tmp") != 0)
	//	outputLog("一時ファイルの更新に失敗しました。");
}
void BGM::clearFile()
{
	if(pMediaControl == NULL)
		return;

	pMediaControl->Stop();

	IEnumFilters *pEnum = NULL;
	HRESULT hr = pGraphBuilder->EnumFilters(&pEnum);

	if (SUCCEEDED(hr))
	{
	    IBaseFilter *pFilter = NULL;
	    while (S_OK == pEnum->Next(1, &pFilter, NULL))
	     {
		     pGraphBuilder->RemoveFilter(pFilter);
			pEnum->Reset();
			pFilter->Release();
			}
		pEnum->Release();
	}

	
}
HRESULT BGM::openFile(BSTR filename, float introTime , float endTime)
{
	clearFile();

	if(pGraphBuilder == NULL) return E_FAIL;


	if(FAILED(pGraphBuilder->RenderFile(filename, NULL)))
	{
		outputLog("BGMのレンダリングに失敗しました。");
		return E_FAIL;
	}

	if(pBasicAudio == NULL) return E_FAIL;
	pBasicAudio->put_Volume(volume);
	this->introTime = introTime;
	this->endTime = endTime;
	return S_OK;
}

HRESULT BGM::openFile(BSTR filename)
{
	 return openFile(filename, 0.0, 0.0);
}

HRESULT BGM::openFile(BSTR filename, float introTime)
{
	 return openFile(filename, introTime, 0.0);
}

void BGM::play()
{
	//g_pMediaPosition->put_CurrentPosition(introTime);
	if(pMediaControl)
		pMediaControl->Run();
	isFinishied = 0;
}

void BGM::cursor(double t) 
{
	if (g_pMediaPosition)
	{
		g_pMediaPosition->put_CurrentPosition(t);
	}
}

void BGM::stop()
{
	if (pMediaControl)
		pMediaControl->Stop();
}

void BGM::reset()
{
	if (g_pMediaPosition)
	{
		g_pMediaPosition->put_CurrentPosition(0);
		//g_pMediaPosition->put_CurrentPosition(introTime);
	}
}

void BGM::setVolume(long v)
{
	if (pBasicAudio)
	{
		long vv;
		HRESULT hr = pBasicAudio->get_Volume(&vv);
		pBasicAudio->put_Volume(v);
		volume = v;
	}
}

void BGM::checkFinished(long evCode, LONG param1, LONG param2)
{
	if(g_pMediaPosition == NULL)return;
	
	REFTIME stoptime;
	REFTIME currenttime;

	if (g_pMediaEventEx == NULL)return;
	if (pMediaControl == NULL)return;
	while (SUCCEEDED(g_pMediaEventEx->GetEvent(&evCode, &param1, &param2, 0)))
	{
		switch (evCode)
		{
		case EC_COMPLETE:
			if (!auto_repeat)
				stop();
			else
			{
				pMediaControl->Stop();
				g_pMediaPosition->put_CurrentPosition(introTime);
				pMediaControl->Run();
			}
			//pMediaControl->Run();
			//isFinishied = 1;
			break;
		}
		g_pMediaEventEx->FreeEventParams(evCode, param1, param2);
	}
}

void BGM::proc(long evCode, LONG param1, LONG param2)
{
	if(g_pMediaEventEx == NULL)return;
	if(pMediaControl == NULL)return;
	while(SUCCEEDED(g_pMediaEventEx->GetEvent(&evCode,&param1, &param2, 0)))
		{
			g_pMediaEventEx->FreeEventParams(evCode, param1, param2);
		}
}