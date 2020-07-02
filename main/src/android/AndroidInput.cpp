#include"android/AndroidMedia.h"
using namespace gfw;

Input::Input()
	:last_touch_x(-1), last_touch_y(-1)
{}
bool Input::initialize()
{
	return true;
}
void Input::finalize() {}
bool Input::poll()
{

	if (z_x <= last_touch_x  && last_touch_x <= z_x + z_w &&
		z_y <= last_touch_y  && last_touch_y <= z_y + z_h)
	{
		key_z.setValue(true);
		//last_touch_x = -99999;
		//last_touch_y = -99999;
	}
	else
	{
		key_z.setValue(false);
	}

	if (x_x <= last_touch_x  && last_touch_x <= x_x + x_w &&
		x_y <= last_touch_y  && last_touch_y <= x_y + x_h)
	{
		key_x.setValue(true);
		//last_touch_x = -99999;
		//last_touch_y = -99999;
	}
	else
	{
		key_x.setValue(false);
	}

	if (up_x <= last_touch_x  && last_touch_x <= up_x + up_w &&
		up_y <= last_touch_y  && last_touch_y <= up_y + up_h)
	{
		key_up.setValue(true);
		//last_touch_x = -99999;
		//last_touch_y = -99999;
	}
	else
	{
		key_up.setValue(false);
	}

	if (right_x <= last_touch_x  && last_touch_x <= right_x + right_w &&
		right_y <= last_touch_y  && last_touch_y <= right_y + right_h)
	{
		key_right.setValue(true);
		//last_touch_x = -99999;
		//last_touch_y = -99999;
	}
	else
	{
		key_right.setValue(false);
	}

	if (down_x <= last_touch_x  && last_touch_x <= down_x + down_w &&
		down_y <= last_touch_y  && last_touch_y <= down_y + down_h)
	{
		key_down.setValue(true);
		//last_touch_x = -99999;
		//last_touch_y = -99999;
	}
	else
	{
		key_down.setValue(false);
	}

	if (left_x <= last_touch_x  && last_touch_x <= left_x + left_w &&
		left_y <= last_touch_y  && last_touch_y <= left_y + left_h)
	{
		key_left.setValue(true);
		//last_touch_x = -99999;
		//last_touch_y = -99999;
	}
	else
	{
		key_left.setValue(false);
	}

	return true;
}

void Input::touch(int x, int y) {
	last_touch_x = x;
	last_touch_y = y;
}

void Input::setZRect(int z_x, int z_y, int  z_w, int z_h)
{
	this->z_x = z_x;
	this->z_y = z_y;
	this->z_w = z_w;
	this->z_h = z_h;
}


void Input::setXRect(int z_x, int z_y, int  z_w, int z_h)
{
	this->x_x = z_x;
	this->x_y = z_y;
	this->x_w = z_w;
	this->x_h = z_h;
}


void Input::setUpRect(int z_x, int z_y, int  z_w, int z_h)
{
	this->up_x = z_x;
	this->up_y = z_y;
	this->up_w = z_w;
	this->up_h = z_h;
}


void Input::setRightRect(int z_x, int z_y, int  z_w, int z_h)
{
	this->right_x = z_x;
	this->right_y = z_y;
	this->right_w = z_w;
	this->right_h = z_h;
}


void Input::setDownRect(int z_x, int z_y, int  z_w, int z_h)
{
	this->down_x = z_x;
	this->down_y = z_y;
	this->down_w = z_w;
	this->down_h = z_h;
}


void Input::setLeftRect(int z_x, int z_y, int  z_w, int z_h)
{
	this->left_x = z_x;
	this->left_y = z_y;
	this->left_w = z_w;
	this->left_h = z_h;
}