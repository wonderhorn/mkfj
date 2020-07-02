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

RegisterMoverClass(Urchin);

void Urchin::initialize(int refx, int refy
	, gmtr::Vector2D p, OWNERHANDLE owner
	, bool turned, double mass, int lifetime, int power)
{
	Enemy::initialize(refx, refy, p, owner);
	this->power = power;
	hp = 25;
	speed_walk = 1.2;
	speed_dash = 1.2;
	this->turned = turned;
	interacting_with_items = false;
	width = height = 32;
	phys.mass = mass;
	smashable = false;
	breathing = false;
	this->lifetime = lifetime;
	dieeffect = eDieEffectType::Hit;
	writeName("Urchin", name);
}


void Urchin::drive(phsc::Physics& phys, jnr::JumpnRunData& data)
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

	if (timer_stun.rest() <= 1)
	{

		if (Controller().Down().pushed())
		{
			timer_stun.reset(180);
			v_target.x = 0;
		}
	}
}

void Urchin::run(jnr::JumpnRunData& data)
{
	Enemy::run(data);
}

void Urchin::interact(jnr::Character& chara, jnr::JumpnRunData& data)
{
	if (chara.Owner() == this->Owner())
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

		if (is_damaging &&
			chara.damage(d) >= 0)
		{
			$P.sound_stack.push_back($P.snd_hit);
			generate<Effect>(*tl)->initialize(REFPOINT_HIT1_X, REFPOINT_HIT1_Y, 32, 32, phys.p, 2, $V(0.0, 0.0), 0);
			generate<Effect>(*tl)->initialize(REFPOINT_HIT2_X, REFPOINT_HIT2_Y, 32, 32, phys.p, 4, $V(0.0, 0.0), 0);
			chara.Smash(dp + $V(0, -4));

			is_damaging = false;
			timer_deactivate_damage = 90;
			Damage d2;
			d2.invinc_time = 0;
			d2.stun_time = 0;
			d2.power = 25;
			d2.isPhysical = true;
			this->damage(d2);
		}
	}
}

void Urchin::think(jnr::JumpnRunData& data)
{}

void Urchin::render(gfw::BaseMedia& media, jnr::JumpnRunData& data)
{
	BaseGraphics* graphics = media.getGraphics();
	graphics->draw($P.img_obj, phys.p.x - data.camera.X(), phys.p.y - data.camera.Y(), 0
		, refx, refy, width, height, turned);
}