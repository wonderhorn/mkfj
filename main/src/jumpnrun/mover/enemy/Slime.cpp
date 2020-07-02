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

void Slime::initialize(int refx, int refy
	, gmtr::Vector2D p, OWNERHANDLE owner, int rank, bool turned)
{
	Enemy::initialize(refx, refy, p, owner);
	power = 10;
	hp = 10;
	ai_timer = 0;
	decel = 0.25;
	height = 32;
	where_to_go = true;
	margin_bottom = margin_side = margin_top = 0;
	this->turned = this->where_to_go = turned;
	this->rank = rank;
	speed_jump = 4;
	writeName("character_enemy_smile", name);
}

void Slime::drive(phsc::Physics& phys, jnr::JumpnRunData& data)
{
	if (!timer_stun.moving() && !isOnSomething)
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

	if (isOnSomething)
		v_target.x = 0.0;

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

void Slime::run(jnr::JumpnRunData& data)
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

void Slime::think(jnr::JumpnRunData& data)
{
	if (ai_timer)
	{
		ai_timer--;
		if (where_to_go)
			mc.push_left = true;
		else
			mc.push_right = true;
		if (v_reaction.x != 0)
			where_to_go = !where_to_go;
	}
	else
	{
		if (isOnSomething)
			mc.push_up = age % 2 == 0;
		else
		{
			ai_timer = 90;
			if (v_reaction.x != 0)
				where_to_go = !where_to_go;
		}
	}
}

void Slime::die(jnr::JumpnRunData& data)
{
	alive = false;
	if (rank == 0)
	{
		Vector2D center = phys.p + Vector2D(width / 2 - 16, height / 2 - 16);
		generate<Effect>(*tl)->initialize(REFPOINT_EXPLOSION_X, REFPOINT_EXPLOSION_Y, 32, 32
			, center, 10, Vector2D(0, 0), 0, false, 0.25, 0.25, 8);
		generate<Effect>(*tl)->initialize(REFPOINT_EXPLOSION_WHITE_X, REFPOINT_EXPLOSION_WHITE_Y, 32, 32
			, center, 4, Vector2D(0, 0), 0);
		generate<Effect>(*tl)->initialize(refx, refy, width, height
			, phys.p, 4, Vector2D(0, 0), 0, Turned(), 1.0, 0, 16);
	}
	else
	{
		generate<Slime>(*tl)->initialize(REFPOINT_SLIME_X, REFPOINT_SLIME_Y, P() + $V(- 16, -8), Owner(), rank - 1, turned);
		generate<Slime>(*tl)->initialize(REFPOINT_SLIME_X, REFPOINT_SLIME_Y, P() + $V(16, -8), Owner(), rank - 1, turned);
	}
}

void Slime::render(gfw::BaseMedia& media, jnr::JumpnRunData& data)
{
	BaseGraphics* graphics = media.getGraphics();
	int offset_y = 0;
	int dy = 0;
	if (age % 90 < 45)
	{
		dy = 2;
		offset_y = 1;
	}
	int alpha = timer_invinc.moving() ? 0xaaffffff : 0xffffffff;
	graphics->draw($P.img_obj, phys.p.x - data.camera.X(), phys.p.y - data.camera.Y() + dy, 0
		, refx, refy + height * offset_y, width, height, turned, alpha);
	graphics->draw($P.img_obj, phys.p.x - data.camera.X() - 8 * (turned * 2 - 1), phys.p.y - data.camera.Y() + dy, 0
		, REFPOINT_FACE_X, REFPOINT_FACE_Y + 32, width, height, turned, alpha);
	if (Constants::debugging)
	{
		graphics->drawString(gfw::num2str(this->hp), phys.p.x - data.camera.X(), phys.p.y - data.camera.Y() + height, 48, 20);
	}
}