#include"framework/Media.h"
#include<limits.h>
using namespace gfw;

Input::Input() : dinput(NULL)
{}

bool Input::initialize(HWND hWnd, WNDCLASSEX wc)
{
	dinput = new DINPUT(&wc, &hWnd);
	return true;
}

void Input::finalize()
{
	SAFE_DELETE(dinput);
}

int Input::deviceNum()const
{
	return dinput->numOfDev;
}

bool Input::poll()
{
	const char* keys = dinput->getkeys();
	if (!keys)
		return false;

	key_z.setValue((keys[DIK_Z] & 0x80) != 0);
	key_x.setValue((keys[DIK_X] & 0x80) != 0);
	key_c.setValue((keys[DIK_C] & 0x80) != 0);
	key_a.setValue((keys[DIK_A] & 0x80) != 0);
	key_s.setValue((keys[DIK_S] & 0x80) != 0);
	key_space.setValue((keys[DIK_SPACE] & 0x80) != 0);
	key_up.setValue((keys[DIK_UP] & 0x80) != 0);
	key_right.setValue((keys[DIK_RIGHT] & 0x80) != 0);
	key_down.setValue((keys[DIK_DOWN] & 0x80) != 0);
	key_left.setValue((keys[DIK_LEFT] & 0x80) != 0);

	for (int i = 0; i < dinput->numOfDev; i++)
	{
		DIJOYSTATE joystate = dinput->getpad(i);
		for (int j = 0; j < 32; j++)
		{
			pads[i].button[j].setValue(joystate.rgbButtons[j] != 0);
		}

		int pov = joystate.rgdwPOV[0];
		bool d = false, u = false, r = false, l = false;
		if (pov != -1)
		{
			int margin = 2000;
			if (pov >= 0 && pov < 4500 + margin || pov >= 31500 - margin && pov < 36000)
				u = u || true;
			if (pov >= 4500 - margin && pov < 13500 + margin)
				r = r || true;
			if (pov >= 13500 - margin && pov < 22500 + margin)
				d = d || true;
			if (pov >= 22500 - margin && pov < 31500 + margin)
				l = l || true;
		}

		d = d || joystate.lY >= USHRT_MAX / 4 * 3;
		u = u || joystate.lY <= USHRT_MAX / 4 * 1;
		r = r || joystate.lX >= USHRT_MAX / 4 * 3;
		l = l || joystate.lX <= USHRT_MAX / 4 * 1;
		pads[i].down.setValue(d);
		pads[i].up.setValue(u);
		pads[i].right.setValue(r);
		pads[i].left.setValue(l);
	}


	return true;
}

void Input::deviceChange() 
{
	dinput->scanDevices();
}


//Input::Button::Button(const std::string& name): value(false), value_prev(false){}