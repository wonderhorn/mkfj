//#include "headers.h"
#include"directx/DSOUND.h"
#include"utils/MyUtil.h"
#include<string.h>

bool DSOUND::SEOn = true;

DSOUND::DSOUND(HWND hWnd)
{
	outputLog("DirectSoundを初期化します。");
	volume = 10000;
	pDS8 = NULL;
	for (int i = 0; i < NUM_OF_BUFFER; i++)
	{
		pDSBuffer[i] = NULL;
	}

	if (FAILED(DirectSoundCreate8(NULL, &pDS8, NULL)))
	{
		outputLog("DirectSoundの初期化に失敗しました。");
		pDS8 = NULL;
	}
	else
	{
		pDS8->SetCooperativeLevel(hWnd, DSSCL_PRIORITY);
		outputLog("DirectSoundの初期化に成功しました。");;
	}

}

DSOUND::~DSOUND()
{
	for (int i = 0; i < NUM_OF_BUFFER; i++)
	{
		if (pDSBuffer[i])
			pDSBuffer[i]->Release();
	}
}

bool DSOUND::openWave(TCHAR *filepath, int index)
{
	if (pDS8 == NULL)
		return false;
	if (filepath == 0)
		return false;

	HMMIO hMmio = NULL;
	MMIOINFO mmioInfo;

	// Waveファイルオープン
	memset(&mmioInfo, 0, sizeof(MMIOINFO));
	hMmio = mmioOpen(filepath, &mmioInfo, MMIO_READ);
	if (!hMmio)
		return false; // ファイルオープン失敗

	// RIFFチャンク検索
	MMRESULT mmRes;
	MMCKINFO riffChunk;
	riffChunk.fccType = mmioFOURCC('W', 'A', 'V', 'E');
	mmRes = mmioDescend(hMmio, &riffChunk, NULL, MMIO_FINDRIFF);
	if (mmRes != MMSYSERR_NOERROR) {
		mmioClose(hMmio, 0);
		return false;
	}

	// フォーマットチャンク検索
	MMCKINFO formatChunk;
	formatChunk.ckid = mmioFOURCC('f', 'm', 't', ' ');
	mmRes = mmioDescend(hMmio, &formatChunk, &riffChunk, MMIO_FINDCHUNK);
	if (mmRes != MMSYSERR_NOERROR) {
		mmioClose(hMmio, 0);
		return false;
	}
	DWORD fmsize = formatChunk.cksize;
	DWORD size = mmioRead(hMmio, (HPSTR)&wFmt, fmsize);
	if (size != fmsize) {
		mmioClose(hMmio, 0);
		return false;
	}

	mmioAscend(hMmio, &formatChunk, 0);

	// データチャンク検索
	MMCKINFO dataChunk;
	dataChunk.ckid = mmioFOURCC('d', 'a', 't', 'a');
	mmRes = mmioDescend(hMmio, &dataChunk, &riffChunk, MMIO_FINDCHUNK);
	if (mmRes != MMSYSERR_NOERROR) {
		mmioClose(hMmio, 0);
		return false;
	}
	pWaveData = new char[dataChunk.cksize];
	size = mmioRead(hMmio, (HPSTR)pWaveData, dataChunk.cksize);
	if (size != dataChunk.cksize) {
		delete[]  pWaveData;
		return false;
	}
	waveSize = size;

	// ハンドルクローズ
	mmioClose(hMmio, 0);

	createDevise(index);

	return true;
}

bool DSOUND::openWaveFromMem(char* data, int i)
{
	if (pDS8 == NULL)return false;
	

	DWORD dwWavSize = 0;

	if (strncmp(data, "RIFF", 4) != 0)
	{
		return false;
	}
	if (strncmp(data + 8, "WAVE", 4) != 0)
	{
		return false;
	}
	if (strncmp(data + 12, "fmt ", 4) != 0)
	{
		return false;
	}

	memcpy(&wFmt, (data + 20), *(data + 16));

	if (strncmp(data + 36, "data", 4) != 0)
	{
		return false;
	}
	//waveSize = (DWORD)(*(data+40));
	DWORD c;
	ZeroMemory(&waveSize, sizeof(DWORD));
	memcpy(&c, data + 40, 4);
	//memcpy(&waveSize, &c, 4);
	waveSize = c;

	pWaveData = new char[waveSize];
	//pWaveData = data + 44;
	memcpy(pWaveData, (data + 44), waveSize);
	createDevise(i);
	return true;
}


HRESULT DSOUND::createDevise(int i)
{
	DSBUFFERDESC DSBufferDesc;
	DSBufferDesc.dwSize = sizeof(DSBUFFERDESC);
	DSBufferDesc.dwFlags = DSBCAPS_CTRLVOLUME;
	DSBufferDesc.dwBufferBytes = waveSize;
	DSBufferDesc.dwReserved = 0;
	DSBufferDesc.lpwfxFormat = &wFmt;
	DSBufferDesc.guid3DAlgorithm = GUID_NULL;

	ptmpBuffer = 0;

	HRESULT hr;
	if (FAILED(hr = pDS8->CreateSoundBuffer(&DSBufferDesc, &ptmpBuffer, NULL)))return E_FAIL;

	ptmpBuffer->QueryInterface(IID_IDirectSoundBuffer8, (void**)&pDSBuffer[i]);
	ptmpBuffer->Release();
	if (pDSBuffer[i] == 0)
	{
		return E_FAIL;
	}

	lpvWrite = 0;
	dwLength = 0;
	if (DS_OK == pDSBuffer[i]->Lock(0, 0, &lpvWrite, &dwLength, NULL, NULL, DSBLOCK_ENTIREBUFFER)) {
		memcpy(lpvWrite, pWaveData, dwLength);
		pDSBuffer[i]->Unlock(lpvWrite, dwLength, NULL, 0);
	}
	delete[] pWaveData; 

	pDSBuffer[i]->SetVolume(volume);
	return S_OK;
}

void DSOUND::Play(int index, double volume_rate)
{
	if (SEOn)
	{
		if (pDSBuffer[index])
		{
			//pDSBuffer[index]->SetVolume(-000);
			pDSBuffer[index]->SetVolume(-10000 + volume * volume_rate);
			pDSBuffer[index]->Stop();
			pDSBuffer[index]->SetCurrentPosition(0);
			pDSBuffer[index]->Play(0, 0, 0);
		}
	}
}