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

RegisterMoverClass(Thunder);

void Thunder::initialize(int refx, int refy
	, gmtr::Vector2D p, OWNERHANDLE owner, int power, gmtr::Vector2D v, double mass, bool rotate_to_direction)
{
	Bullet::initialize(refx, refy, p, owner, power, v, mass, rotate_to_direction);
	writeName("bullet_thunder", name);
}

void Thunder::interact(jnr::Character& chara, jnr::JumpnRunData& data)
{
	if (chara.Owner() == this->Owner())
		return;
	$V center = phys.p + $V(width / 2, height / 2);
	if (chara.Shape().includes(center))
	{
		Damage dam;
		dam.power = power;
		dam.stun_time = 120;
		dam.shiver_time = 90;
		dam.invinc_time = 10;
		dam.isPhysical = false;
		auto smashv = this->phys.v.regularized();
		if (smashv.l2() == 0)
			smashv = $V(0, -1);
		dam.smash = smashv * 4.0 + $V(0, -4) - chara.V();
		dam.owner = this->Owner();
		int d = chara.damage(dam);
		if (d >= 0)
		{
			generate<Effect>(*tl)->initialize(REFPOINT_HIT1_X, REFPOINT_HIT1_Y, 32, 32, phys.p, 2, $V(0.0, 0.0), 0);
			generate<Effect>(*tl)->initialize(REFPOINT_HIT2_X, REFPOINT_HIT2_Y, 32, 32, phys.p, 4, $V(0.0, 0.0), 0);
			auto* eoc = generate<EffectOnChara>(*tl);
			eoc->initialize(REFPOINT_EXPLOSION_WHITE_X, REFPOINT_EXPLOSION_WHITE_Y, 32, 32, phys.p
				, &chara, 75, $V(0.0, 0.0), 0);
			eoc->period_onoff = 3;
			//chara.Smash(dam.smash);
			finalize();
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

void Thunder::render(gfw::BaseMedia& media, jnr::JumpnRunData& data)
{
	BaseGraphics* graphics = media.getGraphics();
	if (!rotate_to_direction)
		graphics->draw($P.img_obj, phys.p.x - data.camera.X(), phys.p.y - data.camera.Y(), 0
		, refx, refy, width, height, turned);
	else
	{
		auto v = this->V();
		bool t = (v.x < 0);
		//v = $V(fabs(v.x), v.y);
		double theta = v.theta();
		if (t)
		{
			theta = 3 * M_PI - theta;
			if (theta > 2 * M_PI)
				theta -= 2 * M_PI;
		}
		int offset_y = (age / 6 % 2) * height;
		graphics->drawSpinResize($P.img_obj, phys.p.x - data.camera.X(), phys.p.y - data.camera.Y(), 0
			, refx, refy + offset_y, width, height, t, 0xffffffff, t ? -1.0 : 1.0, 1.0, theta);

	}
}