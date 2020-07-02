#define _USE_MATH_DEFINES
#include<cmath>
#include"jumpnrun/mover/character/Character.h"
#include"jumpnrun/mover/bullet/Bullet.h"
#include"jumpnrun/mover/effect/Effect.h"
#include"jumpnrun/system/Parmanent.h"
#include"jumpnrun/GRAPHICS.h"
using namespace jnr;
using namespace gfw;
using namespace gmtr;
#define $V gmtr::Vector2D

RegisterMoverClass(ShotGun);

void ShotGun::run(jnr::JumpnRunData& data)
{
	if (lifetime < age)
	{
		finalize();
		return;
	}
	auto prev_v = this->V();
	Mover::run(data);
	if (num_bounding > 0)
	{
		bool b1 = v_reaction.x != 0.0;
		bool b2 = v_reaction.y != 0.0;
		if (b1)
		{
			phys.v.x = -prev_v.x * bound_rate;
		}
		if (b2)
		{
			phys.v.y = -prev_v.y * bound_rate;
		}
		if (b1 || b2)
			num_bounding--;
	}
	else if (num_bounding == 0)
		alive = 0;

	if (this->collided_with_earth)
	{
		for (int i = 0; i < 6; i++)
		{
			double theta = M_PI * 2 / 6 * i;
			$V dp(cos(theta), sin(theta));
			auto ptr = generate<Bullet>(*tl);
			//auto ptr = generateMover(Bullet, tl);
			ptr->initialize(REFPOINT_STAR1_X, REFPOINT_STAR1_Y,
				Center() - $V(16, 16) + 32 * dp, Owner(), power, dp, 0);
			ptr->LifeTime(5);

		}
	}
}


void ShotGun::interact(jnr::Character& chara, jnr::JumpnRunData& data)
{
	if (chara.Owner() == this->Owner())
		return;
	$V center = phys.p + $V(width / 2, height / 2);
	bool hitting = false;
	if (radius <= 0)
		hitting = chara.Shape().includes(center);
	else
	{
		hitting = chara.Shape().includes(center);
		hitting |= chara.Shape().includes(center + $V(radius, 0));
		hitting |= chara.Shape().includes(center + $V(-radius, 0));
		hitting |= chara.Shape().includes(center + $V(0, radius));
		hitting |= chara.Shape().includes(center + $V(0, -radius));
	}

	if (hitting)
	{
		Damage dam;
		dam.power = power;
		dam.stun_time = Constants::time_stun;
		dam.invinc_time = Constants::time_invinc;
		dam.isPhysical = false;
		dam.owner = this->Owner();
		auto smashv = this->phys.v.regularized();
		if (smashv.l2() == 0)
			smashv = $V(0, -1);
		dam.smash = smashv * 4.0 + $V(0, -4) - chara.V();
		int d = chara.damage(dam);
		if (d >= 0)
		{
			generate<Effect>(*tl)->initialize(REFPOINT_HIT1_X, REFPOINT_HIT1_Y, 32, 32, phys.p, 2, $V(0.0, 0.0), 0);
			generate<Effect>(*tl)->initialize(REFPOINT_HIT2_X, REFPOINT_HIT2_Y, 32, 32, phys.p, 4, $V(0.0, 0.0), 0);
			//chara.Smash(smashv * 4.0 + $V(0, -4) - chara.V());
			finalize();
			if (snd_hit >= 0)
				$P.sound_stack.push_back(snd_hit);

			for (int i = 0; i < 6; i++)
			{
				double theta = M_PI * 2 / 6 * i;
				$V dp(cos(theta), sin(theta));
				auto ptr = generate<Bullet>(*tl);
				//auto ptr = generateMover(Bullet, tl);
				ptr->initialize(REFPOINT_STAR1_X, REFPOINT_STAR1_Y,
					Center() - $V(16, 16) + 32 * dp, Owner(), power, dp, 0);
				ptr->LifeTime(5);

			}
		}
		else if (d == eDamage::Repelled)
		{
			collided_with_earth = true;
			generate<Effect>(*tl)->initialize(REFPOINT_REPELED_X, REFPOINT_REPELED_Y, 32, 32
				, phys.p, 4, $V(0.0, 0.0), 0);
			generate<Effect>(*tl)->initialize(REFPOINT_REPELED_X, REFPOINT_REPELED_Y + 32, 32, 32
				, phys.p, 6, $V(0.0, 0.0), 0);
			finalize();
			$P.sound_stack.push_back($P.snd_repell);

			for (int i = 0; i < 6; i++)
			{
				double theta = M_PI * 2 / 6 * i;
				$V dp(cos(theta), sin(theta));
				auto ptr = generateMover(Bullet, tl);
				ptr->initialize(REFPOINT_STAR1_X, REFPOINT_STAR1_Y,
					Center() - $V(16, 16) + 32 * dp, Owner(), power, dp, 0);
				ptr->LifeTime(5);

			}
		}

	}
}


