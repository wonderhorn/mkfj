#pragma once
#define _USE_MATH_DEFINES
#include"jumpnrun/mover/enemy/Enemy.h"
#include"jumpnrun/mover/effect/Effect.h"
#include"jumpnrun/mover/Barrier.h"
#include"jumpnrun/GRAPHICS.h"
#include"jumpnrun/system/Parmanent.h"
#include"jumpnrun/MoverUtilities.h"
#include"jumpnrun/mover/StageAccessor.h"
#include"utils/MyCppUtil.h"
#include<cmath>
#include<functional>
#include<algorithm>
using namespace jnr;
using namespace enm;
using namespace gfw;
using namespace gmtr;
#define $V Vector2D

RegisterMoverClass(Bird);

void Bird::initialize(int refx, int refy
	, gmtr::Vector2D p, OWNERHANDLE owner, bool turned)
{
	Enemy::initialize(refx, refy, p, owner);
	power = 10;
	hp = 50;
	ai_timer = 0;
	//decel = 0.25;
	height = 32;
	//where_to_go = true;
	margin_bottom = margin_side = margin_top = 0;
	phys.mass = 0;
	speed_up = 1;
	speed_down = 2;
	speed_walk = speed_dash = 2;
	direction = 3;
	this->turned = turned;
	where_to_go = true;
	favorite_height = 160;
	writeName("enemy_bird", name);
}

void Bird::drive(phsc::Physics& phys, jnr::JumpnRunData& data)
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

		if (Controller().Down().isDown())
		{
			if (v_target.y < speed_walk)
			{
				//status = eStatus::Walking;
				v_target.y = speed_down;
			}
		}
		else if (Controller().Up().isDown())
		{
			//phys.v.x = -1.5;

			if (v_target.y > -speed_walk)
			{
				//status = eStatus::Walking;
				v_target.y = -speed_up;
			}
		}
		else
		{
			v_target.y = 0.0;
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

	if (v_target.y > 0)
	{
		phys.v.y += accel;
		if (phys.v.y >= v_target.y)
			phys.v.y = v_target.y;
	}
	else if (v_target.y < 0)
	{
		phys.v.y -= accel;
		if (phys.v.y <= v_target.y)
			phys.v.y = v_target.y;
	}
	else
	{
		if (phys.v.y > 0)
		{
			phys.v.y -= decel;
			if (phys.v.y < 0)
				phys.v.y = 0;
		}
		else if (phys.v.y < 0)
		{
			phys.v.y += decel;
			if (phys.v.y > 0)
				phys.v.y = 0;
		}
	}

}

void Bird::run(jnr::JumpnRunData& data)
{
	Enemy::run(data);
	status = eStatus::Flying;

}

void Bird::think(jnr::JumpnRunData& data)
{
	if (ai_timer)
	{
		ai_timer--;
	}
	else
	{
		using namespace myutil;
		EnemySearcher es;
		es.initialize(this);
		auto v = es.search(160);
		auto o = this->Owner();
		v = myutil::filter([o](const Object::Pipe & p)
		{
			const Mover* mvr = static_cast<Mover*>(p.Dst(*tl));
			auto myo = mvr->Owner();
			char c = char(myo);
			return myo != o && mvr->Name().substr(0, 9) == "character";
		}, v);

		if (v.empty() || $P.rand.get() % 4 == 0)
		{
			direction = $P.rand.get() % 8;
			ai_timer = 120;
		}
		else
		{
			auto p = P();
			std::sort(v.begin(), v.end(), [p](const Object::Pipe & p1, const Object::Pipe & p2)
			{
				return (p - static_cast<Mover*>(p1.Dst(*tl))->Center()).sqsum()
					< (p - static_cast<Mover*>(p2.Dst(*tl))->Center()).sqsum();
			});
			auto dp = static_cast<Mover*>(v.begin()->Dst(*tl))->Center() - p;
			double t = dp.theta() + M_PI / 8.0;
			if (t >= 2 * M_PI)
				t -= 2 * M_PI;
			int qtheta = t / M_PI * 4.0;
			int prev_dir = direction;
			direction = qtheta;
			ai_timer = 120;
		}
	}

	StageAccessor sa(*stg);
	double h = sa.distanceToFloor(Center(), data);
	if (h > favorite_height)
		mc.push_down = true;
	else
		mc.push_down = false;
	if (h < favorite_height - 32)
		mc.push_up = true;
	else
		mc.push_up = false;

	if (v_reaction.x != 0.0)
	{
		where_to_go = !where_to_go;
	}

	if (where_to_go)
		mc.push_left = true;
	else
		mc.push_right = true;

	/*switch (direction)
	{
	default:
		break;
	case 0:
		mc.push_right = true;
		break;
	case 1:
		mc.push_right = true;
		mc.push_down = true;
		break;
	case 2:
		mc.push_down = true;
		break;
	case 3:
		mc.push_left = true;
		mc.push_down = true;
		break;
	case 4:
		mc.push_left = true;
		break;
	case 5:
		mc.push_left = true;
		mc.push_up = true;
		break;
	case 6:
		mc.push_up = true;
		break;
	case 7:
		mc.push_right = true;
		mc.push_up = true;
		break;
	}*/
}

void Bird::render(gfw::BaseMedia& media, jnr::JumpnRunData& data)
{
	BaseGraphics* graphics = media.getGraphics();
	int offset_y = age % 20 < 10;

	int alpha = timer_invinc.moving() ? 0xaaffffff : 0xffffffff;
	graphics->draw($P.img_obj, phys.p.x - data.camera.X(), phys.p.y - data.camera.Y(), 0
		, refx, refy +height * offset_y, width, height, turned, alpha);
	graphics->drawString(gfw::num2str(this->hp), phys.p.x - data.camera.X(), phys.p.y - data.camera.Y() + height, 48, 20);
}