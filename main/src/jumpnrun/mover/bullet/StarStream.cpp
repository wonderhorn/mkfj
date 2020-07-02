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

RegisterMoverClass(StarStream);

void StarStream::initialize(jnr::Character& chara, int power, gmtr::Vector2D v, double mass, bool rotate_to_direction)
{
	Bullet::initialize(refx, refy, chara.Center() - $V(16, 16), chara.Owner(), power, v, mass);
	lifetime = 60;
	interval = 12;
	generate_counter = 0;
	touch_and_dissapear = false;
	pipe = gfw::Object::Pipe(&chara, *tl);
	writeName("bullet_starstream", name);
}

void StarStream::interact(jnr::Character& chara, jnr::JumpnRunData& data){}

void StarStream::run(jnr::JumpnRunData& data)
{
	if (Character* c = static_cast<Character*>(pipe.Dst(*tl)))
	{
		phys.p = c->Center() + $V(-16, -16);
		if (c->Status() == Character::eStatus::Damaged)
		{
			finalize();
		}
	}
	else
	{
		finalize();
	}

	if (age % interval == 0)
	{
		int offset_refx = 32 * ($P.rand.get() % 4);
		generate<Bullet>(*tl)->initialize(REFPOINT_STAR1_X + offset_refx, REFPOINT_STAR1_Y,
			phys.p, Owner(), power, phys.v, phys.mass, false);
		$P.sound_stack.push_back($P.snd_magicshot);
	}

	if (lifetime < age)
	{
		finalize();
		return;
	}
	age++;
}