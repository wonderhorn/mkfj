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

RegisterMoverClass(Bullet);

void Bullet::initialize(int refx, int refy
	, gmtr::Vector2D p, OWNERHANDLE owner, int power, gmtr::Vector2D v, double mass, bool rotate_to_direction)
{
	Mover::initialize(refx, refy, p, owner);
	this->power = power;
	phys.v = v;
	phys.mass = mass;
	width = height = 32;
	lifetime = 180;
	touch_and_dissapear = false;
	writeName("bullet", name);
	turned = v.x < 0.0;
	margin_top = margin_side = margin_bottom = 8;
	this->rotate_to_direction = rotate_to_direction;
	this->num_bounding = -1;
	this->bound_rate = 0.8;
	touch_and_dissapear = true;
	this->snd_hit = $P.snd_hit;
	y_anim_num = 1;
	y_anim_interval = 30;
	radius = -1;
}
void Bullet::interact(jnr::Character& chara, jnr::JumpnRunData& data)
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
void Bullet::run(jnr::JumpnRunData& data)
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
		if (b2)
		{
			phys.v.y = -prev_v.y * bound_rate;
		}
		else if (b1)
		{
			phys.v.x = -prev_v.x * bound_rate;
		}

		if (b1 || b2)
			num_bounding--;
	}
	else if (num_bounding == 0)
		alive = 0;
	age++;
}
void Bullet::render(gfw::BaseMedia& media, jnr::JumpnRunData& data)
{
	BaseGraphics* graphics = media.getGraphics();
	int refy_offset = height * (age / y_anim_interval % y_anim_num);
	if (!rotate_to_direction)
		graphics->draw($P.img_obj, phys.p.x - data.camera.X(), phys.p.y - data.camera.Y(), 0
		, refx, refy + refy_offset, width, height, turned);
	else
	{
		auto v = this->V();
		bool t = (v.x < 0);
		//v = $V(fabs(v.x), v.y);
		double theta = v.theta();
		if (t)
		{
			theta += M_PI;
			if (theta > 2 * M_PI)
				theta -= 2 * M_PI;
		}
		graphics->drawSpinResize($P.img_obj, phys.p.x - data.camera.X(), phys.p.y - data.camera.Y(), 0
			, refx, refy + refy_offset, width, height, t, 0xffffffff, t? -1.0: 1.0, 1.0, theta);

	}
}