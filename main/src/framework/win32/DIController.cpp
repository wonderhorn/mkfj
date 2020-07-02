//#include "headers.h"
#include"directx/DIController.h"
#include"utils/MyUtil.h"
#include<limits.h>
int BUTTON_X = 3;
int BUTTON_Z = 2;
int BUTTON_C = 7;
int BUTTON_C2 = 8;
int BUTTON_S = 9;
int BUTTON_A = 10;
int BUTTON_SPACE = 11;

DINPUT* DINPUT::dinput = NULL;
int DINPUT::numOfDev = 0;
int DINPUT::numOfObj = 0;
DIDEVICEINSTANCE* DINPUT::DevIns[] = {};
DIDEVICEOBJECTINSTANCE* DINPUT::ObjIns[] = {};
LPDIRECTINPUT8 DINPUT::pDI = NULL;
LPDIRECTINPUTDEVICE8 DINPUT::pDIDevice[] = {};
LPDIRECTINPUTDEVICE8 DINPUT::pDIDevice_Keyboard = NULL;

DINPUT::DINPUT(WNDCLASSEX* pwc,  HWND* phWnd)
{
	pDI = NULL;
	outputLog("DirectInputを初期化します。");
	hwnd = *phWnd;
	if(FAILED(DirectInput8Create(pwc->hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&pDI, NULL)))
	{
		outputLog("DirectInput8オブジェクトの生成に失敗しました。");
		pDI = NULL;
		return;
	}


	numOfDev = 0;
	//DevIns = new DIDEVICEINSTANCE*[32];
	if(FAILED(pDI->EnumDevices(DI8DEVCLASS_GAMECTRL, DIEnumDevicesCallback, DevIns, DIEDFL_ATTACHEDONLY)))
	{
		outputLog("DirectInput8オブジェクトの生成に失敗しました。");
		//DevIns = NULL;
		return;
	}

	for (int i = 0; i < numOfDev; i++)
	{

		//if(FAILED(pDI->CreateDevice(DevIns[0]->guidInstance, &pDIDevice, NULL))) pDIDevice = NULL;
		pDIDevice[i]->SetDataFormat(&c_dfDIJoystick);

		DIPROPDWORD diprop;
		diprop.diph.dwSize = sizeof(DIPROPDWORD); // 使用される構造体のサイズ
		diprop.diph.dwHeaderSize = sizeof(DIPROPHEADER); // DIPROPHEADER構造体のサイズ
		diprop.diph.dwHow = DIPH_DEVICE; // 対象（ここではデバイス）
		diprop.diph.dwObj = 0; // デバイスを対象とする時はいつも0
		diprop.dwData = DIPROPAXISMODE_ABS; // 絶対値モードに設定

		numOfObj = 0;
		//ObjIns = new DIDEVICEOBJECTINSTANCE*[32];
		pDIDevice[i]->EnumObjects(DIEnumDevObjCallback, ObjIns, DIDFT_ALL);
	}

	if(FAILED(pDI->CreateDevice(GUID_SysKeyboard, &pDIDevice_Keyboard, NULL)))
	{
		pDIDevice_Keyboard = NULL;
		outputLog("キーボードの取得に失敗しました。");
	}
	else
	{
		pDIDevice_Keyboard->SetDataFormat(&c_dfDIKeyboard);
		outputLog("キーボードの取得に成功しました。");
	}


	for (int i = 0; i < numOfDev; i++)
	{
		pDIDevice[i]->SetCooperativeLevel(*phWnd, DISCL_NONEXCLUSIVE);
	}
	if(pDIDevice_Keyboard!= NULL)pDIDevice_Keyboard->SetCooperativeLevel(*phWnd, DISCL_NONEXCLUSIVE);

	if(pDI)
		outputLog("DirectInputの初期化を終了します。");
}

