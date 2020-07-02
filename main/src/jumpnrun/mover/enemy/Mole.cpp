#pragma once
#define _USE_MATH_DEFINES
#include"jumpnrun/mover/enemy/Enemy.h"
#include"jumpnrun/mover/effect/Effect.h"
#include"jumpnrun/mover/Barrier.h"
#include"jumpnrun/GRAPHICS.h"
#include"jumpnrun/system/Parmanent.h"
#include"jumpnrun/MoverUtilities.h"
#include"utils/MyCppUtil.h"
#include<cmath>
#include<functional>
#include<algorithm>
using namespace jnr;
using namespace enm;
using namespace gfw;
using namespace gmtr;
#define $V Vector2D

RegisterMoverClass(Mole);

void Mole::initialize(int refx, int refy
	, gmtr::Vector2D p, OWNERHANDLE owner, bool turned)
{
	Enemy::initialize(refx, refy, p, owner);
	power = 8;
	hp = hp_max = 1;
	ai_timer = 0;
	//decel = 0.25;
	height = 32;
	//where_to_go = true;
	margin_bottom = margin_side = margin_top = 0;
	phys.mass = 0;
	is_damaging = false;
	smashable = false;
	visible_height = 0;
	this->turned = turned;
	hiding = true;
	writeName("character_enemy_mole", name);
}


void Mole::run(jnr::JumpnRunData& data)
{
	Enemy::run(data);
	status = eStatus::Standing;

	if (hiding) 
	{
		visible_height--;
		visible_height = max(0, visible_height);
	}
	if (!hiding)
	{
		visible_height++;
		visible_height = min(height, visible_height);
	}
}

void Mole::think(jnr::JumpnRunData& data)
{
	if (ai_timer)
	{
		ai_timer--;
	}
	else
	{
		//visible_height = 0;
		hiding = true;
	}

	if (($P.rand.get() % (60 * 3)) == 0) 
	{
		ai_timer = 30 + $P.rand.get() % 20;
		hiding = false;
	}

	if (visible_height < 12)
		is_damaging = false;
	else
		is_damaging = true;
}

int Mole::damage(const Damage& d)
{
	if (visible_height < 12)
		return eDamage::Through;
	else
		return Enemy::damage(d);
}

void Mole::render(gfw::BaseMedia& media, jnr::JumpnRunData& data)
{
	BaseGraphics* graphics = media.getGraphics();
	int offset_x = 0;

	int alpha = timer_invinc.moving() ? 0xaaffffff : 0xffffffff;
	graphics->draw($P.img_obj, 
		phys.p.x - data.camera.X(), 
		phys.p.y - data.camera.Y() + (height - visible_height) + 8, 0
		, refx + width * offset_x, refy, width, visible_height, turned, alpha);
	if (Constants::debugging)
	{
		graphics->drawString(gfw::num2str(this->hp), phys.p.x - data.camera.X(), phys.p.y - data.camera.Y() + height, 48, 20);
	}
}