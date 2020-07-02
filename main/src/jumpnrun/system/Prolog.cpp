#pragma once
#include"framework/Control.h"
//#include"Media.h"
#include"jumpnrun/system/Parmanent.h"
#include"framework/Screen.h"
#include"jumpnrun/GRAPHICS.h"
using namespace gfw;
using namespace jnr;
using std::max;
using std::min;

Prolog::Prolog()
	: CONTROL(), age(0)
{}
eControl Prolog::run()
{
	if (age >= 60*10)
	{
		isActive = false;
		return eControl::Continue;
	}

	age++;
	return eControl::Continue;
}
void Prolog::render(gfw::BaseMedia& media)
{
	int a = max(0, 255 - age * 8);
	//graphics->clear(0, 0, 0, a);

	/*int offset_refx = 0;

	if (age / 120 % 2 == 1)
		offset_refx = 1;

	//graphics->draw($P.img_obj, 64 * 4, 64 * 3, 0
	//	, REFPOINT_SLEEPING_X, REFPOINT_SLEEPING_Y, 0, 0, false);
	graphics->draw($P.img_obj, 64 * 4, 64 * 3 + 5, 0
		, REFPOINT_SLEEPING_X + offset_refx * 64, REFPOINT_SLEEPING_Y, 64, 64, false);*/


	//Cd3d9->Whipe(ME::mypoint.getx() - CAMERA::getx(), ME::mypoint.gety() -  CAMERA::gety(), 1024 - age * 32);
}