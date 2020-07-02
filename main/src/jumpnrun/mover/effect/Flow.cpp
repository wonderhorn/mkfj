#include"jumpnrun/mover/effect/Effect.h"
#include"jumpnrun/mover/character/Character.h"
#include"jumpnrun/GRAPHICS.h"
#include"jumpnrun/system/Parmanent.h"
#define _USE_MATH_DEFINES
#include<math.h>
using namespace jnr;
using namespace gfw;
using namespace gmtr;
#define $V gmtr::Vector2D
RegisterMoverClass(Flow);

void Flow::initialize(Character* chara, int refx, int refy, int w, int h
	, gmtr::Vector2D p, int lifetime, double mass
	, int num_particles)
{
	Effect::initialize(refx, refy, w, h, p, lifetime, $V(0, 0), mass);
	pipe = Pipe(chara, *tl);
	this->num_particles = num_particles;
	this->speed = speed;
	this->turned = turned;
	for (int i = 0; i < num_particles; i++)
	{
		auto& ptcl = particles[i];
		ptcl.p = p + $V($P.rand.get() % 48 - 24, $P.rand.get() % 48 - 24);
		ptcl.v = $V($P.rand.get() % 5 - 2, $P.rand.get() % 5 - 2);
		ptcl.age = 0;
		ptcl.lifetime = lifetime;
	}
}
void Flow::run(jnr::JumpnRunData& data)
{
	Effect::run(data);

	if (auto* ptr = pipe.Dst(*tl))
	{
		Character* c = static_cast<Character*>(ptr);
		if (!c->alive)
		{
			finalize();
			pipe = Pipe(nullptr, *tl);
		}
		else
		{
			for (int i = 0; i < num_particles; i++)
			{
				auto& ptcl = particles[i];
				if (ptcl.age < 0)
					continue;
				$V dp = c->Center() - $V(width, height) / 2 - ptcl.p;
				if (dp.l2() < 16) 
				{
					ptcl.age = -1;
					continue;
				}
				dp = dp.regularized();
				ptcl.v = 0.95 * ptcl.v + 0.05 * dp * 6;
				if (ptcl.age >= 0)
					ptcl.p += ptcl.v;
				ptcl.age++;
			}
		}
	}
	
}
void Flow::render(gfw::BaseMedia& media, jnr::JumpnRunData& data)
{
	BaseGraphics* graphics = media.getGraphics();
	for (int i = 0; i < num_particles; i++)
	{
		auto& p = particles[i];
		if (p.age >= 0)
		{
			graphics->draw($P.img_obj, p.p.x - data.camera.X(), p.p.y - data.camera.Y(), 0
				, refx, refy, width, height, turned);
		}
	}
}