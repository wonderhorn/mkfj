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

RegisterMoverClass(Angel);

void Angel::initialize(int refx, int refy
	, gmtr::Vector2D p, OWNERHANDLE owner, bool turned)
{
	Enemy::initialize(refx, refy, p, owner);
	power = 10;
	hp = hp_max = 35;
	ai_timer = 0;
	speed_walk = 1.5;
	//decel = 0.25;
	height = 64;
	//where_to_go = true;
	margin_bottom = margin_side = margin_top = 0;
	phys.mass = 0;
	direction = 3;
	this->turned = turned;
	barrier_timer = 0;
	writeName("character_enemy_angel", name);
}

void Angel::drive(phsc::Physics& phys, jnr::JumpnRunData& data)
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
				v_target.y = speed_walk;
			}
		}
		else if (Controller().Up().isDown())
		{
			//phys.v.x = -1.5;

			if (v_target.y > -speed_walk)
			{
				//status = eStatus::Walking;
				v_target.y = -speed_walk;
			}
		}
		else
		{
			v_target.y = 0.0;
		}

		//attack
		if (Controller().Z().pushed())
		{
			actz(phys, data);
		}
		if (Controller().X().pushed())
		{
			actx(phys, data);
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

void Angel::run(jnr::JumpnRunData& data)
{
	if (auto ptr = pipe.Dst(*tl))
	{
		if (!ptr->alive)
		{
			pipe = Object::Pipe(nullptr, *tl);
		}
	}

	// auto barrier
	if (barrier_timer == 0) 
	{
		gfw::generate<Barrier>(*tl)->initialize(REFPOINT_CIRCLE_LIGHT_X, REFPOINT_CIRCLE_LIGHT_Y,
			P(), Owner(), this);
		barrier_timer = -1;
	}
	if (barrier_timer > 0)
		barrier_timer--;
	if(barrier_timer == -1 && this->getBarrier() == nullptr)
		barrier_timer = 60 * 8;

	Enemy::run(data);
	//status = eStatus::Flying;
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

void Angel::actz(phsc::Physics& phys, jnr::JumpnRunData& data)
{
	status = eStatus::Attacking;
	animation.reset(refx + width * 4, refy, 45, 2, width, height, 0, 1);
	animation.setSpell(spl::eSpell::spl_laserslash2, 55);
	//animation.setSpell(spl::eSpell::spl_bullet, 15);
	timer_stun.reset(100);
	v_target = $V(0, 0);
}

void Angel::think(jnr::JumpnRunData& data)
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
		auto v = es.search(256);
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
			if ($P.rand.get() % 2)
				direction = -1;
			ai_timer = 90;

			if (Character* c = static_cast<Character*>(pipe.Dst(*tl)))
			{
				auto dp = c->Center() - Center();
				if (dp.l2() > 320)
				{
					double t = dp.theta() + M_PI / 8.0;
					if (t >= 2 * M_PI)
						t -= 2 * M_PI;
					int qtheta = t / M_PI * 4.0;
					int prev_dir = direction;
					direction = qtheta;
					ai_timer = 180;
				}
			}

			StageAccessor sa(*stg);
			double dff = sa.distanceToFloor(Center(), data);
			if (dff < 64)
			{
				switch (direction)
				{
				default:
					break;
				case 1:
					direction = 0;
					break;
				case 3:
					direction = 4;
					break;
				case 2:
					direction = 7;
					break;
				}
			}

			if (dff > 256)
			{
				switch (direction)
				{
				default:
					break;
				case 6:
					direction = 3;
					break;
				case 7:
					direction = 0;
					break;
				case 5:
					direction = 4;
					break;
				}
			}

		}
		else
		{
			auto p = Center();
			std::sort(v.begin(), v.end(), [p](const Object::Pipe & p1, const Object::Pipe & p2)
			{return (p - static_cast<Mover*>(p1.Dst(*tl))->Center()).sqsum()
			< (p - static_cast<Mover*>(p2.Dst(*tl))->Center()).sqsum(); });
			auto dp = static_cast<Mover*>(v.begin()->Dst(*tl))->Center() + $V(0, 12) - p;
			double t = dp.theta() + M_PI / 8.0;
			if (t >= 2 * M_PI)
				t -= 2 * M_PI;
			int qtheta = t / M_PI * 4.0;
			int prev_dir = direction;
			direction = qtheta;
			ai_timer = 120;

			if (!turned && qtheta == 0)
			{
				mc.push_z = true;
			}
			if (turned && qtheta == 4)
			{
				mc.push_z = true;
			}
		}
	}

	switch (direction)
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
	}
}

void Angel::render(gfw::BaseMedia& media, jnr::JumpnRunData& data)
{
	BaseGraphics* graphics = media.getGraphics();

	int offset_y = 1;
	int offset_y2 = 1;

	if (!onSomething())
	{
		offset_y = age / 20 % 2;
		offset_y2 = (age - 2) / 20 % 2;
	}
	graphics->draw($P.img_obj, phys.p.x - 20 - data.camera.X(), phys.p.y - data.camera.Y() + 16, 0
		, REFPOINT_WING_X, REFPOINT_WING_Y + offset_y * 32, 32, 32, false, 0xffffffff);
	graphics->draw($P.img_obj, phys.p.x + 20 - data.camera.X(), phys.p.y - data.camera.Y() + 16, 0
		, REFPOINT_WING_X, REFPOINT_WING_Y + offset_y * 32, 32, 32, true, 0xffffffff);

	graphics->draw($P.img_obj, phys.p.x - 16 - data.camera.X(), phys.p.y - data.camera.Y() + 26, 0
		, REFPOINT_WING_X, REFPOINT_WING_Y + offset_y2 * 32, 32, 32, false, 0xffffffff);
	graphics->draw($P.img_obj, phys.p.x + 16 - data.camera.X(), phys.p.y - data.camera.Y() + 26, 0
		, REFPOINT_WING_X, REFPOINT_WING_Y + offset_y2 * 32, 32, 32, true, 0xffffffff);

	int offset_x = age % 6 < 3;

	int alpha = timer_invinc.moving() ? 0xaaffffff : 0xffffffff;
	graphics->draw($P.img_obj, phys.p.x - data.camera.X(), phys.p.y - data.camera.Y(), 0
		, animation.RefX(), animation.RefY(), width, height, turned, alpha);
	if (Constants::debugging)
	{
		graphics->drawString(gfw::num2str(this->hp), phys.p.x - data.camera.X(), phys.p.y - data.camera.Y() + height, 48, 20);
	}
}