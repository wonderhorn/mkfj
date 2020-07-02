#pragma once
#include"jumpnrun/mover/enemy/Enemy.h"
#include"jumpnrun/mover/effect/Effect.h"
#include"jumpnrun/mover/Barrier.h"
#include"jumpnrun/GRAPHICS.h"
#include"jumpnrun/system/Parmanent.h"
#include"jumpnrun/stage/ItemDropper.h"
using namespace jnr;
using namespace enm;
using namespace gfw;
using namespace gmtr;
#define $V Vector2D

void Enemy::initialize(int refx, int refy
	, gmtr::Vector2D p, OWNERHANDLE owner)
{
	Character::initialize(refx, refy, p, owner);
	this->interacting_with_items = false;
	is_damaging = true;
	timer_deactivate_damage = 0;
	this->lifetime = 999999;
	is_weak_to_water = false;
}

void Enemy::run(jnr::JumpnRunData& data)
{
	think(data);
	mc.poll();

	Character::run(data);

	if (lifetime < age)
		alive = false; //not die

	if (timer_deactivate_damage > 0)
		timer_deactivate_damage--;

	if (timer_deactivate_damage == 1)
		is_damaging = true;

	if (is_weak_to_water && location == 'w') 
	{
		Damage d;
		d.power = 1;
		d.invinc_time = 0;
		d.stun_time = 0;
		this->damage(d);
	}
}

void Enemy::interact(jnr::Character& chara, jnr::JumpnRunData& data)
{
	if (chara.Owner() == this->Owner())
		return;
	if (!this->InteractingWithBlocks() && chara.Name().substr(0, 5) == "block")
		return;

	bool hit = false;
	$V center = phys.p + $V(width / 2, height / 2);
	if (width < 64)
		hit = chara.Shape().includes(center);
	else
	{
		hit = this->Shape().includes(chara.P() + $V(chara.Width(), chara.Height()) / 2);
	}
	if (hit)
	{
		$V chara_center = chara.Center();
		$V dp = (phys.v - chara.Phys().v).regularized();
		if ((phys.v - chara.Phys().v).l2() == 0)
			dp = $V(0, 0);
		dp.y = 0;
		dp.x *= 6;

		Damage d;
		d.invinc_time = Constants::time_invinc;
		d.stun_time = Constants::time_stun;
		d.power = power;
		d.isPhysical = true;
		d.owner = this->Owner();
		d.smash = (dp + $V(0, -4));

		if (is_damaging &&
			chara.damage(d) >= 0)
		{
			$P.sound_stack.push_back($P.snd_hit);
			generate<Effect>(*tl)->initialize(REFPOINT_HIT1_X, REFPOINT_HIT1_Y, 32, 32, phys.p, 2, $V(0.0, 0.0), 0);
			generate<Effect>(*tl)->initialize(REFPOINT_HIT2_X, REFPOINT_HIT2_Y, 32, 32, phys.p, 4, $V(0.0, 0.0), 0);
			//chara.Smash(dp + $V(0, -4));

			is_damaging = false;
			timer_deactivate_damage = 90;
		}
	}
}

/*int Enemy::damage(int power, int stun_time, int invinc_time)
{
	if (timer_invinc.moving())
		return -1;
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

int Enemy::damage(const Damage& d)
{
	if (timer_invinc.moving())
		return -1;
	//animation.reset(refx + width * 5, refy, stun_time, 1, width, height);
	//status = eStatus::Damaged;
	if (auto* ptr = barrier.Dst(*tl))
	{
		Barrier* b = static_cast<Barrier*>(ptr);
		b->damage(d);
	}
	timer_invinc.reset(d.invinc_time / 2);
	timer_stun.reset(d.stun_time);
	if (d.stun_time > Constants::time_stun)
	{
		v_target = $V(0, 0);
	}
	this->Smash(d.smash);
	hp -= d.power;
	return d.power;
}

void Enemy::die(jnr::JumpnRunData& data)
{
	if (this->Owner() != OWNERHANDLE(0) && data.encount_boss == -1)
	{
		data.num_enemies_beaten++;
		ItemDropper().drop(*tl, Center(), Name());
	}
	$P.sound_stack.push_back($P.snd_punch);
	Character::die(data);
}


void Boy::initialize(int refx, int refy
	, gmtr::Vector2D p, OWNERHANDLE owner, bool turned)
{
	Enemy::initialize(refx, refy, p, owner);
	power = 10;
	hp = 10;
	ai_timer = 0;
	speed_walk = 1.2;
	speed_dash = 1.2;
	this->turned = this->where_to_go = turned;
	interacting_with_items = false;
	stopping = false;
	writeName("character_enemy_boy", name);
}

void Boy::run(jnr::JumpnRunData& data)
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

void Boy::think(jnr::JumpnRunData& data)
{
	if (ai_timer > 0)
		ai_timer--;

	if (v_reaction.x != 0)
	{
		stopping = false;
		where_to_go = !where_to_go;
		ai_timer = 120;
	}

	if (ai_timer == 0)
	{
		if ($P.rand.get() % 4 == 0)
		{
			stopping = true;
			//where_to_go = !where_to_go;
		}
		else if ($P.rand.get() % 3 == 0)
		{
			stopping = false;
			where_to_go = !where_to_go;
		}

		ai_timer = 120;
	}

	if (!stopping)
	{
		if (where_to_go)
			mc.push_left = true;
		else mc.push_right = true;
	}
}

//void Boy::render(){}