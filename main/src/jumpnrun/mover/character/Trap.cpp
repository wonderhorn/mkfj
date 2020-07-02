#pragma once
#include"jumpnrun/mover/enemy/Enemy.h"
#include"jumpnrun/mover/effect/Effect.h"
#include"jumpnrun/mover/Barrier.h"
#include"jumpnrun/GRAPHICS.h"
#include"jumpnrun/system/Parmanent.h"
#include"jumpnrun/stage/ItemDropper.h"
using namespace jnr;
using namespace enm;
using namespace gfw;
using namespace gmtr;
#define $V Vector2D

RegisterMoverClass(Trap);
RegisterMoverClass(TrapTimered);

void Trap::initialize(int refx, int refy
	, gmtr::Vector2D p, OWNERHANDLE owner, int power, char direnction)
{
	Enemy::initialize(refx, refy, p, owner);
	this->power = power;
	hp = 500;
	speed_walk = 1.2;
	speed_dash = 1.2;
	this->turned = turned;
	interacting_with_items = false;
	width = height = 32;
	this->direction = direction;
	phys.mass = 0;
	breathing = false;
	writeName("trap", name);
}

void Trap::run(jnr::JumpnRunData& data)
{
	timer_invinc = 5;
	auto c = Center();
	bool in_camera = c.x >= data.camera.X() - 128
		&& c.x <= data.camera.X() + data.camera.Width() + 128
		&& c.y >= data.camera.Y() - 64 && c.y <= data.camera.Y() + data.camera.Height() + 64;
	if (in_camera)
		Enemy::run(data);
	else Rect();
	//Enemy::run(data);
}

void Trap::think(jnr::JumpnRunData& data)
{}

void Trap::render(gfw::BaseMedia& media, jnr::JumpnRunData& data)
{
	BaseGraphics* graphics = media.getGraphics();
	graphics->draw($P.img_obj, phys.p.x - data.camera.X(), phys.p.y - data.camera.Y(), 0
		, refx, refy, width, height, turned);
}


#pragma once
#include"jumpnrun/mover/enemy/Enemy.h"
#include"jumpnrun/mover/effect/Effect.h"
#include"jumpnrun/mover/Barrier.h"
#include"jumpnrun/GRAPHICS.h"
#include"jumpnrun/system/Parmanent.h"
#include"jumpnrun/stage/ItemDropper.h"
using namespace jnr;
using namespace enm;
using namespace gfw;
using namespace gmtr;
#define $V Vector2D


void TrapTimered::initialize(int refx, int refy
	, gmtr::Vector2D p, OWNERHANDLE owner, int power, int delay)
{
	Enemy::initialize(refx, refy, p, owner);
	this->power = power;
	hp = 500;
	speed_walk = 1.2;
	speed_dash = 1.2;
	this->turned = turned;
	interacting_with_items = false;
	width = height = 32;
	is_damaging = true;
	interval_on = 120;
	interval_off = 120;
	interval_transit = 30;
	this->delay = delay;
	phys.mass = 0;
	writeName("trap_timerd", name);
}

void TrapTimered::run(jnr::JumpnRunData& data)
{
	timer_invinc = 5;
	int phase = (age - delay) % (interval_on + interval_off + interval_transit * 2);
	is_damaging = phase < interval_on;
	Enemy::run(data);
}

void TrapTimered::think(jnr::JumpnRunData& data)
{}

void TrapTimered::render(gfw::BaseMedia& media, jnr::JumpnRunData& data)
{
	BaseGraphics* graphics = media.getGraphics();
	int dy = 0;
	int phase = (age - delay) % (interval_on + interval_off + interval_transit * 2);
	if (interval_on <= phase && phase < interval_on + interval_transit)
	{
		int dp = phase - interval_on;
		dy = dp * height / interval_transit;
	}
	else if (interval_on + interval_transit <= phase
		&& phase < interval_on + interval_transit + interval_off)
	{
		int dp = interval_transit - 1;
		dy = dp * height / interval_transit;
	}
	else if (interval_on + interval_transit + interval_off <= phase
		&& phase < interval_on + interval_transit * 2 + interval_off)
	{
		int dp = (interval_on + interval_transit * 2 + interval_off) - phase;
		dy = dp * height / interval_transit;
	}

	graphics->draw($P.img_obj, phys.p.x - data.camera.X(), phys.p.y - data.camera.Y() + dy, 0
		, refx, refy, width, height - dy, turned);
}