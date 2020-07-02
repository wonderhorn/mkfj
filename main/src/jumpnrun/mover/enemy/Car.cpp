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

RegisterMoverClass(Car);

void Car::initialize(int refx, int refy, int width, int height
	, gmtr::Vector2D p, OWNERHANDLE owner, bool turned)
{
	Enemy::initialize(refx, refy, p, owner);
	this->width = width;
	this->height = height;
	power = 15;
	hp = 35;
	ai_timer = 0;
	speed_walk = 4;
	speed_dash = 4;
	this->turned = this->where_to_go = turned;
	writeName("character_enemy_car", name);
}

void Car::run(jnr::JumpnRunData& data)
{
	Enemy::run(data);
	if (status == eStatus::Standing || status == eStatus::Walking || status == eStatus::Dashing)
	{
		if (animation.finished)
		{
			if (fabs(phys.v.x) > 0.0)
			{
				if (fabs(v_target.x) <= speed_walk)
				{
					animation.reset(refx + width, refy, 12, 2, width, height);
					status = eStatus::Walking;
				}
				else
				{
					status = eStatus::Dashing;
					animation.reset(refx + width, refy, 10, 2, width, height);
				}
			}
			else
			{
				animation.reset(refx, refy, 2, 1, width, height);
				status = eStatus::Standing;
			}
		}
		if (!isOnSomething)
		{
			animation.reset(refx + 3 * width, refy, 2, 1, width, height);
			status = eStatus::Flying;
		}
	}
	else
	{
		if (animation.finished)
		{
			if (!isOnSomething)
			{
				animation.reset(refx + 3 * width, refy, 2, 1, width, height);
				status = eStatus::Flying;
			}
			else
			{
				animation.reset(refx, refy, 2, 1, width, height);
				status = eStatus::Standing;
			}
		}
	}
}

void Car::actz(phsc::Physics& phys, jnr::JumpnRunData& data)
{
	/*if (isOnSomething)
	{
	//phys.v.y = -speed_jump;
	status = eStatus::Attacking;
	animation.reset(refx + width * 4, refy, 40, 1, width, height, 0, 1);
	animation.setSpell(spl::eSpell::spl_dashattack, 20);
	//animation.setSpell(spl::eSpell::spl_bullet, 15);
	timer_stun.reset(100);
	v_target = $V(0, 0);
	}*/
}

void Car::think(jnr::JumpnRunData& data)
{
	if (ai_timer > 0)
	{
		ai_timer--;

	}
	else if (ai_timer == 0)
	{
		if (where_to_go)
			mc.push_left = true;
		else mc.push_right = true;

		if (fabs(v_reaction.x) > 0.0)
		{
			ai_timer = 60;
			where_to_go = !where_to_go;
		}

		return;
	}
}

void Car::render(gfw::BaseMedia& media, jnr::JumpnRunData& data)
{
	BaseGraphics* graphics = media.getGraphics();
	int offset_x = 0;

	int alpha = timer_invinc.moving() ? 0xaaffffff : 0xffffffff;
	graphics->draw($P.img_obj, phys.p.x - data.camera.X(), phys.p.y - data.camera.Y(), 0
		, refx + width * offset_x, refy, width, height, turned, alpha);
	if (Constants::debugging)
	{
		graphics->drawString(gfw::num2str(this->hp), phys.p.x - data.camera.X(), phys.p.y - data.camera.Y() + height, 48, 20);
	}
}