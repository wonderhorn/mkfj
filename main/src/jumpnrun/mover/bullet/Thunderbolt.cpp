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

RegisterMoverClass(Thunderbolt);

void Thunderbolt::initialize(jnr::Character& chara, int power, gmtr::Vector2D v, double mass, bool rotate_to_direction)
{
	Bullet::initialize(refx, refy, chara.Center() - $V(16, 16),
		chara.Owner(), power, v, mass, rotate_to_direction);
	lifetime = 36;
	interval = 4;
	generate_counter = 0;
	touch_and_dissapear = false;
	pipe = gfw::Object::Pipe(&chara, *tl);
	writeName("bullet_thunderbolt", name);
}

void Thunderbolt::interact(jnr::Character& chara, jnr::JumpnRunData& data){}

void Thunderbolt::run(jnr::JumpnRunData& data)
{
	if (Character* c = static_cast<Character*>(pipe.Dst(*tl)))
	{
		phys.p = c->Center() + $V(-16, -16);
	}
	else
	{
		finalize();
	}

	if (age % interval == 0)
	{
		Effect* e = gfw::generate<Effect>(*tl);
		e->initialize(REFPOINT_EXPLOSION_WHITE_X, REFPOINT_EXPLOSION_WHITE_Y, 32, 32, phys.p, 2, $V(0, 0), 0);
		auto v = $V::polar(4, 2.0 * M_PI * age / lifetime);
		auto* t = generate<Thunder>(*tl);
		t->initialize(REFPOINT_THUNDER_X, REFPOINT_THUNDER_Y,
			phys.p, Owner(), power, v, phys.mass, rotate_to_direction);
		t->LifeTime(28);
		$P.sound_stack.push_back($P.snd_elec);

	}

	if (lifetime < age)
	{
		finalize();
		return;
	}
	age++;
}