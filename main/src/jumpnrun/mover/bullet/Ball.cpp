#include"jumpnrun/mover/bullet/Bubble.h"
#include"jumpnrun/mover/effect/Effect.h"
#include"jumpnrun/mover/effect/Circle.h"
#include"jumpnrun/mover/Barrier.h"
#include"jumpnrun/mover/bullet/Bullet.h"
#include"jumpnrun/system/Parmanent.h"
#include"jumpnrun/spell/Spell.h"
#include"jumpnrun/GRAPHICS.h"
using namespace jnr;
using namespace gfw;
using namespace gmtr;
#define $V Vector2D

RegisterMoverClass(Ball);

void Ball::initialize(int refx, int refy
	, gmtr::Vector2D p, gmtr::Vector2D v, int power, OWNERHANDLE owner)
{
	Bubble::initialize(refx, refy, p, v, power, owner);
	phys.mass = 0;
	interacting_with_stages = true;
	interacting_with_blocks = false;
	touch_and_dissapear = false;
	this->hp = this->hp_max = 30;
	phys.mass = 2;
	wet = false;
	smash_dimrate = 1.025;
	v_target = v;
	//this->Smash(v);
	writeName("ball", name);

}
void Ball::run(jnr::JumpnRunData& data)
{
	double vl2 = phys.v.l2();
	if (vl2 <= 0.75)
	{
		this->owner = -1;
		//return;
	}
	$V prev_v = phys.v;
	Bubble::run(data);
	if (fabs(this->v_reaction.x) >= 0.001)
	{
		this->owner = -1;
		phys.v.x = -prev_v.x * 0.5;
		v_target = $V(0, 0);
	}
	if (fabs(this->v_reaction.y) >= 0.001)
	{
		phys.v.y = -prev_v.y * 0.5;
		
		v_target = $V(0, 0);
	}
	if (fabs(phys.v.x) < 0.25)
		phys.v.x = 0;
	if (phys.v.y < 0 && phys.v.y > -0.5)
		phys.v.y = 0;
}

void Ball::drive(phsc::Physics& phys, jnr::JumpnRunData& data) 
{
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
}



void Ball::interact(jnr::Character& chara, jnr::JumpnRunData& data)
{
	double vl2 = phys.v.l2();
	if (vl2 <= 0.75)
	{
		return;
	}
	Bubble::interact(chara, data);
}

int Ball::damage(const Damage& d)
{
	if (timer_invinc.moving())
		return -1;

	animation.reset(refx + width * 5, refy, d.stun_time, 1, width, height);
	status = eStatus::Damaged;

	v_target = $V(0, 0);
	//this->Smash(d.smash);
	this->phys.v = d.smash;
	this->owner = d.owner;
	hp -= d.power;
	return d.power;
}