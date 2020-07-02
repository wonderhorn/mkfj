#include"framework/VirtualController.h"
using namespace gfw;

JoyPadController::JoyPadController(BaseInput& input) : input(input)
{
	btnidx_z = 0;
	btnidx_x = 1;
	btnidx_c = 2;
	btnidx_a = 3;
	btnidx_s = 4;
	btnidx_space = 5;
	pad_id = 0;
}

int JoyPadController::getPushedButtonId()
{
	for (int i = 0; i < 32; i++)
	{
		if (input.pads[pad_id].button[i].isDown())
		{
			return i;
		}
	}
	return -1;
}

bool JoyPadController::poll()
{
	// detect used pad
	for (int i = 0; i < 256; i++) 
	{
		for (int j = 0; j < 32; j++) 
		{
			if (input.pads[i].button[j].isDown())
				pad_id = i;
		}

	}
	z.setValue(input.pads[pad_id].button[btnidx_z].isDown());
	x.setValue(input.pads[pad_id].button[btnidx_x].isDown());
	c.setValue(input.pads[pad_id].button[btnidx_c].isDown());
	a.setValue(input.pads[pad_id].button[btnidx_a].isDown());
	s.setValue(input.pads[pad_id].button[btnidx_s].isDown());
	space.setValue(input.pads[pad_id].button[btnidx_space].isDown());

	up.setValue(input.pads[pad_id].up.isDown());
	right.setValue(input.pads[pad_id].right.isDown());
	down.setValue(input.pads[pad_id].down.isDown());
	left.setValue(input.pads[pad_id].left.isDown());
	return true;
}