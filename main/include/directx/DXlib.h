#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include<winsock2.h> // this should be included before windows.h
#include<windows.h>
#include<d3d9.h>
#include<d3dx9.h>
#include<dxdiag.h>
#include<tchar.h>
#include<Windows.h>
#include<mmsystem.h>

extern "C"
#define RELEASE(o) if(o){o->Release();o=NULL;}

//メモリ解放用マクロ
#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }

#define PI 3.1415926535

struct CUSTOMVERTEX
{
   float x, y, z; // 頂点座標
   float rhw;     // 除算数
   DWORD dwColor; // 頂点の色
   float u, v;    //　テクスチャ座標
};

HRESULT OutPutDxDiagLog();

class DXFONT;

class CD3DSetup
{
	//メンバ
	HWND hwnd;
	MSG	msg;
	WNDCLASS wc;
	int iadapter;
	int iFULLSCREEN;
	int iSW;
	int m_iwidth, m_iheight;

	//LPD3DXSPRITE psprite;

	LPDIRECT3DTEXTURE9 ptitle;
	LPDIRECT3DTEXTURE9 pforeground, pstencil;

	D3DPRESENT_PARAMETERS param;
	IDirect3D9* (* pDirect3DCreate9)(UINT SDKVersion);
public:
	LPDIRECT3D9 lpD3D;
	LPDIRECT3DDEVICE9	lpD3DDEV;
	LPD3DXSPRITE psprite;
	LPD3DXLINE pline;

public:
	int iloop;
	DXFONT* font;
	DXFONT* fontw;
	CD3DSetup();
	~CD3DSetup();
	LPDIRECT3DTEXTURE9 LoadTexture(LPDIRECT3DTEXTURE9 &ptexture, LPCTSTR filename, int x, int y, int toukasyoku);
	LPDIRECT3DTEXTURE9 LoadTexture(LPDIRECT3DTEXTURE9 &ptexture, LPCTSTR filename, int x, int y);
	LPDIRECT3DTEXTURE9 LoadTextureFromResource(LPDIRECT3DTEXTURE9 &ptexture, LPCTSTR filename, int x, int y, int toukasyoku);
	LPDIRECT3DTEXTURE9 LoadTextureFromMem(LPDIRECT3DTEXTURE9 &ptexture, void* data,int size, int x, int y, int toukasyoku);
	LPDIRECT3DTEXTURE9 CreateRenderingTarget(LPDIRECT3DTEXTURE9 &ptexture, int x, int y);
	//LPDIRECT3DTEXTURE9 LoadTextureFromPack(LPDIRECT3DTEXTURE9 &ptexture, MyPackFile* pac, char* filename, int x, int y, int toukasyoku);
	
	BOOL		WinInit(HINSTANCE hInst, char szWinName[], int width, int height);		//ウィンドウ生成
	HRESULT		MsgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);	//ウィンドウメッセージ
	HRESULT		AppLoop();	//アプリケーションループ
	HRESULT InitD3D(HWND hWnd, BOOL bWin, int width, int height);	//Direct3D9初期化
	void Set2DRectangle(float x, float y, float width, float height, LPDIRECT3DTEXTURE9 ptex);
	void Set2DCircle(float x, float y, float z, float radius, int color);
	//HRESULT put(LPDIRECT3DTEXTURE9 ptex, float x, float y,int x_on_graph, int y_on_graph, int width, int height, int toukasyoku);

	HRESULT put(LPDIRECT3DTEXTURE9 ptex, float x, float y, float z, int toukasyoku);
	HRESULT put(LPDIRECT3DTEXTURE9 ptex, float x, float y, float z);
	HRESULT put(LPDIRECT3DTEXTURE9 ptex, float x, float y, float z,int x_on_graph, int y_on_graph, int width, int height);
	HRESULT put(LPDIRECT3DTEXTURE9 ptex, float x, float y, float z,int x_on_graph, int y_on_graph, int width, int height, bool muki);
	HRESULT put(LPDIRECT3DTEXTURE9 ptex, float x, float y, float z,int x_on_graph, int y_on_graph, int width, int height, bool muki, int alfa);
	HRESULT putScaling(LPDIRECT3DTEXTURE9 ptex, float x, float y, float z,int x_on_graph, int y_on_graph, int width, int height, bool muki, int alfa, float bairitsux, float bairitsuy);
	HRESULT putScalingSpinning(LPDIRECT3DTEXTURE9 ptex, float x, float y, float z
		, int x_on_graph, int y_on_graph, int width, int height, bool muki, int alfa, float bairitsux, float bairitsuy, float theta);
	HRESULT putScalingSpinningNoAlpha(LPDIRECT3DTEXTURE9 ptex, float x, float y, float z
		, int x_on_graph, int y_on_graph, int width, int height, bool muki, int alfa, float bairitsux, float bairitsuy, float theta);

	HRESULT put(LPDIRECT3DTEXTURE9 ptex, int x, int y, float z, int toukasyoku);
	HRESULT put(LPDIRECT3DTEXTURE9 ptex, int x, int y, float z);
	HRESULT put(LPDIRECT3DTEXTURE9 ptex, int x, int y, float z,int x_on_graph, int y_on_graph, int width, int height);
	HRESULT put(LPDIRECT3DTEXTURE9 ptex, int x, int y, float z,int x_on_graph, int y_on_graph, int width, int height, bool muki);
	HRESULT put(LPDIRECT3DTEXTURE9 ptex, int x, int y, float z,int x_on_graph, int y_on_graph, int width, int height, bool muki, int alfa);
	HRESULT putScaling(LPDIRECT3DTEXTURE9 ptex, int x, int y, float z,int x_on_graph, int y_on_graph, int width, int height, bool muki, int alfa, float bairitsux, float bairitsuy);
	HRESULT drawLine(int orix, int oriy, int terx, int tery, int r, int g, int b, int a, int width);

	HRESULT Render();	//レンダリング
	HRESULT StartRendering();
	HRESULT EndRendering();
	HRESULT Clear(int r, int g, int b, int a);
	HRESULT Whipe(int time);
	HRESULT Whipe(float x, float y, float r);
	HRESULT Cleanup();		//オブジェクト解放
	HRESULT Run(HINSTANCE hInst, char szWinName[], int width, int height, BOOL bWin);
	HRESULT SetRenderTarget(LPDIRECT3DSURFACE9 psurface);
	LPDIRECT3DSURFACE9 getBackBuffer();
	HRESULT ScreenShot(const char* file_name);
	HRESULT reset();
	void resize(int w, int h);
};

// calcs FPS
class FPSChecker
{
public:
	FPSChecker();
	void run();
	inline int GetFrameTime()
	{
		return looptime_ave;
	}
	void adjust();
	bool byougaochi;

private:
	int NumFrame;
	int looptime_ave;
	int looptime_sum;
	int time;
};