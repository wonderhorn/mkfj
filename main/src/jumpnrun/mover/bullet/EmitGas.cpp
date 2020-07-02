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

RegisterMoverClass(EmitGas);

void EmitGas::initialize(jnr::Character* chara, OWNERHANDLE owner, int power, gmtr::Vector2D p,
gmtr::Vector2D v, double mass, bool rotate_to_direction)
{
	if(chara)
		Bullet::initialize(refx, refy, chara->Center() - $V(16, 16), chara->Owner(), power, v, mass);
	else
		Bullet::initialize(refx, refy, p, owner, power, v, mass);

	lifetime = 45;
	interval = 11;
	generate_counter = 0;
	touch_and_dissapear = false;
	pipe = gfw::Object::Pipe((gfw::Object*)chara, *tl);
	writeName("bullet_emitgas", name);
}

void EmitGas::interact(jnr::Character& chara, jnr::JumpnRunData& data){}

void EmitGas::run(jnr::JumpnRunData& data)
{
	if (Character* c = static_cast<Character*>(pipe.Dst(*tl)))
	{
		phys.p = c->Center() + $V(-16, -16);
		if(!c->alive)
			finalize();
	}

	if (age % interval == 0)
	{
		auto v = $V::polar(1, (double)$P.rand.get() / RAND_MAX * M_PI + M_PI);
		generate<Gas>(*tl)->initialize(REFPOINT_GAS_X, REFPOINT_GAS_Y, phys.p, Owner(), power, v, phys.mass);
		$P.sound_stack.push_back($P.snd_blink);
	}


	if (lifetime < age)
	{
		finalize();
		return;
	}
	age++;
}