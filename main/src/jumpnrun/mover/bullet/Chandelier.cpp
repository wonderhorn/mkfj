#define _USE_MATH_DEFINES
#include<cmath>
#include"jumpnrun/mover/bullet/Bullet.h"
#include"jumpnrun/mover/character/Character.h"
#include"jumpnrun/mover/effect/Effect.h"
#include"jumpnrun/system/Parmanent.h"
#include"jumpnrun/MoverUtilities.h"
#include"utils/MyCppUtil.h"
#include"jumpnrun/GRAPHICS.h"
using namespace jnr;
using namespace gfw;
using namespace gmtr;
#define $V gmtr::Vector2D

RegisterMoverClass(Chandelier);


void Chandelier::initialize(gmtr::Vector2D p, int power, gmtr::Vector2D v
	, double mass)
{
	Bullet::initialize(REFPOINT_CHANDELIER_X, REFPOINT_CHANDELIER_Y,
		p, -1, power, v, mass);
	this->width = 64;
	this->height = 32;
	radius = 32;
	y_anim_interval = 12;
	y_anim_num = 2;
	lifetime = 999999;
}
//virtual void interact(jnr::Character& chara, jnr::JumpnRunData& data);
void Chandelier::run(jnr::JumpnRunData& data)
{
	if(!$P.getFlag("dropcdl"))
	{
		age++;
		return;
	}
	Bullet::run(data);

	if (collided_with_earth)
	{
		data.camera.setQuake(25, 3);
		generate<Explosion>(*tl)->initialize(REFPOINT_EXPLOSION_X, REFPOINT_EXPLOSION_Y,
			P() + $V(16, 0), -1, 15);
		EmitFire* ef = generate<EmitFire>(*tl);
		ef->initialize(P() + $V(0, 16),
			gmtr::Vector2D(0, -4), 0, 20);
		ef->LifeTime(15);
		ef = generate<EmitFire>(*tl);
		ef->initialize(P() + $V(32, 16),
			gmtr::Vector2D(0, -4), 0, 20);
		ef->LifeTime(15);
		ef = generate<EmitFire>(*tl);
		ef->initialize(P() + $V(16, 16),
			gmtr::Vector2D(0, -4), 0, 20);
		ef->LifeTime(15);
	}
}

void Chandelier::interact(jnr::Character& chara, jnr::JumpnRunData& data)
{
	if (!$P.getFlag("dropcdl"))
	{
		return;
	}
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
		dam.smash = this->phys.v.regularized() * 4.0 + $V(0, -4) - chara.V();
		dam.owner = this->Owner();
		int d = chara.damage(dam);
		if (d >= 0)
		{
			generate<Effect>(*tl)->initialize(REFPOINT_HIT1_X, REFPOINT_HIT1_Y,
				32, 32, phys.p + $V(16, 0.0), 2, $V(0.0, 0.0), 0);
			generate<Effect>(*tl)->initialize(REFPOINT_HIT2_X, REFPOINT_HIT2_Y,
				32, 32, phys.p + $V(16, 0.0), 4, $V(0.0, 0.0), 0);
			//chara.Smash(dam.smash);
			finalize();
			if (snd_hit >= 0)
				$P.sound_stack.push_back(snd_hit);

			data.camera.setQuake(30, 4);
			generate<Explosion>(*tl)->initialize(REFPOINT_EXPLOSION_X, REFPOINT_EXPLOSION_Y,
				P() + $V(16, 0), -1, 15);
		}
		else if (d == eDamage::Repelled)
		{
			collided_with_earth = true;
			generate<Effect>(*tl)->initialize(REFPOINT_REPELED_X, REFPOINT_REPELED_Y, 32, 32
				, phys.p + $V(16, 0.0), 4, $V(0.0, 0.0), 0);
			generate<Effect>(*tl)->initialize(REFPOINT_REPELED_X, REFPOINT_REPELED_Y + 32, 32, 32
				, phys.p + $V(16, 0.0), 6, $V(0.0, 0.0), 0);
			finalize();
			$P.sound_stack.push_back($P.snd_repell);
		}
	}
}