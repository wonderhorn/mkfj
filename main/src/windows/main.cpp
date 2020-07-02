//#include<WinSock2.h>
//#include"DXheaders.h"
#include"utils/MyUtil.h"
//#include"subwindow.h"
#include"framework/Game.h"
#include"jumpnrun/system/Parmanent.h"
#include"jumpnrun/ai/AIOptimizer.h"
#include"jumpnrun/spell/Deck.h"
#include"utils/MyIO.h"
#include"utils/vecutil.h"
#include<Windows.h>
#include<iostream>
#include<assert.h>

//prototype dec.
HRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
HRESULT WINAPI WndProc_sub(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
HRESULT WINAPI DialogProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

HWND InitWindow(bool bWin, char szWinName[], int width, int height);
HWND initSubWindow(bool bWin, char szWinName[], int width, int height, HWND parent);

HWND hWnd, hWnd_sub;
WNDCLASSEX wc;
int winx = 00, winy = 00;
bool initialized = false;
int fin_options = 0;

MSG processMessage()
{
	MSG msg;
	while (PeekMessage(&msg, NULL, 0U, 0U, PM_NOREMOVE))
	{
		if (msg.message == WM_QUIT) break;

		GetMessage(&msg, NULL, 0, 0);
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg;
}

#include"windows/game_main.h"
//#include"replay_main.h"
//#include"evolution_main.h"
//#include"evolution_main2.h"
//#include"simple_test.h"

HRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	long evCode = 0;
	LONG param1 = 0, param2 = 0;
	//HDC hDC;
	//PAINTSTRUCT ps;
	HINSTANCE hInst = (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE);
	switch (msg)
	{
	case WM_GRAPH_NOTIFY:

		//BGM::proc(evCode, param1, param2);
		$P.media.musics.checkLoop(evCode, param1, param2);
		break;
	case WM_DESTROY:
	{
		RECT r;
		GetWindowRect(hWnd, &r);
		$P.window_x = r.left;
		$P.window_y = r.top;
	}
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
	{
		RECT r;
		GetWindowRect(hWnd, &r);
		$P.window_x = r.left;
		$P.window_y = r.top;
	}
		switch (wParam)
		{
		case VK_ESCAPE:
		{
			//�G�X�P�[�v�ŏI��
			HWND hwDialog;
			MSG msg;

			/* �_�C�A���O�{�b�N�X�쐬 */
			
			hwDialog = CreateWindow("CDialog", "�I�����܂����H",
				WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT,
				256, 96, NULL, NULL, hInst, NULL);
			EnableWindow(hWnd, FALSE);
			while (fin_options == 0) 
			{ /* �_�C�A���O�����ʂ�Ԃ��܂Ń��[�v */

				GetMessage(&msg, NULL, 0, 0);

				TranslateMessage(&msg);
				DispatchMessage(&msg);

			}
			if (fin_options == 1) 
			{
				PostQuitMessage(0);
			}
			else 
			{
				EnableWindow(hWnd, TRUE);
			}
			break;
		}
		case VK_F1:
			$P.media.graphics.screenshot("sstest.bmp");
			break;
		}

	case WM_ACTIVATE:
		if (wParam == WA_INACTIVE)
		{

		}
		if ((wParam == WA_ACTIVE) || (wParam == WA_CLICKACTIVE))
		{

		}
		break;
	//case WM_SIZING:
	case WM_EXITSIZEMOVE:
	//case WM_SIZE:
	//case WM_
		if (initialized)
		{
			RECT rw, rc;
			GetWindowRect(hWnd, &rw);
			GetClientRect(hWnd, &rc);
			int current_width = rc.right - rc.left;
			int current_height = rc.bottom - rc.top;
			if (current_width > 0 && current_height > 0)
			{
				$P.current_window_w = current_width;
				$P.current_window_h = current_height;
				jnr::Parmanent::Instance().media.graphics.resizeWindow(current_width, current_height);
				//jnr::Parmanent::Instance().media.graphics.resizeWindow(current_width, current_height);
				//jnr::Parmanent::Instance().media.graphics.resizeWindow(640, 480);
			}
		}
		break;
	case WM_MOVE:
		{
			RECT r;
			GetWindowRect(hWnd, &r);
			$P.window_x = r.left;
			$P.window_y = r.top;
			HMONITOR hMonitor = MonitorFromRect(
				&r, MONITOR_DEFAULTTONEAREST);
			MONITORINFO mi;
			mi.cbSize = sizeof(MONITORINFO);
			GetMonitorInfo(hMonitor, &mi);

			//if out of the nearest monitor, go back
			bool wout = false;
			if ($P.window_x + $P.window_w <= mi.rcMonitor.left)
			{
				wout = true;
				r.left = mi.rcMonitor.left;
			}
			if ($P.window_x >= mi.rcMonitor.right)
			{
				wout = true;
				r.left = mi.rcMonitor.right - $P.window_w;
			}
			if ($P.window_y + $P.window_h <= mi.rcMonitor.top)
			{
				wout = true;
				r.top = mi.rcMonitor.top;
			}
			if ($P.window_y >= mi.rcMonitor.bottom)
			{
				wout = true;
				r.top = mi.rcMonitor.bottom - $P.window_h;
			}

			if (wout)
			{
				SetWindowPos(
					hWnd, NULL, r.left, r.top,
					$P.window_w, $P.window_h,
					SWP_NOZORDER);
			}
		}
		break;
	case WM_MOUSEMOVE:
	{
		tagPOINT point;
		GetCursorPos(&point);
		$P.mouse_x = point.x - $P.window_x;
		$P.mouse_y = point.y - $P.window_y;
	}
		break;

	case WM_DEVICECHANGE:
		$P.media.input.deviceChange();
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

HWND InitWindow(bool bWin, char szWinName[], int width, int height)
{
	outputLog("�E�B���h�E���쐬���܂��B");

	HWND hWnd = NULL;

	// �E�B���h�E�N���X�ݒ�
	wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L,
		GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
		szWinName, NULL };
	int error = RegisterClassEx(&wc);

	int res;
	// �E�B���h�E����
	if (bWin)
	{
		//Window���[�h
		hWnd = CreateWindow(szWinName, szWinName,
			WS_OVERLAPPEDWINDOW , CW_USEDEFAULT, CW_USEDEFAULT, width, height,
			GetDesktopWindow(), NULL, wc.hInstance, NULL);
		if (hWnd == NULL)outputLog("�E�B���h�E�쐬�Ɏ��s���܂����B");
		else outputLog("�E�B���h�E�쐬�ɐ������܂����B");

		RECT rw, rc;
		GetWindowRect(hWnd, &rw);
		GetClientRect(hWnd, &rc);
		int current_width = rw.right - rw.left;
		int current_height = rw.bottom - rw.top;
		int new_width = current_width + width - (rc.right - rc.left);
		int new_height = current_height + height - (rc.bottom - rc.top);
		res = SetWindowPos(hWnd, HWND_TOP, $P.window_x, $P.window_y, new_width, new_height, SWP_NOZORDER);
		assert(res != 0);
		//AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME, 0);*/
	}
	else
	{
		//�t���X�N���[��
		hWnd = CreateWindow(szWinName, szWinName,
			WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, width, height,
			HWND_DESKTOP, NULL, wc.hInstance, NULL);
		if (hWnd == NULL)outputLog("�t���X�N���[�����[�h�ł̃E�B���h�E�쐬�Ɏ��s���܂����B");
		else outputLog("�t���X�N���[�����[�h�ł̃E�B���h�E�쐬�ɐ������܂����B");
	}
	ValidateRect(hWnd, 0);

	//�I���_�C�A���O
	
	wc = { sizeof(WNDCLASSEX), CS_HREDRAW | CS_VREDRAW, DialogProc, 0L, 0L,
		GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
		"CDialog", NULL };
	error = RegisterClassEx(&wc);

	RegisterClassEx(&wc);

	return hWnd;
}

HWND initSubWindow(bool bWin, char szWinName[], int width, int height, HWND parent)
{
	HWND hWnd = NULL;

	// �E�B���h�E�N���X�ݒ�
	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc_sub, 0L, 0L,
		GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
		szWinName, NULL };
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	int error = RegisterClassEx(&wc);
	int d = GetLastError();

	hWnd = CreateWindow(szWinName, szWinName,
		WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME | WS_SIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT, width, height,
		parent, NULL, wc.hInstance, NULL);
	ValidateRect(hWnd, 0);
	return hWnd;
}

HRESULT WINAPI WndProc_sub(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	long evCode = 0;
	LONG param1 = 0, param2 = 0;
	HDC hDC;
	PAINTSTRUCT ps;
	switch (msg)
	{
	case WM_DESTROY:
		//iloop = 0;
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			//�G�X�P�[�v�ŏI��
			//iloop = 0;
			PostQuitMessage(0);
			break;
		}
	case WM_PAINT:
		//case WM_ACTIVATE:
		/*
		hDC = BeginPaint(hWnd_sub, &ps);
		SUBWINDOW subwindow = SUBWINDOW::getInstance();
		subwindow.paint(hDC);
		EndPaint(hWnd_sub, &ps);
		*/
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

HRESULT WINAPI DialogProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam) 
{

	HWND btN1, btN2;
	static LPDWORD lpRes;
	HINSTANCE hInst = (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE);

	if (hwnd == NULL) {
		lpRes = (LPDWORD)iMsg;
		*lpRes = 0;
		return 0;
	}

	switch (iMsg) {

	case WM_CREATE: /* �E�C���h�E�쐬���̏��� */
	{
		btN1 = CreateWindow("Button", "�͂�", WS_CHILD | WS_VISIBLE,
			28, 8, 80, 24, hwnd, (HMENU)0, hInst, NULL);
		btN2 = CreateWindow("Button", "�L�����Z��", WS_CHILD | WS_VISIBLE,
			126, 8, 80, 24, hwnd, (HMENU)1, hInst, NULL);
		RECT r;
		GetWindowRect(hWnd, &r);
		SetWindowPos(hwnd, HWND_TOP, 
			$P.window_x + $P.current_window_w / 2 - 140, 
			$P.window_y + $P.current_window_h / 2 -40, 
			280, 80, SWP_NOZORDER);
	}
		return 0;

	case WM_COMMAND: /* �R�}���h���b�Z�[�W */
		switch (LOWORD(wParam)) {
		case 0: /* �P�ԃ{�^�� */

			fin_options = 1;
			DestroyWindow(hwnd);
			break;

		case 1: /* �Q�ԃ{�^�� */
			fin_options = 2;
			DestroyWindow(hwnd);
			break;
		}
		return 0;
	case WM_DESTROY: /* �I������ */

		if (fin_options == 0) /* �ǂ�����I������Ă��Ȃ����-1 */
			fin_options = -1;
		return 0;
	}

	return DefWindowProc(hwnd, iMsg, wParam, lParam);

}