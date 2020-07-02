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

void Knight::initialize(int refx, int refy
	, gmtr::Vector2D p, OWNERHANDLE owner, bool turned)
{
	Enemy::initialize(refx, refy, p, owner);
	power = 8;
	hp = 20;
	speed_walk = speed_dash = 0.75;
	speed_jump = 4.0;
	ai_timer = 0;
	this->turned = this->where_to_go = turned;
	writeName("character_enemy_knight", name);
}

void Knight::run(jnr::JumpnRunData& data)
{
	timer_guard.advance();
	//check timer_stun to avoid moving during gurding
	isGuarding = timer_guard.rest() >= 30 && timer_stun.rest() >= 1;
	Enemy::run(data);
	if (isGuarding)
	{
		animation.reset(refx + 4*width, refy, 3, 1, width + 16, height);

	}
	else if (status == eStatus::Standing || status == eStatus::Walking || status == eStatus::Dashing)
	{
		if (animation.finished)
		{
			if (fabs(phys.v.x) > 0.0)
			{
				if (fabs(v_target.x) <= speed_walk)
				{
					animation.reset(refx + width, refy, 12, 2, width + 16, height);
					status = eStatus::Walking;
				}
				else
				{
					status = eStatus::Dashing;
					animation.reset(refx + width, refy, 10, 2, width + 16, height);
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
			animation.reset(refx + 1 * width, refy, 2, 1, width + 16, height);
			status = eStatus::Flying;
		}
	}
	else
	{
		if (animation.finished)
		{
			if (!isOnSomething)
			{
				animation.reset(refx + 1 * width, refy, 2, 1, width, height);
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

int Knight::damage(const Damage& d)
{
	if (isGuarding)
		return eDamage::Repelled;
	return Enemy::damage(d);
}

void Knight::actz(phsc::Physics& phys, jnr::JumpnRunData& data)
{
	if (isOnSomething)
	{
		phys.v.y = -speed_jump;
		status = eStatus::Attacking;
		animation.reset(refx + width * 1, refy, 30, 1, width, height +16, 0, 1);
		animation.setSpell(spl::eSpell::spl_dashattack, 5);
		//animation.setSpell(spl::eSpell::spl_bullet, 15);
		timer_stun.reset(90);
		v_target = $V(0, 0);
	}
}

void Knight::actx(phsc::Physics& phys, jnr::JumpnRunData& data)
{
	if (isOnSomething)
	{
		//phys.v.y = -speed_jump;
		status = eStatus::Casting;
		//animation.reset(refx + width * 4, refy, 150, 1, width, height, 0, 1);
		//animation.setSpell(spl::eSpell::spl_arrow, 40);
		//animation.setSpell(spl::eSpell::spl_bullet, 15);
		timer_guard.reset(150);
		timer_stun.reset(150);
		v_target = $V(0, 0);
		this->isGuarding = true;
	}
}

void Knight::think(jnr::JumpnRunData& data)
{
	if (ai_timer > 0)
	{
		ai_timer--;
	}
	if (v_reaction.x != 0)
	{
		//check jump
		auto newp = this->Center() + $V((1- turned * 2) * 16, -48);
		bool shoudJump = stg->enterable(newp);
		if (shoudJump)
		{
			mc.push_up = true;
		}
		else
		{
			where_to_go = !where_to_go;
			ai_timer = 45;
		}
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
		return myo != o &&
			(mvr->Name().substr(0, 9) == "character" ||
			mvr->Name().substr(0, 6) == "bullet");
	}, v);
	auto p = this->Center();
	std::sort(v.begin(), v.end(), [p](const Object::Pipe & p1, const Object::Pipe & p2)
	{
		return (p - static_cast<Mover*>(p1.Dst(*tl))->Center()).sqsum()
			< (p - static_cast<Mover*>(p2.Dst(*tl))->Center()).sqsum();
	});
	if (!v.empty())
	{
		if ($P.rand.get() % 4 == 0)
		{
			if (ai_timer == 0)
			{
				{
					if ($P.rand.get() % 3 == 0)
					{
						where_to_go = !where_to_go;
					}
					ai_timer = 120;
				}
			}
		}
		else
		{
			auto* nearest = static_cast<Mover*>(v[0].Dst(*tl));
			auto dp = nearest->Center() - Center();
			if (nearest->Name().substr(0, 9) == "character")
			{
				if (dp.x < 0 == turned)
				{
					//forwarding
					if (turned)
						mc.push_left = true;
					else mc.push_right = true;
				}
				else
				{
					if (ai_timer == 0)
					{
						//turning
						if (turned)
							mc.push_right = true;
						else mc.push_left = true;
						ai_timer = 60;
					}
				}
			}
			else if (nearest->Name().substr(0, 6) == "bullet")
			{
				mc.push_x = status != eStatus::Casting;
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
				if (ai_timer == 0)
				{
					if ($P.rand.get() % 4 == 0)
					{
						where_to_go = !where_to_go;
					}
					ai_timer = 120;
				}
			}
			else
			{
				auto* nearest = static_cast<Mover*>(v[0].Dst(*tl));
				auto dp = nearest->Center() - Center();
				if (nearest->Name().substr(0, 9) == "character")
				{
					if (dp.x < 0 == turned)
					{
						//forwarding
						if (turned)
							mc.push_left = true;
						else mc.push_right = true;
					}
					else
					{
						if (ai_timer == 0)
						{
							if (turned)
								mc.push_right = true;
							else mc.push_left = true;
							ai_timer = 60;
						}
					}
				}
			}
		}

		if (where_to_go)
			mc.push_left = true;
		else mc.push_right = true;
	}
}