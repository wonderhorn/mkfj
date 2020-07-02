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

RegisterMoverClass(Bug);

void Bug::initialize(int refx, int refy
	, gmtr::Vector2D p, OWNERHANDLE owner, bool turned)
{
	Enemy::initialize(refx, refy, p, owner);
	power = 10;
	hp = hp_max = 12;
	ai_timer = 0;
	speed_walk = speed_dash = 2.25;
	decel = 0.25;
	height = 32;
	where_to_go = true;
	margin_bottom = margin_side = margin_top = 0;
	this->turned = this->where_to_go = turned;
	writeName("character_enemy_bug", name);
}

void Bug::drive(phsc::Physics& phys, jnr::JumpnRunData& data)
{
	if (!timer_stun.moving())
	{
		//walk
		bool rd = Controller().Right().isDown();
		if (rd)
		{
			turned = false;
			if (v_target.x < speed_walk)
			{
				status = eStatus::Walking;
				v_target.x = speed_walk;
			}
		}
		else if (Controller().Left().isDown())
		{
			turned = true;
			//phys.v.x = -1.5;

			if (v_target.x > -speed_walk)
			{
				status = eStatus::Walking;
				v_target.x = -speed_walk;
			}
		}
		else
		{
			v_target.x = 0.0;
		}

	}

	//drive and stop
	if (v_target.x > 0)
	{
		phys.v.x += accel;
		if (phys.v.x >= v_target.x)
			phys.v.x = v_target.x;
	}
	else if (v_target.x < 0)
	{
		phys.v.x -= accel;
		if (phys.v.x <= v_target.x)
			phys.v.x = v_target.x;
	}
	else
	{
		if (phys.v.x > 0)
		{
			phys.v.x -= decel;
			if (phys.v.x < 0)
				phys.v.x = 0;
		}
		else if (phys.v.x < 0)
		{
			phys.v.x += decel;
			if (phys.v.x > 0)
				phys.v.x = 0;
		}
	}

	if (timer_stun.rest() <= 1)
	{

		//jump
		if (Controller().Up().pushed())
		{
			//phys.v.x = -1.5;
			if (isOnSomething)
				phys.v.y = -speed_jump;
		}
	}
}

void Bug::run(jnr::JumpnRunData& data)
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
					//animation.reset(refx + width, refy, 12, 2, width, height);
					status = eStatus::Walking;
				}
				else
				{
					status = eStatus::Dashing;
					//animation.reset(refx + width, refy, 10, 2, width, height);
				}
			}
			else
			{
				//animation.reset(refx, refy, 2, 1, width, height);
				status = eStatus::Standing;
			}
		}
		if (!isOnSomething)
		{
			//animation.reset(refx + 3 * width, refy, 2, 1, width, height);
			status = eStatus::Flying;
		}
	}
	else
	{
		if (animation.finished)
		{
			if (!isOnSomething)
			{
				//animation.reset(refx + 3 * width, refy, 2, 1, width, height);
				status = eStatus::Flying;
			}
			else
			{
				//animation.reset(refx, refy, 2, 1, width, height);
				status = eStatus::Standing;
			}
		}
	}
}

void Bug::think(jnr::JumpnRunData& data)
{
	if (ai_timer)
	{
		ai_timer--;
		mc.push_up = false;
		if (where_to_go)
			mc.push_left = true;
		else
			mc.push_right = true;
		if (v_reaction.x != 0)
		{
			if ($P.rand.get() % 4 == 0)
			{
				mc.push_up = true;
				ai_timer = 60;
			}
			else
			{
				where_to_go = !where_to_go;
				ai_timer = 120;
			}
		}
	}
	else
	{
		//if (isOnSomething)
		//	mc.push_up = age % 2 == 0;
		//else
		{
			ai_timer = 120;
			if (v_reaction.x != 0)
			{
				if ($P.rand.get() % 4 == 0)
				{
					mc.push_up = true;
					ai_timer = 60;
				}
				else
				{
					where_to_go = !where_to_go;
					ai_timer = 120;
				}
			}
			mc.push_left = false;
			mc.push_right = false;
		}
	}
}

void Bug::render(gfw::BaseMedia& media, jnr::JumpnRunData& data)
{
	BaseGraphics* graphics = media.getGraphics();
	int offset_x = 0;
	if ((status == Walking || status == Dashing) && age / 10 % 2 == 0)
	{
		offset_x = 1;
	}
	int alpha = timer_invinc.moving() ? 0xaaffffff : 0xffffffff;
	graphics->draw($P.img_obj, phys.p.x - data.camera.X(), phys.p.y - data.camera.Y(), 0
		, refx + width * offset_x, refy, width, height, turned, alpha);
	if (Constants::debugging)
	{
		graphics->drawString(gfw::num2str(this->hp), phys.p.x - data.camera.X(), phys.p.y - data.camera.Y() + height, 48, 20);
	}
}