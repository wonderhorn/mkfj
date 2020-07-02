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

RegisterMoverClass(LaserSlash);

void LaserSlash::initialize(int color, double length, double theta_center
	, jnr::Character* chara, int power)
{
	Bullet::initialize(0, 0, chara->Center(), chara->Owner(), power, $V(0, 0), 0);
	pipe = Object::Pipe(chara, *tl);
	p_ter = chara->Center();
	this->length = length;
	this->theta_center = theta_center;
	this->color = color;
	this->phys.v = $V::polar(6, theta_center);
	lifetime = 20;
	reflected = false;
	num_reflect = 0;
	writeName("bullet_laserslash", name);
}
void LaserSlash::interact(jnr::Character& chara, jnr::JumpnRunData& data)
{
	if (chara.Owner() == this->Owner())
		return;
	Line l(phys.p, p_ter);
	double d = l.distance(chara.Center());
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
	if (d < 32)
	{
		if (chara.damage(dam) >= 0)
		{
			generate<Effect>(*tl)->initialize(REFPOINT_HIT1_X, REFPOINT_HIT1_Y, 32, 32, chara.Center() - $V(16, 16), 2, $V(0.0, 0.0), 0);
			generate<Effect>(*tl)->initialize(REFPOINT_HIT2_X, REFPOINT_HIT2_Y, 32, 32, chara.Center() - $V(16, 16), 4, $V(0.0, 0.0), 0);
			//chara.Smash(this->phys.v.regularized() * 4.0 + $V(0, -4));
			//finalize();
		}
	}
}
void LaserSlash::run(jnr::JumpnRunData& data)
{
	if (lifetime < age)
	{
		finalize();
		return;
	}
	if (auto* ptr = pipe.Dst(*tl))
	{
		Character* chara = static_cast<Character*>(ptr);
		this->phys.p = chara->Center();
		double theta = theta_center + M_PI / 12 - M_PI / 6 * age / lifetime;
		//double theta = M_PI / 12 - M_PI / 6 * age / lifetime;
		//double theta = 0;
		length = age < lifetime / 2 ? 192.0 * age / lifetime
			: 192.0 * (lifetime - age) / lifetime;
		p_ter = phys.p + $V::polar(length* 2, theta);
	}
	age++;
}
void LaserSlash::render(gfw::BaseMedia& media, jnr::JumpnRunData& data)
{
	BaseGraphics* graphics = media.getGraphics();
	int a = (color & 0xff000000) >> 24;
	int r = (color & 0x00ff0000) >> 16;
	int g = (color & 0x0000ff00) >> 8;
	int b = (color & 0x000000ff) >> 0;
	graphics->drawLine(phys.p.x - data.camera.X(), phys.p.y - data.camera.Y(), p_ter.x - data.camera.X(), p_ter.y - data.camera.Y()
		, r, g, b, a, 6);
	graphics->drawLine(phys.p.x - data.camera.X(), phys.p.y - data.camera.Y(), p_ter.x - data.camera.X(), p_ter.y - data.camera.Y()
		, 255, 255, 255, 128, 4);
}