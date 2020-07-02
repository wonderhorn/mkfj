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

RegisterMoverClass(DarkWave);

void DarkWave::initialize(jnr::Character& chara, int power, gmtr::Vector2D v, double mass, bool rotate_to_direction)
{
	refx = REFPOINT_CIRCLE_LIGHT_X;
	refy = REFPOINT_CIRCLE_LIGHT_Y;
	Bullet::initialize(refx, refy, chara.Center() - $V(width / 2, height / 2), chara.Owner(), power, v, mass);
	lifetime = 40;
	interval = 8;
	generate_counter = 0;
	touch_and_dissapear = false;
	width = height = 64;
	pipe = gfw::Object::Pipe(&chara, *tl);
	writeName("bullet_darkwave", name);
}

void DarkWave::interact(jnr::Character& chara, jnr::JumpnRunData& data)
{
	if (age % interval != 0)
		return;
	if (chara.Owner() == this->Owner())
		return;
	$V center = phys.p + $V(width / 2, height / 2);
	auto dp = center - chara.Center();
	double rate = 0. + (double)age / lifetime * 4.0;
	bool included = dp.l2() < (width * rate - 48);
	if (included)
	{
		Damage d;
		d.power = power;
		d.stun_time = Constants::time_stun;
		d.invinc_time = 0;
		d.ignoring_invinc = true;
		d.owner = this->Owner();

		if (chara.damage(d) >= 0)
		{
			generate<Effect>(*tl)->initialize(REFPOINT_HIT1_X, REFPOINT_HIT1_Y, 32, 32, chara.Center() - $V(16, 16), 2, $V(0.0, 0.0), 0);
			generate<Effect>(*tl)->initialize(REFPOINT_HIT2_X, REFPOINT_HIT2_Y, 32, 32, chara.Center() - $V(16, 16), 4, $V(0.0, 0.0), 0);
			dp += $V(0, 16);

			chara.MP() = chara.MP() + 3;
			if (chara.MP() >= chara.MPMax())
				chara.MP() = chara.MPMax();
			//chara.Smash(-dp.regularized() * 8.0 + $V(0, -4));
		}
	}
}

void DarkWave::run(jnr::JumpnRunData& data)
{
	if (Character* c = static_cast<Character*>(pipe.Dst(*tl)))
	{
		phys.p = c->Center() - $V(width / 2, height / 2);
	}
	else
	{
		finalize();
	}

	/*if (age % interval == 0)
	{
		Effect* e = gfw::generate<Effect>(*tl);
		e->initialize(REFPOINT_EXPLOSION_WHITE_X, REFPOINT_EXPLOSION_WHITE_Y, 32, 32, phys.p, 2, $V(0, 0), 0);
		auto v = $V::polar(4, 2.0 * M_PI * age / lifetime);
		auto* t = generate<Thunder>(*tl);
		t->initialize(REFPOINT_THUNDER_X, REFPOINT_THUNDER_Y, phys.p, Owner(), power, v, phys.mass, rotate_to_direction);
		t->LifeTime(15);
	}*/

	if (lifetime < age)
	{
		finalize();
		return;
	}
	age++;
}

void DarkWave::render(gfw::BaseMedia& media, jnr::JumpnRunData& data)
{
	BaseGraphics* graphics = media.getGraphics();
	{
		double mag = 0. + (double)age / lifetime * 4.0;
		int alpha = (lifetime - age) * 255 / lifetime;
		if (alpha < 0)
			alpha = 0;
		alpha = alpha << 8 * 3;
		alpha = alpha | 0x00222222;
		graphics->drawSpinResize($P.img_obj, phys.p.x - data.camera.X(), phys.p.y - data.camera.Y(), 0
			, refx, refy, width, height, turned, alpha, mag, mag, 0.0);
	}
	if (age > 8)
	{
		int age = this->age - 8;
		double mag = 0. + (double)age / lifetime * 4.0;
		int alpha = (lifetime - age) * 255 / lifetime;
		if (alpha < 0)
			alpha = 0;
		alpha = alpha << 8 * 3;
		alpha = alpha | 0x00222222;
		graphics->drawSpinResize($P.img_obj, phys.p.x - data.camera.X(), phys.p.y - data.camera.Y(), 0
			, refx, refy, width, height, turned, alpha, mag, mag, 0.0);
	}
}