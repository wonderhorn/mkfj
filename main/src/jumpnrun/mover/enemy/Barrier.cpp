#define _USE_MATH_DEFINES
#include<math.h>
#include"jumpnrun/mover/Barrier.h"
#include"jumpnrun/mover/bullet/Bubble.h"
#include"jumpnrun/GRAPHICS.h"
#include"jumpnrun/system/Parmanent.h"
using namespace gmtr;
using namespace gfw;
using namespace jnr;

RegisterMoverClass(Barrier);
RegisterMoverClass(HealBarrier);
RegisterMoverClass(BubbleBarrier);

void Barrier::initialize(int refx, int refy
	, gmtr::Vector2D p, OWNERHANDLE owner, Character* chara)
{
	Mover::initialize(refx, refy, p, owner);
	pipe = Object::Pipe(chara, *tl);
	chara->setBarrier(this);
	lifetime = 60 * 30;
	width = height = 64;
	writeName("barrier", name);
}
void Barrier::run(jnr::JumpnRunData& data)
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
		phys.p = c->Center() - gmtr::Vector2D(width / 2, height /2);
		//c->setBarrier
	}
	if (lifetime >= 0 && age > lifetime)
		die(data);
}
void Barrier::render(gfw::BaseMedia& media, jnr::JumpnRunData& data)
{
	BaseGraphics* graphics = media.getGraphics();
	graphics->draw($P.img_obj, phys.p.x - data.camera.X(), phys.p.y - data.camera.Y(), 0
		, refx, refy, width, height, turned, 0xaaffffff);
}
int Barrier::damage(int power, int stun_time, int invinc_time)
{
	if (!alive)
	{
		return -1;
	}
	//die();
	lifetime = 0;
	return eDamage::Repelled;
}
int Barrier::damage(const Damage& d)
{
	if (!alive)
	{
		return -1;
	}
	lifetime = 0;
	//die(data);
	return eDamage::Repelled;
}
void Barrier::cancel()
{
	lifetime = 0;

	//die(data);
}
void Barrier::die(jnr::JumpnRunData& data)
{
	if (auto* ptr = pipe.Dst(*tl))
	{
		Character* c = static_cast<Character*>(ptr);
		if(c->getBarrier() == this)
			c->setBarrier(nullptr);
	}
	alive = false;
}


void HealBarrier::initialize(int refx, int refy
	, gmtr::Vector2D p, OWNERHANDLE owner, Character* chara)
{
	Barrier::initialize(refx, refy, p, owner, chara);
	writeName("barrier_heal", name);
}

void HealBarrier::run(jnr::JumpnRunData& data)
{
	Barrier::run(data);
	if (age % 60 == 0)
	{
		if (pipe.Dst(*tl) && pipe.Dst(*tl)->alive)
		{
			Character* c = static_cast<Character*>(pipe.Dst(*tl));
			c->HP() = c->HP() + 1;
			if (c->HP() > c->HPMax())
				c->HP() = c->HPMax();
		}
	}
}


void BubbleBarrier::initialize(int refx, int refy
	, gmtr::Vector2D p, OWNERHANDLE owner, Character* chara)
{
	Mover::initialize(refx, refy, p, owner);
	pipe = Object::Pipe(chara, *tl);
	chara->setBarrier(this);
	lifetime = 60 * 30;
	width = height = 64;
	writeName("barrier_bubble", name);
}
void BubbleBarrier::run(jnr::JumpnRunData& data)
{
	Barrier::run(data);
	if(age % 12 == 11)
	{
		Vector2D dir = Vector2D(0, -2);
		if (pipe.Dst(*tl) && pipe.Dst(*tl)->alive)
		{
			Character* c = static_cast<Character*>(pipe.Dst(*tl));
			dir += c->V();
		}
		if (dir.y < -2)
			dir.y = -2;

		double theta = (double)$P.rand.get() / RAND_MAX * M_PI * 2;
		//Vector2D dir = Vector2D($P.rand.get() % 5 - 2, -2);
		generate<Bubble>(*tl)->initialize(REFPOINT_BUBBLE_X, REFPOINT_BUBBLE_Y
			, Center() - Vector2D(16, 16 + 6) + Vector2D::polar(32, theta), dir, 1, Owner());
		$P.sound_stack.push_back($P.snd_blink);
	}
}
void BubbleBarrier::render(gfw::BaseMedia& media, jnr::JumpnRunData& data)
{
	//BaseGraphics* graphics = media.getGraphics();
	//graphics->draw($P.img_obj, phys.p.x - data.camera.X(), phys.p.y - data.camera.Y(), 0
	//	, refx, refy, width, height, turned, 0xaaffffff);
}
int BubbleBarrier::damage(int power, int stun_time, int invinc_time)
{
	if (!alive)
	{
		return -1;
	}
	//die();
	lifetime = 0;
	return eDamage::Through;
}
int BubbleBarrier::damage(const Damage& d)
{
	if (!alive)
	{
		return -1;
	}
	lifetime = 0;
	//die(data);
	return eDamage::Through;
}