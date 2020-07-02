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

RegisterMoverClass(Gas);

void Gas::initialize(int refx, int refy
	, gmtr::Vector2D p, OWNERHANDLE owner, int power, gmtr::Vector2D v, double mass)
{
	Bullet::initialize(refx, refy, p, owner, power, v, mass);

	writeName("bullet_gas", name);
	this->rotate_to_direction = false;
	this->touch_and_dissapear = false;
	this->snd_hit = -1;
	margin_top = margin_side = margin_bottom = 0;
}
void Gas::interact(jnr::Character& chara, jnr::JumpnRunData& data)
{
	if (chara.Owner() == this->Owner())
		return;
	$V center = phys.p + $V(width / 2, height / 2);
	if (chara.Shape().includes(center))
	{
		Damage d;
		d.ignoring_barrier = true;
		d.invinc_time = d.stun_time = 0;
		d.power = true;
		d.owner = this->Owner();
		if (chara.damage(d) >= 0)
		{
			generate<Effect>(*tl)->initialize(REFPOINT_HIT1_X, REFPOINT_HIT1_Y, 32, 32, phys.p, 2, $V(0.0, 0.0), 0);
			generate<Effect>(*tl)->initialize(REFPOINT_HIT2_X, REFPOINT_HIT2_Y, 32, 32, phys.p, 4, $V(0.0, 0.0), 0);
			//chara.Smash(this->phys.v.regularized() * 4.0 + $V(0, -4));
			finalize();
		}
	}
}
void Gas::run(jnr::JumpnRunData& data)
{
	Bullet::run(data);
	turned = V().x < 0;
	double l = V().l2();
	if (l >= 2.0 && l != 0.0)
		phys.v = phys.v / l * 2.0;
	if ($P.rand.get() % 45 == 0)
		this->phys.v = Vector2D::polar(this->phys.v.l2(), (double)$P.rand.get() / RAND_MAX * M_PI + M_PI);
}
void Gas::render(gfw::BaseMedia& media, jnr::JumpnRunData& data)
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