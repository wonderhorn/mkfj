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
using std::max;
using std::min;
#define $V gmtr::Vector2D

RegisterMoverClass(Whip);

void Whip::initialize(jnr::Character& chara, int power, gmtr::Vector2D v, gmtr::Vector2D connector, double mass)
{
	Bullet::initialize(refx, refy, chara.P() + connector, chara.Owner(), power, v, mass);
	lifetime = 60 * 10;
	touch_and_dissapear = false;
	width = height = 24;
	pipe = gfw::Object::Pipe(&chara, *tl);
	max_length_seg = 16;
	timer_dissapear = -1;
	alive_length = 99;

	this->connector = connector;
	sprine.push_back(P());
	sprine.push_back(P());

	r = 64;
	g = 64;
	b = 64;

	writeName("bullet_whip", name);
}
void Whip::interact(jnr::Character& chara, jnr::JumpnRunData& data)
{
	if (chara.Owner() == this->Owner())
		return;

	for (int i = 1; i < min(sprine.length, alive_length) - 1; i++)
	{
		$V center = sprine.points[i];
		bool hitting = false;
		if (radius <= 0)
			hitting = chara.Shape().includes(center);
		else
		{
			hitting = chara.Shape().includes(center);
			hitting |= chara.Shape().includes(center + $V(radius, 0));
			hitting |= chara.Shape().includes(center + $V(-radius, 0));
			hitting |= chara.Shape().includes(center + $V(0, radius));
			hitting |= chara.Shape().includes(center + $V(0, -radius));
		}

		if (hitting)
		{
			Damage dam;
			dam.power = power;
			dam.stun_time = Constants::time_stun * 2;
			dam.invinc_time = 0;
			dam.isPhysical = false;
			auto smashv = -this->phys.v.regularized() * 4;
			//if (smashv.l2() == 0)
			smashv += $V(0, -2);
			dam.smash = smashv;
			auto efp = center - $V(16, 16);
			int d = chara.damage(dam);
			if (d >= 0)
			{
				generate<Effect>(*tl)->initialize(REFPOINT_HIT1_X, REFPOINT_HIT1_Y, 32, 32,
					efp, 2, $V(0.0, 0.0), 0);
				generate<Effect>(*tl)->initialize(REFPOINT_HIT2_X, REFPOINT_HIT2_Y, 32, 32,
					efp, 4, $V(0.0, 0.0), 0);

				//chara.Smash(smashv * 4.0 + $V(0, -4) - chara.V());
				finalize();
				if (snd_hit >= 0)
					$P.sound_stack.push_back(snd_hit);
			}
			else if (d == eDamage::Repelled)
			{
				collided_with_earth = true;
				generate<Effect>(*tl)->initialize(REFPOINT_REPELED_X, REFPOINT_REPELED_Y, 32, 32
					, efp, 4, $V(0.0, 0.0), 0);
				generate<Effect>(*tl)->initialize(REFPOINT_REPELED_X, REFPOINT_REPELED_Y + 32, 32, 32
					, efp, 6, $V(0.0, 0.0), 0);
				finalize();
				$P.sound_stack.push_back($P.snd_repell);
			}
		}
	}
}
void Whip::run(jnr::JumpnRunData& data)
{
	const int disappear_speed = 1;
	if (lifetime - age < disappear_speed * sprine.length)
		timer_dissapear = disappear_speed * sprine.length;
	if (lifetime < age)
	{
		finalize();
		return;
	}
	if (timer_dissapear > 0)
	{
		timer_dissapear--;
		//alive_length = (double)sprine.length * timer_dissapear / (disappear_speed * sprine.length);
		if (timer_dissapear % disappear_speed == 0)
		{
			reelup();
			//tenseForward();
		}
	}
	if (timer_dissapear == 0)
	{
		finalize();
		return;
	}

	if (Character* c = static_cast<Character*>(pipe.Dst(*tl)))
	{
		sprine.points[0] = connector + c->P();
		//sprine.points[0] = c->Center();
	}

	auto newphys = phys;
	newphys.a = Vector2D(0.0, 0.2 * phys.mass);
	newphys.v += newphys.a;
	$V p_ter = sprine.points[sprine.length - 1];
	p_ter += V();
	newphys.p = p_ter;
	//phys = newphys;

	if (!stg->enterable(P()) || sprine.length >= 12)
	{
		if (timer_dissapear < 0)
			timer_dissapear = disappear_speed * sprine.length;
		//alive = false;
	}
	else
	{
		phys = newphys;
		sprine.points[sprine.length - 1] = p_ter;
	}

	double l = (p_ter - sprine.points[sprine.length - 2]).l2();
	if (l >= max_length_seg)
	{
		if (timer_dissapear < 0)
		{
			sprine.push_back(p_ter);
			alive_length = sprine.length;
		}
	}
	tenseForward();

	age++;

	//Mover::run(data);
}

void Whip::tenseForward()
{
	const double eps = 0.01;
	for (int i = 1; i < sprine.length - 1; i++)
	{
		sprine.points[i].y += 0.5;
		auto dp = sprine.points[i] - sprine.points[i - 1];
		double l2 = dp.l2();
		if (l2 >= eps && l2 > max_length_seg)
		{
			if (l2 > max_length_seg)
				sprine.points[i] = sprine.points[i - 1] + (sprine.points[i] - sprine.points[i - 1]) * (max_length_seg / l2);
		}
	}
}


void Whip::tenseBackward()
{
	const double eps = 0.01;
	for (int i = sprine.length - 2; i >= 0; i--)
	{
		auto dp = sprine.points[i + 1] - sprine.points[i];
		double l2 = dp.l2();
		if (l2 >= eps && l2 > max_length_seg)
		{
			if (l2 > max_length_seg)
				sprine.points[i] = sprine.points[i + 1] - (sprine.points[i + 1] - sprine.points[i]) * (max_length_seg / l2);
		}
	}
}

void Whip::reelup()
{
	//for (int i = sprine.length - 2; i < sprine.length - 1; i++)
	for (int i = 1; i < sprine.length - 1; i++)
	{
		sprine.points[i] = sprine.points[i + 1];
		//sprine.points[i] = sprine.points[i - 1] + (sprine.points[i + 1] - sprine.points[i]);
	}
	//tenseForward();
	//if (sprine.length > 0)
	//	sprine.length = sprine.length - 1;
	alive_length--;
}


void Whip::render(gfw::BaseMedia& media, jnr::JumpnRunData& data)
{
	BaseGraphics* graphics = media.getGraphics();
	for (int i = 0; i < min(sprine.length, alive_length) - 1; i++)
	{
		graphics->drawLine(sprine.points[i].x - data.camera.X(), sprine.points[i].y - data.camera.Y()
			, sprine.points[i + 1].x - data.camera.X(), sprine.points[i + 1].y - data.camera.Y()
			, r, g, b, 255, 1);
	}
}