BOOL WINAPI DIEnumDevicesCallback(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef)
{
	DINPUT::DevIns[DINPUT::numOfDev] = (DIDEVICEINSTANCE*)pvRef;
	std::string productname = DINPUT::DevIns[DINPUT::numOfDev]->tszProductName;
	HRESULT hr = DINPUT::pDI->CreateDevice(lpddi->guidInstance, &DINPUT::pDIDevice[DINPUT::numOfDev], NULL);
	DINPUT::numOfDev++;
	outputLog("入力デバイスを取得しました: " + productname);

	return DIENUM_CONTINUE;

}
BOOL WINAPI DIEnumDevObjCallback(LPCDIDEVICEOBJECTINSTANCE lpddoi, LPVOID pvRef)
{
	DINPUT::ObjIns[DINPUT::numOfObj] =  (DIDEVICEOBJECTINSTANCE*)pvRef;
	//lpddoi[DINPUT::numOfObj] = (DIDEVICEOBJECTINSTANCE*)pvRef;

	DIPROPRANGE diprg;

	diprg.diph.dwSize       = sizeof(DIPROPRANGE);
	diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	diprg.diph.dwHow        = DIPH_BYID;
	diprg.diph.dwObj        = DINPUT::ObjIns[DINPUT::numOfObj]->dwType;
	diprg.lMin              = -1000;
	diprg.lMax              = +1000;


	HRESULT hr = DINPUT::pDIDevice[0]->SetProperty(DIPROP_RANGE, &diprg.diph);
	//RESULT hr = DINPUT::ObjIns[DINPUT::numOfObj]->SetProperty(DIPROP_RANGE, &diprg.diph);
	DINPUT::numOfObj++;
	return DIENUM_CONTINUE;
}

int DINPUT::getCurrentButton(int i)
{
	if(pDIDevice == NULL)
		return -1;
	if(GetForegroundWindow() != hwnd)return -1;

	ZeroMemory(&state, sizeof(DIJOYSTATE));
	HRESULT hr = pDIDevice[i]->Acquire();
	hr = pDIDevice[i]->Poll();
	hr = pDIDevice[i]->GetDeviceState(sizeof(DIJOYSTATE), &state);
	for(int i = 0; i < numOfObj; i++)
	{
		if(state.rgbButtons[i])
			return i;
	}
	return -1;
}

DINPUT::~DINPUT()
{
	for (int i = 0; i < numOfDev; i++)
	{
		pDIDevice[i]->Unacquire();
		pDIDevice[i]->Release();
	}
	if(pDIDevice_Keyboard != NULL)
	{
		pDIDevice_Keyboard->Unacquire();
		pDIDevice_Keyboard->Release();
	}
	if(pDI != NULL) pDI->Release();
}

const char* DINPUT::getkeys()
{
	if(pDIDevice_Keyboard == NULL)
		return NULL;
	if(GetForegroundWindow() != hwnd)
		return NULL;

	static char keys[256];
	HRESULT hr = pDIDevice_Keyboard->Acquire();
	if((hr == DI_OK)||(hr == S_FALSE))
	{
		pDIDevice_Keyboard->GetDeviceState(sizeof(keys), &keys);
		return keys;
	}

	return NULL;
}

DIJOYSTATE DINPUT::getpad(int i)
{

	DIJOYSTATE state;
	ZeroMemory(&state, sizeof(DIJOYSTATE));
	state.lX = state.lY = USHRT_MAX / 2;
	if(GetForegroundWindow() != hwnd)
		return state;
	if(pDIDevice[i] == NULL)
		return state;

	HRESULT hr = pDIDevice[i]->Acquire();
	hr = pDIDevice[i]->Poll();
	hr = pDIDevice[i]->GetDeviceState(sizeof(DIJOYSTATE), &state);
	return state;
}

MYINPUT::MYINPUT()
{
	input_up = 0, input_down = 0, input_right = 0, input_left = 0, input_x = 0, input_z = 0, input_c = 0, input_c2 = 0, input_a = 0, input_s = 0, input_space = 0;
	pressed_up = 0, pressed_down = 0, pressed_right = 0, pressed_left = 0, pressed_x = 0, pressed_z = 0, pressed_c = 0, pressed_c2 = 0, pressed_a = 0, pressed_s = 0, pressed_space = 0;
	up_up = 0, up_down = 0, up_right = 0, up_left = 0, up_x = 0, up_z = 0, up_c = 0, up_c2 = 0, up_a = 0, up_s = 0, up_space = 0;
	raised_up = 0, raised_down = 0, raised_right = 0, raised_left = 0, raised_x = 0, raised_z = 0, raised_c = 0, raised_c2 = 0, raised_a = 0, raised_s = 0, raised_space = 0;
}


