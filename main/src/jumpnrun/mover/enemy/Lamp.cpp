#pragma once
#define _USE_MATH_DEFINES
#include"jumpnrun/mover/bullet/Bullet.h"
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

void Lamp::initialize(int refx, int refy
	, gmtr::Vector2D p, OWNERHANDLE owner, bool turned)
{
	Enemy::initialize(refx, refy, p, owner);
	power = 10;
	hp = hp_max = 30;
	ai_timer = 0;
	//decel = 0.25;
	height = 32;
	//where_to_go = true;
	margin_bottom = margin_side = margin_top = 0;
	phys.mass = 0;
	direction = 3;
	this->turned = turned;
	this->speed_walk = this->speed_dash = 1;
	slime = Object::Pipe();

	writeName("character_enemy_lamp", name);
}

void Lamp::drive(phsc::Physics& phys, jnr::JumpnRunData& data)
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


void Lamp::actz(phsc::Physics& phys, jnr::JumpnRunData& data)
{
	auto* ptr = generate<enm::BigSlime>(*tl);
	ptr->initialize(REFPOINT_MERCURY_X, REFPOINT_MERCURY_Y, P(), Owner(), Turned());
	ptr->resetHP_HPMax(20);
	generate<Particles>(*tl)->initialize(REFPOINT_STAR1_X, REFPOINT_STAR1_Y, 32, 32, Center() - Vector2D(16, 16)
		, 12, Vector2D(0, 0), 0, 6, 5.0);
	this->slime = Object::Pipe(ptr, *tl);
}

void Lamp::actx(phsc::Physics& phys, jnr::JumpnRunData& data)
{
	animation.reset(refx, refy, 60, 1, width, height, 0, 1);
}

void Lamp::run(jnr::JumpnRunData& data)
{
	enm::BigSlime* ptr = static_cast<enm::BigSlime*>(slime.Dst(*tl));
	if (ptr)
	{
		if (!ptr->alive)
		{
			slime = Object::Pipe();
		}
	}

	if (!animation.finished && animation.counter == 45)
	{
		auto v = $V::polar(6.0, M_PI / 4.0 * attack_direction);
		Effect* e = gfw::generate<Effect>(*tl);
		e->initialize(REFPOINT_EXPLOSION_WHITE_X, REFPOINT_EXPLOSION_WHITE_Y, 32, 32, Center() + v + $V(-16, -16.0), 2, $V(0, 0), 0);
		Flame* b = gfw::generate<Flame>(*tl);
		b->initialize(REFPOINT_FIRE_X + 32, REFPOINT_FIRE_Y, Center() + v + $V(-16, -16.0), Owner(), 15, v, 0);
		$P.sound_stack.push_back($P.snd_ignition);
	}

	Enemy::run(data);
	status = eStatus::Flying;

}

void Lamp::think(jnr::JumpnRunData& data)
{
	if (ai_timer)
	{
		ai_timer--;
	}
	else
	{
		using namespace myutil;
		Slime* ptr = static_cast<Slime*>(slime.Dst(*tl));
		if (!ptr && $P.rand.get() % 5 == 0)
		{
			ai_timer = 180;
			mc.push_z = true;
			direction = -1;
		}
		else
		{

			EnemySearcher es;
			es.initialize(this);
			auto v = es.search(192);
			auto o = this->Owner();
			v = myutil::filter([o](const Object::Pipe & p)
			{
				const Mover* mvr = static_cast<Mover*>(p.Dst(*tl));
				auto myo = mvr->Owner();
				char c = char(myo);
				return myo != o && mvr->Name().substr(0, 9) == "character";
			}, v);

			if ($P.rand.get() % 3 == 0)
			{
				ai_timer = 180;
				direction = -1;
				return;
			}
			if (v.empty() || $P.rand.get() % 4 == 0)
			{
				direction = $P.rand.get() % 8;
				ai_timer = 120;

				StageAccessor sa(*stg);
				double dff = sa.distanceToFloor(Center(), data);
				if (dff < 48)
				{
					switch (direction)
					{
					default:
						break;
					case 3:
						direction = 2;
						break;
					case 4:
						direction = 0;
						break;
					case 5:
						direction = 6;
						break;
					}
				}

				if (dff > 64 * 3)
				{
					switch (direction)
					{
					default:
						break;
					case 1:
						direction = 2;
						break;
					case 8:
						direction = 7;
						break;
					case 0:
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
				if (dp.l2() < 180)
				{
					ai_timer = 180;
					mc.push_x = true;
					attack_direction = qtheta;
					direction = -1;
				}
				else
				{
					ai_timer = 45;
					direction = qtheta;

				}
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

void Lamp::render(gfw::BaseMedia& media, jnr::JumpnRunData& data)
{
	BaseGraphics* graphics = media.getGraphics();
	int offset_y = age % 50 < 25;

	if (!animation.finished)
	{
		offset_y += ((age / 2) % 2) * 2;
	}

	int alpha = timer_invinc.moving() ? 0xaaffffff : 0xffffffff;
	graphics->draw($P.img_obj, phys.p.x - data.camera.X(), phys.p.y + offset_y - data.camera.Y(), 0
		, refx, refy, width, height, turned, alpha);
	if (Constants::debugging)
	{
		graphics->drawString(gfw::num2str(this->hp), phys.p.x - data.camera.X(), phys.p.y - data.camera.Y() + height, 48, 20);
	}
}