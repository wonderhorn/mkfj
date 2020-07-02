#include"jumpnrun/system/Camera.h"
#include<math.h>
#include<stdlib.h>
using namespace gfw;


void Camera::setPos(int x, int y)
{
	this->x = x, this->y = y;

	if (!outOfStage)
	{
		if (this->x + w >= max_x )
			this->x = max_x - w;
		if (this->y + h >= max_y + 96)
			this->y = max_y - h + 96;

		if (this->x < -64)
			this->x = -64;
		if (this->y < 0)
			this->y = 0;
	}
}
void Camera::chase(int x, int y)
{
	if (quake_timer > 0)
	{
		quake_timer--;
	}
	else
	{
		quake_mag = 0;
	}
	//if (isFixed)
	//	return;
	//int dy = (y - this->y) / 8;
	//dy = dy / 10 * 10;
	int dy = (y - this->y);
	int sign = dy > 0 ? 1 : -1;
	//dy = abs(dy) > 8 ? (abs(dy) - 8) * sign : 0;
	if (abs(dy) < 4)
		dy = 0;
	this->x += (x - this->x) / 8;
	this->y += dy / 4;

	if (!outOfStage)
	{
		if (this->x + w >= max_x)
			this->x = max_x - w;
		if (this->y + h >= max_y + 96)
			this->y = max_y - h + 96;

		if (this->x < -64)
			this->x = -64;
		if (this->y < 0)
			this->y = 0;
	}

	if (quake_timer > 0)
	{
		this->y += quake_mag * (quake_timer / 2 % 2);
	}

	/*if (this->x > x)
	{
	this->x -= speed;
	if (this->x < x)
	this->x = x;
	}
	if (this->x < x)
	{
	this->x += speed;
	if (this->x > x)
	this->x = x;
	}

	if (this->y > y)
	{
	this->y -= speed;
	if (this->y < y)
	this->y = y;
	}
	if (this->y < y)
	{
	this->y += speed;
	if (this->y > y)
	this->y = y;
	}*/
}

void Camera::setQuake(int period, int magnitude)
{
	quake_timer = period;
	quake_mag = magnitude;
}