void MYINPUT::run()
{
	input_up = 0, input_down = 0, input_right = 0, input_left = 0, input_z = 0, input_c = 0, input_c2 = 0, input_a = 0, input_s = 0, input_space = 0;
	pressed_up = 0, pressed_down = 0, pressed_right = 0, pressed_left = 0, pressed_x = 0, pressed_z = 0, pressed_c = 0, pressed_c2 = 0, pressed_a = 0, pressed_s = 0, pressed_space = 0;
	raised_up = 0, raised_down = 0, raised_right = 0, raised_left = 0, raised_x = 0, raised_z = 0, raised_c = 0, raised_c2 = 0, raised_a = 0, raised_s = 0, raised_space = 0;
	//up_up = 0, up_down = 0, up_right = 0, up_left = 0, up_x = 0, up_z = 0, up_dir = 0;


	DIJOYSTATE pad = DINPUT::dinput->getpad(0);
	const char* keys = DINPUT::dinput->getkeys();

	if (DINPUT::dinput->numOfDev != 0)
		if(!keys || !((keys[DIK_RIGHT]&0x80)||(keys[DIK_DOWN]&0x80)||(keys[DIK_LEFT]&0x80)||(keys[DIK_UP]&0x80)||(keys[DIK_Z]&0x80)||(keys[DIK_X]&0x80)||(keys[DIK_C]&0x80)||(keys[DIK_V]&0x80)||(keys[DIK_A]&0x80||(keys[DIK_S]&0x80))))
	{
		if((pad.lY>40000))
		{
			input_down = 1;
				if(up_down)
				{
					pressed_down=1;
					up_down = 0;
				}
			}
			else up_down = 1;
			if((pad.lY<20000))
			{
				input_up = 1;
				if(up_up)
				{
					pressed_up=1;
					up_up = 0;
				}
			}
			else up_up = raised_up = 1;
			if((pad.lX>40000))
			{
				input_right = 1;
				if(up_right)
				{
					pressed_right=1;
					up_right = 0;
				}
			}
			else up_right = raised_right = 1;
			if((pad.lX<20000))
			{
				input_left = 1;
				if(up_left)
				{
					pressed_left=1;
					up_left = 0;
				}
			}
			else up_left = 1;
			if((pad.rgbButtons[BUTTON_X]))
			{
				input_x = 1;
				if(up_x)
				{
					pressed_x = 1;
					up_x = 0;
				}
			}
			else
			{
				up_x = 1;
				if(input_x)
				{
					raised_x = 1;
				}
				input_x = 0;
			}
			if(pad.rgbButtons[BUTTON_Z])
			{
				input_z = 1;
				if(up_z)
				{
					pressed_z = 1;
					up_z = 0;
				}
			}
			else
			{
				up_z = raised_z = 1;
			}

			if(pad.rgbButtons[BUTTON_C])
			{
				input_c = 1;
				if(up_c)
				{
					pressed_c = 1;
					up_c = 0;
				}
			}
			else
			{
				up_c = raised_c = 1;
			}

			if(pad.rgbButtons[BUTTON_C2])
			{
				input_c2 = 1;
				if(up_c2)
				{
					pressed_c2 = 1;
					up_c2 = 0;
			}
		}
		else
		{
			up_c2 = raised_c2 = 1;
		}

		if(pad.rgbButtons[BUTTON_A])
		{
			input_a = 1;
			if(up_a)
			{
				pressed_a = 1;
				up_a = 0;
			}
		}
		else
		{
			up_a = raised_a = 1;
		}

		if(pad.rgbButtons[BUTTON_S])
		{
			input_s = 1;
			if(up_s)
			{
				pressed_s = 1;
				up_s = 0;
			}
		}
		else
		{
			up_s = raised_s = 1;
		}

		if(pad.rgbButtons[BUTTON_SPACE])
		{
			input_space = 1;
			if(up_space)
			{
				pressed_space = 1;
				up_space = 0;
			}
		}
		else
		{
			up_space = raised_space = 1;
		}
	}



	if((input_up || input_down || input_right || input_left) == 0)
	{

		if(keys != NULL)
		{
			if((keys[DIK_DOWN]&0x80))
			{
				input_down = 1;
				if(up_down)
				{
					pressed_down=1;
					up_down = 0;
				}
			}
			else
			{
				up_down = raised_down = 1;
			}
			if((keys[DIK_UP]&0x80))
			{
				input_up = 1;
				if(up_up)
				{
					pressed_up=1;
					up_up = 0;
				}
			}
			else
			{
				up_up = raised_up = 1;
			}
			if((keys[DIK_RIGHT]&0x80))
			{
				input_right = 1;
				if(up_right)
				{
					pressed_right=1;
					up_right = 0;
				}
			}
			else up_right = raised_right = 1;
			if((keys[DIK_LEFT]&0x80))
			{
				input_left = 1;
				if(up_left)
				{
					pressed_left=1;
					up_left = 0;
				}
			}
			else up_left = raised_left = 1;
			if((keys[DIK_X]&0x80))
			{
				input_x = 1;
				if(up_x)
				{
					pressed_x = 1;
					up_x = 0;
				}
			}
			else
			{
				up_x = 1;
				if(input_x)
				{
					raised_x = 1;
				}
				input_x = 0;
			}

			if(keys[DIK_Z]&0x80)
			{
				input_z = 1;
				if(up_z)
				{
					pressed_z = 1;
					up_z = 0;
				}
			}
			else
			{
				up_z = raised_z = 1;
			}

			if(keys[DIK_C]&0x80)
			{
				input_c = 1;
				if(up_c)
				{
					pressed_c = 1;
					up_c = 0;
				}
			}
			else
			{
				up_c = raised_c = 1;
			}

			if(keys[DIK_V]&0x80)
			{
				input_c2 = 1;
				if(up_c2)
				{
					pressed_c2 = 1;
					up_c2 = 0;
				}
			}
			else
			{
				up_c2 = raised_c2 = 1;
			}

			if(keys[DIK_A]&0x80)
			{
				input_a = 1;
				if(up_a)
				{
					pressed_a = 1;
					up_a = 0;
				}
			}
			else
			{
				up_a = raised_a = 1;
			}

			if(keys[DIK_S]&0x80)
			{
				input_s = 1;
				if(up_s)
				{
					pressed_s = 1;
					up_s = 0;
				}
			}
			else
			{
				up_s = raised_s = 1;
			}

			if(keys[DIK_SPACE]&0x80)
			{
				input_space = 1;
				if(up_space)
				{
					pressed_space = 1;
					up_space = 0;
				}
			}
			else
			{
				up_space = raised_space = 1;
			}
		}
		}
}

