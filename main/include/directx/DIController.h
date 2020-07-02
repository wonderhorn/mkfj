#pragma once

#include<dinput.h>



BOOL WINAPI DIEnumDevicesCallback(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef);
BOOL WINAPI DIEnumDevObjCallback(LPCDIDEVICEOBJECTINSTANCE lpddoi, LPVOID pvRef);


class DINPUT
{
	friend BOOL WINAPI DIEnumDevicesCallback(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef);
	friend BOOL WINAPI DIEnumDevObjCallback(LPCDIDEVICEOBJECTINSTANCE lpddoi, LPVOID pvRef);

private:
	static LPDIRECTINPUT8 pDI;
	static LPDIRECTINPUTDEVICE8 pDIDevice[256];
	static LPDIRECTINPUTDEVICE8 pDIDevice_Keyboard;

	static DIDEVICEINSTANCE* DevIns[256];
	static DIDEVICEOBJECTINSTANCE* ObjIns[256];

	DIJOYSTATE state;

public:
	DINPUT(WNDCLASSEX* pwc,  HWND* phWnd);
	~DINPUT();
	static int numOfDev;
	static int numOfObj;
	const char* getkeys();
	DIJOYSTATE getpad(int i);
	int getCurrentButton(int i);
	void scanDevices();
	HWND hwnd;

	static DINPUT* dinput;
};

class MYINPUT
{
public:
	MYINPUT();
	virtual void run();
	bool input_up, input_down, input_right, input_left, input_x, input_z, input_c, input_c2, input_a, input_s, input_space;
	bool pressed_up, pressed_down, pressed_right, pressed_left, pressed_x, pressed_z, pressed_c, pressed_c2, pressed_a, pressed_s, pressed_space;
	bool up_up, up_down, up_right, up_left, up_x, up_z, up_dir, up_c, up_c2, up_a, up_s, up_space;
	//bool up_up, up_down, up_right, up_left, up_x, up_z, up_dir, up_c, up_c2, up_s;
	bool raised_up, raised_down, raised_right, raised_left, raised_x, raised_z, raised_c, raised_c2, raised_a, raised_s, raised_space;

protected:

};

class INPUT_RANDOM: public MYINPUT
{
public:
	INPUT_RANDOM();
	void run();
};