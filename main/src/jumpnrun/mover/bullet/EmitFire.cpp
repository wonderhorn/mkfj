#define _USE_MATH_DEFINES
#include<cmath>
#include"jumpnrun/mover/bullet/Bullet.h"
#include"jumpnrun/mover/character/Character.h"
#include"jumpnrun/mover/effect/Effect.h"
#include"jumpnrun/system/Parmanent.h"
#include"jumpnrun/MoverUtilities.h"
#include"utils/MyCppUtil.h"
#include"jumpnrun/GRAPHICS.h"
using namespace jnr;
using namespace gfw;
using namespace gmtr;
#define $V gmtr::Vector2D

RegisterMoverClass(EmitFire);


void EmitFire::initialize(gmtr::Vector2D p, gmtr::Vector2D v, OWNERHANDLE o, int power)
{
	Bullet::initialize(refx, refy, p,
		o, power, v, 0, rotate_to_direction);
	lifetime = 999999;
	interval = 15;
	generate_counter = 0;
	touch_and_dissapear = false;
	writeName("bullet_emitfire", name);
}

void EmitFire::interact(jnr::Character& chara, jnr::JumpnRunData& data) {}

void EmitFire::run(jnr::JumpnRunData& data)
{


	if (age % interval == 0)
	{
		//auto v = $V::polar(4, 2.0 * M_PI * age / lifetime);
		auto* t = generate<Flame>(*tl);
		t->initialize(REFPOINT_FIRE_X, REFPOINT_FIRE_Y,
			phys.p, Owner(), power, V(), phys.mass, 4);
		//t->LifeTime(45);
		$P.sound_stack.push_back($P.snd_ignition);

	}

	if (lifetime < age)
	{
		finalize();
		return;
	}
	age++;
}