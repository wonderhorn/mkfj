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

void Elf::initialize(int refx, int refy
	, gmtr::Vector2D p, OWNERHANDLE owner, bool turned)
{
	Enemy::initialize(refx, refy, p, owner);
	power = 10;
	hp = 15;
	speed_walk = speed_dash = 2.0;
	speed_jump = 4.0;
	ai_timer = 0;
	this->turned = this->where_to_go = turned;
	writeName("character_enemy_elf", name);
}

void Elf::run(jnr::JumpnRunData& data)
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

void Elf::actz(phsc::Physics& phys, jnr::JumpnRunData& data)
{
	if (isOnSomething)
	{
		phys.v.y = -speed_jump;
		status = eStatus::Attacking;
		animation.reset(refx + width * 4, refy, 30, 1, width, height, 0, 1);
		animation.setSpell(spl::eSpell::spl_dashattack, 5);
		//animation.setSpell(spl::eSpell::spl_bullet, 15);
		timer_stun.reset(90);
		v_target = $V(0, 0);
	}
}

void Elf::actx(phsc::Physics& phys, jnr::JumpnRunData& data)
{
	if (isOnSomething)
	{
		//phys.v.y = -speed_jump;
		status = eStatus::Casting;
		animation.reset(refx + width * 6, refy, 30, 2, width, height, 20, 1);
		animation.setSpell(spl::eSpell::spl_arrow, 40);
		//animation.setSpell(spl::eSpell::spl_bullet, 15);
		timer_stun.reset(90);
		v_target = $V(0, 0);
	}
}

void Elf::think(jnr::JumpnRunData& data)
{
	if (ai_timer > 0)
	{
		ai_timer--;

		if (v_reaction.x != 0)
		{
			where_to_go = !where_to_go;
			ai_timer = 45;
		}

		if (where_to_go)
			mc.push_left = true;
		else mc.push_right = true;
		return;
	}

	EnemySearcher es;
	es.initialize(this);
	auto v = es.search(64 * 2);
	auto o = Owner();
	v = myutil::filter([o](const Object::Pipe & p)
	{
		const Mover* mvr = static_cast<Mover*>(p.Dst(*tl));
		auto myo = mvr->Owner();
		char c = char(myo);
		return myo != o && mvr->Name().substr(0, 9) == "character";
	}, v);
	if (!v.empty())
	{
		if ($P.rand.get() % 4 == 0)
		{
			if ($P.rand.get() % 3 == 0)
			{
				where_to_go = !where_to_go;
			}
			ai_timer = 120;
		}
		else
		{
			auto dp = static_cast<Mover*>(v[0].Dst(*tl))->Center() - Center();
			if (dp.x < 0 == turned)
			{
				mc.push_z = status != eStatus::Attacking;
			}
			else
			{
				if (turned)
					mc.push_right = true;
				else mc.push_left = true;
			}
		}
	}
	else
	{
		auto v = es.search(64 * 5);
		auto o = Owner();
		v = myutil::filter([o](const Object::Pipe & p)
		{
			const Mover* mvr = static_cast<Mover*>(p.Dst(*tl));
			auto myo = mvr->Owner();
			char c = char(myo);
			return myo != o && mvr->Name().substr(0, 9) == "character";
		}, v);
		if (!v.empty())
		{
			if ($P.rand.get() % 4 == 0)
			{
				if ($P.rand.get() % 3 == 0)
				{
					where_to_go = !where_to_go;
				}
				ai_timer = 120;
			}
			else
			{
				auto dp = static_cast<Mover*>(v[0].Dst(*tl))->Center() - Center();
				if (dp.x < 0 == turned)
				{
					mc.push_x = status != eStatus::Casting;
				}
				else
				{
					if (turned)
						mc.push_right = true;
					else mc.push_left = true;
				}
			}
		}
		else if (ai_timer == 0)
		{
			if ($P.rand.get() % 3 == 0)
			{
				where_to_go = !where_to_go;
			}
			ai_timer = 120;
		}
		if (where_to_go)
			mc.push_left = true;
		else mc.push_right = true;
	}
}