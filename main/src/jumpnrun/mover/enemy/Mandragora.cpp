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

void Mandragora::initialize(int refx, int refy
	, gmtr::Vector2D p, OWNERHANDLE owner, bool turned)
{
	Enemy::initialize(refx, refy, p, owner);
	power = 10;
	hp = 10;
	ai_timer = 0;
	decel = 0.25;
	height = 32;
	speed_dash = speed_walk = 1;
	speed_jump = 1.5;
	where_to_go = true;
	margin_bottom = margin_side = margin_top = 0;
	this->turned = this->where_to_go = turned;
	writeName("character_enemy_mandragora", name);
}

void Mandragora::actz(phsc::Physics& phys, jnr::JumpnRunData& data)
{
	status = eStatus::Attacking;
	animation.reset(refx + width * 4, refy, 60, 1, width, height, 0, 1);
	animation.setSpell(spl::eSpell::spl_leafshoot, 50);
	//animation.setSpell(spl::eSpell::spl_bullet, 15);
	timer_stun.reset(100);
	v_target = $V(0, 0);
}

void Mandragora::drive(phsc::Physics& phys, jnr::JumpnRunData& data)
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
		//attack
		if (Controller().Z().pushed())
		{
			actz(phys, data);
		}

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

		if (Controller().Down().isDown())
		{
			timer_stun.reset(60);
			v_target.x = 0;
		}
		if (Controller().Up().pushed())
		{
			//phys.v.x = -1.5;
			if (isOnSomething)
				phys.v.y = -speed_jump;
		}
	}
}

void Mandragora::run(jnr::JumpnRunData& data)
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

/*int Mandragora::damage(int power, int stun_time, int invinc_time)
{
	if (timer_invinc.moving())
		return eDamage::Through;
	//animation.reset(refx + width * 5, refy, stun_time, 1, width, height);
	//status = eStatus::Damaged;
	if (auto* ptr = barrier.Dst(*tl))
	{
		Barrier* b = static_cast<Barrier*>(ptr);
		b->damage(power, stun_time, invinc_time);
	}
	timer_invinc.reset(invinc_time / 2);
	timer_stun.reset(stun_time);
	//v_target = $V(0, 0);
	hp -= power;
	return power;
}*/

int Mandragora::damage(const Damage& d)
{
	if (timer_invinc.moving())
		return eDamage::Through;
	//animation.reset(refx + width * 5, refy, stun_time, 1, width, height);
	//status = eStatus::Damaged;
	if (auto* ptr = barrier.Dst(*tl))
	{
		Barrier* b = static_cast<Barrier*>(ptr);
		b->damage(d);
	}
	timer_invinc.reset(d.invinc_time / 2);
	timer_stun.reset(d.stun_time);
	//v_target = $V(0, 0);
	hp -= d.power;
	return d.power;
}

void Mandragora::think(jnr::JumpnRunData& data)
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
		using namespace myutil;
		EnemySearcher es;
		es.initialize(this);
		auto v = es.search(128);
		auto o = this->Owner();
		v = myutil::filter([o](const Object::Pipe & p)
		{
			const Mover* mvr = static_cast<Mover*>(p.Dst(*tl));
			auto myo = mvr->Owner();
			char c = char(myo);
			return myo != o && mvr->Name().substr(0, 9) == "character";
		}, v);

		if (v.empty() || $P.rand.get() % 2 == 0)
		{
			if (isOnSomething)
				mc.push_up = age % 2 == 0;
			else
			{
				ai_timer = 45;
				if (v_reaction.x != 0 || $P.rand.get() % 5 == 0)
					where_to_go = !where_to_go;
			}
		}
		else
		{
			ai_timer = 45;
			auto dp = static_cast<Mover*>(v[0].Dst(*tl))->Center() - Center();
			if (dp.x < 0 == turned)
			{
				mc.push_z = status != eStatus::Attacking;
			}
			else
			{
				where_to_go = !where_to_go;
			}
			//mc.push_z = true;
		}
	}
}

void Mandragora::render(gfw::BaseMedia& media, jnr::JumpnRunData& data)
{
	BaseGraphics* graphics = media.getGraphics();
	int offset_x = 0;
	if (status == eStatus::Attacking)
	{
		offset_x = 2;
	}
	else if (!isOnSomething)
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