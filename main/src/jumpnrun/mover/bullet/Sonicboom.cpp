#define _USE_MATH_DEFINES
#include<cmath>
#include"jumpnrun/mover/bullet/Bullet.h"
#include"jumpnrun/mover/character/Character.h"
#include"jumpnrun/mover/effect/Effect.h"
#include"jumpnrun/system/Parmanent.h"
#include"jumpnrun/GRAPHICS.h"
using namespace jnr;
using namespace gfw;
using namespace gmtr;
#define $V gmtr::Vector2D

RegisterMoverClass(Sonicboom);

void Sonicboom::initialize(int refx, int refy
	, gmtr::Vector2D p, OWNERHANDLE owner, int power, gmtr::Vector2D v, double mass, int rank)
{
	Bullet::initialize(refx, refy, p, owner, power, v, mass);
	this->rank = rank;
	lifetime = 60;
}

void Sonicboom::interact(jnr::Character& chara, jnr::JumpnRunData& data)
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
		if (rank)
		{
			auto* ptr = generate<Sonicboom>(*tl);
			ptr->initialize(refx, refy, P() + 4*V(), owner, power, V(), phys.mass, rank - 1);
		}

		Damage dam;
		dam.power = power;
		dam.stun_time = Constants::time_stun;
		dam.invinc_time = Constants::time_invinc;
		dam.isPhysical = false;
		dam.smash = this->phys.v.regularized() * 4.0 + $V(0, -4) - chara.V();
		dam.owner = this->Owner();

		int d = chara.damage(dam);
		if (d >= 0)
		{
			generate<Effect>(*tl)->initialize(REFPOINT_HIT1_X, REFPOINT_HIT1_Y, 32, 32, phys.p, 2, $V(0.0, 0.0), 0);
			generate<Effect>(*tl)->initialize(REFPOINT_HIT2_X, REFPOINT_HIT2_Y, 32, 32, phys.p, 4, $V(0.0, 0.0), 0);
			//chara.Smash(dam.smash);
			finalize();
			if (snd_hit >= 0)
				$P.sound_stack.push_back(snd_hit);
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
		}
	}
}