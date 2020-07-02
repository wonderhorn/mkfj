#include"jumpnrun/mover/effect/Effect.h"
#include"jumpnrun/GRAPHICS.h"
#include"jumpnrun/system/Parmanent.h"
#define _USE_MATH_DEFINES
#include<math.h>
using namespace jnr;
using namespace gfw;
using namespace gmtr;
#define $V gmtr::Vector2D
RegisterMoverClass(Particles);

void Particles::initialize(int refx, int refy, int w, int h
	, gmtr::Vector2D p, int lifetime, gmtr::Vector2D v, double mass
	, int num_particles, double speed, ParticleType type, bool turned)
{
	Effect::initialize(refx, refy, w, h, p, lifetime, v, mass);
	this->num_particles = num_particles;
	this->type = type;
	this->speed = speed;
	this->turned = turned;
	for (int i = 0; i < num_particles; i++)
	{
		auto& ptcl = particles[i];
		ptcl.p = p;
		if (type == ParticleRadiation)
		{
			ptcl.v = $V(speed * cos(2.0 * M_PI / num_particles * i)
				, speed * sin(2.0 * M_PI / num_particles * i));
			ptcl.age = 0;
		}
		else if (type == ParticleSpiral)
		{
			ptcl.v = $V(0, -speed);
			ptcl.age = -5 * i;
			//if (turned)
			//	ptcl.p.x += 48;
		}

		ptcl.lifetime = lifetime;
	}
}
void Particles::run(jnr::JumpnRunData& data)
{
	Effect::run(data);
	for (int i = 0; i < num_particles; i++)
	{
		auto& ptcl = particles[i];
		if (type == ParticleSpiral) 
		{
			ptcl.v = $V(6 * cos(M_PI / 16 * (ptcl.age)), -speed);
			if (turned)
				ptcl.v.x = -ptcl.v.x;
		}
		if (ptcl.age >= 0)
			ptcl.p += ptcl.v;
		ptcl.age++;
	}
}
void Particles::render(gfw::BaseMedia& media, jnr::JumpnRunData& data)
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