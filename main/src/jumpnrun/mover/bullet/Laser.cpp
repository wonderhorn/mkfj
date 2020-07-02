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

RegisterMoverClass(Laser);

void Laser::initialize(int color, double length, gmtr::Vector2D p, OWNERHANDLE owner, int power, gmtr::Vector2D v, int num_ref)
{
	Bullet::initialize(0, 0, p, owner, power, v, 0);
	p_ter = p;
	this->length = length;
	this->color = color;
	lifetime = 60 * 5;
	reflected = false;
	num_reflect = num_ref;
	writeName("bullet_laser", name);
}
void Laser::interact(jnr::Character& chara, jnr::JumpnRunData& data)
{
	if (chara.Owner() == this->Owner())
		return;
	Line l(phys.p, p_ter);
	double d = l.distance(chara.Center());
	if (d < 32)
	{
		Damage dam;
		dam.power = power;
		dam.stun_time = Constants::time_stun;
		dam.invinc_time = Constants::time_invinc;
		dam.isPhysical = false;
		auto smashv = this->phys.v.regularized();
		if (smashv.l2() == 0)
			smashv = $V(0, -1);
		dam.smash = smashv * 4.0 + $V(0, -4) - chara.V();
		dam.owner = this->Owner();

		if (chara.damage(dam) >= 0)
		{
			generate<Effect>(*tl)->initialize(REFPOINT_HIT1_X, REFPOINT_HIT1_Y, 32, 32, phys.p, 2, $V(0.0, 0.0), 0);
			generate<Effect>(*tl)->initialize(REFPOINT_HIT2_X, REFPOINT_HIT2_Y, 32, 32, phys.p, 4, $V(0.0, 0.0), 0);
			//chara.Smash(this->phys.v.regularized() * 4.0 + $V(0, -4));
			//finalize();
		}
	}
}
void Laser::run(jnr::JumpnRunData& data)
{
	if (lifetime < age)
	{
		finalize();
		return;
	}
	auto newp = phys.p + phys.v;
	if (!stg->enterable(newp))
	{
		if (!reflected && num_reflect > 0)
		{
			$V newv = this->V();
			auto newp_x = phys.p, newp_y = phys.p;
			newp_x.x += phys.v.x;
			newp_y.y += phys.v.y;
			if (!stg->enterable(newp_x))
				newv.x = -newv.x;
			if (!stg->enterable(newp_y))
				newv.y = -newv.y;
			reflected = true;
			gfw::generate<Laser>(*tl)->initialize(this->color, this->length, phys.p, Owner()
				, power, newv, num_reflect - 1);
		}
		p_ter += phys.v;
		Effect* e = gfw::generate<Effect>(*tl);
		e->initialize(REFPOINT_EXPLOSION_WHITE_X, REFPOINT_EXPLOSION_WHITE_Y, 32, 32, newp + $V(-16, -16.0), 2, $V(0, 0), 0);
		if ((phys.p - p_ter) * phys.v < 0)
			finalize();
	}
	else
	{
		phys.p = newp;
		if ((phys.p - p_ter).sqsum() > length * length)
			p_ter += phys.v;
	}
	age++;
}
void Laser::render(gfw::BaseMedia& media, jnr::JumpnRunData& data)
{
	BaseGraphics* graphics = media.getGraphics();
	graphics->drawLine(phys.p.x - data.camera.X(), phys.p.y - data.camera.Y(), p_ter.x - data.camera.X(), p_ter.y - data.camera.Y()
		, 255, 0, 0, 255, 6);
	graphics->drawLine(phys.p.x - data.camera.X(), phys.p.y - data.camera.Y(), p_ter.x - data.camera.X(), p_ter.y - data.camera.Y()
		, 255, 255, 255, 128, 4);
}