INPUT_RANDOM::INPUT_RANDOM():MYINPUT()
{}
void INPUT_RANDOM::run()
{
	input_up = 0, input_down = 0, input_right = 0, input_left = 0, input_x = 0, input_z = 0, input_c = 0;
	pressed_up = 0, pressed_down = 0, pressed_right = 0, pressed_left = 0, pressed_x = 0, pressed_z = 0, pressed_c = 0;
	int ransu = rand();
	if((ransu & (1)))
	{
		input_down = 1;
		if(up_down)
		{
			pressed_down=1;
			up_down = 0;
		}
	}
	else up_down = 1;
	if((ransu & (1<<1)))
	{
		input_up = 1;
		if(up_up)
		{
			pressed_up=1;
			up_up = 0;
		}
	}
	else up_up = 1;
	if((ransu & (1<<2)))
	{
		input_right = 1;
		if(up_right)
		{
			pressed_right=1;
			up_right = 0;
		}
	}
	else up_right = 1;
	if((ransu & (1<<3)))
	{
		input_left = 1;
		if(up_left)
		{
			pressed_left=1;
			up_left = 0;
		}
	}
	else up_left = 1;
	if((ransu & (1<<3)))
	{
		input_x = 1;
		if(up_x)
		{
			pressed_x = 1;
			up_x = 0;
		}
	}
	else up_x = 1;

	if((ransu & (1<<4)))
	{
		input_z = 1;
		if(up_z)
		{
			pressed_z = 1;
			up_z = 0;
		}
	}
	else
	{
		up_z = 1;
	}
	if((ransu & (1<<5)))
	{
		input_c = 1;
		if(up_c)
		{
			pressed_c = 1;
			up_c = 0;
		}
	}
	else
	{
		up_c = 1;
	}
}