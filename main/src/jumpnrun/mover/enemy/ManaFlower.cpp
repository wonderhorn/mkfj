#pragma once
#define _USE_MATH_DEFINES
#include"jumpnrun/mover/enemy/Enemy.h"
#include"jumpnrun/mover/effect/Effect.h"
#include"jumpnrun/mover/Barrier.h"
#include"jumpnrun/mover/Item.h"
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

void ManaFlower::initialize(int refx, int refy
	, gmtr::Vector2D p, OWNERHANDLE owner)
{
	Enemy::initialize(refx, refy, p, owner);
	power = 0;
	hp = hp_max = 10;
	//decel = 0.25;
	height = 32;
	speed_dash = speed_walk = speed_jump = 0;
	//where_to_go = true;
	margin_bottom = margin_side = 0;
	margin_top = 12;
	phys.mass = 1;
	this->turned = false;
	writeName("character_enemy_manaflower", name);
}

void ManaFlower::run(jnr::JumpnRunData& data)
{
	if (age == 60 * 16 + 15)
	{
		Item* ptr1 = generate<Item>(*tl);
		ptr1->initialize(REFPOINT_JEWEL_X, REFPOINT_JEWEL_Y + 32, P(), -1, 10, $V(2, -4), 1);
		ptr1->LifeTime(60 * 10);
		Item* ptr2 = generate<Item>(*tl);
		ptr2->initialize(REFPOINT_JEWEL_X, REFPOINT_JEWEL_Y + 32, P(), -1, 10, $V(0, -4), 1);
		ptr2->LifeTime(60 * 10);
		Item* ptr3 = generate<Item>(*tl);
		ptr3->initialize(REFPOINT_JEWEL_X, REFPOINT_JEWEL_Y + 32, P(), -1, 10, $V(-2, -4), 1);
		ptr3->LifeTime(60 * 10);
	}
	Enemy::run(data);
	status = eStatus::Flying;

}

void ManaFlower::interact(jnr::Character& chara, jnr::JumpnRunData& data)
{
	return;
}

void ManaFlower::think(jnr::JumpnRunData& data){}


void ManaFlower::render(gfw::BaseMedia& media, jnr::JumpnRunData& data)
{
	BaseGraphics* graphics = media.getGraphics();
	int offset_x = 0;
	if (age >= 60 * 8 && age < 60 * 16)
		offset_x = 1;
	if (age >= 60 * 16)
		offset_x = 2;

	int alpha = timer_invinc.moving() ? 0xaaffffff : 0xffffffff;
	graphics->draw($P.img_obj, phys.p.x - data.camera.X(), phys.p.y - data.camera.Y(), 0
		, refx + width * offset_x, refy, width, height, turned, alpha);
	if (Constants::debugging)
	{
		graphics->drawString(gfw::num2str(this->hp), phys.p.x - data.camera.X(), phys.p.y - data.camera.Y() + height, 48, 20);
	}
}