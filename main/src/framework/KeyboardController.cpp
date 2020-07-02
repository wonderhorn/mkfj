#include"framework/VirtualController.h"
using namespace gfw;

KeyboardController::KeyboardController(BaseInput& input) : input(input){}

bool KeyboardController::poll()
{
	z.setValue(input.key_z.isDown());
	x.setValue(input.key_x.isDown());
	c.setValue(input.key_c.isDown());
	a.setValue(input.key_a.isDown());
	s.setValue(input.key_s.isDown());
	space.setValue(input.key_space.isDown());

	up.setValue(input.key_up.isDown());
	right.setValue(input.key_right.isDown());
	down.setValue(input.key_down.isDown());
	left.setValue(input.key_left.isDown());
	return true;
}