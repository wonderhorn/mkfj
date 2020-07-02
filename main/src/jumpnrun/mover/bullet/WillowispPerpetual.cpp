#define _USE_MATH_DEFINES
#include<math.h>
#include"jumpnrun/mover/Barrier.h"
#include"jumpnrun/mover/bullet/Bubble.h"
#include"jumpnrun/GRAPHICS.h"
#include"jumpnrun/system/Parmanent.h"

using namespace gfw;
using namespace jnr;
using namespace gmtr;
#define $V Vector2D

RegisterMoverClass(WillowispPerpetual);

void WillowispPerpetual::initialize(int refx, int refy
	, gmtr::Vector2D p, OWNERHANDLE owner, Character* chara, int num_fire)
{
	Mover::initialize(refx, refy, p, owner);
	pipe = Object::Pipe(chara, *tl);
	chara->setBarrier(this);
	lifetime = 60 * 60 * 15;
	width = height = 32;
	this->num_fire = num_fire;
	for (int i = 0; i < num_fire; i++)
	{
		auto* ptr = generate<Willowisp>(*tl);
		ptr->initialize(REFPOINT_FIRE_X, REFPOINT_FIRE_Y, *chara, p, $V(0, 0),
			3, owner, i * 2.0 * M_PI / num_fire);
		pipes[i] = Pipe(ptr, *tl);
	}
	for (int i = 0; i < MAX_NUM_PIPE; i++)
	{
		pipe_timers[i] = -1;
	}

	writeName("barrier_willperp", name);
}
void WillowispPerpetual::run(jnr::JumpnRunData& data)
{

	Mover::run(data);
	if (!pipe.Dst(*tl) || !pipe.Dst(*tl)->alive)
	{
		die(data);
		return;
	}
	else
	{
		Character* c = static_cast<Character*>(pipe.Dst(*tl));
		phys.p = c->Center() - gmtr::Vector2D(width / 2, height / 2);

		for (int i = 0; i < num_fire; i++)
		{
			if (pipe_timers[i] == 0)
			{
				auto ptr = generate<Willowisp>(*tl);
				ptr->initialize(REFPOINT_FIRE_X, REFPOINT_FIRE_Y, *c, Center(), $V(0, 0),
					3, owner, i * 2.0 * M_PI / num_fire + (float)age / 45);
				pipes[i] = Pipe(ptr, *tl);
				pipe_timers[i] = -1;
			}
			if (pipe_timers[i] >= 0)
			{
				pipe_timers[i]--;
			}

			auto& pipe = pipes[i];
			Willowisp* ptr = static_cast<Willowisp*>(pipe.Dst(*tl));
			if ((!ptr || !ptr->alive) && pipe_timers[i] == -1)
			{
				pipes[i] = Pipe(nullptr, *tl);
				pipe_timers[i] = 90 * 6;
			}
		}

	}
	if (lifetime >= 0 && age > lifetime)
		die(data);
}
void WillowispPerpetual::render(gfw::BaseMedia& media, jnr::JumpnRunData& data)
{
	BaseGraphics* graphics = media.getGraphics();
}
int WillowispPerpetual::damage(int power, int stun_time, int invinc_time)
{
	return -1;
}
int WillowispPerpetual::damage(const Damage& d)
{
	return -1;

}
void WillowispPerpetual::cancel()
{
	//die();
	lifetime = 0;
}
void WillowispPerpetual::die(jnr::JumpnRunData& data)
{
	if (auto* ptr = pipe.Dst(*tl))
	{
		Character* c = static_cast<Character*>(ptr);
		c->setBarrier(nullptr);
	}
	alive = false;

}