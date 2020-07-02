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

RegisterMoverClass(SwordStream);

void SwordStream::initialize(jnr::Character& chara, int power, gmtr::Vector2D v, double mass, bool rotate_to_direction)
{
	Bullet::initialize(refx, refy, chara.Center() - $V(16, 16), chara.Owner(), power, v, mass);
	lifetime = 40;
	interval = 19;
	generate_counter = 0;
	touch_and_dissapear = false;
	pipe = gfw::Object::Pipe(&chara, *tl);
	writeName("bullet_swordstream", name);
}

void SwordStream::interact(jnr::Character& chara, jnr::JumpnRunData& data) {}

void SwordStream::run(jnr::JumpnRunData& data)
{
	double vx = 0.0;
	if (Character* c = static_cast<Character*>(pipe.Dst(*tl)))
	{
		phys.p = c->Center() + $V(-16, -16);
		vx = c->Turned() ? -8.0 : 8.0;
	}
	else
	{
		finalize();
	}

	if (age % interval == 0)
	{
		int id = age / interval;
		double theta = 2.0 * M_PI * id / 3;
		$V ori(24 * cos(theta), 16 * sin(theta));
		double dy = 48;

		generate<Sword>(*tl)->initialize(REFPOINT_SWORD_X, REFPOINT_SWORD_Y,
			phys.p + $V(0, - dy) + ori, Owner(), power,
			phys.v, phys.mass, true);
	}

	if (lifetime < age)
	{
		finalize();
		return;
	}
	age++;
}