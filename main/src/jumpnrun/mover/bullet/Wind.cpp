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

RegisterMoverClass(Wind);

void Wind::initialize(int refx, int refy
	, gmtr::Vector2D p, OWNERHANDLE owner, int power, gmtr::Vector2D v, double mass, bool rotate_to_direction)
{
	Bullet::initialize(refx, refy, p, owner, power, v, 0);
	lifetime = 45;
}

void Wind::interact(jnr::Character& chara, jnr::JumpnRunData& data)
{
	if (chara.Owner() == this->Owner())
		return;
	$V center = phys.p + $V(width / 2, height / 2);
	if (chara.Shape().includes(center))
	{
		Damage dam;
		dam.power = 0;
		dam.stun_time = 0;
		dam.invinc_time = 0;
		dam.isPhysical = false;
		auto smashv = this->phys.v.regularized();
		if (smashv.l2() == 0)
			smashv = $V(0, -1);
		dam.smash = smashv * 8.0 + $V(0, -6);

		int d = chara.damage(dam);
		if (d >= 0)
		{
			generate<Effect>(*tl)->initialize(REFPOINT_HIT1_X, REFPOINT_HIT1_Y, 32, 32, phys.p, 2, $V(0.0, 0.0), 0);
			generate<Effect>(*tl)->initialize(REFPOINT_HIT2_X, REFPOINT_HIT2_Y, 32, 32, phys.p, 4, $V(0.0, 0.0), 0);
			//chara.Smash(smashv * 4.0 + $V(0, -4) - chara.V());
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
void Wind::render(gfw::BaseMedia& media, jnr::JumpnRunData& data)
{
	BaseGraphics* graphics = media.getGraphics();
	if (!rotate_to_direction)
		graphics->draw($P.img_obj, phys.p.x - data.camera.X(), phys.p.y - data.camera.Y(), 0
		, refx, refy, width, height, turned, 0xaaffffff);
	else
	{
		auto v = this->V();
		bool t = v.x < 0;
		//v = $V(fabs(v.x), v.y);
		double theta = v.theta();
		if (t)
		{
			theta += M_PI;
			if (theta > 2 * M_PI)
				theta -= 2 * M_PI;
		}
		graphics->drawSpinResize($P.img_obj, phys.p.x - data.camera.X(), phys.p.y - data.camera.Y(), 0
			, refx, refy, width, height, true, 0x88ffffff, 1.0, 1.0, theta);

	}
}