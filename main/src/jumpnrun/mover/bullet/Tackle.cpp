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

RegisterMoverClass(Tackle);

void Tackle::initialize(Character& chara, int power, gmtr::Vector2D v)
{
	Bullet::initialize(0, 0, chara.P(), chara.Owner(), power, v, 0);
	pipe.init(&chara, *tl);
	margin_top = margin_side = margin_bottom = 8;
	this->width = chara.Width();
	this->height = chara.Height();
	lifetime = 20;
	timer_finish = -1;
	particle_refx = particle_refy = -1;
	writeName("bullet_tackle", name);
}
void Tackle::interact(jnr::Character& chara, jnr::JumpnRunData& data)
{
	if (chara.Owner() == this->Owner() || timer_finish >= 0)
		return;
	$V center = phys.p + $V(width / 2, height / 2);
	if (chara.Shape().includes(center))
	{
		Damage d;
		d.invinc_time = Constants::time_invinc;
		d.power = power;
		d.stun_time = Constants::time_stun;
		d.isPhysical = true;
		auto smashv = this->phys.v.regularized();
		if (smashv.l2() == 0)
			smashv = $V(0, -1);
		d.smash = smashv * 4.0 + $V(0, -4) - chara.V();
		d.owner = this->Owner();
		if (chara.damage(d) >= 0)
		{
			generate<Effect>(*tl)->initialize(REFPOINT_HIT1_X, REFPOINT_HIT1_Y, 32, 32, phys.p, 2, $V(0.0, 0.0), 0);
			generate<Effect>(*tl)->initialize(REFPOINT_HIT2_X, REFPOINT_HIT2_Y, 32, 32, phys.p, 4, $V(0.0, 0.0), 0);
			//chara.Smash(this->phys.v.regularized() * 4.0 + $V(0, -4));
			//finalize();
			timer_finish = 10;
		}
	}
}
void Tackle::run(jnr::JumpnRunData& data)
{
	if (timer_finish > 0)
		timer_finish--;
	else if (timer_finish == 0)
	{
		finalize();
		return;
	}


	if (Character* c = static_cast<Character*>(pipe.Dst(*tl)))
	{
		if(!c->alive)
		{
			pipe.init(nullptr, *tl);
		}
		else
		{
			if (c->Status() == Character::eStatus::Damaged)
				finalize();
			c->Smash(this->V());
			phys.p = c->P() + 2 * this->V();
			c->setInvincPhys(5);

			// generates trace
			if (particle_refx >= 0 && age % 3 == 0)
			{
				int vx = c->Turned() ? 24 : -24;
				Effect* e = gfw::generate<Effect>(*tl);
				e->initialize(particle_refx, particle_refy, 16, 16, Center() + $V(vx - 8, $P.rand.get() % 8)
					, 8, $V(0, 0), 0, false);
	
			}
		}
	}
	else
	{
		finalize();
	}

	if (lifetime < age)
	{
		finalize();
		return;
	}
	Mover::run(data);
}

void Tackle::render(gfw::BaseMedia& media, jnr::JumpnRunData& data) 
{}