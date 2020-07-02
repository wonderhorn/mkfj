#define _USE_MATH_DEFINES
#include<cmath>
#include"jumpnrun/mover/character/Character.h"
#include"jumpnrun/mover/bullet/Bullet.h"
#include"jumpnrun/mover/bullet/Bubble.h"
#include"jumpnrun/mover/effect/Effect.h"
#include"jumpnrun/system/Parmanent.h"
#include"jumpnrun/GRAPHICS.h"
using namespace jnr;
using namespace gfw;
using namespace gmtr;
#define $V gmtr::Vector2D

RegisterMoverClass(BubbleStream);

void BubbleStream::initialize(jnr::Character& chara, int power, gmtr::Vector2D v, double mass, bool rotate_to_direction)
{
	Bullet::initialize(refx, refy, chara.Center() - $V(16, 16), chara.Owner(), power, v, mass);
	lifetime = 45;
	interval = 4;
	generate_counter = 0;
	touch_and_dissapear = false;
	pipe = gfw::Object::Pipe(&chara, *tl);
	writeName("bullet_bubblestream", name);
}

void BubbleStream::interact(jnr::Character& chara, jnr::JumpnRunData& data){}

void BubbleStream::run(jnr::JumpnRunData& data)
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
		generate<Bubble>(*tl)->initialize(REFPOINT_BUBBLE_X, REFPOINT_BUBBLE_Y, phys.p
			, phys.v + $V((double)($P.rand.get() % 3 - 1) / 2, (double)($P.rand.get() % 3 - 2) / 4), 2, Owner());
	}

	if (lifetime < age)
	{
		finalize();
		return;
	}
	age++;
}